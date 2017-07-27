/*********************************************************************************************************************/
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*********************************************************************************************************************/

#ifndef __ANG_FOUNDATION_H__
#define __ANG_FOUNDATION_H__

#include <angsys.h>

#ifdef LINK
#undef LINK
#endif//LINK


#if defined WINDOWS_PLATFORM
#if defined ANGSYS_DYNAMIC_LIBRARY

#ifdef ANGSYS_EXPORTS
#define LINK __declspec(dllexport)
#else
#define LINK __declspec(dllimport)
#endif//ANGSYS_EXPORTS
#else//#elif defined ANGSYS_STATIC_LIBRARY
#define LINK
#endif//ANGSYS_DYNAMIC_LIBRARY
#elif defined LINUX_PLATFORM || defined ANDROID_PLATFORM
#define LINK
#endif

namespace ang
{
	namespace foundation
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

#ifndef ANDROID_PLATFORM

		template<> struct LINK point<float>
		{
			float x;
			float y;

		public:
			point();
			point(point<float> const&);
			point(float x, float y);

			point<float>& operator = (const point<float>&);
			point<float>& operator += (const point<float>&);
			point<float>& operator -= (const point<float>&);
			point<float>& operator *= (float);
			point<float>& operator /= (float);
		};

		template<> struct LINK size<float>
		{
			float width;
			float height;

		public:
			size();
			size(size<float> const&);
			size(float w, float h);

			size<float>& operator = (const size<float>&);
			size<float>& operator += (const size<float>&);
			size<float>& operator -= (const size<float>&);
			size<float>& operator *= (float);
			size<float>& operator /= (float);
		};

