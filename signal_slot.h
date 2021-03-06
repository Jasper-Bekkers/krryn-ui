#pragma once
#include <vector>
#include <functional>

namespace gui{
/*
template<typename _Argument>
class basic_observer{
public:
	virtual void call(_Argument a_Argument) =0;
};

template<typename _Argument>
class basic_observer_fun : public basic_observer<_Argument>{
public:
	typedef void (*function_t)(_Argument);

	basic_observer_fun(function_t a_Function) : m_Function(a_Function) {}

	virtual void call(_Argument a_Argument){
		m_Function(a_Argument);
	}
private:
	function_t m_Function;
};

template<typename _ClassPointer, typename _Argument>
class basic_observer_mem : public basic_observer<_Argument>{
public:
	typedef void (_ClassPointer::*member_t)(_Argument);
	basic_observer_mem(_ClassPointer *a_Object, member_t a_Function)
		: m_Object(a_Object), m_Function(a_Function) {}

	virtual void call(_Argument a_Argument){
		((*m_Object).*m_Function)(a_Argument);
	}
private:
	_ClassPointer *m_Object;
	member_t m_Function;
};
*/
/*
template<typename _ClassPointer, typename... _Argument>
basic_observer_mem<_ClassPointer, _Argument> slot(_ClassPointer *a_Class, void (_ClassPointer::*a_Function)(_Argument)){
	return basic_observer_mem<_ClassPointer, _Argument>(a_Class, a_Function);
}*/

template<typename... _Argument>
class signal{
public:
	//void operator +=(const basic_observer<_Argument> &a_Callback){
	//	m_Handlers.push_back(a_Callback);
	//}
	//
	//void operator +=(void (*a_Callback)(_Argument)){
	//	m_Handlers.push_back(basic_observer_fun<_Argument>(a_Callback));
	//}

	void operator +=(std::function<void(_Argument...)> const& a_Callback) {
		m_Handlers.push_back(a_Callback);
	}


	void trigger(_Argument... a_Arguments){
		for(auto handler : m_Handlers){
			handler(a_Arguments...);
		}
	}
private:
	std::vector<std::function<void(_Argument...)>> m_Handlers;
};

}