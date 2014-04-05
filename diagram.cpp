#include "diagram.h"
#include "widget_factory.h"

using namespace gui;
using namespace view;
using namespace math;

diagram_id_t diagram_port_base::get_id() const{
	return (get_node()->get_id() << 16) + (get_node_local_id() & 0xFFFF);
}

diagram_node::diagram_node(const std::string &a_Title, diagram_id_t a_Id) 
	: m_Title(a_Title), m_PortIdCounter(0), m_Id(a_Id), m_HeaderColor(0xD1DDF4), m_Image(0) {}

void diagram_node::move_to(int x, int y){
	m_Position.m_X = x;
	m_Position.m_Y = y;
}

point diagram_node::get_position() const {
	return m_Position;
}

std::string diagram_node::get_title() const {
	return m_Title;
}

diagram_port_base *diagram_node::get_port(size_t i) {
	return m_Ports[i];
}

diagram_id_t diagram_node::get_id() const{
	return m_Id;
}

// graphical 
void diagram_node::set_header_color(pixel_t a_Color){
	m_HeaderColor = a_Color;
}

pixel_t diagram_node::get_header_color() const{
	return m_HeaderColor;
}

void diagram_node::set_image(pixel_t *a_Pixels, int a_Width, int a_Height){
	if(a_Width != 64 || a_Height != 64) throw widget_exception("Invalid image dimensions");

	m_Image = a_Pixels;
}

diagram_port_base *diagram_node::create_port(port_type a_Type, const std::string &a_Name){
	m_PortIdCounter++;
	diagram_port_base *l_Port = get_factory().create_port(a_Type, a_Name, this, m_PortIdCounter);
	m_Ports.push_back(l_Port);
	return l_Port;
}

size_t diagram_node::get_port_count() {
	return m_Ports.size();
}

diagram_port_base *diagram_node::get_port_by_id(diagram_id_t a_Id) const{
	for(std::vector<diagram_port_base *>::const_iterator i = m_Ports.begin(); i != m_Ports.end(); i++){
		if((*i)->get_id() == a_Id){
			return *i;
		}
	}

	return 0;
}

diagram::diagram(const with &a_Initializer)
: widget(create_widget_base<impl>()){

	m_Pimpl = get_impl<impl>();
	m_Pimpl->make(a_Initializer);
	m_Pimpl->post_make(*this, a_Initializer.get_parent());
}

diagram_node_base *diagram::create_node(const std::string &a_Title, diagram_id_t a_Id){
	return m_Pimpl->create_node(a_Title, a_Id);
}

void diagram::connect(diagram_id_t a_FullPortIdA, diagram_id_t a_FullPortIdB){
	m_Pimpl->connect(a_FullPortIdA, a_FullPortIdB);
}

void diagram::load(const std::string &a_Filename){
	m_Pimpl->get_serializer()->load(this, a_Filename);
}

void diagram::save(const std::string &a_Filename){
	m_Pimpl->get_serializer()->save(this, a_Filename);
}

size_t diagram::get_node_count(){
	return m_Pimpl->get_node_count();
}

diagram_node_base *diagram::get_node(size_t i){
	return m_Pimpl->get_node(i);
}

size_t diagram::get_connection_count(){
	return m_Pimpl->get_connection_count();
}

connection diagram::get_connection(size_t i){
	return m_Pimpl->get_connection(i);
}

void diagram::set_center(point a_Center){
	m_Pimpl->set_center(a_Center);
}

point diagram::get_center() const{
	return m_Pimpl->get_center();
}

void *diagram::handle(){
	return m_Pimpl->handle();
}

diagram_initializer::diagram_initializer(){
	width(640);
	height(480);
	factory(0);
}

diagram_initializer &diagram_initializer::factory(diagram_factory *a_Factory){
	m_Factory = a_Factory;
	return *this;
}

diagram_factory* diagram_initializer::get_factory() const{
	return m_Factory;
}

diagram* diagram_initializer::create(){
	return new diagram(*this);
}
