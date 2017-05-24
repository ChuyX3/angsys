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
					absolute_zero = 0X0048BA,
					acajou = 0X4C2F27,
					acid_green = 0XB0BF1A,
					aero = 0X7CB9E8,
					aero_blue = 0XC9FFE5,
					african_violet = 0XB284BE,
					air_force_blue = 0X5D8AA8,
					air_force_deep_blue = 0X00308F,
					air_superiority_blue = 0X72A0C1,
					alabama_crimson = 0XAF002A,
					alabaster = 0XF2F0E6,
					alice_blue = 0XF0F8FF,
					alien_armpit = 0X84DE02,
					alizarin_crimson = 0XE32636,
					alloy_orange = 0XC46210,
					almond = 0XEFDECD,
					amaranth = 0XE52B50,
					amaranth_deep_purple = 0X9F2B68,
					amaranth_pink = 0XF19CBB,
					amaranth_purple = 0XAB274F,
					amaranth_red = 0XD3212D,
					amazon = 0X3B7A57,
					amazonite = 0X00C4B0,
					amber = 0XFFBF00,
					amber_deep = 0XFF7E00,
					american_rose = 0XFF033E,
					amethyst = 0X9966CC,
					android_green = 0XA4C639,
					antiflash_white = 0XF2F3F4,
					antique_brass = 0XCD9575,
					antique_bronze = 0X665D1E,
					antique_fuchsia = 0X915C83,
					antique_ruby = 0X841B2D,
					antique_white = 0XFAEBD7,
					ao = 0X008000,
					appleGreen = 0X8DB600,
					apricot = 0XFBCEB1,
					aqua = 0X00FFFF,
					aquamarine = 0X7FFFD4,
					arctic_lime = 0XD0FF14,
					army_green = 0X4B5320,
					arsenic = 0X3B444B,
					artichoke = 0X8F9779,
					arylide_yellow = 0XE9D66B,
					ash_grey = 0XB2BEB5,
					asparagus = 0X87A96B,
					atomic_tangerine = 0XFF9966,
					auburn = 0XA52A2A,
					aureolin = 0XFDEE00,
					auro_metal_saurus = 0X6E7F80,
					avocado = 0X568203,
					aztec_gold = 0XC39953,
					azure = 0X007FFF,
					azure_web = 0XF0FFFF,
					azure_mist = 0XF0FFFF,
					azureish_white = 0XDBE9F4,
					baby_blue = 0X89CFF0,
					baby_blue_eyes = 0XA1CAF1,
					baby_pink = 0XF4C2C2,
					baby_powder = 0XFEFEFA,
					baker_miller_pink = 0XFF91AF,
					ball_blue = 0X21ABCD,
					banana_mania = 0XFAE7B5,
					banana_yellow = 0XFFE135,
					bangladesh_green = 0X006A4E,
					barbie_pink = 0XE0218A,
					barn_red = 0X7C0A02,
					battery_charged_blue = 0X1DACD6,
					battleship_grey = 0X848482,
					bazaar = 0X98777B,
					beau_blue = 0XBCD4E6,
					beaver = 0X9F8170,
					begonia = 0XFA6E79,
					beige = 0XF5F5DC,
					bdazzled_blue = 0X2E5894,
					big_dip_o_ruby = 0X9C2542,
					big_foot_feet = 0XE88E5A,
					bisque = 0XFFE4C4,
					bistre = 0X3D2B1F,
					bistre_brown = 0X967117,
					bitter_Lemon = 0XCAE00D,
					bitter_lime = 0XBFFF00,
					bitter_sweet = 0XFE6F5E,
					bitter_sweet_shimmer = 0XBF4F51,
					black = 0X000000,
					black_bean = 0X3D0C02,
					black_coral = 0X54626F,
					black_leather_jacket = 0X253529,
					black_olive = 0X3B3C36,
					black_shadows = 0XBFAFB2,
					blanched_almond = 0XFFEBCD,
					blast_off_bronze = 0XA57164,
					bleu_de_france = 0X318CE7,
					blizzard_blue = 0XACE5EE,
					blond = 0XFAF0BE,
					blue = 0X0000FF,
					blue_crayola = 0X1F75FE,
					blue_munsell = 0X0093AF,
					blue_NCS = 0X0087BD,
					blue_pantone = 0X0018A8,
					blue_pigment = 0X333399,
					blue_RYB = 0X0247FE,
					blue_bell = 0XA2A2D0,
					blue_bolt = 0X00B9FB,
					blue_gray = 0X6699CC,
					blue_green = 0X0D98BA,
					blue_jeans = 0X5DADEC,
					blue_lagoon = 0XACE5EE,
					blue_magenta_violet = 0X553592,
					blue_sapphire = 0X126180,
					blue_violet = 0X8A2BE2,
					blue_yonder = 0X5072A7,
					blueberry = 0X4F86F7,
					bluebonnet = 0X1C1CF0,
					blush = 0XDE5D83,
					bole = 0X79443B,
					bondi_blue = 0X0095B6,
					bone = 0XE3DAC9,
					booger_buster = 0XDDE26A,
					boston_university_red = 0XCC0000,
					bottle_green = 0X006A4E,
					boysenberry = 0X873260,
					brandeis_blue = 0X0070FF,
					brass = 0XB5A642,
					brick_red = 0XCB4154,
					bright_cerulean = 0X1DACD6,
					bright_green = 0X66FF00,
					bright_lavender = 0XBF94E4,
					bright_lilac = 0XD891EF,
					bright_maroon = 0XC32148,
					bright_navy_blue = 0X1974D2,
					bright_pink = 0XFF007F,
					bright_turquoise = 0X08E8DE,
					bright_ube = 0XD19FE8,
					bright_yellow_crayola = 0XFFAA1D,
					brilliant_azure = 0X3399FF,
					brilliant_lavender = 0XF4BBFF,
					brilliant_rose = 0XFF55A3,
					brink_pink = 0XFB607F,
					british_racing_green = 0X004225,
					bronze = 0XCD7F32,
					bronze_yellow = 0X737000,
					brown = 0X964B00,
					brown_web = 0XA52A2A,
					brown_nose = 0X6B4423,
					brown_sugar = 0XAF6E4D,
					brown_yellow = 0Xcc9966,
					brunswick_green = 0X1B4D3E,
					bubble_gum = 0XFFC1CC,
					bubbles = 0XE7FEFF,
					bud_green = 0X7BB661,
					buff = 0XF0DC82,
					bulgarian_rose = 0X480607,
					burgundy = 0X800020,
					burlywood = 0XDEB887,
					burnished_brown = 0XA17A74,
					burnt_orange = 0XCC5500,
					burnt_sienna = 0XE97451,
					burnt_umber = 0X8A3324,
					byzantine = 0XBD33A4,
					byzantium = 0X702963,
					cadet = 0X536872,
					cadet_blue = 0X5F9EA0,
					cadet_grey = 0X91A3B0,
					cadmium_green = 0X006B3C,
					cadmium_orange = 0XED872D,
					cadmium_red = 0XE30022,
					cadmium_yellow = 0XFFF600,
					cafe_au_lait = 0XA67B5B,
					cafe_noir = 0X4B3621,
					cal_poly_green = 0X1E4D2B,
					cambridge_blue = 0XA3C1AD,
					camel = 0XC19A6B,
					cameo_pink = 0XEFBBCC,
					camouflage_green = 0X78866B,
					canary = 0XFFFF99,
					canary_yellow = 0XFFEF00,
					candy_apple_red = 0XFF0800,
					candy_pink = 0XE4717A,
					capri = 0X00BFFF,
					caput_mortuum = 0X592720,
					cardinal = 0XC41E3A,
					caribbean_green = 0X00CC99,
					carmine = 0X960018,
					carmine_MP = 0XD70040,
					carmine_pink = 0XEB4C42,
					carmine_red = 0XFF0038,
					carnation_pink = 0XFFA6C9,
					carnelian = 0XB31B1B,
					carolina_blue = 0X56A0D3,
					carrot_orange = 0XED9121,
					castleton_green = 0X00563F,
					catalina_blue = 0X062A78,
					catawba = 0X703642,
					cedar_chest = 0XC95A49,
					ceil = 0X92A1CF,
					celadon = 0XACE1AF,
					celadon_blue = 0X007BA7,
					celadon_green = 0X2F847C,
					celeste = 0XB2FFFF,
					celestial_blue = 0X4997D0,
					cerise = 0XDE3163,
					cerise_pink = 0XEC3B83,
					cerulean = 0X007BA7,
					cerulean_blue = 0X2A52BE,
					cerulean_frost = 0X6D9BC3,
					cg_blue = 0X007AA5,
					cg_red = 0XE03C31,
					chamoisee = 0XA0785A,
					champagne = 0XF7E7CE,
					champagne_pink = 0XF1DDCF,
					charcoal = 0X36454F,
					charleston_green = 0X232B2B,
					charm_pink = 0XE68FAC,
					chartreuse = 0XDFFF00,
					chartreuse_web = 0X7FFF00,
					cherry = 0XDE3163,
					cherry_blossom_pink = 0XFFB7C5,
					chestnut = 0X954535,
					china_pink = 0XDE6FA1,
					china_rose = 0XA8516E,
					chinese_red = 0XAA381E,
					chinese_violet = 0X856088,
					chlorophyll_green = 0X4AFF00,
					chocolate = 0X7B3F00,
					chocolate_web = 0XD2691E,
					chrome_yellow = 0XFFA700,
					cinereous = 0X98817B,
					cinnabar = 0XE34234,
					cinnamon = 0XD2691E,
					cinnamon_satin = 0XCD607E,
					citrine = 0XE4D00A,
					citron = 0X9FA91F,
					claret = 0X7F1734,
					classic_rose = 0XFBCCE7,
					cobalt_blue = 0X0047AB,
					cocoa_brown = 0XD2691E,
					coconut = 0X965A3E,
					coffee = 0X6F4E37,
					columbia_blue = 0XC4D8E2,
					congo_pink = 0XF88379,
					cool_black = 0X002E63,
					cool_grey = 0X8C92AC,
					copper = 0XB87333,
					copper_crayola = 0XDA8A67,
					copper_penny = 0XAD6F69,
					copper_red = 0XCB6D51,
					copper_rose = 0X996666,
					coquelicot = 0XFF3800,
					coral = 0XFF7F50,
					coral_pink = 0XF88379,
					coral_red = 0XFF4040,
					coral_reef = 0XFD7C6E,
					cordovan = 0X893F45,
					corn = 0XFBEC5D,
					cornell_red = 0XB31B1B,
					cornflower_blue = 0X6495ED,
					cornsilk = 0XFFF8DC,
					cosmic_cobalt = 0X2E2D88,
					cosmic_latte = 0XFFF8E7,
					coyote_brown = 0X81613C,
					cotton_candy = 0XFFBCD9,
					cream = 0XFFFDD0,
					crimson = 0XDC143C,
					crimson_glory = 0XBE0032,
					crimson_red = 0X990000,
					cultured = 0XF5F5F5,
					cyan = 0X00FFFF,
					cyan_azure = 0X4E82B4,
					cyan_blue_azure = 0X4682BF,
					cyan_cobalt_blue = 0X28589C,
					cyan_cornflower_blue = 0X188BC2,
					cyan_process = 0X00B7EB,
					cyber_grape = 0X58427C,
					cyber_yellow = 0XFFD300,
					cyclamen = 0XF56FA1,
					daffodil = 0XFFFF31,
					dandelion = 0XF0E130,
					dark_blue = 0X00008B,
					dark_blue_gray = 0X666699,
					dark_brown = 0X654321,
					dark_brown_tangelo = 0X88654E,
					dark_byzantium = 0X5D3954,
					dark_candy_apple_red = 0XA40000,
					dark_cerulean = 0X08457E,
					dark_chestnut = 0X986960,
					dark_coral = 0XCD5B45,
					dark_cyan = 0X008B8B,
					dark_electric_blue = 0X536878,
					dark_goldenrod = 0XB8860B,
					dark_gray_X11 = 0XA9A9A9,
					dark_green = 0X013220,
					dark_green_X11 = 0X006400,
					dark_gunmetal = 0X1F262A,
					dark_imperial_blue = 0X00416A,
					dark_imperial_blue_web = 0X6E6EF9,
					dark_jungle_green = 0X1A2421,
					dark_khaki = 0XBDB76B,
					dark_lava = 0X483C32,
					dark_lavender = 0X734F96,
					dark_liver = 0X534B4F,
					dark_liver_horses = 0X543D37,
					dark_magenta = 0X8B008B,
					dark_medium_gray = 0XA9A9A9,
					dark_midnight_blue = 0X003366,
					dark_moss_green = 0X4A5D23,
					dark_olive_green = 0X556B2F,
					dark_orange = 0XFF8C00,
					dark_orchid = 0X9932CC,
					dark_pastel_blue = 0X779ECB,
					dark_pastel_green = 0X03C03C,
					dark_pastel_purple = 0X966FD6,
					dark_pastel_red = 0XC23B22,
					dark_pink = 0XE75480,
					dark_powder_blue = 0X003399,
					dark_puce = 0X4F3A3C,
					dark_purple = 0X301934,
					dark_raspberry = 0X872657,
					dark_red = 0X8B0000,
					dark_salmon = 0XE9967A,
					dark_scarlet = 0X560319,
					dark_sea_green = 0X8FBC8F,
					dark_sienna = 0X3C1414,
					dark_Sky_blue = 0X8CBED6,
					dark_slate_blue = 0X483D8B,
					dark_slate_gray = 0X2F4F4F,
					dark_spring_green = 0X177245,
					dark_tan = 0X918151,
					dark_tangerine = 0XFFA812,
					dark_taupe = 0X483C32,
					dark_terra_cotta = 0XCC4E5C,
					dark_turquoise = 0X00CED1,
					dark_vanilla = 0XD1BEA8,
					dark_violet = 0X9400D3,
					dark_yellow = 0X9B870C,
					dartmouth_green = 0X00703C,
					davy_grey = 0X555555,
					debian_red = 0XD70A53,
					deep_aquamarine = 0X40826D,
					deep_carmine = 0XA9203E,
					deep_carmine_pink = 0XEF3038,
					deep_carrot_orange = 0XE9692C,
					deep_cerise = 0XDA3287,
					deep_champagne = 0XFAD6A5,
					deep_chestnut = 0XB94E48,
					deep_coffee = 0X704241,
					deep_fuchsia = 0XC154C1,
					deep_green = 0X056608,
					deep_green_cyan_turquoise = 0X0E7C61,
					deep_jungle_green = 0X004B49,
					deep_koamaru = 0X333366,
					deep_lemon = 0XF5C71A,
					deep_lilac = 0X9955BB,
					deep_magenta = 0XCC00CC,
					deep_maroon = 0X820000,
					deep_mauve = 0XD473D4,
					deep_moss_green = 0X355E3B,
					deep_peach = 0XFFCBA4,
					deep_pink = 0XFF1493,
					deep_puce = 0XA95C68,
					deep_red = 0X850101,
					deep_ruby = 0X843F5B,
					deep_saffron = 0XFF9933,
					deep_sky_blue = 0X00BFFF,
					deep_space_sparkle = 0X4A646C,
					deep_spring_bud = 0X556B2F,
					deep_taupe = 0X7E5E60,
					deep_tuscan_red = 0X66424D,
					deep_violet = 0X330066,
					deer = 0XBA8759,
					denim = 0X1560BD,
					denim_blue = 0X2243B6,
					desaturated_cyan = 0X669999,
					desert = 0XC19A6B,
					desert_sand = 0XEDC9AF,
					desire = 0XEA3C53,
					diamond = 0XB9F2FF,
					dim_gray = 0X696969,
					dingy_dungeon = 0XC53151,
					dirt = 0X9B7653,
					dodger_blue = 0X1E90FF,
					dogwood_rose = 0XD71868,
					dollar_bill = 0X85BB65,
					dolphin_gray = 0X828E84,
					donkeyBrown = 0X664C28,
					drab = 0X967117,
					duke_blue = 0X00009C,
					dust_storm = 0XE5CCC9,
					dutch_white = 0XEFDFBB,
					earth_yellow = 0XE1A95F,
					ebony = 0X555D50,
					ecru = 0XC2B280,
					eerie_black = 0X1B1B1B,
					eggplant = 0X614051,
					eggshell = 0XF0EAD6,
					egyptian_blue = 0X1034A6,
					electric_blue = 0X7DF9FF,
					electric_crimson = 0XFF003F,
					electric_cyan = 0X00FFFF,
					electric_green = 0X00FF00,
					electric_indigo = 0X6F00FF,
					electric_lavender = 0XF4BBFF,
					electric_lime = 0XCCFF00,
					electric_purple = 0XBF00FF,
					electric_ultramarine = 0X3F00FF,
					electric_violet = 0X8F00FF,
					electric_yellow = 0XFFFF33,
					emerald = 0X50C878,
					eminence = 0X6C3082,
					english_green = 0X1B4D3E,
					english_lavender = 0XB48395,
					english_red = 0XAB4B52,
					english_vermillion = 0XCC474B,
					english_violet = 0X563C5C,
					eton_blue = 0X96C8A2,
					eucalyptus = 0X44D7A8,
					eallow = 0XC19A6B,
					ealu_red = 0X801818,
					fandango = 0XB53389,
					fandango_pink = 0XDE5285,
					fashion_fuchsia = 0XF400A1,
					fawn = 0XE5AA70,
					feldgrau = 0X4D5D53,
					feldspar = 0XFDD5B1,
					fern_green = 0X4F7942,
					ferrari_red = 0XFF2800,
					field_drab = 0X6C541E,
					fiery_rose = 0XFF5470,
					firebrick = 0XB22222,
					fire_engine_red = 0XCE2029,
					flame = 0XE25822,
					flamingo_pink = 0XFC8EAC,
					flattery = 0X6B4423,
					flavescent = 0XF7E98E,
					flax = 0XEEDC82,
					flirt = 0XA2006D,
					floral_white = 0XFFFAF0,
					fluorescent_orange = 0XFFBF00,
					fluorescent_pink = 0XFF1493,
					fluorescent_yellow = 0XCCFF00,
					folly = 0XFF004F,
					forest_green = 0X014421,
					forest_greenWeb = 0X228B22,
					french_beige = 0XA67B5B,
					french_bistre = 0X856D4D,
					french_blue = 0X0072BB,
					french_fuchsia = 0XFD3F92,
					french_lilac = 0X86608E,
					french_lime = 0X9EFD38,
					french_mauve = 0XD473D4,
					french_pink = 0XFD6C9E,
					french_plum = 0X811453,
					french_puce = 0X4E1609,
					french_raspberry = 0XC72C48,
					french_rose = 0XF64A8A,
					french_sky_blue = 0X77B5FE,
					french_violet = 0X8806CE,
					french_wine = 0XAC1E44,
					fresh_air = 0XA6E7FF,
					frostbite = 0XE936A7,
					fuchsia = 0XFF00FF,
					fuchsia_crayola = 0XC154C1,
					fuchsia_pink = 0XFF77FF,
					fuchsia_purple = 0XCC397B,
					fuchsia_rose = 0XC74375,
					fulvous = 0XE48400,
					fuzzy_wuzzy = 0XCC6666,
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

#include <ang/inline/foundation.hpp>

#endif//__ANG_FOUNDATION_H__