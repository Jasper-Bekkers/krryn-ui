#pragma once
#include "diagram.h"
#include "string_util.h"
#include "lexical_cast.h"
#include "widget.h"
#include <fstream>
#include <iostream>

namespace usr{

	class node_animation : public gui::diagram_node{
	public:
		node_animation(gui::diagram_impl_base *a_Parent, const std::string &a_Title, gui::diagram_id_t a_Id) : diagram_node(a_Parent, a_Title, a_Id) {}

		virtual void init(gui::diagram_factory *a_Factory){
			diagram_node::init(a_Factory);

			create_port(gui::out, "Output");
		}
	};

	class node_blend : public gui::diagram_node{
	public:
		node_blend(gui::diagram_impl_base *a_Parent, gui::diagram_id_t a_Id) : diagram_node(a_Parent, "Blend", a_Id) {}

		virtual void init(gui::diagram_factory *a_Factory){
			diagram_node::init(a_Factory);

			create_port(gui::in, "Animation 1");
			create_port(gui::in, "Animation 2");
			create_port(gui::out, "Output");
		}
	};

	class node_output : public gui::diagram_node{
	public:
		node_output(gui::diagram_impl_base *a_Parent, gui::diagram_id_t a_Id) : diagram_node(a_Parent, "Output", a_Id) {}

		virtual void init(gui::diagram_factory *a_Factory){
			diagram_node::init(a_Factory);

			set_header_color(0xAF6464);
			create_port(gui::in, "Result");
		}
	};

	class diagram_serializer_txt : public gui::diagram_serializer{
	public:
		virtual void load(gui::diagram *a_Diagram, const std::string &a_Filename){
			std::ifstream l_In(a_Filename.c_str());

			if(!l_In) throw std::runtime_error("Error opening file " + a_Filename);

			std::string l_Line;

			std::getline(l_In, l_Line);

			if(l_Line != "Version 1"){
				throw std::runtime_error("Invalid file version");
			}

			std::getline(l_In, l_Line);

			std::vector<std::string> l_CenterString = core::explode(l_Line, "\t");
			if(l_CenterString.size() != 2){
				throw std::runtime_error("Parse error invalid center");
			}

			a_Diagram->set_center(math::point(core::lexical_cast<int>(l_CenterString[0]), core::lexical_cast<int>(l_CenterString[1])));

			std::getline(l_In, l_Line);

			if(l_Line != "#") std::cout << "Parse error, expected #" << std::endl;

			while(std::getline(l_In, l_Line)){
				if(l_Line != "#"){
					parse_node(a_Diagram, l_Line);
				}else break;
			}

			while(std::getline(l_In, l_Line)){
				if(l_Line != "#"){
					parse_connection(a_Diagram, l_Line);
				}
			}
		}

		virtual void save(gui::diagram *a_Diagram, const std::string &a_Filename){
			std::ofstream l_Out(a_Filename.c_str());

			if(!l_Out){
				throw std::runtime_error("Cannot open file " + a_Filename);
			}

			l_Out << "Version 1\n";
			l_Out << a_Diagram->get_center().m_X << "\t" << a_Diagram->get_center().m_Y << "\n";
			l_Out << "#\n";

			for(size_t i = 0; i < a_Diagram->get_node_count(); i++){
				save_node(l_Out, a_Diagram->get_node(i));
			}

			l_Out << "#\n";

			for(size_t i = 0; i < a_Diagram->get_connection_count(); i++){
				save_connection(l_Out, a_Diagram->get_connection(i));
			}

			l_Out.close();
		}

	private:
		void parse_node(gui::diagram *a_Diagram, const std::string &a_Line){
			std::vector<std::string> l_NodeLine = core::explode(a_Line, "\t");
			if(l_NodeLine.size() != 4){
				std::cout << "Error: invalid node" << std::endl;
			}

			gui::diagram_id_t l_Id = core::lexical_cast<gui::diagram_id_t>(l_NodeLine[0]);
			int l_X = core::lexical_cast<int>(l_NodeLine[1]);
			int l_Y = core::lexical_cast<int>(l_NodeLine[2]);
			std::string l_Name = l_NodeLine[3];

			gui::diagram_node_base *l_Node = a_Diagram->create_node(l_Name, l_Id);
			l_Node->move_to(l_X, l_Y);
		}

		void parse_connection(gui::diagram *a_Diagram, const std::string &a_Line){
			std::vector<std::string> l_ConnectionLine = core::explode(a_Line, "\t");

			if(l_ConnectionLine.size() != 2){
				std::cout << "Error: invalid connection" << std::endl;
			}

			gui::diagram_id_t l_PortA = core::lexical_cast<gui::diagram_id_t>(l_ConnectionLine[0]);
			gui::diagram_id_t l_PortB = core::lexical_cast<gui::diagram_id_t>(l_ConnectionLine[1]);

			a_Diagram->connect(l_PortA, l_PortB);
		}

		void save_node(std::ostream &a_Out, gui::diagram_node_base *a_Node){
			a_Out 
				<< a_Node->get_id() << "\t"
				<< a_Node->get_position().m_X << "\t"
				<< a_Node->get_position().m_Y<< "\t"
				<< a_Node->get_title() << "\n";
		}

		void save_connection(std::ostream &a_Out, gui::connection &a_Connection){
			a_Out
				<< a_Connection.m_PortA << "\t"
				<< a_Connection.m_PortB << "\n";

		}
	};

	class diagram_factory_default : public gui::diagram_factory{
	public:
		virtual gui::diagram_node* create_node(gui::diagram_impl_base *a_Parent, const std::string &a_Title, gui::diagram_id_t a_Id){
			std::string l_Title = core::to_lower(a_Title);

			if(core::starts_with(l_Title, "animation"))	return new node_animation(a_Parent, a_Title, a_Id);
			if(l_Title == "blend")					return new node_blend(a_Parent, a_Id);
			if(l_Title == "output")					return new node_output(a_Parent, a_Id);

			return new gui::diagram_node(a_Parent, a_Title, a_Id);
		}

		virtual gui::diagram_port* create_port(gui::port_type a_Type, const std::string &a_Name, gui::diagram_node_base *a_Node, gui::diagram_id_t a_Id){
			return new gui::diagram_port(a_Type, a_Name, a_Node, a_Id);
		}

		virtual gui::diagram_connection_policy* create_connection_policy(){
			return new gui::diagram_connection_policy();
		}

		virtual gui::diagram_serializer* create_serializer(){
			return new diagram_serializer_txt();
		}
	};

}
