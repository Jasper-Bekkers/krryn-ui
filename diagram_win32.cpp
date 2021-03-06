#include "diagram.h"
#include "diagram_win32.h"
#include "widget_helper_win32.h"
#include <algorithm>
#include <stdexcept>

#pragma comment(lib, "gdiplus.lib")

using namespace gui_imp;
using namespace gui;
using namespace Gdiplus;

namespace gui_imp{
	struct diagram_colors{
		SolidBrush selectedTitleBrush;
		SolidBrush blackBrush;
		SolidBrush highlightBrush;
		SolidBrush backgroundBrush;
		SolidBrush selectedBackgroundBrush;
		SolidBrush selectionBackground;
		SolidBrush bulletBrush;

		Pen bezierPen;
		Pen selectedBezierPen;
		Pen selectionBorder;

		Font font;
		Font headerFont;

		diagram_colors() :
			selectedTitleBrush(Color(255, 156, 226, 122)),
			blackBrush(Color(255, 20, 20, 20)),
			highlightBrush(Color(255, 132, 172, 217)),
			backgroundBrush(Color(255, 243, 246, 252)),
			selectedBackgroundBrush(Color(255, 232, 249, 223)),
			selectionBackground(Color(127, 215, 195, 162)),
			bulletBrush(Color(255, 170, 170, 200)),
			bezierPen(Color(255, 132, 172, 217), 2.f),
			selectedBezierPen(Color(255, 156, 226, 122), 2.f),
			selectionBorder(Color(127, 195, 163, 104), 1.f),
			font(L"Tahoma", 10),
			headerFont(L"Tahoma", 10, FontStyleBold){}
	};
}

namespace{
	char* diagram_class(){
		static bool m_IsRegistered = false;

		if(!m_IsRegistered){
			WNDCLASS l_WindowClass;
			memset(&l_WindowClass, 0, sizeof(l_WindowClass));

			l_WindowClass.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
			l_WindowClass.lpfnWndProc	= (WNDPROC)DefWindowProc;
			l_WindowClass.hIcon			= LoadIcon(NULL, IDI_WINLOGO);
			l_WindowClass.hCursor		= LoadCursor(NULL, IDC_ARROW);
			l_WindowClass.lpszClassName	= "diagram_class";

			if(!RegisterClass(&l_WindowClass)){
				throw widget_exception("Can't register window class");
			}else{
				m_IsRegistered = true;
			}
		}

		return "diagram_class";
	}

	std::wstring to_wstring(const std::string &a_String){
		return std::wstring(a_String.begin(), a_String.end());
	}

	// As a side effect we update the AABB of the port because this function knows exactly what data
	// is used to draw it and thus gives us pixel accurate results
	void draw_bullet(Graphics &g, diagram_colors &c, int x, int y, int bulletOffset, diagram_port_win32 *a_Port, bool a_HighlightForSelection){
		RectF rect(float(x + bulletOffset), float(y + 6), 5.0f, 5.0f);
		g.FillEllipse(&c.bulletBrush, rect);

		a_Port->m_BulletCenter.X = rect.X + rect.Width / 2;
		a_Port->m_BulletCenter.Y = rect.Y + rect.Height / 2;

		RectF l_OutRect;
		RectF l_Aabb;

		std::wstring l_Name = to_wstring(a_Port->get_name());

		Brush* brush = a_HighlightForSelection ? &c.highlightBrush : &c.blackBrush;

		if(bulletOffset != 0){
			StringFormat format;
			format.SetAlignment(StringAlignmentFar);

			PointF origin((REAL)x + bulletOffset - 4, (REAL)y);

			g.DrawString(l_Name.c_str(), l_Name.size(), &c.font, origin, &format, brush);
			g.MeasureString(l_Name.c_str(), l_Name.size(), &c.font, origin, &format, &l_OutRect);

			rect.Width += 6; // extend a bit so we have a better clicking region
		}else{
			PointF origin((REAL)x + 10.f, (REAL)y);
			g.DrawString(l_Name.c_str(), l_Name.size(), &c.font, origin, brush);
			g.MeasureString(l_Name.c_str(), l_Name.size(), &c.font, origin, &l_OutRect);

			rect.X -= 6; // extend region
		}

		l_Aabb.Union(l_Aabb, l_OutRect, rect);
		a_Port->m_WorldBounds = l_Aabb;
	}
}

