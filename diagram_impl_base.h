#pragma once
#include "widget_impl_base.h"
#include <string>

namespace gui{
	class diagram_initializer;
	class diagram_node_base;
	class diagram_serializer;

	typedef unsigned long long diagram_id_t;
	static const diagram_id_t g_InvalidId = -1;

	struct connection{
		diagram_id_t m_PortA;
		diagram_id_t m_PortB;

		connection()
			: m_PortA(0), m_PortB(0) {}

		connection(diagram_id_t a_PortA, diagram_id_t a_PortB)
			: m_PortA(a_PortA), m_PortB(a_PortB) {}

		connection(const connection &a_Connection)
			: m_PortA(a_Connection.m_PortA), m_PortB(a_Connection.m_PortB) {}
	};

	class diagram_impl_base : public virtual widget_impl_base{
	public:
		virtual void make(const diagram_initializer &a_Initializer) = 0;
		virtual diagram_node_base* create_node(const std::string &a_Title, diagram_id_t a_Id = g_InvalidId) = 0;
		virtual bool connect(diagram_id_t a_FullPortIdA, diagram_id_t a_FullPortIdB) = 0;
		virtual diagram_serializer* get_serializer() = 0;
		virtual size_t get_node_count() = 0;
		virtual diagram_node_base *get_node(size_t i) = 0;
		virtual size_t get_connection_count() = 0;
		virtual connection get_connection(size_t i) = 0;
		virtual void set_center(math::point a_Center) = 0;
		virtual math::point get_center() const = 0;
	};
}