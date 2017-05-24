/*********************************************************************************************************************/
/*   File Name: foundation.cpp                                                                                           */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#include "pch.h"
#include "ang/foundation.h"
#include "ang_inlines.h"

using namespace ang;
using namespace ang::foundation;


point<float>::point()
	: x(0)
	, y(0)
{
}

point<float>::point(point const& p)
	: x(p.x)
	, y(p.y)
{
}

point<float>::point(float x, float y)
	: x(x)
	, y(y)
{
}

point<float>& point<float>::operator = (const point<float>& p)
{
	x = p.x;
	y = p.y;
	return*this;
}

point<float>& point<float>::operator += (const point<float>& p)
{
	x += p.x;
	y += p.y;
	return *this;
}

point<float>& point<float>::operator -= (const point<float>& p)
{
	x -= p.x;
	y -= p.y;
	return *this;
}

point<float>& point<float>::operator *= (float k)
{
	x *= k;
	y *= k;
	return *this;
}

point<float>& point<float>::operator /= (float k)
{
	x /= k;
	y /= k;
	return *this;
}

////////////////////////////////////////////////////////////////////////////

size<float>::size()
	: width(0)
	, height(0)
{

}

size<float>::size(size const& b)
	: width(b.width)
	, height(b.height)
{

}

size<float>::size(float width, float height)
	: width(width)
	, height(height)
{

}

size<float>& size<float>::operator = (const size<float>& b)
{
	width = b.width;
	height = b.height;
	return*this;
}

size<float>& size<float>::operator += (const size<float>& b)
{
	width += b.width;
	height += b.height;
	return *this;
}

size<float>& size<float>::operator -= (const size<float>& b)
{
	width -= b.width;
	height -= b.height;
	return *this;
}

size<float>& size<float>::operator *= (float k)
{
	width *= k;
	height *= k;
	return *this;
}

size<float>& size<float>::operator /= (float k)
{
	width /= k;
	height /= k;
	return *this;
}

////////////////////////////////////////////////////////////////////

rect<float>::rect()
	: left(0)
	, top(0)
	, right(0)
	, bottom(0)
{

}

rect<float>::rect(rect<float> const& rc)
	: left(rc.left)
	, top(rc.top)
	, right(rc.right)
	, bottom(rc.bottom)
{

}

rect<float>::rect(float l, float t, float r, float b)
	: left(l)
	, top(t)
	, right(r < l ? l : r)
	, bottom(b < t ? t : b)
{

}

rect<float>::rect(point<float> lt, point<float> rb)
	: left(lt.x)
	, top(lt.y)
	, right(rb.x)
	, bottom(rb.y)
{

}

rect<float>::rect(point<float> lt, size<float> sz)
	: left(lt.x)
	, top(lt.y)
	, right(lt.x + sz.width)
	, bottom(lt.y + sz.height)
{

}

float rect<float>::width()const { return right - left; }

float rect<float>::height()const { return bottom - top; }

point<float> rect<float>::left_top()const { return{ left, top }; }

point<float> rect<float>::right_top()const { return{ right, top }; }

point<float> rect<float>::left_bottom()const { return{ left, bottom }; }

point<float> rect<float>::right_bottom()const { return{ right, bottom }; }

void rect<float>::move_to(point<float> p)
{
	right = right - left + p.x;
	bottom = bottom - top + p.y;
	left = p.x;
	top = p.y;
}

void rect<float>::move_to(float x, float y)
{
	right = right - left + x;
	bottom = bottom - top + y;
	left = x;
	top = y;
}

void rect<float>::resize(size<float> sz)
{
	right = left + sz.width;
	bottom = top + sz.height;
}

void rect<float>::resize(float cx, float cy)
{
	right = left + cx;
	bottom = top + cy;
}

void rect<float>::deflate(float l, float t, float r, float b)
{
	left += l;
	top += t;
	right -= r;
	bottom -= b;
}

void rect<float>::inflate(float l, float t, float r, float b)
{
	left -= l;
	top -= t;
	right += r;
	bottom += b;
}

rect<float>& rect<float>::operator = (const rect<float>& rc)
{
	left = rc.left;
	top = rc.top;
	right = rc.right;
	bottom = rc.bottom;
	return *this;
}

rect<float>& rect<float>::operator += (const rect<float>& rc)
{
	left += rc.left;
	top += rc.top;
	right += rc.right;
	bottom += rc.bottom;
	return *this;
}

rect<float>& rect<float>::operator -= (const rect<float>& rc)
{
	left -= rc.left;
	top -= rc.top;
	right -= rc.right;
	bottom -= rc.bottom;
	return *this;
}

rect<float>& rect<float>::operator += (const point<float>& pt)
{
	left += pt.x;
	top += pt.y;
	right += pt.x;
	bottom += pt.y;
	return *this;
}

rect<float>& rect<float>::operator -= (const point<float>& pt)
{
	left -= pt.x;
	top -= pt.y;
	right -= pt.x;
	bottom -= pt.y;
	return *this;
}

rect<float>& rect<float>::operator += (const size<float>& sz)
{
	right += sz.width;
	bottom += sz.height;
	return *this;
}

rect<float>& rect<float>::operator -= (const size<float>& sz)
{
	right -= sz.width;
	bottom -= sz.height;
	if (left > right) right = left;
	if (top > bottom) bottom = top;
	return *this;
}

rect<float>& rect<float>::operator *= (float k)
{
	left *= k;
	top *= k;
	right *= k;
	bottom *= k;
	return *this;
}