		template<> struct LINK rect<float>
		{
			float left;
			float top;
			float right;
			float bottom;

		public:
			rect();
			rect(rect const&);
			rect(float l, float t, float r, float b);
			rect(point<float> lt, point<float> rb);
			rect(point<float> lt, size<float> sz);

			float width()const;
			float height()const;
			point<float> left_top()const;
			point<float> right_top()const;
			point<float> left_bottom()const;
			point<float> right_bottom()const;

			void move_to(point<float>);
			void move_to(float x, float y);

			void resize(size<float>);
			void resize(float cx, float cy);

			void deflate(float l, float t, float r, float b);
			void inflate(float l, float t, float r, float b);

			rect& operator = (const rect&);
			rect& operator += (const rect&);
			rect& operator -= (const rect&);
			rect& operator += (const point<float>&);
			rect& operator -= (const point<float>&);
			rect& operator += (const size<float>&);
			rect& operator -= (const size<float>&);
			rect& operator *= (float);
			rect& operator /= (float);
		};
#endif
	}

	namespace graphics
	{
		using namespace foundation;

		typedef struct color color_t;

		ANG_BEGIN_ENUM(LINK, graph_driver_type, byte)
			null = 0,
			OpenGL,
			OpenGLES,
			DirectX,
			DirectX11 = DirectX,
			Vulkan,//todo
			DirectX12,//todo
		ANG_END_ENUM(graph_driver_type);

		ANG_BEGIN_ENUM(LINK, primitive, byte)
			point,
			line,
			triangle,
			squere,
			poligon
		ANG_END_ENUM(primitive);

		struct LINK color
		{
			union
			{
				dword code;
				struct
				{
					byte red;
					byte green;
					byte blue;
					byte alpha;
				}components;
			};

			struct colors
			{
				enum enum_t : uint
				{
					null = 0X00000000,
					alice_blue = 0XFFFFF8F0,
					antique_white = 0XFFD7EBFA,
					aqua = 0XFFFFFF00,
					aquamarine = 0XFFD4FF7F,
					azure = 0XFFFFFFF0,
					beige = 0XFFDCF5F5,
					bisque = 0XFFC4E4FF,
					black = 0XFF000000,
					blanched_almond = 0XFFCDEBFF,
					blue = 0XFFFF0000,
					blue_violet = 0XFFE22B8A,
					brown = 0XFF2A2AA5,
					burly_wood = 0XFF87B8DE,
					cadet_blue = 0XFFA09E5F,
					chartreuse = 0XFF00FF7F,
					chocolate = 0XFF1E69D2,
					coral = 0XFF507FFF,
					cornflower_blue = 0XFFED9564,
					cornsilk = 0XFFDCF8FF,
					crimson = 0XFF3C14DC,
					cyan = 0XFFFFFF00,
					dark_blue = 0XFF8B0000,
					dark_cyan = 0XFF8B8B00,
					dark_goldenrod = 0XFF0B86B8,
					dark_gray = 0XFFA9A9A9,
					dark_green = 0XFF006400,
					dark_khaki = 0XFF6BB7BD,
					dark_magenta = 0XFF8B008B,
					dark_olive_green = 0XFF2F6B55,
					dark_orange = 0XFF008CFF,
					dark_orchid = 0XFFCC3299,
					dark_red = 0XFF00008B,
					dark_salmon = 0XFF7A96E9,
					dark_sea_green = 0XFF8FBC8F,
					dark_slate_blue = 0XFF8B3D48,
					dark_slate_gray = 0XFF4F4F2F,
					dark_turquoise = 0XFFD1CE00,
					dark_violet = 0XFFD30094,
					deep_pink = 0XFF9314FF,
					deep_sky_blue = 0XFFFFBF00,
					dim_gray = 0XFF696969,
					dodger_blue = 0XFFFF901E,
					firebrick = 0XFF2222B2,
					floral_white = 0XFFF0FAFF,
					forest_green = 0XFF228B22,
					fuchsia = 0XFFFF00FF,
					gainsboro = 0XFFDCDCDC,
					ghost_white = 0XFFFFF8F8,
					gold = 0XFF00D7FF,
					goldenrod = 0XFF20A5DA,
					gray = 0XFF808080,
					green = 0XFF008000,
					green_yellow = 0XFF2FFFAD,
					honeydew = 0XFFF0FFF0,
					hot_pink = 0XFFB469FF,
					indian_red = 0XFF5C5CCD,
					indigo = 0XFF82004B,
					ivory = 0XFFF0FFFF,
					khaki = 0XFF8CE6F0,
					lavender = 0XFFFAE6E6,
					lavender_blush = 0XFFF5F0FF,
					lawn_green = 0XFF00FC7C,
					lemon_chiffon = 0XFFCDFAFF,
					light_blue = 0XFFE6D8AD,
					light_coral = 0XFF8080F0,
					light_cyan = 0XFFFFFFE0,
					light_goldenrod_yellow = 0XFFD2FAFA,
					light_gray = 0XFFD3D3D3,
					light_green = 0XFF90EE90,
					light_pink = 0XFFC1B6FF,
					light_salmon = 0XFF7AA0FF,
					light_sea_green = 0XFFAAB220,
					light_sky_blue = 0XFFFACE87,
					light_slate_gray = 0XFF998877,
					light_steel_blue = 0XFFDEC4B0,
					light_yellow = 0XFFE0FFFF,
					lime = 0XFF00FF00,
					lime_green = 0XFF32CD32,
					linen = 0XFFE6F0FA,
					magenta = 0XFFFF00FF,
					maroon = 0XFF000080,
					medium_aquamarine = 0XFFAACD66,
					medium_blue = 0XFFCD0000,
					medium_orchid = 0XFFD355BA,
					medium_purple = 0XFFDB7093,
					medium_sea_green = 0XFF71B33C,
					medium_slate_blue = 0XFFEE687B,
					medium_spring_green = 0XFF9AFA00,
					medium_turquoise = 0XFFCCD148,
					medium_violet_red = 0XFF8515C7,
					midnight_blue = 0XFF701919,
					mint_cream = 0XFFFAFFF5,
					misty_rose = 0XFFE1E4FF,
					moccasin = 0XFFB5E4FF,
					navajo_white = 0XFFADDEFF,
					navy = 0XFF800000,
					old_lace = 0XFFE6F5FD,
					olive = 0XFF008080,
					olive_drab = 0XFF238E6B,
					orange = 0XFF00A5FF,
					orange_red = 0XFF0045FF,
					orchid = 0XFFD670DA,
					pale_goldenrod = 0XFFAAE8EE,
					pale_green = 0XFF98FB98,
					pale_turquoise = 0XFFEEEEAF,
					pale_violet_red = 0XFF9370DB,
					papaya_whip = 0XFFD5EFFF,
					peach_puff = 0XFFB9DAFF,
					peru = 0XFF3F85CD,
					pink = 0XFFCBC0FF,
					plum = 0XFFDDA0DD,
					powder_blue = 0XFFE6E0B0,
					purple = 0XFF800080,
					red = 0XFF0000FF,
					rosy_brown = 0XFF8F8FBC,
					royal_blue = 0XFFE16941,
					saddle_brown = 0XFF13458B,
					salmon = 0XFF7280FA,
					sandy_brown = 0XFF60A4F4,
					sea_green = 0XFF578B2E,
					sea_shell = 0XFFEEF5FF,
					sienna = 0XFF2D52A0,
					silver = 0XFFC0C0C0,
					sky_blue = 0XFFEBCE87,
					slate_blue = 0XFFCD5A6A,
					slate_gray = 0XFF908070,
					snow = 0XFFFAFAFF,
					spring_green = 0XFF7FFF00,
					steel_blue = 0XFFB48246,
					tan = 0XFF8CB4D2,
					teal = 0XFF808000,
					thistle = 0XFFD8BFD8,
					tomato = 0XFF4763FF,
					transparent = 0X00FFFFFF,
					turquoise = 0XFFD0E040,
					violet = 0XFFEE82EE,
					wheat = 0XFFB3DEF5,
					white = 0XFFFFFFFF,
					white_smoke = 0XFFF5F5F5,
					yellow = 0XFF00FFFF,
				};
			};
		
		public:
			typedef colors::enum_t enum_t;

		public:
			color();
			color(const color&);
			color(enum_t rgba);
			color(dword rgba);
			color(enum_t rgb, byte a);
			color(dword rgb, byte a);
			color(byte r, byte g, byte b, byte a);
			virtual~color();

			void parse(cstr_t);
			void parse(cwstr_t);

		public: //Override
			string to_string()const;

		public: //Operators

			color& operator = (const color&);
			color& operator = (enum_t);
			color& operator = (dword);

			bool operator == (const color& other)const { return code == other.code; }
			bool operator != (const color& other)const { return code != other.code; }
			bool operator >= (const color& other)const { return code >= other.code; }
			bool operator <= (const color& other)const { return code <= other.code; }
			bool operator > (const color& other)const { return code > other.code; }
			bool operator < (const color& other)const { return code < other.code; }
		};

		typedef color::enum_t colors, colors_t;

	}
}


ANG_REGISTER_RUNTIME_TYPE_INFORMATION(ang::graphics::color_t)
ANG_REGISTER_RUNTIME_TYPE_INFORMATION(ang::graphics::graph_driver_type_t)
ANG_REGISTER_RUNTIME_TYPE_INFORMATION(ang::graphics::primitive_t)

#ifdef LINK
#undef LINK
#endif//LINK

#include <ang/inline/foundation.inl>

#endif//__ANG_FOUNDATION_H__