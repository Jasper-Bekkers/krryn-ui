#pragma once
#include <stdexcept>
#include <list>
#include "signal_slot.h"

namespace math
{
	struct point
	{
		int m_X;
		int m_Y;

		point() : m_X(0), m_Y(0) {}
		point(int a_X, int a_Y) : m_X(a_X), m_Y(a_Y) {}

		point operator+=(const point &a) { m_X += a.m_X; m_Y += a.m_Y; return *this; }
	};

	struct size
	{
		union
		{
			struct
			{
				int m_Width;
				int m_Height;
			};

			struct
			{
				int m_X;
				int m_Y;
			};
		};

		size() : m_Width(0), m_Height(0) {}
		size(int a_Width, int a_Height) : m_Width(a_Width), m_Height(a_Height) {}
	};

	template<typename T>
	inline T abs(const T &a) { return a > 0 ? a : -a; }

	//inline point operator-(const point &a, const point &b) { return point(a.m_X - b.m_X, a.m_Y - b.m_Y); }
}
namespace gui{
	inline std::string to_string(unsigned int a)
	{
		char c[100];
		sprintf_s(c, 100, "%u", a);
		return std::string(c);
	}
}

namespace view
{
	typedef unsigned int pixel_t;
}

namespace gui{
	class widget_exception : public std::runtime_error{
	public:
		widget_exception(const std::string &a_Error) : runtime_error(a_Error) {}
		virtual ~widget_exception() throw() {}
	};

	struct event_args{
	public:	
	};

	struct resize_args{
	public:
		int m_Width;
		int m_Height;
	};

	class widget_impl_base;

	class widget{
	public:
		typedef widget_impl_base impl;

		void add_child(widget& a_Widget);
		widget& get_child(size_t i);
		size_t count() const;

		void set_size(int a_Width, int a_Height);
		void set_pos(int a_X, int a_Y);
		void set_visible(bool a_IsVisible);

		math::point get_pos();
		math::size get_size();
		bool get_visible();

		virtual ~widget();
		void *handle();

		signal<resize_args*> m_OnResize;

	protected:
		widget(impl *a_Impl);

		template<typename _Type>
		_Type* get_impl() { return dynamic_cast<_Type*>(m_Pimpl); }

	private:
		impl *m_Pimpl;
	};

	template<typename _Super>
	class widget_initializer{
	public:
		widget_initializer()
			: m_Top(0), m_Left(0), m_Width(100), m_Height(100), m_Parent(0), m_Visible(true) {}

		~widget_initializer() {}

		_Super &top(unsigned int a_Top){
			m_Top = a_Top;
			return static_cast<_Super&>(*this);
		}

		_Super &left(unsigned int a_Left){
			m_Left = a_Left;
			return static_cast<_Super&>(*this);
		}

		_Super &width(unsigned int a_Width){
			m_Width = a_Width;
			return static_cast<_Super&>(*this);
		}

		_Super &height(unsigned int a_Height){
			m_Height = a_Height;
			return static_cast<_Super&>(*this);
		}

		_Super &parent(widget* a_Widget){
			m_Parent = a_Widget;
			return static_cast<_Super&>(*this);
		}

		_Super &visible(bool a_Visible){
			m_Visible = a_Visible;
			return static_cast<_Super&>(*this);
		}

		widget* get_parent() const { return m_Parent; }

		bool get_visible() const { return m_Visible; }
		unsigned int get_top() const { return m_Top; }
		unsigned int get_left() const { return m_Left; }
		unsigned int get_height() const { return m_Height; }
		unsigned int get_width() const { return m_Width; }

	private:
		unsigned int m_Top, m_Left, m_Width, m_Height;
		bool m_Visible;
		widget* m_Parent;
	};
}
