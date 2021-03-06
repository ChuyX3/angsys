/*********************************************************************************************************************/
/*   File Name: ang/base/defines.h                                                                                   */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANG_BASE_H__
#error ang/base/base.h is not included
#elif !defined __ANG_BASE_DEFINES_H__
#define __ANG_BASE_DEFINES_H__

#ifdef  _MSC_VER 

#define ANG_EXPAND_ARGS(...) ANG_EXPAND(__VA_ARGS__)
#define ANG_EXPAND(x) x
#define ANG_CATAB(A, B) ANG_CATAB_(A, B)
#define ANG_CATAB_(A, B) A##B
#define ANG_CATABC(A, B, C) ANG_CATABC_(A, B, C)
#define ANG_CATABC_(A, B, C) A##B##C
#define ANG_CATABCD(A, B, C, D) ANG_CATABCD_(A, B, C, D)
#define ANG_CATABCD_(A, B, C, D) A##B##C##D

#define ANG_NUM_ARGS_EXPAND__(...) unused, __VA_ARGS__
#define ANG_NUM_ARGS(...)	ANG_EXPAND(ANG_NUM_ARGS_(ANG_NUM_ARGS_EXPAND__(__VA_ARGS__)))
#define ANG_NUM_ARGS_(...)	ANG_EXPAND(ANG_NUM_ARGS__( __VA_ARGS__, ANG_PP_RSEQ_N()))
#define ANG_NUM_ARGS__(...)	ANG_EXPAND(ANG_PPARG_N(__VA_ARGS__))

#define ANG_PPARG_N(\
        _1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,\
		_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39,_40,\
		_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,_51,_52,_53,_54,_55,_56,_57,_58,_59,_60,\
		_61,_62,_63,_64,_65,_66,_67,_68,_69,_70,_71,_72,_73,_74,_75,_76,_77,_78,_79,_80,\
		_81,_82,_83,_84,_85,_86,_87,_88,_89,_90,_91,_92,_93,_94,_95,_96,_97,_98,_99,_100,\
		N,...)	N

#define ANG_PP_RSEQ_N() \
		99,98,97,96,95,94,93,92,91,90,89,88,87,86,85,84,83,82,81,\
		80,79,78,77,76,75,74,73,72,71,70,69,68,67,66,65,64,63,62,61, \
		60,59,58,57,56,55,54,53,52,51,50,49,48,47,46,45,44,43,42,41,\
		40,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25,24,23,22,21,\
		20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0

#else

#define ANG_EXPAND(...) __VA_ARGS__
#define ANG_CATAB(A, B) ANG_CATAB_(A, B)
#define ANG_CATAB_(A, B) A##B

#define ANG_PPARG_N(\
        _1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,\
		_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39,_40,\
		_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,_51,_52,_53,_54,_55,_56,_57,_58,_59,_60,\
		_61,_62,_63,_64,_65,_66,_67,_68,_69,_70,_71,_72,_73,_74,_75,_76,_77,_78,_79,_80,\
		_81,_82,_83,_84,_85,_86,_87,_88,_89,_90,_91,_92,_93,_94,_95,_96,_97,_98,_99,_100,\
		N,...)	N

#define ANG_PP_RSEQ_N() \
		99,98,97,96,95,94,93,92,91,90,89,88,87,86,85,84,83,82,81, \
		80,79,78,77,76,75,74,73,72,71,70,69,68,67,66,65,64,63,62,61, \
		60,59,58,57,56,55,54,53,52,51,50,49,48,47,46,45,44,43,42,41, \
		40,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25,24,23,22,21, \
		20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0


#define ANG_NUM_ARGS_EXPAND__(...) unused,##__VA_ARGS__
#define ANG_NUM_ARGS_(...)	ANG_PPARG_N( __VA_ARGS__)
#define ANG_NUM_ARGS(...)	ANG_NUM_ARGS_(ANG_NUM_ARGS_EXPAND__(__VA_ARGS__), ANG_PP_RSEQ_N())

#endif


#include <ang/base/inline/APPLY_FUNCX_N.inl>

#define APPLY_FUNCX_(FUNCX, SEPARATOR, M, ...) ANG_EXPAND(M(FUNCX, SEPARATOR, __VA_ARGS__))
#define APPLY_FUNCX_N(FUNCX, SEPARATOR, ...) ANG_EXPAND(APPLY_FUNCX_(FUNCX, SEPARATOR, ANG_CATAB(APPLY_FUNCX, ANG_NUM_ARGS(__VA_ARGS__)), __VA_ARGS__ ))

