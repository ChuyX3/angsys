#pragma once

namespace ang
{
	class except_code_map
		: public object
		, public singleton<except_code_map>
	{
	private:
		ang_hash_table<ulong, cstr_t> _code_map;

	public:
		except_code_map();
		virtual~except_code_map();

		ANG_DECLARE_INTERFACE();

		ang_hash_table<ulong, cstr_t>& code_map() {
			return _code_map;
		}
	};
}
