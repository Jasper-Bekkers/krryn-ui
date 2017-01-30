#pragma once
#include "widget.h"
#include "diagram_impl_base.h"

// TODO: Create connection policy class to disalow cycles

namespace gui{
	enum port_type{
		in,
		out
	};

	class diagram_node_base;
	class diagram_factory;
	class diagram;

	class diagram_port_base{
	public:
		virtual ~diagram_port_base() { }
		virtual diagram_node_base *get_node() const = 0;

		// Always implemented as (get_node()->get_id() << 16) + (get_node_local_id() & 0xFFFF)
		// this means that the id returned by this function is unique for the diagram
		diagram_id_t get_id() const;

		// The id returned by this function is unique for the node this port is attached to
		virtual diagram_id_t get_node_local_id() const = 0;
		virtual std::string get_name() const = 0;
		virtual port_type get_type() const = 0;
	};

	class diagram_node_base{
	public:
		virtual ~diagram_node_base() { m_Parent->destroy_node(this); }
		diagram_node_base(diagram_impl_base *a_Parent) : m_Factory(0), m_UserData(0), m_Parent(a_Parent) {}

		virtual void init(diagram_factory *a_Factory) { m_Factory = a_Factory; }

		virtual std::string get_title() const = 0;
		virtual size_t get_port_count() = 0;
		virtual diagram_id_t get_id() const = 0;
		virtual diagram_port_base *get_port(size_t i) = 0;
		virtual diagram_port_base *get_port_by_id(diagram_id_t a_Id) const = 0;
		virtual diagram_port_base *create_port(port_type a_Type, const std::string &a_Name) = 0;
		virtual diagram_factory &get_factory() { return *m_Factory; }

		// graphical 
		virtual void set_image(view::pixel_t *a_Pixels, int a_Width, int a_Height) = 0;
		virtual view::pixel_t *get_image() const = 0;

		virtual void set_header_color(view::pixel_t a_Color) = 0;
		virtual view::pixel_t get_header_color() const = 0;
		virtual math::point get_position() const  = 0;
		virtual void move_to(int x, int y) = 0;

		void *m_UserData; 
	private:
		diagram_impl_base *m_Parent;
		diagram_factory *m_Factory;
	};

	class diagram_serializer{
	public:
		virtual void load(diagram *a_Diagram, const std::string &a_Filename) = 0;
		virtual void save(diagram *a_Diagram, const std::string &a_Filename) = 0;
	};
	
	class diagram_connection_policy{
	};

	class diagram_factory{
	public:
		virtual diagram_node_base* create_node(gui::diagram_impl_base *a_Parent, const std::string &a_Title, diagram_id_t a_Id) = 0;
		virtual diagram_port_base* create_port(port_type a_Type, const std::string &a_Name, diagram_node_base *a_Node, diagram_id_t a_Id) = 0;

		virtual diagram_connection_policy* create_connection_policy() = 0;
		virtual diagram_serializer* create_serializer() = 0;
	};
	
	class diagram_port : public diagram_port_base{
	public:
		diagram_port(port_type a_Type, const std::string &a_Name, diagram_node_base* a_Node, diagram_id_t a_Id) 
			: m_Type(a_Type), m_Name(a_Name), m_Id(a_Id), m_Node(a_Node) {}

		diagram_node_base *get_node() const{ return m_Node; }
		diagram_id_t get_node_local_id() const{ return m_Id; }
		std::string get_name() const{ return m_Name; }
		port_type get_type() const{ return m_Type; }
	private:
		port_type m_Type;
		std::string m_Name;
		diagram_id_t m_Id;
		diagram_factory *m_Factory;
		diagram_node_base *m_Node;
	};

	class diagram_node : public diagram_node_base{
	public:
		diagram_node(diagram_impl_base *a_Diagram, const std::string &a_Title, diagram_id_t a_Id);

		virtual std::string get_title() const;
		virtual size_t get_port_count();
		virtual diagram_id_t get_id() const;
		virtual diagram_port_base *get_port_by_id(diagram_id_t a_Id) const;
		virtual diagram_port_base *get_port(size_t i);
		virtual diagram_port_base *create_port(port_type a_Type, const std::string &a_Name);

		// graphical 
		
		/// Image is required to be 64x64, 
		virtual void set_image(view::pixel_t *a_Pixels, int a_Width, int a_Height);
		virtual view::pixel_t *get_image() const { return m_Image; }
		virtual void set_header_color(view::pixel_t a_Color);
		virtual view::pixel_t get_header_color() const;
		virtual void move_to(int x, int y);
		virtual math::point get_position() const;
	private:
		math::point m_Position;
		std::string m_Title;
		std::vector<diagram_port_base *> m_Ports;
		diagram_id_t m_PortIdCounter;
		diagram_id_t m_Id;

		view::pixel_t *m_Image;
		view::pixel_t m_HeaderColor;
	};

	struct selection_args{
		std::vector<diagram_node_base*> m_SelectedNodes;
	};

	class diagram : public widget{
	public:
		typedef diagram_impl_base impl;
		typedef diagram_initializer with;

		diagram(const with &a_Initialize);

		diagram_node_base *create_node(const std::string &a_Title, diagram_id_t a_Id = g_InvalidId);
		size_t get_node_count();
		diagram_node_base *get_node(size_t i);

		void connect(diagram_id_t a_FullPortIdA, diagram_id_t a_FullPortIdB);
		size_t get_connection_count();
		connection get_connection(size_t i);

		void set_center(math::point a_Center);
		math::point get_center() const;

		void load(const std::string &a_Filename);
		void save(const std::string &a_Filename);

		signal<selection_args*> m_OnSelectionChanged;

		virtual void *handle();
		virtual ~diagram() { m_Pimpl->destroy(); }
	private:
		impl *m_Pimpl;
	};

	class diagram_initializer : public widget_initializer<diagram_initializer>{
	public:
		diagram_initializer();

		diagram_initializer &factory(diagram_factory *a_Factory);
		diagram_factory* get_factory() const;

		diagram* create();
	private:
		diagram_factory *m_Factory;
	};
}