void diagram_impl_win32::make(const diagram_initializer &a_Initializer){
	widget_helper_win32::load_common_controls();

	HWND l_ParentWnd = (HWND)a_Initializer.get_parent()->handle();

	{
		widget_hook_win32(this);

		m_hWnd = CreateWindowEx(
			0,
			diagram_class(), 
			TEXT(""), 
			WS_TABSTOP | widget_helper_win32::visible(a_Initializer) | WS_CHILD, 
			a_Initializer.get_left(), a_Initializer.get_top(), 
			a_Initializer.get_width(), a_Initializer.get_height(), 
			l_ParentWnd, 0, 0, 0);
	}
	
	m_Hdc = GetDC(m_hWnd);

	// Deselect items
	m_MovingSelection = false;

	m_Serializer = 0;
	m_NodeIdCounter = 0;
	m_BeginPort = 0;

	m_ScaleX = 1.0;
	m_ScaleY = 1.0;

	m_Colors = new diagram_colors;

	m_Factory = new diagram_factory_win32(
		a_Initializer.get_factory());

	m_ConnectionPolicy = m_Factory->create_connection_policy();

	m_SelectionChanged = false;

	// Initialize panning
	m_CenterX = 0;
	m_CenterY = 0;
	m_DeltaX = 0;
	m_DeltaY = 0;
	m_PanStartX = 0;
	m_PanStartY = 0;
}

diagram_serializer* diagram_impl_win32::get_serializer(){
	if(!m_Serializer){
		m_Serializer = m_Factory->create_serializer();
	}

	return m_Serializer;
}

// This function has the side effect of updating the ports bounding box
std::pair<int, int> diagram_node_win32::get_max_width_and_total_height(Graphics &g, Font &a_Font, RectF &a_Layout, gui::port_type a_Type){
	int width = 0;
	int height = 0;

	for(size_t i = 0; i < get_port_count(); i++){
		diagram_port_base *l_Port = get_port(i);

		if(l_Port->get_type() == a_Type){
			RectF l_OutRect;

			std::wstring l_Name = to_wstring(l_Port->get_name());
			g.MeasureString(l_Name.c_str(), l_Name.size(), &a_Font, a_Layout, &l_OutRect);

			height += (int)l_OutRect.Height;

			if(l_OutRect.Width > width){
				width = (int)l_OutRect.Width;
			}
		}
	}

	return std::make_pair(width, height);
}

diagram_node_win32::diagram_node_win32(gui::diagram_impl_base *a_Parent, gui::diagram_node_base *a_Node)
	: m_Node(a_Node)
	, gui::diagram_node_base(a_Parent)
	, m_Win32Parent(dynamic_cast<diagram_impl_win32*>(a_Parent)){}

void diagram_node_win32::set_image(view::pixel_t *a_Pixel, int a_Width, int a_Height)
{
	m_Node->set_image(a_Pixel, a_Width, a_Height);
	InvalidateRect((HWND)m_Win32Parent->handle(), NULL, FALSE);
}

view::pixel_t* diagram_node_win32::get_image() const 
{
	return m_Node->get_image(); 
}