#define DECLARE_VARS0() 
#define DECLARE_VARS1(A0) A0 arg0;
#define DECLARE_VARS2(A0,A1) A0 arg0; A1 arg1;
#define DECLARE_VARS3(A0,A1,A2) A0 arg0; A1 arg1; A2 arg2;
#define DECLARE_VARS4(A0,A1,A2,A3) A0 arg0; A1 arg1; A2 arg2; A3 arg3;
#define DECLARE_VARS5(A0,A1,A2,A3,A4) A0 arg0; A1 arg1; A2 arg2; A3 arg3; A4 arg4;
#define DECLARE_VARS6(A0,A1,A2,A3,A4,A5) A0 arg0; A1 arg1; A2 arg2; A3 arg3; A4 arg4; A5 arg5;
#define DECLARE_VARS7(A0,A1,A2,A3,A4,A5,A6) A0 arg0; A1 arg1; A2 arg2; A3 arg3; A4 arg4; A5 arg5; A6 arg6;
#define DECLARE_VARS8(A0,A1,A2,A3,A4,A5,A6,A7) A0 arg0; A1 arg1; A2 arg2; A3 arg3; A4 arg4; A5 arg5; A6 arg6; A7 arg7;
#define DECLARE_VARS9(A0,A1,A2,A3,A4,A5,A6,A7,A8) A0 arg0; A1 arg1; A2 arg2; A3 arg3; A4 arg4; A5 arg5; A6 arg6; A7 arg7; A8 arg8;
#define DECLARE_VARS10(A0,A1,A2,A3,A4,A5,A6,A7,A8,A9) A0 arg0; A1 arg1; A2 arg2; A3 arg3; A4 arg4; A5 arg5; A6 arg6; A7 arg7; A8 arg8; A9 arg9;
#define DECLARE_VARS_(_DECLARE_VARS_, ...) ANG_EXPAND(_DECLARE_VARS_(__VA_ARGS__))
#define DECLARE_VARSN(...) ANG_EXPAND(DECLARE_VARS_(ANG_CATAB(DECLARE_VARS, ANG_EXPAND(ANG_NUM_ARGS(__VA_ARGS__))), __VA_ARGS__))

#define COMA ,
#define NON_SEPARATOR()
#define COMA_SEPARATOR() ,
#define OR_SEPARATOR() ||
#define AND_SEPARATOR() &&
#define ELSE_SEPARATOR() else
#define FUNCX_TOSTRING(A0) #A0
#define FUNCX_TYPENAME(A0) typename A0
#define FUNCX_TYPE_OF(A0) ang::rtti::type_of<A0>()
#define FUNCX_TYPE_OF_PTR(A0) &ang::rtti::type_of<A0>()
#define FUNCX_QUERY_INTERFACE(A0) if(id.type_id() ==  FUNCX_TYPE_OF(A0).type_id()) { if (out == null) return false; *out = static_cast<A0*>(this); return true; } \

#define TEMPLATE(...) template<ANG_EXPAND(APPLY_FUNCX_N(FUNCX_TYPENAME, COMA_SEPARATOR,##__VA_ARGS__))>
#define TYPE_OF_ARRAY(...) { ANG_EXPAND(APPLY_FUNCX_N(FUNCX_TYPE_OF, COMA_SEPARATOR,##__VA_ARGS__)) }
#define TYPE_OF_PTR_ARRAY(...) { ANG_EXPAND(APPLY_FUNCX_N(FUNCX_TYPE_OF_PTR, COMA_SEPARATOR,##__VA_ARGS__)) }
#define QUERY_INTERFACE_SWITCH(...) { ANG_EXPAND(APPLY_FUNCX_N(FUNCX_QUERY_INTERFACE, ELSE_SEPARATOR,##__VA_ARGS__)) }


#if defined WINDOWS_PLATFORM && defined ANG_DEVELOPPER
#define _DECLSPEC_ALLOCATOR __declspec(allocator)
#define _DECLSPEC_NOVTABLE __declspec(novtable)
#else
#define _DECLSPEC_ALLOCATOR 
#define _DECLSPEC_NOVTABLE 
#endif // WINDOWS_PLATFORM

#define ANG_UTILS_TO_STRING_(...) #__VA_ARGS__
#define ANG_UTILS_TO_STRING(...) ANG_UTILS_TO_STRING_(__VA_ARGS__)
#define ANG_UTILS_TO_STRING_OBJ_(...) ang::castr_t(#__VA_ARGS__)
#define ANG_UTILS_TO_STRING_OBJ(...) ANG_UTILS_TO_STRING_OBJ_(__VA_ARGS__)

#ifdef __cplusplus
#ifndef ANG_NO_RUNTIME_ERRORS
#define ANG_RUNTIME_ERROR(_FLAG, _MSG) static_assert(_FLAG, _MSG)

#define STATIC_WARNING1(_COND) \
struct ANG_CATAB(warning,__LINE__) { \
  ANG_DEPRECATE(void warning(ang::false_type const& ), __FILE__ "(" ANG_UTILS_TO_STRING(__LINE__) "): warning :"  ) {}; \
  void warning(ang::true_type const& ) {}; \
  ANG_CATAB(warning,__LINE__)() {warning(ang::integer_constant<bool,(_COND)>());} \
}

#define STATIC_WARNING2(_COND, _MSG) \
struct ANG_CATAB(warning,__LINE__) { \
  ANG_DEPRECATE(void warning(ang::false_type const& ), __FILE__ "(" ANG_UTILS_TO_STRING(__LINE__) "): warning :" _MSG ) {}; \
  void warning(ang::true_type const& ) {}; \
  ANG_CATAB(warning,__LINE__)() {warning(ang::integer_constant<bool,(_COND)>());} \
}

#define STATIC_WARNING_(_WARNING_, ...) ANG_EXPAND(_WARNING_(__VA_ARGS__))
#define STATIC_WARNING(...) ANG_EXPAND(STATIC_WARNING_(ANG_CATAB(STATIC_WARNING, ANG_EXPAND(ANG_NUM_ARGS(__VA_ARGS__))), __VA_ARGS__))


#else
#define ANG_RUNTIME_ERROR(...)do{}while(0)
#endif
#endif



#endif//__ANG_BASE_DEFINES_H__
