
#include "../widget_factory.h"
#include "../window.h"
#include "../diagram.h"
#include "../event_loop.h"
#include "../textbox.h"
#include "../button.h"
#include "../opengl_context.h"
#include "../label.h"
#include "../tabs.h"
#include "../animation_diagram.h"
#include "../code_highlighter.h"
#include "../property_editor.h"


// tmp
#include <windows.h>

#pragma warning(push)
#pragma warning(disable:4458)
#include <gdiplus.h>
#pragma warning(pop)

#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/wglew.h>


// Dc.h & Dc.cpp can parse the property definition files (a la DDF)
// those will need to be turned into "property sheets" and we will need some kind of content storage files

using namespace gui;

window* m_Window;
diagram *m_Diagram;
event_loop m_EventLoop;
textbox *m_Textbox;
label *m_Label;
button *m_Button, *m_Button1;
opengl_context *m_OpenGL;
tabs *m_Tabs;
code_highlighter *m_Code;
diagram_node_base * j;
property_editor *m_Props;

view::pixel_t *heart, *ball;

view::pixel_t* read(wchar_t *filename){
	Gdiplus::Bitmap *bmp = Gdiplus::Bitmap::FromFile(filename);
	if (bmp->GetLastStatus() != Gdiplus::Ok) return 0;

	Gdiplus::Rect r;
	r.X = 0;
	r.Y = 0;
	r.Width = bmp->GetWidth();
	r.Height = bmp->GetHeight();

	Gdiplus::BitmapData data;
	bmp->LockBits(&r, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &data);

	return (view::pixel_t*)data.Scan0;
}

void resize(gui::resize_args *a)
{
	m_Diagram->set_size(a->m_Width - 400, a->m_Height / 2);
	m_Props->set_pos(a->m_Width - 400, 0);
	m_Props->set_size(400, a->m_Height);
	m_OpenGL->set_size(a->m_Width - 400, a->m_Height / 2);
	m_OpenGL->set_pos(0, a->m_Height / 2);
}

void selection(gui::selection_args *a)
{
	for(size_t i = 0; i < a->m_SelectedNodes.size(); i++){
		void *data = a->m_SelectedNodes[i]->m_UserData;
		if(data){
			std::string &l_Str = *(std::string*)data;
			std::cout << l_Str;
		}

		a->m_SelectedNodes[i]->set_image(heart, 64, 64);
	}
}

void click(gui::event_args *a)
{
	diagram_node_base *b = m_Diagram->create_node(m_Textbox->get_text() + "a");
	int c = rand() % 10;
	for(int i = 0; i < c; i++)
		b->create_port(rand() % 2 ? in : out, "Asdf");
	m_Textbox->set_text(m_Textbox->get_text() + "Hallo dit is text");
}

void change(gui::event_args *e)
{
	m_Label->set_text(m_Textbox->get_text());
}


void draw_to_node(gui::diagram_node_base* a_Node, int a_Width, int a_Height, view::pixel_t *a_Pixels, std::function<void()> a_Fn)
{
	GLuint texture, fbo;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, a_Width, a_Height, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) 
		__debugbreak();

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	a_Fn();

	glReadPixels(0, 0, a_Width, a_Height, GL_BGRA, GL_UNSIGNED_BYTE, a_Pixels);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &fbo);
	glDeleteTextures(1, &texture);

	a_Node->set_image(a_Pixels, a_Width, a_Height);
}