void diagram_node_win32::draw_node(Graphics &g, diagram_colors &c, int x, int y, bool selected){
	// make sure everything fits inside container
	int arcSize = 19;
	int headerHeight = max(0, 36 - arcSize);

	RectF layout((REAL)x + arcSize / 2, (REAL)y, 10000, 10000); // last two don't matter we use outrect
	RectF outrect;
	std::wstring l_Title = to_wstring(get_title());
	g.MeasureString(l_Title.c_str(), l_Title.size(), &c.headerFont, layout, &outrect);

	std::pair<int, int> l_InSize = get_max_width_and_total_height(g, c.font, layout, gui::in);
	std::pair<int, int> l_OutSize = get_max_width_and_total_height(g, c.font, layout, gui::out);

	l_InSize.first += (l_OutSize.second == 0 ? 0 : 9);
	l_OutSize.first += (l_OutSize.second == 0 ? 0 : 3);

	int width = max((int)outrect.Width, l_InSize.first + l_OutSize.first);
	int height = max(l_InSize.second, l_OutSize.second);
	int bulletOffset = y;

	if(get_image()){
		height += 75;
		width = max(width, 75);
		bulletOffset += 70;
	}

	RectF l_Aabb;
	RectF l_HeaderAabb;
	RectF l_BodyAabb;
	SolidBrush l_titleBrush(Color((ARGB)get_header_color() | 0xFF000000));

	{
		GraphicsPath gp;

		// draw header
		gp.AddLine(x, y + headerHeight + arcSize / 2, x, y + arcSize / 2);
		gp.AddArc(x, y, arcSize, arcSize, 180.f, 90.f);
		gp.AddArc(x + width, y, arcSize, arcSize, 270.f, 90.f);
		gp.AddLine(x + width + arcSize, y + arcSize / 2, x + width + arcSize, y + headerHeight + arcSize / 2);

		g.FillPath(selected ? &c.selectedTitleBrush : &l_titleBrush, &gp);

		gp.GetBounds(&l_HeaderAabb);
	}

	g.DrawString(l_Title.c_str(), l_Title.size(), &c.headerFont, PointF((REAL)x + arcSize / 2, (REAL)y + 5), &c.blackBrush);

	{
		GraphicsPath gp;

		gp.AddLine(x, y + headerHeight + arcSize / 2, x, y + headerHeight + height + arcSize / 2);
		gp.AddArc(x, y + headerHeight  + height, arcSize, arcSize, 180.f, -90.f);
		gp.AddArc(x + width, y + headerHeight + height, arcSize, arcSize, 90.f, -90.f);
		gp.AddLine(x + width + arcSize, y + headerHeight  + height + arcSize / 2, x + width + arcSize, y + headerHeight  + arcSize / 2);

		g.FillPath(selected ? &c.selectedBackgroundBrush : &c.backgroundBrush, &gp);

		gp.GetBounds(&l_BodyAabb);
	}

	if(get_image())
	{
		Bitmap bmp(64, 64, 64 * 4, PixelFormat32bppARGB, (BYTE*)get_image());
		int imgx = x + ((int)l_BodyAabb.GetRight() - (int)l_BodyAabb.GetLeft()) / 2 - 32;
		g.DrawImage(&bmp, imgx, y + headerHeight + 5 + arcSize / 2, 64, 64);
	}

	// Recalculate bounding box after drawing because GDI+ can do it for us (so it'll be pixel accurate)
	l_Aabb.Union(l_Aabb, l_HeaderAabb, l_BodyAabb);
	m_WorldBounds = l_Aabb;

	gui::diagram_connection_policy *l_Policy = m_Win32Parent->get_connection_policy();
	diagram_port_win32* l_SelectedPort = m_Win32Parent->get_current_selected_port();

	int l_PreviousY = bulletOffset;
	for(size_t i = 0; i < get_port_count(); i++){
		diagram_port_win32 *l_Port = (diagram_port_win32*)get_port(i);
		if(l_Port->get_type() == gui::in){

			bool l_Highlight = false;
			if (l_SelectedPort) {
				if (l_Policy->can_connect_ports(l_Port, l_SelectedPort)) {
					l_Highlight = true;
				}
			}

			draw_bullet(g, c, x + 6, bulletOffset + headerHeight + arcSize / 2, 0, l_Port, l_Highlight);
			bulletOffset += 14;
		}
	}

	bulletOffset = l_PreviousY;

	for(size_t i = 0; i < get_port_count(); i++){
		diagram_port_win32 *l_Port = (diagram_port_win32*)get_port(i);
		if(l_Port->get_type() == gui::out){

			bool l_Highlight = false;
			if (l_SelectedPort) {
				if (l_Policy->can_connect_ports(l_Port, l_SelectedPort)) {
					l_Highlight = true;
				}
			}

			draw_bullet(g, c, width - l_OutSize.first + x + 6, bulletOffset + headerHeight + arcSize / 2, l_OutSize.first + 3, l_Port, l_Highlight);
			bulletOffset += 14;
		}
	}
}

