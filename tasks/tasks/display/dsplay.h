#pragma once

#define __ANG_DISPLAY_H__

#include <angsys.h>
#include <Windows.h>

namespace ang
{
	namespace platform
	{
		template<typename T> struct point;
		template<typename T> struct size;
		template<typename T> struct rect;

		template<typename T> struct point
		{
			T x;
			T y;

		public:
			point();
			point(point const&);
			point(T x, T y);

			point& operator = (const point&);
			point& operator += (const point&);
			point& operator -= (const point&);
			point& operator *= (T);
			point& operator /= (T);
		};

		template<typename T> struct size
		{
			T width;
			T height;

		public:
			size();
			size(size const&);
			size(T w, T h);

			size& operator = (const size&);
			size& operator += (const size&);
			size& operator -= (const size&);
			size& operator *= (T);
			size& operator /= (T);
		};

		template<typename T> struct rect
		{
			T left;
			T top;
			T right;
			T bottom;

		public:
			rect();
			rect(rect const&);
			rect(T l, T t, T r, T b);
			rect(point<T> lt, point<T> rb);
			rect(point<T> lt, size<T> sz);

			T width()const;
			T height()const;
			point<T> left_top()const;
			point<T> right_top()const;
			point<T> left_bottom()const;
			point<T> right_bottom()const;

			void move_to(point<T>);
			void move_to(T x, T y);

			void resize(size<T>);
			void resize(T cx, T cy);

			void deflate(T l, T t, T r, T b);
			void inflate(T l, T t, T r, T b);

			rect<T>& operator = (const rect<T>&);
			rect<T>& operator += (const rect<T>&);
			rect<T>& operator -= (const rect<T>&);
			rect<T>& operator += (const point<T>&);
			rect<T>& operator -= (const point<T>&);
			rect<T>& operator += (const size<T>&);
			rect<T>& operator -= (const size<T>&);
			rect<T>& operator *= (T);
			rect<T>& operator /= (T);
		};

		template<typename T> point<T> operator + (const point<T>&, const point<T>&);
		template<typename T> point<T> operator - (const point<T>&, const point<T>&);
		template<typename T> point<T> operator * (const point<T>&, T);
		template<typename T> point<T> operator * (T, const point<T>&);
		template<typename T> point<T> operator / (const point<T>&, T);

		template<typename T> size<T> operator + (const size<T>&, const size<T>&);
		template<typename T> size<T> operator - (const size<T>&, const size<T>&);
		template<typename T> size<T> operator * (const size<T>&, T);
		template<typename T> size<T> operator * (T, const size<T>&);
		template<typename T> size<T> operator / (const size<T>&, T);

		template<typename T> rect<T> operator + (const point<T>&, const size<T>&);
		template<typename T> rect<T> operator + (const size<T>&, const point<T>&);

		template<typename T> rect<T> operator + (const rect<T>&, const rect<T>&);
		template<typename T> rect<T> operator - (const rect<T>&, const rect<T>&);
		template<typename T> rect<T> operator + (const rect<T>&, const point<T>&);
		template<typename T> rect<T> operator - (const rect<T>&, const point<T>&);
		template<typename T> rect<T> operator + (const point<T>&, const rect<T>&);
		template<typename T> rect<T> operator + (const rect<T>&, const size<T>&);
		template<typename T> rect<T> operator - (const rect<T>&, const size<T>&);
		template<typename T> rect<T> operator + (const size<T>&, const rect<T>&);
		template<typename T> rect<T> operator * (const rect<T>&, T);
		template<typename T> rect<T> operator * (T, const rect<T>&);
		template<typename T> rect<T> operator / (const rect<T>&, T);


		namespace windows
		{

			typedef object_wrapper<class window> window_t;
			typedef object_wrapper_ptr<class window> window_ptr_t;

			
			class window : public object
			{
			private:
				static LRESULT WINAPI WndProc(HWND, UINT, WPARAM, LPARAM);
				
				HWND handle;

			public:
				window();
				window(wstring title, wstring name = "default", rect<float> size = {100, 100, 800, 600}, HWND parent = null);


			public:
				point<float> position()const;
				void position(point<float>)const;

				size<float> layout()const;
				void layout(size<float>)const;

				HWND hwnd()const;
				HDC hdc()const;

			public:
				core::delegates::listener<void(window_t, objptr)> create_event;
				core::delegates::listener<void(window_t, objptr)> destroy_event;
				core::delegates::listener<void(window_t, objptr)> draw_event;

				core::delegates::listener<void(window_t, shared_ptr<point<float>>)> mouse_move_event;
				core::delegates::listener<void(window_t, shared_ptr<point<float>>)> left_click_down_event;
				core::delegates::listener<void(window_t, shared_ptr<point<float>>)> left_click_up_event;
				core::delegates::listener<void(window_t, shared_ptr<point<float>>)> right_click_down_event;
				core::delegates::listener<void(window_t, shared_ptr<point<float>>)> right_click_up_event;

				core::delegates::listener<void(objptr, shared_ptr<size<float>>)> size_change_event;

			public:
				virtual int run(wstring title = "default", wstring name = "default", rect<float> size = { 100, 100, 800, 600 }, HWND parent = null);
				virtual bool create(wstring title, wstring name = "default", rect<float> size = { 100, 100, 800, 600 }, HWND parent = null);
				virtual bool close();
			protected:		
				virtual bool destroy();
				virtual pointer wndproc(uint msg, pointer wprm, pointer lprm);

			protected:
				virtual~window();
			};


		}
	}
}

#include "display.inl"