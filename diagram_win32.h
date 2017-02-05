#ifdef _WIN32
#pragma once

#include "diagram.h"
#include "diagram_impl_base.h"
#include "widget_win32.h"
#include <set>
#include <map>

namespace gui_imp{
	struct diagram_colors;
	class diagram_port_win32 : public gui::diagram_port_base{
	public:
		diagram_port_win32(gui::diagram_port_base *a_Port) : m_Port(a_Port) {}

		virtual gui::diagram_node_base *get_node() const { return m_Port->get_node(); }
		virtual gui::diagram_id_t get_node_local_id() const { return m_Port->get_node_local_id(); }
		virtual std::string get_name() const { return m_Port->get_name(); }
		virtual gui::port_type get_type() const { return m_Port->get_type(); }

		Gdiplus::PointF m_BulletCenter;
		Gdiplus::RectF m_WorldBounds;
	private:
		gui::diagram_port_base *m_Port;
	};

	typedef std::map<diagram_port_win32*, Gdiplus::RectF> port_rect_map_t;

	class diagram_impl_win32;
	class diagram_node_win32 : public gui::diagram_node_base{
	public:
		diagram_node_win32(gui::diagram_impl_base *a_Parent, gui::diagram_node_base *a_Node);
		void draw_node(Gdiplus::Graphics &g, diagram_colors &c, int x, int y, bool border = false);
		std::pair<int, int> get_max_width_and_total_height(Gdiplus::Graphics &g, Gdiplus::Font &a_Font, Gdiplus::RectF &a_Layout, gui::port_type a_Type);

		virtual void move_to(int x, int y){ m_Node->move_to(x, y); }
		virtual math::point get_position() const { return m_Node->get_position(); }
		virtual std::string get_title() const { return m_Node->get_title(); }
		virtual gui::diagram_port_base* get_port(size_t i) { return m_Node->get_port(i); }
		virtual size_t get_port_count() { return m_Node->get_port_count(); }
		virtual void init(gui::diagram_factory *a_Factory) { m_Node->init(a_Factory); }
		virtual gui::diagram_factory &get_factory() { return m_Node->get_factory(); }
		virtual gui::diagram_port_base* create_port(gui::port_type a_Type, const std::string &a_Name){ return m_Node->create_port(a_Type, a_Name); }
		virtual gui::diagram_id_t get_id() const { return m_Node->get_id(); }
		virtual gui::diagram_port_base *get_port_by_id(gui::diagram_id_t a_Id) const { return m_Node->get_port_by_id(a_Id); }
		virtual void set_header_color(view::pixel_t a_Color) { m_Node->set_header_color(a_Color); }
		virtual view::pixel_t get_header_color() const { return m_Node->get_header_color(); }

		virtual void set_image(view::pixel_t *a_Pixel, int a_Width, int a_Height);
		virtual view::pixel_t* get_image() const;

		Gdiplus::RectF m_WorldBounds;
		Gdiplus::PointF m_DistanceFromMouse;
	private:
		gui::diagram_node_base *m_Node;
		diagram_impl_win32 *m_Win32Parent;
	};

	class diagram_factory_win32 : public gui::diagram_factory{
	public:
		diagram_factory_win32(gui::diagram_factory *a_Factory) : m_Factory(a_Factory) {}

		virtual gui::diagram_node_base* create_node(gui::diagram_impl_base *a_Diagram, const std::string &a_Title, gui::diagram_id_t a_Id){
			return new diagram_node_win32(a_Diagram, m_Factory->create_node(a_Diagram, a_Title, a_Id));
		}

		virtual gui::diagram_port_base* create_port(gui::port_type a_Type, const std::string &a_Name, gui::diagram_node_base *a_Node, gui::diagram_id_t a_Id){
			return new diagram_port_win32(m_Factory->create_port(a_Type, a_Name, a_Node, a_Id));
		}

		virtual gui::diagram_connection_policy* create_connection_policy(){
			return m_Factory->create_connection_policy();
		}

		virtual gui::diagram_serializer* create_serializer(){
			return m_Factory->create_serializer();
		}
	private:
		gui::diagram_factory *m_Factory;
	};


	class diagram_impl_win32 : public virtual widget_win32, public virtual gui::diagram_impl_base{
	public:
		virtual void make(const gui::diagram_initializer &a_Initializer);
		virtual gui::diagram_node_base *create_node(const std::string &a_Title, gui::diagram_id_t a_Id = gui::g_InvalidId);
		virtual LRESULT process_message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual bool connect(gui::diagram_id_t a_FullPortIdA, gui::diagram_id_t a_FullPortIdB);
		virtual gui::diagram_serializer *get_serializer();

		virtual size_t get_node_count();
		virtual gui::diagram_node_base *get_node(size_t i);

		virtual size_t get_connection_count();
		virtual gui::connection get_connection(size_t i);

		virtual void set_center(math::point a_Center) { m_CenterX = a_Center.m_X; m_CenterY = a_Center.m_Y; }
		virtual math::point get_center() const { return math::point(m_CenterX, m_CenterY); }
		virtual void destroy_node(gui::diagram_node_base* a_Node);
	private:
		HDC m_Hdc;
		bool m_MovingSelection;
		bool m_LeftButtonWasDown;

		gui::diagram_serializer *m_Serializer;
		diagram_port_win32 *m_BeginPort;
		int m_MouseX, m_MouseY;

		typedef std::set<diagram_node_win32*> selection_t;
		selection_t m_CurrentSelection;
		Gdiplus::RectF m_Selection;

		diagram_colors *m_Colors;

		int m_CenterX, m_CenterY;
		int m_DeltaX, m_DeltaY;
		int m_PanStartX, m_PanStartY;

		bool m_SelectionChanged;

		Gdiplus::PointF m_SelectionCenter;

		std::vector<gui::connection> m_Connections;
		std::vector<diagram_node_win32*> m_Nodes;
		std::map<gui::diagram_id_t, gui::diagram_node_base*> m_IdToNodes;
		gui::diagram_id_t m_NodeIdCounter;
		gui::diagram_factory *m_Factory;

		Gdiplus::RectF get_clamped_selection_rect();

		void deselect_all();
		void select_node(gui::diagram_node_base *a_Node);
		void deselect_node(gui::diagram_node_base *a_Node);

		void store_relative_positions_selected(int x, int y);
		void find_node_or_port(Gdiplus::REAL x, Gdiplus::REAL y, diagram_node_win32 **a_Node, diagram_port_win32 **a_Port);

		void wm_paint(HWND hWnd);
		void wm_mbuttondown(WPARAM wParam, LPARAM lParam);
		void wm_mbuttonup(HWND hWnd, WPARAM wParam, LPARAM lParam);
		void wm_lbuttondown(WPARAM wParam, LPARAM lParam);
		void wm_lbuttonup(HWND hWnd, WPARAM wParam, LPARAM lParam);
		void wm_mousemove(HWND hWnd, WPARAM wParam, LPARAM lParam);
	};
}

#endif