Gdiplus::RectF gui_imp::diagram_impl_win32::get_clamped_scaled_selection_rect()
{
	// Fix the selection bounding box
	// because it can't handle negative Width/Heights

	RectF l_Selection = m_Selection;

	if (l_Selection.Width < 0) {
		l_Selection.X += l_Selection.Width;
		l_Selection.Width = abs(l_Selection.Width);
	}

	if (l_Selection.Height < 0) {
		l_Selection.Y += l_Selection.Height;
		l_Selection.Height = abs(l_Selection.Height);
	}

	l_Selection.X /= m_ScaleX;
	l_Selection.Y /= m_ScaleY;
	l_Selection.Width /= m_ScaleX;
	l_Selection.Height /= m_ScaleY;

	l_Selection.X -= m_CenterX;
	l_Selection.Y -= m_CenterY;

	return l_Selection;
}

void diagram_impl_win32::deselect_all(){
	for(selection_t::iterator i = m_CurrentSelection.begin(); i != m_CurrentSelection.end(); i++){
		(*i)->m_DistanceFromMouse.X = 0;
		(*i)->m_DistanceFromMouse.Y = 0;
	}

	selection_t l_Empty;
	m_CurrentSelection.swap(l_Empty);
	m_SelectionChanged = true;
}

size_t diagram_impl_win32::get_node_count(){
	return m_Nodes.size();
}

diagram_node_base *diagram_impl_win32::get_node(size_t i){
	return m_Nodes[i];
}

void diagram_impl_win32::store_relative_positions_selected(float x, float y){
	math::size l_Size = get_size();

	// calculate distance from mouse pointer
	// so we can use this to keep the nodes positioned relative to each other
	REAL l_MouseWorldX = (x - l_Size.m_X / 2 - m_CenterX - m_DeltaX);
	REAL l_MouseWorldY = (y - l_Size.m_Y / 2 - m_CenterY - m_DeltaY);

	for(selection_t::iterator i = m_CurrentSelection.begin(); i != m_CurrentSelection.end(); i++){
		(*i)->m_DistanceFromMouse.X = (*i)->get_position().m_X - l_MouseWorldX;
		(*i)->m_DistanceFromMouse.Y = (*i)->get_position().m_Y - l_MouseWorldY;
	}
}

diagram_node_base* diagram_impl_win32::create_node(const std::string &a_Title, diagram_id_t a_Id){
	diagram_node_base *l_Node = 0;

	if(a_Id == g_InvalidId){
		m_NodeIdCounter++;
		l_Node = m_Factory->create_node(this, a_Title, m_NodeIdCounter);
	}else{
		l_Node = m_Factory->create_node(this, a_Title, a_Id);
	}

	if(m_IdToNodes[l_Node->get_id()]){
		std::runtime_error("Error: duplicate diagram node id for node " + a_Title);
	}

	l_Node->init(m_Factory);
	m_Nodes.push_back((diagram_node_win32*)l_Node);
	m_IdToNodes[l_Node->get_id()] = l_Node;

	InvalidateRect(m_hWnd, NULL, TRUE);

	return l_Node;
}