rect<float>& rect<float>::operator /= (float k)
{
	left /= k;
	top /= k;
	right /= k;
	bottom /= k;
	return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static ang_pair<graphics::colors, cstr_t> to_string_color_map[] =
{
	{ graphics::colors::black, "black"_s },
	{ graphics::colors::dark_blue, "dark_blue"_s },
	{ graphics::colors::duke_blue, "duke_blue"_s },
	{ graphics::colors::blue, "blue"_s },
	{ graphics::colors::blue_pantone, "blue_pantone"_s },
	{ graphics::colors::cool_black, "cool_black"_s },
	{ graphics::colors::air_force_deep_blue, "air_force_deep_blue"_s },
	{ graphics::colors::dark_midnight_blue, "dark_midnight_blue"_s },
	{ graphics::colors::dark_powder_blue, "dark_powder_blue"_s },
	{ graphics::colors::dark_imperial_blue, "dark_imperial_blue"_s },
	{ graphics::colors::british_racing_green, "british_racing_green"_s },
	{ graphics::colors::cobalt_blue, "cobalt_blue"_s },
	{ graphics::colors::absolute_zero, "absolute_zero"_s },
	{ graphics::colors::deep_jungle_green, "deep_jungle_green"_s },
	{ graphics::colors::castleton_green, "castleton_green"_s },
	{ graphics::colors::dark_green_X11, "dark_green_X11"_s },
	{ graphics::colors::bangladesh_green, "bangladesh_green"_s },
	{ graphics::colors::cadmium_green, "cadmium_green"_s },
	{ graphics::colors::dartmouth_green, "dartmouth_green"_s },
	{ graphics::colors::brandeis_blue, "brandeis_blue"_s },
	{ graphics::colors::french_blue, "french_blue"_s },
	{ graphics::colors::cg_blue, "cg_blue"_s },
	{ graphics::colors::celadon_blue, "celadon_blue"_s },
	{ graphics::colors::azure, "azure"_s },
	{ graphics::colors::ao, "ao"_s },
	{ graphics::colors::blue_NCS, "blue_NCS"_s },
	{ graphics::colors::dark_cyan, "dark_cyan"_s },
	{ graphics::colors::blue_munsell, "blue_munsell"_s },
	{ graphics::colors::bondi_blue, "bondi_blue"_s },
	{ graphics::colors::cyan_process, "cyan_process"_s },
	{ graphics::colors::blue_bolt, "blue_bolt"_s },
	{ graphics::colors::capri, "capri"_s },
	{ graphics::colors::amazonite, "amazonite"_s },
	{ graphics::colors::caribbean_green, "caribbean_green"_s },
	{ graphics::colors::dark_turquoise, "dark_turquoise"_s },
	{ graphics::colors::electric_green, "electric_green"_s },
	{ graphics::colors::aqua, "aqua"_s },
	{ graphics::colors::dark_green, "dark_green"_s },
	{ graphics::colors::forest_green, "forest_green"_s },
	{ graphics::colors::blue_RYB, "blue_RYB"_s },
	{ graphics::colors::dark_pastel_green, "dark_pastel_green"_s },
	{ graphics::colors::deep_green, "deep_green"_s },
	{ graphics::colors::catalina_blue, "catalina_blue"_s },
	{ graphics::colors::dark_cerulean, "dark_cerulean"_s },
	{ graphics::colors::bright_turquoise, "bright_turquoise"_s },
	{ graphics::colors::blue_green, "blue_green"_s },
	{ graphics::colors::deep_green_cyan_turquoise, "deep_green_cyan_turquoise"_s },
	{ graphics::colors::egyptian_blue, "egyptian_blue"_s },
	{ graphics::colors::blue_sapphire, "blue_sapphire"_s },
	{ graphics::colors::denim, "denim"_s },
	{ graphics::colors::dark_spring_green, "dark_spring_green"_s },
	{ graphics::colors::cyan_cornflower_blue, "cyan_cornflower_blue"_s },
	{ graphics::colors::bright_navy_blue, "bright_navy_blue"_s },
	{ graphics::colors::dark_jungle_green, "dark_jungle_green"_s },
	{ graphics::colors::eerie_black, "eerie_black"_s },
	{ graphics::colors::brunswick_green, "brunswick_green"_s },
	{ graphics::colors::bluebonnet, "bluebonnet"_s },
	{ graphics::colors::battery_charged_blue, "battery_charged_blue"_s },
	{ graphics::colors::cal_poly_green, "cal_poly_green"_s },
	{ graphics::colors::dodger_blue, "dodger_blue"_s },
	{ graphics::colors::dark_gunmetal, "dark_gunmetal"_s },
	{ graphics::colors::blue_crayola, "blue_crayola"_s },
	{ graphics::colors::ball_blue, "ball_blue"_s },
	{ graphics::colors::denim_blue, "denim_blue"_s },
	{ graphics::colors::forest_greenWeb, "forest_greenWeb"_s },
	{ graphics::colors::charleston_green, "charleston_green"_s },
	{ graphics::colors::black_leather_jacket, "black_leather_jacket"_s },
	{ graphics::colors::cyan_cobalt_blue, "cyan_cobalt_blue"_s },
	{ graphics::colors::cerulean_blue, "cerulean_blue"_s },
	{ graphics::colors::cosmic_cobalt, "cosmic_cobalt"_s },
	{ graphics::colors::bdazzled_blue, "bdazzled_blue"_s },
	{ graphics::colors::dark_slate_gray, "dark_slate_gray"_s },
	{ graphics::colors::celadon_green, "celadon_green"_s },
	{ graphics::colors::dark_purple, "dark_purple"_s },
	{ graphics::colors::bleu_de_france, "bleu_de_france"_s },
	{ graphics::colors::deep_violet, "deep_violet"_s },
	{ graphics::colors::deep_koamaru, "deep_koamaru"_s },
	{ graphics::colors::blue_pigment, "blue_pigment"_s },
	{ graphics::colors::brilliant_azure, "brilliant_azure"_s },
	{ graphics::colors::deep_moss_green, "deep_moss_green"_s },
	{ graphics::colors::charcoal, "charcoal"_s },
	{ graphics::colors::black_olive, "black_olive"_s },
	{ graphics::colors::arsenic, "arsenic"_s },
	{ graphics::colors::amazon, "amazon"_s },
	{ graphics::colors::dark_sienna, "dark_sienna"_s },
	{ graphics::colors::black_bean, "black_bean"_s },
	{ graphics::colors::bistre, "bistre"_s },
	{ graphics::colors::electric_ultramarine, "electric_ultramarine"_s },
	{ graphics::colors::deep_aquamarine, "deep_aquamarine"_s },
	{ graphics::colors::eucalyptus, "eucalyptus"_s },
	{ graphics::colors::cyan_blue_azure, "cyan_blue_azure"_s },
	{ graphics::colors::bulgarian_rose, "bulgarian_rose"_s },
	{ graphics::colors::dark_lava, "dark_lava"_s },
	{ graphics::colors::dark_slate_blue, "dark_slate_blue"_s },
	{ graphics::colors::celestial_blue, "celestial_blue"_s },
	{ graphics::colors::dark_moss_green, "dark_moss_green"_s },
	{ graphics::colors::deep_space_sparkle, "deep_space_sparkle"_s },
	{ graphics::colors::chlorophyll_green, "chlorophyll_green"_s },
	{ graphics::colors::cafe_noir, "cafe_noir"_s },
	{ graphics::colors::army_green, "army_green"_s },
	{ graphics::colors::acajou, "acajou"_s },
	{ graphics::colors::feldgrau, "feldgrau"_s },
	{ graphics::colors::french_puce, "french_puce"_s },
	{ graphics::colors::cyan_azure, "cyan_azure"_s },
	{ graphics::colors::dark_puce, "dark_puce"_s },
	{ graphics::colors::fern_green, "fern_green"_s },
	{ graphics::colors::blueberry, "blueberry"_s },
	{ graphics::colors::blue_yonder, "blue_yonder"_s },
	{ graphics::colors::emerald, "emerald"_s },
	{ graphics::colors::dark_liver, "dark_liver"_s },
	{ graphics::colors::cadet, "cadet"_s },
	{ graphics::colors::dark_electric_blue, "dark_electric_blue"_s },
	{ graphics::colors::dark_liver_horses, "dark_liver_horses"_s },
	{ graphics::colors::black_coral, "black_coral"_s },
	{ graphics::colors::blue_magenta_violet, "blue_magenta_violet"_s },
	{ graphics::colors::davy_grey, "davy_grey"_s },
	{ graphics::colors::ebony, "ebony"_s },
	{ graphics::colors::dark_olive_green, "dark_olive_green"_s },
	{ graphics::colors::dark_scarlet, "dark_scarlet"_s },
	{ graphics::colors::english_violet, "english_violet"_s },
	{ graphics::colors::avocado, "avocado"_s },
	{ graphics::colors::carolina_blue, "carolina_blue"_s },
	{ graphics::colors::cyber_grape, "cyber_grape"_s },
	{ graphics::colors::caput_mortuum, "caput_mortuum"_s },
	{ graphics::colors::dark_byzantium, "dark_byzantium"_s },
	{ graphics::colors::air_force_blue, "air_force_blue"_s },
	{ graphics::colors::blue_jeans, "blue_jeans"_s },
	{ graphics::colors::cadet_blue, "cadet_blue"_s },
	{ graphics::colors::eggplant, "eggplant"_s },
	{ graphics::colors::cornflower_blue, "cornflower_blue"_s },
	{ graphics::colors::dark_brown, "dark_brown"_s },
	{ graphics::colors::deep_tuscan_red, "deep_tuscan_red"_s },
	{ graphics::colors::donkeyBrown, "donkeyBrown"_s },
	{ graphics::colors::antique_bronze, "antique_bronze"_s },
	{ graphics::colors::dark_blue_gray, "dark_blue_gray"_s },
	{ graphics::colors::desaturated_cyan, "desaturated_cyan"_s },
	{ graphics::colors::blue_gray, "blue_gray"_s },
	{ graphics::colors::bright_green, "bright_green"_s },
	{ graphics::colors::dim_gray, "dim_gray"_s },
	{ graphics::colors::brown_nose, "brown_nose"_s },
	{ graphics::colors::eminence, "eminence"_s },
	{ graphics::colors::field_drab, "field_drab"_s },
	{ graphics::colors::cerulean_frost, "cerulean_frost"_s },
	{ graphics::colors::dark_imperial_blue_web, "dark_imperial_blue_web"_s },
	{ graphics::colors::auro_metal_saurus, "auro_metal_saurus"_s },
	{ graphics::colors::electric_indigo, "electric_indigo"_s },
	{ graphics::colors::coffee, "coffee"_s },
	{ graphics::colors::byzantium, "byzantium"_s },
	{ graphics::colors::catawba, "catawba"_s },
	{ graphics::colors::deep_coffee, "deep_coffee"_s },
	{ graphics::colors::air_superiority_blue, "air_superiority_blue"_s },
	{ graphics::colors::dark_lavender, "dark_lavender"_s },
	{ graphics::colors::bronze_yellow, "bronze_yellow"_s },
	{ graphics::colors::dark_pastel_blue, "dark_pastel_blue"_s },
	{ graphics::colors::french_sky_blue, "french_sky_blue"_s },
	{ graphics::colors::camouflage_green, "camouflage_green"_s },
	{ graphics::colors::bole, "bole"_s },
	{ graphics::colors::chocolate, "chocolate"_s },
	{ graphics::colors::bud_green, "bud_green"_s },
	{ graphics::colors::barn_red, "barn_red"_s },
	{ graphics::colors::aero, "aero"_s },
	{ graphics::colors::electric_blue, "electric_blue"_s },
	{ graphics::colors::deep_taupe, "deep_taupe"_s },
	{ graphics::colors::claret, "claret"_s },
	{ graphics::colors::chartreuse_web, "chartreuse_web"_s },
	{ graphics::colors::aquamarine, "aquamarine"_s },
	{ graphics::colors::burgundy, "burgundy"_s },
	{ graphics::colors::ealu_red, "ealu_red"_s },
	{ graphics::colors::french_plum, "french_plum"_s },
	{ graphics::colors::coyote_brown, "coyote_brown"_s },
	{ graphics::colors::deep_maroon, "deep_maroon"_s },
	{ graphics::colors::dolphin_gray, "dolphin_gray"_s },
	{ graphics::colors::antique_ruby, "antique_ruby"_s },
	{ graphics::colors::deep_ruby, "deep_ruby"_s },
	{ graphics::colors::battleship_grey, "battleship_grey"_s },
	{ graphics::colors::alien_armpit, "alien_armpit"_s },
	{ graphics::colors::deep_red, "deep_red"_s },
	{ graphics::colors::chinese_violet, "chinese_violet"_s },
	{ graphics::colors::french_bistre, "french_bistre"_s },
	{ graphics::colors::dollar_bill, "dollar_bill"_s },
	{ graphics::colors::french_lilac, "french_lilac"_s },
	{ graphics::colors::dark_raspberry, "dark_raspberry"_s },
	{ graphics::colors::boysenberry, "boysenberry"_s },
	{ graphics::colors::asparagus, "asparagus"_s },
	{ graphics::colors::french_violet, "french_violet"_s },
	{ graphics::colors::dark_brown_tangelo, "dark_brown_tangelo"_s },
	{ graphics::colors::cordovan, "cordovan"_s },
	{ graphics::colors::baby_blue, "baby_blue"_s },
	{ graphics::colors::blue_violet, "blue_violet"_s },
	{ graphics::colors::burnt_umber, "burnt_umber"_s },
	{ graphics::colors::dark_red, "dark_red"_s },
	{ graphics::colors::dark_magenta, "dark_magenta"_s },
	{ graphics::colors::cool_grey, "cool_grey"_s },
	{ graphics::colors::dark_Sky_blue, "dark_Sky_blue"_s },
	{ graphics::colors::appleGreen, "appleGreen"_s },
	{ graphics::colors::electric_violet, "electric_violet"_s },
	{ graphics::colors::artichoke, "artichoke"_s },
	{ graphics::colors::dark_sea_green, "dark_sea_green"_s },
	{ graphics::colors::antique_fuchsia, "antique_fuchsia"_s },
	{ graphics::colors::dark_tan, "dark_tan"_s },
	{ graphics::colors::cadet_grey, "cadet_grey"_s },
	{ graphics::colors::ceil, "ceil"_s },
	{ graphics::colors::dark_violet, "dark_violet"_s },
	{ graphics::colors::chestnut, "chestnut"_s },
	{ graphics::colors::carmine, "carmine"_s },
	{ graphics::colors::brown, "brown"_s },
	{ graphics::colors::coconut, "coconut"_s },
	{ graphics::colors::dark_pastel_purple, "dark_pastel_purple"_s },
	{ graphics::colors::bistre_brown, "bistre_brown"_s },
	{ graphics::colors::eton_blue, "eton_blue"_s },
	{ graphics::colors::dark_chestnut, "dark_chestnut"_s },
	{ graphics::colors::bazaar, "bazaar"_s },
	{ graphics::colors::cinereous, "cinereous"_s },
	{ graphics::colors::crimson_red, "crimson_red"_s },
	{ graphics::colors::dark_orchid, "dark_orchid"_s },
	{ graphics::colors::deep_lilac, "deep_lilac"_s },
	{ graphics::colors::copper_rose, "copper_rose"_s },
	{ graphics::colors::amethyst, "amethyst"_s },
	{ graphics::colors::dirt, "dirt"_s },
	{ graphics::colors::dark_yellow, "dark_yellow"_s },
	{ graphics::colors::big_dip_o_ruby, "big_dip_o_ruby"_s },
	{ graphics::colors::french_lime, "french_lime"_s },
	{ graphics::colors::amaranth_deep_purple, "amaranth_deep_purple"_s },
	{ graphics::colors::beaver, "beaver"_s },
	{ graphics::colors::citron, "citron"_s },
	{ graphics::colors::chamoisee, "chamoisee"_s },
	{ graphics::colors::burnished_brown, "burnished_brown"_s },
	{ graphics::colors::baby_blue_eyes, "baby_blue_eyes"_s },
	{ graphics::colors::flirt, "flirt"_s },
	{ graphics::colors::blue_bell, "blue_bell"_s },
	{ graphics::colors::cambridge_blue, "cambridge_blue"_s },
	{ graphics::colors::dark_candy_apple_red, "dark_candy_apple_red"_s },
	{ graphics::colors::android_green, "android_green"_s },
	{ graphics::colors::auburn, "auburn"_s },
	{ graphics::colors::blast_off_bronze, "blast_off_bronze"_s },
	{ graphics::colors::cafe_au_lait, "cafe_au_lait"_s },
	{ graphics::colors::fresh_air, "fresh_air"_s },
	{ graphics::colors::china_rose, "china_rose"_s },
	{ graphics::colors::deep_carmine, "deep_carmine"_s },
	{ graphics::colors::deep_puce, "deep_puce"_s },
	{ graphics::colors::dark_gray_X11, "dark_gray_X11"_s },
	{ graphics::colors::chinese_red, "chinese_red"_s },
	{ graphics::colors::amaranth_purple, "amaranth_purple"_s },
	{ graphics::colors::english_red, "english_red"_s },
	{ graphics::colors::french_wine, "french_wine"_s },
	{ graphics::colors::celadon, "celadon"_s },
	{ graphics::colors::blizzard_blue, "blizzard_blue"_s },
	{ graphics::colors::copper_penny, "copper_penny"_s },
	{ graphics::colors::alabama_crimson, "alabama_crimson"_s },
	{ graphics::colors::brown_sugar, "brown_sugar"_s },
	{ graphics::colors::acid_green, "acid_green"_s },
	{ graphics::colors::firebrick, "firebrick"_s },
	{ graphics::colors::african_violet, "african_violet"_s },
	{ graphics::colors::ash_grey, "ash_grey"_s },
	{ graphics::colors::celeste, "celeste"_s },
	{ graphics::colors::carnelian, "carnelian"_s },
	{ graphics::colors::english_lavender, "english_lavender"_s },
	{ graphics::colors::fandango, "fandango"_s },
	{ graphics::colors::brass, "brass"_s },
	{ graphics::colors::copper, "copper"_s },
	{ graphics::colors::dark_goldenrod, "dark_goldenrod"_s },
	{ graphics::colors::deep_chestnut, "deep_chestnut"_s },
	{ graphics::colors::diamond, "diamond"_s },
	{ graphics::colors::deer, "deer"_s },
	{ graphics::colors::beau_blue, "beau_blue"_s },
	{ graphics::colors::byzantine, "byzantine"_s },
	{ graphics::colors::dark_khaki, "dark_khaki"_s },
	{ graphics::colors::crimson_glory, "crimson_glory"_s },
	{ graphics::colors::electric_purple, "electric_purple"_s },
	{ graphics::colors::bitter_sweet_shimmer, "bitter_sweet_shimmer"_s },
	{ graphics::colors::bright_lavender, "bright_lavender"_s },
	{ graphics::colors::black_shadows, "black_shadows"_s },
	{ graphics::colors::bitter_lime, "bitter_lime"_s },
	{ graphics::colors::deep_fuchsia, "deep_fuchsia"_s },
	{ graphics::colors::camel, "camel"_s },
	{ graphics::colors::dark_pastel_red, "dark_pastel_red"_s },
	{ graphics::colors::ecru, "ecru"_s },
	{ graphics::colors::bright_maroon, "bright_maroon"_s },
	{ graphics::colors::aztec_gold, "aztec_gold"_s },
	{ graphics::colors::cardinal, "cardinal"_s },
	{ graphics::colors::alloy_orange, "alloy_orange"_s },
	{ graphics::colors::columbia_blue, "columbia_blue"_s },
	{ graphics::colors::dingy_dungeon, "dingy_dungeon"_s },
	{ graphics::colors::french_raspberry, "french_raspberry"_s },
	{ graphics::colors::fuchsia_rose, "fuchsia_rose"_s },
	{ graphics::colors::cedar_chest, "cedar_chest"_s },
	{ graphics::colors::aero_blue, "aero_blue"_s },
	{ graphics::colors::bitter_Lemon, "bitter_Lemon"_s },
	{ graphics::colors::brick_red, "brick_red"_s },
	{ graphics::colors::copper_red, "copper_red"_s },
	{ graphics::colors::boston_university_red, "boston_university_red"_s },
	{ graphics::colors::deep_magenta, "deep_magenta"_s },
	{ graphics::colors::fuchsia_purple, "fuchsia_purple"_s },
	{ graphics::colors::english_vermillion, "english_vermillion"_s },
	{ graphics::colors::dark_terra_cotta, "dark_terra_cotta"_s },
	{ graphics::colors::burnt_orange, "burnt_orange"_s },
	{ graphics::colors::fuzzy_wuzzy, "fuzzy_wuzzy"_s },
	{ graphics::colors::brown_yellow, "brown_yellow"_s },
	{ graphics::colors::electric_lime, "electric_lime"_s },
	{ graphics::colors::dark_coral, "dark_coral"_s },
	{ graphics::colors::cinnamon_satin, "cinnamon_satin"_s },
	{ graphics::colors::bronze, "bronze"_s },
	{ graphics::colors::antique_brass, "antique_brass"_s },
	{ graphics::colors::fire_engine_red, "fire_engine_red"_s },
	{ graphics::colors::arctic_lime, "arctic_lime"_s },
	{ graphics::colors::bright_ube, "bright_ube"_s },
	{ graphics::colors::dark_vanilla, "dark_vanilla"_s },
	{ graphics::colors::chocolate_web, "chocolate_web"_s },
	{ graphics::colors::amaranth_red, "amaranth_red"_s },
	{ graphics::colors::deep_mauve, "deep_mauve"_s },
	{ graphics::colors::carmine_MP, "carmine_MP"_s },
	{ graphics::colors::debian_red, "debian_red"_s },
	{ graphics::colors::dogwood_rose, "dogwood_rose"_s },
	{ graphics::colors::bright_lilac, "bright_lilac"_s },
	{ graphics::colors::deep_cerise, "deep_cerise"_s },
	{ graphics::colors::copper_crayola, "copper_crayola"_s },
	{ graphics::colors::azureish_white, "azureish_white"_s },
	{ graphics::colors::crimson, "crimson"_s },
	{ graphics::colors::booger_buster, "booger_buster"_s },
	{ graphics::colors::cerise, "cerise"_s },
	{ graphics::colors::fandango_pink, "fandango_pink"_s },
	{ graphics::colors::blush, "blush"_s },
	{ graphics::colors::china_pink, "china_pink"_s },
	{ graphics::colors::burlywood, "burlywood"_s },
	{ graphics::colors::chartreuse, "chartreuse"_s },
	{ graphics::colors::barbie_pink, "barbie_pink"_s },
	{ graphics::colors::cg_red, "cg_red"_s },
	{ graphics::colors::earth_yellow, "earth_yellow"_s },
	{ graphics::colors::flame, "flame"_s },
	{ graphics::colors::cadmium_red, "cadmium_red"_s },
	{ graphics::colors::alizarin_crimson, "alizarin_crimson"_s },
	{ graphics::colors::cinnabar, "cinnabar"_s },
	{ graphics::colors::bone, "bone"_s },
	{ graphics::colors::candy_pink, "candy_pink"_s },
	{ graphics::colors::fulvous, "fulvous"_s },
	{ graphics::colors::citrine, "citrine"_s },
	{ graphics::colors::amaranth, "amaranth"_s },
	{ graphics::colors::fawn, "fawn"_s },
	{ graphics::colors::dust_storm, "dust_storm"_s },
	{ graphics::colors::charm_pink, "charm_pink"_s },
	{ graphics::colors::dark_pink, "dark_pink"_s },
	{ graphics::colors::bubbles, "bubbles"_s },
	{ graphics::colors::big_foot_feet, "big_foot_feet"_s },
	{ graphics::colors::frostbite, "frostbite"_s },
	{ graphics::colors::deep_carrot_orange, "deep_carrot_orange"_s },
	{ graphics::colors::burnt_sienna, "burnt_sienna"_s },
	{ graphics::colors::dark_salmon, "dark_salmon"_s },
	{ graphics::colors::arylide_yellow, "arylide_yellow"_s },
	{ graphics::colors::desire, "desire"_s },
	{ graphics::colors::carmine_pink, "carmine_pink"_s },
	{ graphics::colors::cerise_pink, "cerise_pink"_s },
	{ graphics::colors::cadmium_orange, "cadmium_orange"_s },
	{ graphics::colors::carrot_orange, "carrot_orange"_s },
	{ graphics::colors::desert_sand, "desert_sand"_s },
	{ graphics::colors::flax, "flax"_s },
	{ graphics::colors::deep_carmine_pink, "deep_carmine_pink"_s },
	{ graphics::colors::cameo_pink, "cameo_pink"_s },
	{ graphics::colors::almond, "almond"_s },
	{ graphics::colors::dutch_white, "dutch_white"_s },
	{ graphics::colors::buff, "buff"_s },
	{ graphics::colors::dandelion, "dandelion"_s },
	{ graphics::colors::eggshell, "eggshell"_s },
	{ graphics::colors::alice_blue, "alice_blue"_s },
	{ graphics::colors::azure_web, "azure_web"_s },
	{ graphics::colors::amaranth_pink, "amaranth_pink"_s },
	{ graphics::colors::champagne_pink, "champagne_pink"_s },
	{ graphics::colors::alabaster, "alabaster"_s },
	{ graphics::colors::antiflash_white, "antiflash_white"_s },
	{ graphics::colors::fashion_fuchsia, "fashion_fuchsia"_s },
	{ graphics::colors::brilliant_lavender, "brilliant_lavender"_s },
	{ graphics::colors::baby_pink, "baby_pink"_s },
	{ graphics::colors::cyclamen, "cyclamen"_s },
	{ graphics::colors::deep_lemon, "deep_lemon"_s },
	{ graphics::colors::beige, "beige"_s },
	{ graphics::colors::cultured, "cultured"_s },
	{ graphics::colors::french_rose, "french_rose"_s },
	{ graphics::colors::champagne, "champagne"_s },
	{ graphics::colors::flavescent, "flavescent"_s },
	{ graphics::colors::congo_pink, "congo_pink"_s },
	{ graphics::colors::begonia, "begonia"_s },
	{ graphics::colors::deep_champagne, "deep_champagne"_s },
	{ graphics::colors::banana_mania, "banana_mania"_s },
	{ graphics::colors::antique_white, "antique_white"_s },
	{ graphics::colors::blond, "blond"_s },
	{ graphics::colors::brink_pink, "brink_pink"_s },
	{ graphics::colors::classic_rose, "classic_rose"_s },
	{ graphics::colors::apricot, "apricot"_s },
	{ graphics::colors::corn, "corn"_s },
	{ graphics::colors::flamingo_pink, "flamingo_pink"_s },
	{ graphics::colors::french_fuchsia, "french_fuchsia"_s },
	{ graphics::colors::french_pink, "french_pink"_s },
	{ graphics::colors::coral_reef, "coral_reef"_s },
	{ graphics::colors::feldspar, "feldspar"_s },
	{ graphics::colors::aureolin, "aureolin"_s },
	{ graphics::colors::bitter_sweet, "bitter_sweet"_s },
	{ graphics::colors::baby_powder, "baby_powder"_s },
	{ graphics::colors::carmine_red, "carmine_red"_s },
	{ graphics::colors::electric_crimson, "electric_crimson"_s },
	{ graphics::colors::folly, "folly"_s },
	{ graphics::colors::bright_pink, "bright_pink"_s },
	{ graphics::colors::fuchsia, "fuchsia"_s },
	{ graphics::colors::american_rose, "american_rose"_s },
	{ graphics::colors::candy_apple_red, "candy_apple_red"_s },
	{ graphics::colors::deep_pink, "deep_pink"_s },
	{ graphics::colors::ferrari_red, "ferrari_red"_s },
	{ graphics::colors::coquelicot, "coquelicot"_s },
	{ graphics::colors::coral_red, "coral_red"_s },
	{ graphics::colors::fiery_rose, "fiery_rose"_s },
	{ graphics::colors::brilliant_rose, "brilliant_rose"_s },
	{ graphics::colors::fuchsia_pink, "fuchsia_pink"_s },
	{ graphics::colors::amber_deep, "amber_deep"_s },
	{ graphics::colors::coral, "coral"_s },
	{ graphics::colors::dark_orange, "dark_orange"_s },
	{ graphics::colors::baker_miller_pink, "baker_miller_pink"_s },
	{ graphics::colors::deep_saffron, "deep_saffron"_s },
	{ graphics::colors::atomic_tangerine, "atomic_tangerine"_s },
	{ graphics::colors::carnation_pink, "carnation_pink"_s },
	{ graphics::colors::chrome_yellow, "chrome_yellow"_s },
	{ graphics::colors::dark_tangerine, "dark_tangerine"_s },
	{ graphics::colors::bright_yellow_crayola, "bright_yellow_crayola"_s },
	{ graphics::colors::cherry_blossom_pink, "cherry_blossom_pink"_s },
	{ graphics::colors::cotton_candy, "cotton_candy"_s },
	{ graphics::colors::amber, "amber"_s },
	{ graphics::colors::bubble_gum, "bubble_gum"_s },
	{ graphics::colors::deep_peach, "deep_peach"_s },
	{ graphics::colors::cyber_yellow, "cyber_yellow"_s },
	{ graphics::colors::banana_yellow, "banana_yellow"_s },
	{ graphics::colors::bisque, "bisque"_s },
	{ graphics::colors::blanched_almond, "blanched_almond"_s },
	{ graphics::colors::canary_yellow, "canary_yellow"_s },
	{ graphics::colors::cadmium_yellow, "cadmium_yellow"_s },
	{ graphics::colors::cornsilk, "cornsilk"_s },
	{ graphics::colors::cosmic_latte, "cosmic_latte"_s },
	{ graphics::colors::floral_white, "floral_white"_s },
	{ graphics::colors::cream, "cream"_s },
	{ graphics::colors::daffodil, "daffodil"_s },
	{ graphics::colors::electric_yellow, "electric_yellow"_s },
	{ graphics::colors::canary, "canary"_s }
};

static ang_pair<cstr_t, graphics::colors> _parse_color_map[] =
{
	{ "absolute_zero"_s , graphics::colors::absolute_zero },
	{ "acajou"_s , graphics::colors::acajou },
	{ "acid_green"_s , graphics::colors::acid_green },
	{ "aero"_s , graphics::colors::aero },
	{ "aero_blue"_s , graphics::colors::aero_blue },
	{ "african_violet"_s , graphics::colors::african_violet },
	{ "air_force_blue"_s , graphics::colors::air_force_blue },
	{ "air_force_deep_blue"_s , graphics::colors::air_force_deep_blue },
	{ "air_superiority_blue"_s , graphics::colors::air_superiority_blue },
	{ "alabama_crimson"_s , graphics::colors::alabama_crimson },
	{ "alabaster"_s , graphics::colors::alabaster },
	{ "alice_blue"_s , graphics::colors::alice_blue },
	{ "alien_armpit"_s , graphics::colors::alien_armpit },
	{ "alizarin_crimson"_s , graphics::colors::alizarin_crimson },
	{ "alloy_orange"_s , graphics::colors::alloy_orange },
	{ "almond"_s , graphics::colors::almond },
	{ "amaranth"_s , graphics::colors::amaranth },
	{ "amaranth_deep_purple"_s , graphics::colors::amaranth_deep_purple },
	{ "amaranth_pink"_s , graphics::colors::amaranth_pink },
	{ "amaranth_purple"_s , graphics::colors::amaranth_purple },
	{ "amaranth_red"_s , graphics::colors::amaranth_red },
	{ "amazon"_s , graphics::colors::amazon },
	{ "amazonite"_s , graphics::colors::amazonite },
	{ "amber"_s , graphics::colors::amber },
	{ "amber_deep"_s , graphics::colors::amber_deep },
	{ "american_rose"_s , graphics::colors::american_rose },
	{ "amethyst"_s , graphics::colors::amethyst },
	{ "android_green"_s , graphics::colors::android_green },
	{ "antiflash_white"_s , graphics::colors::antiflash_white },
	{ "antique_brass"_s , graphics::colors::antique_brass },
	{ "antique_bronze"_s , graphics::colors::antique_bronze },
	{ "antique_fuchsia"_s , graphics::colors::antique_fuchsia },
	{ "antique_ruby"_s , graphics::colors::antique_ruby },
	{ "antique_white"_s , graphics::colors::antique_white },
	{ "ao"_s , graphics::colors::ao },
	{ "appleGreen"_s , graphics::colors::appleGreen },
	{ "apricot"_s , graphics::colors::apricot },
	{ "aqua"_s , graphics::colors::aqua },
	{ "aquamarine"_s , graphics::colors::aquamarine },
	{ "arctic_lime"_s , graphics::colors::arctic_lime },
	{ "army_green"_s , graphics::colors::army_green },
	{ "arsenic"_s , graphics::colors::arsenic },
	{ "artichoke"_s , graphics::colors::artichoke },
	{ "arylide_yellow"_s , graphics::colors::arylide_yellow },
	{ "ash_grey"_s , graphics::colors::ash_grey },
	{ "asparagus"_s , graphics::colors::asparagus },
	{ "atomic_tangerine"_s , graphics::colors::atomic_tangerine },
	{ "auburn"_s , graphics::colors::auburn },
	{ "aureolin"_s , graphics::colors::aureolin },
	{ "auro_metal_saurus"_s , graphics::colors::auro_metal_saurus },
	{ "avocado"_s , graphics::colors::avocado },
	{ "aztec_gold"_s , graphics::colors::aztec_gold },
	{ "azure"_s , graphics::colors::azure },
	{ "azure_mist"_s , graphics::colors::azure_mist },
	{ "azure_web"_s , graphics::colors::azure_web },
	{ "azureish_white"_s , graphics::colors::azureish_white },
	{ "baby_blue"_s , graphics::colors::baby_blue },
	{ "baby_blue_eyes"_s , graphics::colors::baby_blue_eyes },
	{ "baby_pink"_s , graphics::colors::baby_pink },
	{ "baby_powder"_s , graphics::colors::baby_powder },
	{ "baker_miller_pink"_s , graphics::colors::baker_miller_pink },
	{ "ball_blue"_s , graphics::colors::ball_blue },
	{ "banana_mania"_s , graphics::colors::banana_mania },
	{ "banana_yellow"_s , graphics::colors::banana_yellow },
	{ "bangladesh_green"_s , graphics::colors::bangladesh_green },
	{ "barbie_pink"_s , graphics::colors::barbie_pink },
	{ "barn_red"_s , graphics::colors::barn_red },
	{ "battery_charged_blue"_s , graphics::colors::battery_charged_blue },
	{ "battleship_grey"_s , graphics::colors::battleship_grey },
	{ "bazaar"_s , graphics::colors::bazaar },
	{ "bdazzled_blue"_s , graphics::colors::bdazzled_blue },
	{ "beau_blue"_s , graphics::colors::beau_blue },
	{ "beaver"_s , graphics::colors::beaver },
	{ "begonia"_s , graphics::colors::begonia },
	{ "beige"_s , graphics::colors::beige },
	{ "big_dip_o_ruby"_s , graphics::colors::big_dip_o_ruby },
	{ "big_foot_feet"_s , graphics::colors::big_foot_feet },
	{ "bisque"_s , graphics::colors::bisque },
	{ "bistre"_s , graphics::colors::bistre },
	{ "bistre_brown"_s , graphics::colors::bistre_brown },
	{ "bitter_Lemon"_s , graphics::colors::bitter_Lemon },
	{ "bitter_lime"_s , graphics::colors::bitter_lime },
	{ "bitter_sweet"_s , graphics::colors::bitter_sweet },
	{ "bitter_sweet_shimmer"_s , graphics::colors::bitter_sweet_shimmer },
	{ "black"_s , graphics::colors::black },
	{ "black_bean"_s , graphics::colors::black_bean },
	{ "black_coral"_s , graphics::colors::black_coral },
	{ "black_leather_jacket"_s , graphics::colors::black_leather_jacket },
	{ "black_olive"_s , graphics::colors::black_olive },
	{ "black_shadows"_s , graphics::colors::black_shadows },
	{ "blanched_almond"_s , graphics::colors::blanched_almond },
	{ "blast_off_bronze"_s , graphics::colors::blast_off_bronze },
	{ "bleu_de_france"_s , graphics::colors::bleu_de_france },
	{ "blizzard_blue"_s , graphics::colors::blizzard_blue },
	{ "blond"_s , graphics::colors::blond },
	{ "blue"_s , graphics::colors::blue },
	{ "blue_NCS"_s , graphics::colors::blue_NCS },
	{ "blue_RYB"_s , graphics::colors::blue_RYB },
	{ "blue_bell"_s , graphics::colors::blue_bell },
	{ "blue_bolt"_s , graphics::colors::blue_bolt },
	{ "blue_crayola"_s , graphics::colors::blue_crayola },
	{ "blue_gray"_s , graphics::colors::blue_gray },
	{ "blue_green"_s , graphics::colors::blue_green },
	{ "blue_jeans"_s , graphics::colors::blue_jeans },
	{ "blue_lagoon"_s , graphics::colors::blue_lagoon },
	{ "blue_magenta_violet"_s , graphics::colors::blue_magenta_violet },
	{ "blue_munsell"_s , graphics::colors::blue_munsell },
	{ "blue_pantone"_s , graphics::colors::blue_pantone },
	{ "blue_pigment"_s , graphics::colors::blue_pigment },
	{ "blue_sapphire"_s , graphics::colors::blue_sapphire },
	{ "blue_violet"_s , graphics::colors::blue_violet },
	{ "blue_yonder"_s , graphics::colors::blue_yonder },
	{ "blueberry"_s , graphics::colors::blueberry },
	{ "bluebonnet"_s , graphics::colors::bluebonnet },
	{ "blush"_s , graphics::colors::blush },
	{ "bole"_s , graphics::colors::bole },
	{ "bondi_blue"_s , graphics::colors::bondi_blue },
	{ "bone"_s , graphics::colors::bone },
	{ "booger_buster"_s , graphics::colors::booger_buster },
	{ "boston_university_red"_s , graphics::colors::boston_university_red },
	{ "bottle_green"_s , graphics::colors::bottle_green },
	{ "boysenberry"_s , graphics::colors::boysenberry },
	{ "brandeis_blue"_s , graphics::colors::brandeis_blue },
	{ "brass"_s , graphics::colors::brass },
	{ "brick_red"_s , graphics::colors::brick_red },
	{ "bright_cerulean"_s , graphics::colors::bright_cerulean },
	{ "bright_green"_s , graphics::colors::bright_green },
	{ "bright_lavender"_s , graphics::colors::bright_lavender },
	{ "bright_lilac"_s , graphics::colors::bright_lilac },
	{ "bright_maroon"_s , graphics::colors::bright_maroon },
	{ "bright_navy_blue"_s , graphics::colors::bright_navy_blue },
	{ "bright_pink"_s , graphics::colors::bright_pink },
	{ "bright_turquoise"_s , graphics::colors::bright_turquoise },
	{ "bright_ube"_s , graphics::colors::bright_ube },
	{ "bright_yellow_crayola"_s , graphics::colors::bright_yellow_crayola },
	{ "brilliant_azure"_s , graphics::colors::brilliant_azure },
	{ "brilliant_lavender"_s , graphics::colors::brilliant_lavender },
	{ "brilliant_rose"_s , graphics::colors::brilliant_rose },
	{ "brink_pink"_s , graphics::colors::brink_pink },
	{ "british_racing_green"_s , graphics::colors::british_racing_green },
	{ "bronze"_s , graphics::colors::bronze },
	{ "bronze_yellow"_s , graphics::colors::bronze_yellow },
	{ "brown"_s , graphics::colors::brown },
	{ "brown_nose"_s , graphics::colors::brown_nose },
	{ "brown_sugar"_s , graphics::colors::brown_sugar },
	{ "brown_web"_s , graphics::colors::brown_web },
	{ "brown_yellow"_s , graphics::colors::brown_yellow },
	{ "brunswick_green"_s , graphics::colors::brunswick_green },
	{ "bubble_gum"_s , graphics::colors::bubble_gum },
	{ "bubbles"_s , graphics::colors::bubbles },
	{ "bud_green"_s , graphics::colors::bud_green },
	{ "buff"_s , graphics::colors::buff },
	{ "bulgarian_rose"_s , graphics::colors::bulgarian_rose },
	{ "burgundy"_s , graphics::colors::burgundy },
	{ "burlywood"_s , graphics::colors::burlywood },
	{ "burnished_brown"_s , graphics::colors::burnished_brown },
	{ "burnt_orange"_s , graphics::colors::burnt_orange },
	{ "burnt_sienna"_s , graphics::colors::burnt_sienna },
	{ "burnt_umber"_s , graphics::colors::burnt_umber },
	{ "byzantine"_s , graphics::colors::byzantine },
	{ "byzantium"_s , graphics::colors::byzantium },
	{ "cadet"_s , graphics::colors::cadet },
	{ "cadet_blue"_s , graphics::colors::cadet_blue },
	{ "cadet_grey"_s , graphics::colors::cadet_grey },
	{ "cadmium_green"_s , graphics::colors::cadmium_green },
	{ "cadmium_orange"_s , graphics::colors::cadmium_orange },
	{ "cadmium_red"_s , graphics::colors::cadmium_red },
	{ "cadmium_yellow"_s , graphics::colors::cadmium_yellow },
	{ "cafe_au_lait"_s , graphics::colors::cafe_au_lait },
	{ "cafe_noir"_s , graphics::colors::cafe_noir },
	{ "cal_poly_green"_s , graphics::colors::cal_poly_green },
	{ "cambridge_blue"_s , graphics::colors::cambridge_blue },
	{ "camel"_s , graphics::colors::camel },
	{ "cameo_pink"_s , graphics::colors::cameo_pink },
	{ "camouflage_green"_s , graphics::colors::camouflage_green },
	{ "canary"_s , graphics::colors::canary },
	{ "canary_yellow"_s , graphics::colors::canary_yellow },
	{ "candy_apple_red"_s , graphics::colors::candy_apple_red },
	{ "candy_pink"_s , graphics::colors::candy_pink },
	{ "capri"_s , graphics::colors::capri },
	{ "caput_mortuum"_s , graphics::colors::caput_mortuum },
	{ "cardinal"_s , graphics::colors::cardinal },
	{ "caribbean_green"_s , graphics::colors::caribbean_green },
	{ "carmine"_s , graphics::colors::carmine },
	{ "carmine_MP"_s , graphics::colors::carmine_MP },
	{ "carmine_pink"_s , graphics::colors::carmine_pink },
	{ "carmine_red"_s , graphics::colors::carmine_red },
	{ "carnation_pink"_s , graphics::colors::carnation_pink },
	{ "carnelian"_s , graphics::colors::carnelian },
	{ "carolina_blue"_s , graphics::colors::carolina_blue },
	{ "carrot_orange"_s , graphics::colors::carrot_orange },
	{ "castleton_green"_s , graphics::colors::castleton_green },
	{ "catalina_blue"_s , graphics::colors::catalina_blue },
	{ "catawba"_s , graphics::colors::catawba },
	{ "cedar_chest"_s , graphics::colors::cedar_chest },
	{ "ceil"_s , graphics::colors::ceil },
	{ "celadon"_s , graphics::colors::celadon },
	{ "celadon_blue"_s , graphics::colors::celadon_blue },
	{ "celadon_green"_s , graphics::colors::celadon_green },
	{ "celeste"_s , graphics::colors::celeste },
	{ "celestial_blue"_s , graphics::colors::celestial_blue },
	{ "cerise"_s , graphics::colors::cerise },
	{ "cerise_pink"_s , graphics::colors::cerise_pink },
	{ "cerulean"_s , graphics::colors::cerulean },
	{ "cerulean_blue"_s , graphics::colors::cerulean_blue },
	{ "cerulean_frost"_s , graphics::colors::cerulean_frost },
	{ "cg_blue"_s , graphics::colors::cg_blue },
	{ "cg_red"_s , graphics::colors::cg_red },
	{ "chamoisee"_s , graphics::colors::chamoisee },
	{ "champagne"_s , graphics::colors::champagne },
	{ "champagne_pink"_s , graphics::colors::champagne_pink },
	{ "charcoal"_s , graphics::colors::charcoal },
	{ "charleston_green"_s , graphics::colors::charleston_green },
	{ "charm_pink"_s , graphics::colors::charm_pink },
	{ "chartreuse"_s , graphics::colors::chartreuse },
	{ "chartreuse_web"_s , graphics::colors::chartreuse_web },
	{ "cherry"_s , graphics::colors::cherry },
	{ "cherry_blossom_pink"_s , graphics::colors::cherry_blossom_pink },
	{ "chestnut"_s , graphics::colors::chestnut },
	{ "china_pink"_s , graphics::colors::china_pink },
	{ "china_rose"_s , graphics::colors::china_rose },
	{ "chinese_red"_s , graphics::colors::chinese_red },
	{ "chinese_violet"_s , graphics::colors::chinese_violet },
	{ "chlorophyll_green"_s , graphics::colors::chlorophyll_green },
	{ "chocolate"_s , graphics::colors::chocolate },
	{ "chocolate_web"_s , graphics::colors::chocolate_web },
	{ "chrome_yellow"_s , graphics::colors::chrome_yellow },
	{ "cinereous"_s , graphics::colors::cinereous },
	{ "cinnabar"_s , graphics::colors::cinnabar },
	{ "cinnamon"_s , graphics::colors::cinnamon },
	{ "cinnamon_satin"_s , graphics::colors::cinnamon_satin },
	{ "citrine"_s , graphics::colors::citrine },
	{ "citron"_s , graphics::colors::citron },
	{ "claret"_s , graphics::colors::claret },
	{ "classic_rose"_s , graphics::colors::classic_rose },
	{ "cobalt_blue"_s , graphics::colors::cobalt_blue },
	{ "cocoa_brown"_s , graphics::colors::cocoa_brown },
	{ "coconut"_s , graphics::colors::coconut },
	{ "coffee"_s , graphics::colors::coffee },
	{ "columbia_blue"_s , graphics::colors::columbia_blue },
	{ "congo_pink"_s , graphics::colors::congo_pink },
	{ "cool_black"_s , graphics::colors::cool_black },
	{ "cool_grey"_s , graphics::colors::cool_grey },
	{ "copper"_s , graphics::colors::copper },
	{ "copper_crayola"_s , graphics::colors::copper_crayola },
	{ "copper_penny"_s , graphics::colors::copper_penny },
	{ "copper_red"_s , graphics::colors::copper_red },
	{ "copper_rose"_s , graphics::colors::copper_rose },
	{ "coquelicot"_s , graphics::colors::coquelicot },
	{ "coral"_s , graphics::colors::coral },
	{ "coral_pink"_s , graphics::colors::coral_pink },
	{ "coral_red"_s , graphics::colors::coral_red },
	{ "coral_reef"_s , graphics::colors::coral_reef },
	{ "cordovan"_s , graphics::colors::cordovan },
	{ "corn"_s , graphics::colors::corn },
	{ "cornell_red"_s , graphics::colors::cornell_red },
	{ "cornflower_blue"_s , graphics::colors::cornflower_blue },
	{ "cornsilk"_s , graphics::colors::cornsilk },
	{ "cosmic_cobalt"_s , graphics::colors::cosmic_cobalt },
	{ "cosmic_latte"_s , graphics::colors::cosmic_latte },
	{ "cotton_candy"_s , graphics::colors::cotton_candy },
	{ "coyote_brown"_s , graphics::colors::coyote_brown },
	{ "cream"_s , graphics::colors::cream },
	{ "crimson"_s , graphics::colors::crimson },
	{ "crimson_glory"_s , graphics::colors::crimson_glory },
	{ "crimson_red"_s , graphics::colors::crimson_red },
	{ "cultured"_s , graphics::colors::cultured },
	{ "cyan"_s , graphics::colors::cyan },
	{ "cyan_azure"_s , graphics::colors::cyan_azure },
	{ "cyan_blue_azure"_s , graphics::colors::cyan_blue_azure },
	{ "cyan_cobalt_blue"_s , graphics::colors::cyan_cobalt_blue },
	{ "cyan_cornflower_blue"_s , graphics::colors::cyan_cornflower_blue },
	{ "cyan_process"_s , graphics::colors::cyan_process },
	{ "cyber_grape"_s , graphics::colors::cyber_grape },
	{ "cyber_yellow"_s , graphics::colors::cyber_yellow },
	{ "cyclamen"_s , graphics::colors::cyclamen },
	{ "daffodil"_s , graphics::colors::daffodil },
	{ "dandelion"_s , graphics::colors::dandelion },
	{ "dark_Sky_blue"_s , graphics::colors::dark_Sky_blue },
	{ "dark_blue"_s , graphics::colors::dark_blue },
	{ "dark_blue_gray"_s , graphics::colors::dark_blue_gray },
	{ "dark_brown"_s , graphics::colors::dark_brown },
	{ "dark_brown_tangelo"_s , graphics::colors::dark_brown_tangelo },
	{ "dark_byzantium"_s , graphics::colors::dark_byzantium },
	{ "dark_candy_apple_red"_s , graphics::colors::dark_candy_apple_red },
	{ "dark_cerulean"_s , graphics::colors::dark_cerulean },
	{ "dark_chestnut"_s , graphics::colors::dark_chestnut },
	{ "dark_coral"_s , graphics::colors::dark_coral },
	{ "dark_cyan"_s , graphics::colors::dark_cyan },
	{ "dark_electric_blue"_s , graphics::colors::dark_electric_blue },
	{ "dark_goldenrod"_s , graphics::colors::dark_goldenrod },
	{ "dark_gray_X11"_s , graphics::colors::dark_gray_X11 },
	{ "dark_green"_s , graphics::colors::dark_green },
	{ "dark_green_X11"_s , graphics::colors::dark_green_X11 },
	{ "dark_gunmetal"_s , graphics::colors::dark_gunmetal },
	{ "dark_imperial_blue"_s , graphics::colors::dark_imperial_blue },
	{ "dark_imperial_blue_web"_s , graphics::colors::dark_imperial_blue_web },
	{ "dark_jungle_green"_s , graphics::colors::dark_jungle_green },
	{ "dark_khaki"_s , graphics::colors::dark_khaki },
	{ "dark_lava"_s , graphics::colors::dark_lava },
	{ "dark_lavender"_s , graphics::colors::dark_lavender },
	{ "dark_liver"_s , graphics::colors::dark_liver },
	{ "dark_liver_horses"_s , graphics::colors::dark_liver_horses },
	{ "dark_magenta"_s , graphics::colors::dark_magenta },
	{ "dark_medium_gray"_s , graphics::colors::dark_medium_gray },
	{ "dark_midnight_blue"_s , graphics::colors::dark_midnight_blue },
	{ "dark_moss_green"_s , graphics::colors::dark_moss_green },
	{ "dark_olive_green"_s , graphics::colors::dark_olive_green },
	{ "dark_orange"_s , graphics::colors::dark_orange },
	{ "dark_orchid"_s , graphics::colors::dark_orchid },
	{ "dark_pastel_blue"_s , graphics::colors::dark_pastel_blue },
	{ "dark_pastel_green"_s , graphics::colors::dark_pastel_green },
	{ "dark_pastel_purple"_s , graphics::colors::dark_pastel_purple },
	{ "dark_pastel_red"_s , graphics::colors::dark_pastel_red },
	{ "dark_pink"_s , graphics::colors::dark_pink },
	{ "dark_powder_blue"_s , graphics::colors::dark_powder_blue },
	{ "dark_puce"_s , graphics::colors::dark_puce },
	{ "dark_purple"_s , graphics::colors::dark_purple },
	{ "dark_raspberry"_s , graphics::colors::dark_raspberry },
	{ "dark_red"_s , graphics::colors::dark_red },
	{ "dark_salmon"_s , graphics::colors::dark_salmon },
	{ "dark_scarlet"_s , graphics::colors::dark_scarlet },
	{ "dark_sea_green"_s , graphics::colors::dark_sea_green },
	{ "dark_sienna"_s , graphics::colors::dark_sienna },
	{ "dark_slate_blue"_s , graphics::colors::dark_slate_blue },
	{ "dark_slate_gray"_s , graphics::colors::dark_slate_gray },
	{ "dark_spring_green"_s , graphics::colors::dark_spring_green },
	{ "dark_tan"_s , graphics::colors::dark_tan },
	{ "dark_tangerine"_s , graphics::colors::dark_tangerine },
	{ "dark_taupe"_s , graphics::colors::dark_taupe },
	{ "dark_terra_cotta"_s , graphics::colors::dark_terra_cotta },
	{ "dark_turquoise"_s , graphics::colors::dark_turquoise },
	{ "dark_vanilla"_s , graphics::colors::dark_vanilla },
	{ "dark_violet"_s , graphics::colors::dark_violet },
	{ "dark_yellow"_s , graphics::colors::dark_yellow },
	{ "dartmouth_green"_s , graphics::colors::dartmouth_green },
	{ "davy_grey"_s , graphics::colors::davy_grey },
	{ "debian_red"_s , graphics::colors::debian_red },
	{ "deep_aquamarine"_s , graphics::colors::deep_aquamarine },
	{ "deep_carmine"_s , graphics::colors::deep_carmine },
	{ "deep_carmine_pink"_s , graphics::colors::deep_carmine_pink },
	{ "deep_carrot_orange"_s , graphics::colors::deep_carrot_orange },
	{ "deep_cerise"_s , graphics::colors::deep_cerise },
	{ "deep_champagne"_s , graphics::colors::deep_champagne },
	{ "deep_chestnut"_s , graphics::colors::deep_chestnut },
	{ "deep_coffee"_s , graphics::colors::deep_coffee },
	{ "deep_fuchsia"_s , graphics::colors::deep_fuchsia },
	{ "deep_green"_s , graphics::colors::deep_green },
	{ "deep_green_cyan_turquoise"_s , graphics::colors::deep_green_cyan_turquoise },
	{ "deep_jungle_green"_s , graphics::colors::deep_jungle_green },
	{ "deep_koamaru"_s , graphics::colors::deep_koamaru },
	{ "deep_lemon"_s , graphics::colors::deep_lemon },
	{ "deep_lilac"_s , graphics::colors::deep_lilac },
	{ "deep_magenta"_s , graphics::colors::deep_magenta },
	{ "deep_maroon"_s , graphics::colors::deep_maroon },
	{ "deep_mauve"_s , graphics::colors::deep_mauve },
	{ "deep_moss_green"_s , graphics::colors::deep_moss_green },
	{ "deep_peach"_s , graphics::colors::deep_peach },
	{ "deep_pink"_s , graphics::colors::deep_pink },
	{ "deep_puce"_s , graphics::colors::deep_puce },
	{ "deep_red"_s , graphics::colors::deep_red },
	{ "deep_ruby"_s , graphics::colors::deep_ruby },
	{ "deep_saffron"_s , graphics::colors::deep_saffron },
	{ "deep_sky_blue"_s , graphics::colors::deep_sky_blue },
	{ "deep_space_sparkle"_s , graphics::colors::deep_space_sparkle },
	{ "deep_spring_bud"_s , graphics::colors::deep_spring_bud },
	{ "deep_taupe"_s , graphics::colors::deep_taupe },
	{ "deep_tuscan_red"_s , graphics::colors::deep_tuscan_red },
	{ "deep_violet"_s , graphics::colors::deep_violet },
	{ "deer"_s , graphics::colors::deer },
	{ "denim"_s , graphics::colors::denim },
	{ "denim_blue"_s , graphics::colors::denim_blue },
	{ "desaturated_cyan"_s , graphics::colors::desaturated_cyan },
	{ "desert"_s , graphics::colors::desert },
	{ "desert_sand"_s , graphics::colors::desert_sand },
	{ "desire"_s , graphics::colors::desire },
	{ "diamond"_s , graphics::colors::diamond },
	{ "dim_gray"_s , graphics::colors::dim_gray },
	{ "dingy_dungeon"_s , graphics::colors::dingy_dungeon },
	{ "dirt"_s , graphics::colors::dirt },
	{ "dodger_blue"_s , graphics::colors::dodger_blue },
	{ "dogwood_rose"_s , graphics::colors::dogwood_rose },
	{ "dollar_bill"_s , graphics::colors::dollar_bill },
	{ "dolphin_gray"_s , graphics::colors::dolphin_gray },
	{ "donkeyBrown"_s , graphics::colors::donkeyBrown },
	{ "drab"_s , graphics::colors::drab },
	{ "duke_blue"_s , graphics::colors::duke_blue },
	{ "dust_storm"_s , graphics::colors::dust_storm },
	{ "dutch_white"_s , graphics::colors::dutch_white },
	{ "eallow"_s , graphics::colors::eallow },
	{ "ealu_red"_s , graphics::colors::ealu_red },
	{ "earth_yellow"_s , graphics::colors::earth_yellow },
	{ "ebony"_s , graphics::colors::ebony },
	{ "ecru"_s , graphics::colors::ecru },
	{ "eerie_black"_s , graphics::colors::eerie_black },
	{ "eggplant"_s , graphics::colors::eggplant },
	{ "eggshell"_s , graphics::colors::eggshell },
	{ "egyptian_blue"_s , graphics::colors::egyptian_blue },
	{ "electric_blue"_s , graphics::colors::electric_blue },
	{ "electric_crimson"_s , graphics::colors::electric_crimson },
	{ "electric_cyan"_s , graphics::colors::electric_cyan },
	{ "electric_green"_s , graphics::colors::electric_green },
	{ "electric_indigo"_s , graphics::colors::electric_indigo },
	{ "electric_lavender"_s , graphics::colors::electric_lavender },
	{ "electric_lime"_s , graphics::colors::electric_lime },
	{ "electric_purple"_s , graphics::colors::electric_purple },
	{ "electric_ultramarine"_s , graphics::colors::electric_ultramarine },
	{ "electric_violet"_s , graphics::colors::electric_violet },
	{ "electric_yellow"_s , graphics::colors::electric_yellow },
	{ "emerald"_s , graphics::colors::emerald },
	{ "eminence"_s , graphics::colors::eminence },
	{ "english_green"_s , graphics::colors::english_green },
	{ "english_lavender"_s , graphics::colors::english_lavender },
	{ "english_red"_s , graphics::colors::english_red },
	{ "english_vermillion"_s , graphics::colors::english_vermillion },
	{ "english_violet"_s , graphics::colors::english_violet },
	{ "eton_blue"_s , graphics::colors::eton_blue },
	{ "eucalyptus"_s , graphics::colors::eucalyptus },
	{ "fandango"_s , graphics::colors::fandango },
	{ "fandango_pink"_s , graphics::colors::fandango_pink },
	{ "fashion_fuchsia"_s , graphics::colors::fashion_fuchsia },
	{ "fawn"_s , graphics::colors::fawn },
	{ "feldgrau"_s , graphics::colors::feldgrau },
	{ "feldspar"_s , graphics::colors::feldspar },
	{ "fern_green"_s , graphics::colors::fern_green },
	{ "ferrari_red"_s , graphics::colors::ferrari_red },
	{ "field_drab"_s , graphics::colors::field_drab },
	{ "fiery_rose"_s , graphics::colors::fiery_rose },
	{ "fire_engine_red"_s , graphics::colors::fire_engine_red },
	{ "firebrick"_s , graphics::colors::firebrick },
	{ "flame"_s , graphics::colors::flame },
	{ "flamingo_pink"_s , graphics::colors::flamingo_pink },
	{ "flattery"_s , graphics::colors::flattery },
	{ "flavescent"_s , graphics::colors::flavescent },
	{ "flax"_s , graphics::colors::flax },
	{ "flirt"_s , graphics::colors::flirt },
	{ "floral_white"_s , graphics::colors::floral_white },
	{ "fluorescent_orange"_s , graphics::colors::fluorescent_orange },
	{ "fluorescent_pink"_s , graphics::colors::fluorescent_pink },
	{ "fluorescent_yellow"_s , graphics::colors::fluorescent_yellow },
	{ "folly"_s , graphics::colors::folly },
	{ "forest_green"_s , graphics::colors::forest_green },
	{ "forest_greenWeb"_s , graphics::colors::forest_greenWeb },
	{ "french_beige"_s , graphics::colors::french_beige },
	{ "french_bistre"_s , graphics::colors::french_bistre },
	{ "french_blue"_s , graphics::colors::french_blue },
	{ "french_fuchsia"_s , graphics::colors::french_fuchsia },
	{ "french_lilac"_s , graphics::colors::french_lilac },
	{ "french_lime"_s , graphics::colors::french_lime },
	{ "french_mauve"_s , graphics::colors::french_mauve },
	{ "french_pink"_s , graphics::colors::french_pink },
	{ "french_plum"_s , graphics::colors::french_plum },
	{ "french_puce"_s , graphics::colors::french_puce },
	{ "french_raspberry"_s , graphics::colors::french_raspberry },
	{ "french_rose"_s , graphics::colors::french_rose },
	{ "french_sky_blue"_s , graphics::colors::french_sky_blue },
	{ "french_violet"_s , graphics::colors::french_violet },
	{ "french_wine"_s , graphics::colors::french_wine },
	{ "fresh_air"_s , graphics::colors::fresh_air },
	{ "frostbite"_s , graphics::colors::frostbite },
	{ "fuchsia"_s , graphics::colors::fuchsia },
	{ "fuchsia_crayola"_s , graphics::colors::fuchsia_crayola },
	{ "fuchsia_pink"_s , graphics::colors::fuchsia_pink },
	{ "fuchsia_purple"_s , graphics::colors::fuchsia_purple },
	{ "fuchsia_rose"_s , graphics::colors::fuchsia_rose },
	{ "fulvous"_s , graphics::colors::fulvous },
	{ "fuzzy_wuzzy"_s , graphics::colors::fuzzy_wuzzy }
};

using namespace ang::graphics;

ANG_IMPLEMENT_ENUM(ang::graphics, graph_driver_type, byte, graph_driver_type::null);
ANG_IMPLEMENT_ENUM(ang::graphics, primitive, byte, primitive::point);

graphics::color::color()
{
	code = 0;
}

graphics::color::color(const color& c)
{
	code = c.code;
}

graphics::color::color(enum_t c)
{
	code = c;
}

graphics::color::color(dword rgba)
{
	code = rgba;
}

graphics::color::color(byte r, byte g, byte b, byte a)
{
	components.red = r;
	components.green = g;
	components.blue = b;
	components.alpha = a;
}

graphics::color::~color() {}

void  graphics::color::parse(cstr_t cstr)
{
	if (cstr[0] == '#')
	{
		code = interger::parse(&cstr[1], 16);
	}
	else
	{
		wsize idx = ang_binary_search<cstr_t>(cstr, _parse_color_map);
		if (idx > array_size(_parse_color_map))
			code = 0;
		else
			code = _parse_color_map[idx].value;
	}
}

void graphics::color::parse(cwstr_t cstr)
{
	if (cstr[0] == '#')
	{
		code = interger::parse(&cstr[1], 16);
	}
	else
	{
		wsize idx = ang_binary_search<cwstr_t>(cstr, _parse_color_map);
		if (idx > array_size(_parse_color_map))
			code = 0;
		else
			code = _parse_color_map[idx].value;
	}
}

string_t graphics::color::to_string()const
{
	wsize idx = ang_binary_search(code, to_string_color_map);
	if (idx > array_size(to_string_color_map))
	{
		string out = ""_s;
		out->format("#%08x"_s, code);
		return move(out);
	}
	else
	{
		return to_string_color_map[idx].value;
	}
}

graphics::color& graphics::color::operator = (const graphics::color& c)
{
	code = c.code;
	return*this;
}

graphics::color& graphics::color::operator = (graphics::colors c)
{
	code = c;
	return*this;
}

graphics::color& graphics::color::operator = (dword rgba)
{
	code = rgba;
	return*this;
}