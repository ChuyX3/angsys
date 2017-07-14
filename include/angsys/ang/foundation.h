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

			float width()const;
			float height()const;
			point<T> left_top()const;
			point<T> right_top()const;
			point<T> left_bottom()const;
			point<T> right_bottom()const;

			void move_to(point<T>);
			void move_to(float x, float y);

			void resize(size<T>);
			void resize(float cx, float cy);

			void deflate(float l, float t, float r, float b);
			void inflate(float l, float t, float r, float b);

			rect<T>& operator = (const rect<T>&);
			rect<T>& operator += (const rect<T>&);
			rect<T>& operator -= (const rect<T>&);
			rect<T>& operator += (const point<T>&);
			rect<T>& operator -= (const point<T>&);
			rect<T>& operator += (const size<T>&);
			rect<T>& operator -= (const size<T>&);
			rect<T>& operator *= (float);
			rect<T>& operator /= (float);
		};


		template<> struct LINK point<float>
		{
			float x;
			float y;

		public:
			point();
			point(point const&);
			point(float x, float y);

			point& operator = (const point&);
			point& operator += (const point&);
			point& operator -= (const point&);
			point& operator *= (float);
			point& operator /= (float);
		};

		template<> struct LINK size<float>
		{
			float width;
			float height;

		public:
			size();
			size(size const&);
			size(float w, float h);

			size& operator = (const size&);
			size& operator += (const size&);
			size& operator -= (const size&);
			size& operator *= (float);
			size& operator /= (float);
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

		private:
			struct colors
			{
				enum enum_t : uint
				{
					alice_blue = 0XFFF0F8FF,
					antique_white = 0XFFFAEBD7,
					aqua = 0XFF00FFFF,
					aquamarine = 0XFF7FFFD4,
					azure = 0XFFF0FFFF,
					beige = 0XFFF5F5DC,
					bisque = 0XFFFFE4C4,
					black = 0XFF000000,
					blanched_almond = 0XFFFFEBCD,
					blue = 0XFF0000FF,
					blue_violet = 0XFF8A2BE2,
					brown = 0XFFA52A2A,
					burly_wood = 0XFFDEB887,
					cadet_blue = 0XFF5F9EA0,
					chartreuse = 0XFF7FFF00,
					chocolate = 0XFFD2691E,
					coral = 0XFFFF7F50,
					cornflower_blue = 0XFF6495ED,
					cornsilk = 0XFFFFF8DC,
					crimson = 0XFFDC143C,
					cyan = 0XFF00FFFF,
					dark_blue = 0XFF00008B,
					dark_cyan = 0XFF008B8B,
					dark_goldenrod = 0XFFB8860B,
					dark_gray = 0XFFA9A9A9,
					dark_green = 0XFF006400,
					dark_khaki = 0XFFBDB76B,
					dark_magenta = 0XFF8B008B,
					dark_olive_green = 0XFF556B2F,
					dark_orange = 0XFFFF8C00,
					dark_orchid = 0XFF9932CC,
					dark_red = 0XFF8B0000,
					dark_salmon = 0XFFE9967A,
					dark_sea_green = 0XFF8FBC8F,
					dark_slate_blue = 0XFF483D8B,
					dark_slate_gray = 0XFF2F4F4F,
					dark_turquoise = 0XFF00CED1,
					dark_violet = 0XFF9400D3,
					deep_pink = 0XFFFF1493,
					deep_sky_blue = 0XFF00BFFF,
					dim_gray = 0XFF696969,
					dodger_blue = 0XFF1E90FF,
					firebrick = 0XFFB22222,
					floral_white = 0XFFFFFAF0,
					forest_green = 0XFF228B22,
					fuchsia = 0XFFFF00FF,
					gainsboro = 0XFFDCDCDC,
					ghost_white = 0XFFF8F8FF,
					gold = 0XFFFFD700,
					goldenrod = 0XFFDAA520,
					gray = 0XFF808080,
					green = 0XFF008000,
					green_yellow = 0XFFADFF2F,
					honeydew = 0XFFF0FFF0,
					hot_pink = 0XFFFF69B4,
					indian_red = 0XFFCD5C5C,
					indigo = 0XFF4B0082,
					ivory = 0XFFFFFFF0,
					khaki = 0XFFF0E68C,
					lavender = 0XFFE6E6FA,
					lavender_blush = 0XFFFFF0F5,
					lawn_green = 0XFF7CFC00,
					lemon_chiffon = 0XFFFFFACD,
					light_blue = 0XFFADD8E6,
					light_coral = 0XFFF08080,
					light_cyan = 0XFFE0FFFF,
					light_goldenrod_yellow = 0XFFFAFAD2,
					light_gray = 0XFFD3D3D3,
					light_green = 0XFF90EE90,
					light_pink = 0XFFFFB6C1,
					light_salmon = 0XFFFFA07A,
					light_sea_green = 0XFF20B2AA,
					light_sky_blue = 0XFF87CEFA,
					light_slate_gray = 0XFF778899,
					light_steel_blue = 0XFFB0C4DE,
					light_yellow = 0XFFFFFFE0,
					lime = 0XFF00FF00,
					lime_green = 0XFF32CD32,
					linen = 0XFFFAF0E6,
					magenta = 0XFFFF00FF,
					maroon = 0XFF800000,
					medium_aquamarine = 0XFF66CDAA,
					medium_blue = 0XFF0000CD,
					medium_orchid = 0XFFBA55D3,
					medium_purple = 0XFF9370DB,
					medium_sea_green = 0XFF3CB371,
					medium_slate_blue = 0XFF7B68EE,
					medium_spring_green = 0XFF00FA9A,
					medium_turquoise = 0XFF48D1CC,
					medium_violet_red = 0XFFC71585,
					midnight_blue = 0XFF191970,
					mint_cream = 0XFFF5FFFA,
					misty_rose = 0XFFFFE4E1,
					moccasin = 0XFFFFE4B5,
					navajo_white = 0XFFFFDEAD,
					navy = 0XFF000080,
					old_lace = 0XFFFDF5E6,
					olive = 0XFF808000,
					olive_drab = 0XFF6B8E23,
					orange = 0XFFFFA500,
					orange_red = 0XFFFF4500,
					orchid = 0XFFDA70D6,
					pale_goldenrod = 0XFFEEE8AA,
					pale_green = 0XFF98FB98,
					pale_turquoise = 0XFFAFEEEE,
					pale_violet_red = 0XFFDB7093,
					papaya_whip = 0XFFFFEFD5,
					peach_puff = 0XFFFFDAB9,
					peru = 0XFFCD853F,
					pink = 0XFFFFC0CB,
					plum = 0XFFDDA0DD,
					powder_blue = 0XFFB0E0E6,
					purple = 0XFF800080,
					red = 0XFFFF0000,
					rosy_brown = 0XFFBC8F8F,
					royal_blue = 0XFF4169E1,
					saddle_brown = 0XFF8B4513,
					salmon = 0XFFFA8072,
					sandy_brown = 0XFFF4A460,
					sea_green = 0XFF2E8B57,
					sea_shell = 0XFFFFF5EE,
					sienna = 0XFFA0522D,
					silver = 0XFFC0C0C0,
					sky_blue = 0XFF87CEEB,
					slate_blue = 0XFF6A5ACD,
					slate_gray = 0XFF708090,
					snow = 0XFFFFFAFA,
					spring_green = 0XFF00FF7F,
					steel_blue = 0XFF4682B4,
					tan = 0XFFD2B48C,
					teal = 0XFF008080,
					thistle = 0XFFD8BFD8,
					tomato = 0XFFFF6347,
					transparent = 0X00FFFFFF,
					turquoise = 0XFF40E0D0,
					violet = 0XFFEE82EE,
					wheat = 0XFFF5DEB3,
					white = 0XFFFFFFFF,
					white_smoke = 0XFFF5F5F5,
				};
			};
		
		public:
			typedef colors::enum_t enum_t;

		public:
			color();
			color(const color&);
			color(enum_t rgba);
			color(dword rgba);
			color(byte r, byte g, byte b, byte a);
			virtual~color();

			void parse(cstr_t);
			void parse(cwstr_t);

		public: //Override
			string_t to_string()const;

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