void diagram_impl_win32::wm_paint(HWND hWnd){
	math::size l_Size = get_size();
	diagram_colors &c = *m_Colors;

	PAINTSTRUCT l_Paint;
	HDC hDC = BeginPaint(hWnd, &l_Paint);

	HDC hDCCompat = CreateCompatibleDC(hDC);
	RECT rect;
	GetClientRect(hWnd, &rect);
	HBITMAP hbmScreen = CreateCompatibleBitmap(hDC, rect.right, rect.bottom);
    HBITMAP hbmOldBitmap = (HBITMAP)SelectObject(hDCCompat, hbmScreen);

	Graphics g(hDCCompat);

	// render background
	l_Paint.fErase = FALSE; // we're handling background here - don't send WM_ERASEBKGND

	PointF p1, p2;
	p1.X = 0;
	p1.Y = 0;
	p2.X = 0;
	p2.Y = (REAL)l_Size.m_Y;
	LinearGradientBrush lb(p1, p2, Color(255, 54, 56, 95), Color(255, 18, 21, 64));
	g.FillRectangle(&lb, 0, 0, l_Size.m_X, l_Size.m_Y);
	
	// set up drawing state
	g.SetSmoothingMode(SmoothingModeAntiAlias);
	g.ScaleTransform(m_ScaleX, m_ScaleY, Gdiplus::MatrixOrderAppend);
	g.TranslateTransform((Gdiplus::REAL)m_CenterX, (Gdiplus::REAL)m_CenterY);

	// Draw nodes
	math::point l_Project(l_Size.m_X / 2 + m_DeltaX,
						  l_Size.m_Y / 2 + m_DeltaY);

	// draw in reverse order so clicks register on the right node
	for(int i = m_Nodes.size() - 1; i >= 0; i--){
		math::point l_Position = m_Nodes[i]->get_position();

		// Project position to 'diagram space' (eg. apply panning)
		l_Position += l_Project;

		m_Nodes[i]->draw_node(
			g, 
			*m_Colors,
			l_Position.m_X, 
			l_Position.m_Y,
			m_CurrentSelection.find(m_Nodes[i]) != m_CurrentSelection.end());
	}

	for(size_t i = 0; i < m_Connections.size(); i++){
		diagram_node_win32 *l_NodeA = (diagram_node_win32*)m_IdToNodes[m_Connections[i].m_PortA >> 16];
		diagram_node_win32 *l_NodeB = (diagram_node_win32*)m_IdToNodes[m_Connections[i].m_PortB >> 16];

		bool selected = m_CurrentSelection.find(l_NodeA) != m_CurrentSelection.end() 
			|| m_CurrentSelection.find(l_NodeB) != m_CurrentSelection.end();

		diagram_port_win32 *l_Start = (diagram_port_win32 *)l_NodeA->get_port_by_id(m_Connections[i].m_PortA);
		diagram_port_win32 *l_End = (diagram_port_win32 *)l_NodeB->get_port_by_id(m_Connections[i].m_PortB);

		Point l_Points[] = {
			Point(
				(int)(l_Start->m_BulletCenter.X),
				(int)(l_Start->m_BulletCenter.Y)),
			Point(
				(int)(l_Start->m_BulletCenter.X + (l_Start->get_type() == gui::in ? -100 : 100)),
				(int)(l_Start->m_BulletCenter.Y)),
			Point(
				(int)(l_End->m_BulletCenter.X + (l_End->get_type() == gui::in ? -100 : 100)),
				(int)(l_End->m_BulletCenter.Y)),
			Point(
				(int)(l_End->m_BulletCenter.X),
				(int)(l_End->m_BulletCenter.Y)),
		};

		g.DrawBezier(selected ? &c.selectedBezierPen : &c.bezierPen, l_Points[0], l_Points[1], l_Points[2], l_Points[3]);
	}

	if(m_BeginPort){
		int x = m_MouseX - m_CenterX;
		int y = m_MouseY - m_CenterY;

		Point l_Points[] = {
			Point(
				(INT)(m_BeginPort->m_BulletCenter.X),
				(INT)(m_BeginPort->m_BulletCenter.Y)),
			Point(
				(INT)(m_BeginPort->m_BulletCenter.X + (m_BeginPort->get_type() == gui::in ? -100 : 100)),
				(INT)(m_BeginPort->m_BulletCenter.Y)),
			Point(
				(INT)(x + (x > m_BeginPort->m_BulletCenter.X ? -100 : 100)), 
				(INT)(y)),
			Point(x, y),
		};

		g.DrawBezier(&c.bezierPen, l_Points[0], l_Points[1], l_Points[2], l_Points[3]);
	} else if(!m_MovingSelection){
		RectF l_Selection = get_clamped_scaled_selection_rect();

		g.FillRectangle(&c.selectionBackground, l_Selection);
		g.DrawRectangle(&c.selectionBorder, l_Selection);
	}

	BitBlt(hDC, l_Paint.rcPaint.left, l_Paint.rcPaint.top, l_Paint.rcPaint.right, l_Paint.rcPaint.bottom, hDCCompat, 0, 0, SRCCOPY);

	SelectObject(hDCCompat, hbmOldBitmap);
	DeleteObject(hbmScreen);
	DeleteDC(hDCCompat);


	EndPaint(hWnd, &l_Paint);
}

