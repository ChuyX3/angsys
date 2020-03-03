#ifndef __ANG_BASE_VALUE_H__
#error ang/base/base.h is not included
#else

namespace ang //constants
{
	template<>
	struct LINK value<MY_TYPE, gender_of<MY_TYPE>()> {
		typedef MY_TYPE type;
		static constexpr gender_t gender_id = gender_of<MY_TYPE>();

		static runtime::rtti_t const& class_info();

		template<typename T, text::encoding E> 
		static inline MY_TYPE parse(str_view<T, E> str) {
			return str_to_value<MY_TYPE>(str);
		}
		template<typename T, text::encoding E>
		static inline MY_TYPE parse(str_view<T, E> str, windex& i) {
			return str_to_value<MY_TYPE>(str, i);
		}

		value();
		value(MY_TYPE const& v);
		value(value const& v);
		value(MY_TYPE && v);
		value(value && v);

		operator MY_TYPE& ();
		operator MY_TYPE const& ()const;

		MY_TYPE* operator &();
		MY_TYPE const* operator &()const;

		value& operator = (MY_TYPE const& v);
		value& operator = (value const& v);
		value& operator = (MY_TYPE && v);
		value& operator = (value && v);

		MY_TYPE& get();
		MY_TYPE const& get()const;
		void set(MY_TYPE const& v);
		void move(MY_TYPE && v);

	protected:
		MY_TYPE m_value;
	};

}
#endif//__ANG_BASE_VALUE_H__
