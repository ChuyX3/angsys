#pragma once
#include <coffe.h>

namespace coffe
{
	namespace text
	{
		//template<>
		//struct text_parser<encoding::auto_detect>
		//	: public bean
		//	, public iparser
		//{
		//public:
		//	text_parser();

		//	COFFE_DECLARE_INTERFACE();

		//private:
		//	virtual~text_parser();

		//public: /*overrides*/
		//	virtual text_format_t default_format(text_format_target_t)const override;
		//	virtual cstr_t seek(cstr_t, windex& i, cstr_t)const override;
		//	virtual long64 to_signed(cstr_t, windex& i, bool increment, int base)const override;
		//	virtual ulong64 to_unsigned(cstr_t, windex& i, bool increment, int base)const override;
		//	virtual double to_floating(cstr_t, windex& i, bool increment, bool ex)const override;
		//	virtual cstr_t format(cstr_t, var_args_t args, encoding_t e, istring_ptr_t out)const override;
		//	virtual text_format_t parse(cstr_t)const override;
		//	virtual text_format_t parse(cstr_t, wsize& beg, int& arg)const override;
		//};
	}
}