size_t diagram_impl_win32::get_connection_count(){
	return m_Connections.size();
}

connection diagram_impl_win32::get_connection(size_t i){
	return m_Connections[i];
}

// It is guaranteed by this function that if *a_Port != 0 then *a_Node != 0
// however, it isn't guaranteed that if *a_Node != 0 then *a_Port != 0
void diagram_impl_win32::find_node_or_port(REAL x, REAL y, diagram_node_win32 **a_Node, diagram_port_win32 **a_Port){
	*a_Node = 0;
	*a_Port = 0;

	typedef port_rect_map_t::iterator itr;

	for(size_t i = 0; i < m_Nodes.size(); i++){
		if(m_Nodes[i]->m_WorldBounds.Contains(x, y)){
			*a_Node = m_Nodes[i];

			for(size_t j = 0; j < m_Nodes[i]->get_port_count(); j++)
			{
				diagram_port_win32 *l_Port = (diagram_port_win32*)m_Nodes[i]->get_port(j);
				if(l_Port->m_WorldBounds.Contains(x, y)){
					*a_Port = l_Port;
					return;
				}
			}

			return;
		}
	}
}

void diagram_impl_win32::destroy_node(diagram_node_base* a_Node){
	// remove all connections
	for (auto it = m_Connections.begin(); it != m_Connections.end(); ) {
		if (m_IdToNodes[it->m_PortA >> 16] == a_Node ||
			m_IdToNodes[it->m_PortB >> 16] == a_Node) {
			it = m_Connections.erase(it);
		} else {
			it++;
		}
	}

	// remove id mapping
	for (auto it = m_IdToNodes.begin(); it != m_IdToNodes.end(); ) {
		if (it->second == a_Node) {
			it = m_IdToNodes.erase(it);
		} else {
			it++;
		}
	}

	// remove node
	for (auto it = m_Nodes.begin(); it != m_Nodes.end();) {
		if (*it == a_Node) {
			it = m_Nodes.erase(it);
		} else {
			it++;
		}
	}

	// typically called from node dtor, so no need to delete the node
}

gui::diagram_connection_policy* gui_imp::diagram_impl_win32::get_connection_policy() {
	return m_ConnectionPolicy;
}

diagram_port_win32 * gui_imp::diagram_impl_win32::get_current_selected_port() {
	return m_BeginPort;
}

bool diagram_impl_win32::connect(diagram_id_t a_FullPortIdA, diagram_id_t a_FullPortIdB){
	diagram_node_base *l_NodeA = m_IdToNodes[a_FullPortIdA >> 16];
	diagram_node_base *l_NodeB = m_IdToNodes[a_FullPortIdB >> 16];

	if (!l_NodeA || !l_NodeB) {
		return false;
	}

	diagram_port_base* l_PortA = l_NodeA->get_port_by_id(a_FullPortIdA);
	diagram_port_base* l_PortB = l_NodeB->get_port_by_id(a_FullPortIdB);

	if (!l_PortA || !l_PortB) {
		return false;
	}

	if (!m_ConnectionPolicy->can_connect_ports(l_PortA, l_PortB))
		return false;

	m_Connections.push_back(connection(a_FullPortIdA, a_FullPortIdB));
	return true;
}

void diagram_impl_win32::select_node(diagram_node_base *a_Node){
	m_CurrentSelection.insert((diagram_node_win32*)a_Node);
	m_SelectionChanged = true;
}