int main(int argc, char **argv)
{
	m_Window = window::with()
		.top(100)
		.left(100)
		.width(1000)
		.height(700)
	.create();

	m_Tabs = tabs::with()
		.top(0)
		.left(0)
		.parent(m_Window)
	.create();

	tab_panel *l_Panel = m_Tabs->named_panel("Test 1");
	tab_panel *l_Panel2 = m_Tabs->named_panel("Other");
	//tab_panel *l_Panel3 = m_Tabs->named_panel("Test propgrid");
//	tab_panel *l_Panel4 = m_Tabs->named_panel("Code");


	m_Diagram = diagram::with()
		.top(0)
		.width(l_Panel2->get_size().m_Width - 400)
		.height(l_Panel2->get_size().m_Height / 2)
		.parent(l_Panel2)
		.factory(new usr::diagram_factory_default())
	.create();


	m_OpenGL = opengl_context::with()
		.parent(l_Panel2)
		.fullscreen(false)
		.height(l_Panel2->get_size().m_Height / 2)
		.top(l_Panel2->get_size().m_Height / 2)
		.width(l_Panel2->get_size().m_Width - 400)
	.create();

	//m_Code = code_highlighter::with()
	//	.top(0)
	//	.width(800)
	//	.height(600)
	//	.parent(l_Panel4)
	//	//.text("Hallo allemaal")
	//.create();

	m_Props = property_editor::with()
		.top(0)
		.left(l_Panel2->get_size().m_Width - 400)
		.width(400)
		.height(l_Panel2->get_size().m_Height  - 30)
		.parent(l_Panel2)
	.create();

	m_Textbox = textbox::with()
		.top(0)
		.left(0)
		.parent(l_Panel)
	.create();

	m_Button = button::with()
		.top(25)
		.left(0)
		.parent(l_Panel)
		.text("Create")
	.create();

	m_Button1 = button::with()
		.top(25)
		.left(120)
		.parent(l_Panel)
		.text("Create 1")
	.create();

	m_Label = label::with()
		.top(0)
		.left(300)
		.parent(l_Panel)
		.text("Label")
	.create();


	m_Tabs->m_OnResize += resize;

	m_Button->m_OnClick += click;

	//m_Diagram->m_OnSelectionChanged += selection;

	m_Textbox->m_OnTextChange += change;

	m_Button->set_size(100, 100);

	j = m_Diagram->create_node("Name");
	j->move_to(-20, 0);

	heart = read(L"../../krryn-ui/Images/Heart.png");
	ball = read(L"../../krryn-ui/Images/Ball.png");

	j->set_image(ball, 64, 64);
	auto p = j->create_port(gui::out, "Temp");
	j->m_UserData = (void*)new std::string("User data");

	diagram_node_base * m = m_Diagram->create_node("Other");
	m->move_to(-100, 0);
	m->create_port(gui::in, "Waddup");

	diagram_node_base *w = m_Diagram->create_node("wat?");
	w->set_image(heart, 64, 64);
	w->move_to(0, 110);
	auto k = w->create_port(gui::in, "Test");

	diagram_node_base *h = m_Diagram->create_node("Hallo");
	h->move_to(110, 110);
	h->create_port(gui::out, "QQQQ");
	h->create_port(gui::in, "Mul");

	m_Diagram->connect(k->get_id(), p->get_id());

	delete w; w = nullptr;

	glewExperimental = GL_TRUE;

	GLenum glew_err = glewInit();
	if (glew_err != GLEW_OK)
	{
		auto errorStr = glewGetErrorString(glew_err);
		printf("%s", errorStr);
	}

	////////////////

	view::pixel_t *tmp = new view::pixel_t[64 * 64];

	float r = 0, g = 0, b = 0;
	while(true)
	{
		m_EventLoop.update();
		
		draw_to_node(h, 64, 64, tmp, [&]() {
			glClearColor(0, 0, 0, 0);
			glClear(GL_COLOR_BUFFER_BIT);
			glViewport(0, 0, 64, 64);

			glRotatef(0.01f * 2 * 3.14f, 0, 0, 1);
			glBegin(GL_TRIANGLES);
			glColor3f(r, g, b);
			glVertex2f(-0.7f, -0.5f);
			glVertex2f(0.7f, -0.5f);
			glVertex2f(0, 0.7f);
			glEnd();
		});
		if (r < 1) r += 0.001f;
		else if (g < 1) g += 0.001f;
		else if (b < 1) b += 0.001f;
		else r = b = g = 0;
		//angle += 0.0001;

		glClearColor(1, 0, 1, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		m_OpenGL->swap_buffers();
	}
}