void diagram_impl_win32::deselect_node(diagram_node_base *a_Node){
	// if we're not inside we might need to flag the node as
	// not being selected
	selection_t::iterator j = std::find(
		m_CurrentSelection.begin(), 
		m_CurrentSelection.end(), 
		(diagram_node_win32*)a_Node
	);

	if(j != m_CurrentSelection.end()){
		m_CurrentSelection.erase(j);
	}

	m_SelectionChanged = true;
}

void diagram_impl_win32::wm_mbuttonup(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	// reset and commit pan control
	m_CenterX += m_DeltaX;
	m_CenterY += m_DeltaY;
	m_DeltaX = 0;
	m_DeltaY = 0;
}

void diagram_impl_win32::wm_mbuttondown(WPARAM wParam, LPARAM lParam) {
	// set up panning variables
	math::size l_Size = get_size();

	m_PanStartX = int(GET_X_LPARAM(lParam) / m_ScaleX - l_Size.m_X / 2);
	m_PanStartY = int(GET_Y_LPARAM(lParam) / m_ScaleY - l_Size.m_Y / 2);
	m_DeltaX = 0;
	m_DeltaY = 0;
}

void diagram_impl_win32::wm_lbuttondown(WPARAM wParam, LPARAM lParam){
	math::size l_Size = get_size();
	m_LeftButtonWasDown = true;
	m_MovingSelection = false; // Deselect when we click

	REAL x = (REAL)GET_X_LPARAM(lParam) / m_ScaleX;
	REAL y = (REAL)GET_Y_LPARAM(lParam) / m_ScaleY;

	x -= m_CenterX;
	y -= m_CenterY;

	diagram_node_win32 *l_Node = 0;
	diagram_port_win32 *l_Port = 0;

	// test if we clicked a node
	if(!m_MovingSelection){
		find_node_or_port(x, y, &l_Node, &l_Port);

		if(!l_Port && m_BeginPort){
			// stop connecting points
			m_BeginPort = 0;
		}

		// don't select the port when multiple nodes are selected
		// so that we can easily grab the entire selection
		if(l_Port && m_CurrentSelection.size() <= 1){

			if(!m_BeginPort){
				// We need to draw a bezier to the mouse cursor
				m_BeginPort = l_Port;
			}else{
				// Commit the connection
				connect(m_BeginPort->get_id(), l_Port->get_id());

				m_BeginPort = 0;
			}
		}else if(l_Node){
			// we've selected a node, deselect everything else
			if(m_CurrentSelection.find(l_Node) == m_CurrentSelection.end()){
				deselect_all();
			}
			select_node(l_Node);
		}

		m_MovingSelection = l_Port || l_Node;
	}

	store_relative_positions_selected(x, y);

	if(!m_MovingSelection && !l_Port){
		m_Selection.X = (x + m_CenterX) * m_ScaleX;
		m_Selection.Y = (y + m_CenterY) * m_ScaleY;

		deselect_all();
	}
}

void diagram_impl_win32::wm_lbuttonup(HWND hWnd, WPARAM wParam, LPARAM lParam){
	// deselect everything
	m_MovingSelection = !m_MovingSelection;
	m_LeftButtonWasDown = false;

	REAL x = (REAL)GET_X_LPARAM(lParam) / m_ScaleX;
	REAL y = (REAL)GET_Y_LPARAM(lParam) / m_ScaleY;

	store_relative_positions_selected(x, y);

	if(m_SelectionChanged){
		selection_args l_Args;
		for(selection_t::iterator i = m_CurrentSelection.begin(); i != m_CurrentSelection.end(); i++){ 
			l_Args.m_SelectedNodes.push_back(*i);
		}
		((diagram*)get_widget())->m_OnSelectionChanged.trigger(&l_Args);
		m_SelectionChanged = false;
	}

	m_Selection.X = m_Selection.Y = m_Selection.Width = m_Selection.Height = 0;
	ReleaseCapture(); // corresponds to SetCapture
	InvalidateRect(hWnd, NULL, TRUE);
}

void diagram_impl_win32::wm_mousemove(HWND hWnd, WPARAM wParam, LPARAM lParam){
	math::size l_Size = get_size();

	m_MouseX = int(GET_X_LPARAM(lParam) / m_ScaleX);
	m_MouseY = int(GET_Y_LPARAM(lParam) / m_ScaleY);
	
	if(wParam & MK_MBUTTON){
		SetCapture(hWnd);

		// pan the window
		m_DeltaX = m_MouseX - l_Size.m_X / 2 - m_PanStartX;
		m_DeltaY = m_MouseY - l_Size.m_Y / 2 - m_PanStartY;
		InvalidateRect(hWnd, NULL, TRUE);
	}else if(wParam & MK_LBUTTON){
		// capture mouse so the selection rect can extend outside of the window
		SetCapture(hWnd);

		if(m_MovingSelection){
			if (!m_BeginPort) { // make sure we're not holding a port when moving the nodes
				for (selection_t::iterator i = m_CurrentSelection.begin(); i != m_CurrentSelection.end(); i++) {
					(*i)->move_to(
						(int)(*i)->m_DistanceFromMouse.X + m_MouseX - m_CenterX - l_Size.m_X / 2 - m_CenterX - m_DeltaX,
						(int)(*i)->m_DistanceFromMouse.Y + m_MouseY - m_CenterY - l_Size.m_Y / 2 - m_CenterY - m_DeltaY);
				}
			}
		}else if(m_LeftButtonWasDown){
			// extend selection rectangle
			m_Selection.Width  = GET_X_LPARAM(lParam) - m_Selection.X;
			m_Selection.Height = GET_Y_LPARAM(lParam) - m_Selection.Y;

			RectF l_Selection = get_clamped_scaled_selection_rect();

			for (size_t i = 0; i < m_Nodes.size(); i++) {
				RectF l_ProjectedRect = m_Nodes[i]->m_WorldBounds;

				// test the selection rectangle to see what's inside
				if (l_Selection.IntersectsWith(l_ProjectedRect)) {
					select_node(m_Nodes[i]);
				}
			}
		}

		// post WM_PAINT message
		InvalidateRect(hWnd, NULL, TRUE);
	}else if(m_BeginPort){
		// we've selected a port, the bezier curves are drawn in the wm_paint handler
		// so we only need to signal the invalidate
		InvalidateRect(hWnd, NULL, TRUE);
	}
}
void diagram_impl_win32::wm_mousewheel(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	float zDelta = (float)GET_WHEEL_DELTA_WPARAM(wParam);
	zDelta /= WHEEL_DELTA;

	m_ScaleX += zDelta * 0.1f;
	m_ScaleY += zDelta * 0.1f;

	if (m_ScaleX < 0.3f) m_ScaleX = 0.3f;
	if (m_ScaleY < 0.3f) m_ScaleY = 0.3f;

	if (m_ScaleX > 3.0f) m_ScaleX = 3.0f;
	if (m_ScaleY > 3.0f) m_ScaleY = 3.0f;

	InvalidateRect(hWnd, NULL, TRUE);
}

LRESULT diagram_impl_win32::process_message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ){
	math::size l_Size = get_size();

	if (uMsg == WM_ERASEBKGND) {
		return 1;
	}else if(uMsg == WM_PAINT){
		wm_paint(hWnd);
		return 0;
	}else if(uMsg == WM_MBUTTONDOWN){
		wm_mbuttondown(wParam, lParam);
		return 0;
	}else if(uMsg == WM_MBUTTONUP){
		wm_mbuttonup(hWnd, wParam, lParam);
		return 0;
	}else if(uMsg == WM_LBUTTONDOWN){
		wm_lbuttondown(wParam, lParam);
		return 0;
	}else if(uMsg == WM_LBUTTONUP){
		wm_lbuttonup(hWnd, wParam, lParam);
		return 0;
	}else if(uMsg == WM_MOUSEMOVE){
		wm_mousemove(hWnd, wParam, lParam);
		return 0;
	}else if(uMsg == WM_MOUSEWHEEL){
		wm_mousewheel(hWnd, wParam, lParam);
		return 0;
	}else{
		return widget_win32::process_message(hWnd, uMsg, wParam, lParam);
	}
}