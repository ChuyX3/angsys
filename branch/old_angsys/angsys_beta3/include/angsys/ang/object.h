#ifndef __ANGSYS_H__
#elif !defined __OBJECT_H__
#define __OBJECT_H__

LINK ang::object* ang_alloc_object_memory(ang_size_t, ang_memory_hint_t = ang_object_memory);

#ifdef _DEBUG
LINK ang::object* ang_alloc_object_memory(ang_size_t, const char* file, int line, ang_memory_hint_t = ang_object_memory);
#endif // _DEBUG


namespace ang
{

	/******************************************************************/
	/* enum ang::comparision_result_t :                                */
	/*  -> represents the result of a comparison operation            */
	/******************************************************************/
	safe_enum(LINK, comparision_result, int)
	{
		minor = -1,
		same = 0,
		mayor = 1,
		diferent = 20
	};

	/******************************************************************/
	/* class ang::object :                                      */
	/*  -> implements the base class for all library's objects        */
	/******************************************************************/
	class LINK object
		: public iobject
	{
	private:
		dword& m_ref_count;

	public:
		object(bool inc_ref = false);

	protected:
		virtual~object();

		object(object &&) = delete;
		object(object const&) = delete;
		object& operator = (object &&) = delete;
		object& operator = (object const&) = delete;

	public:
		//virtual void clear();
		virtual comparision_result_t compare(object const* obj)const;
		virtual string to_string()const;
		virtual string to_string(text::text_format_t)const;
//		virtual wsize serialize(streams::itext_output_stream_t)const;
//		virtual wsize serialize(streams::ibinary_output_stream_t)const;
//		virtual wsize deserialize(streams::itext_input_stream_t);
//		virtual wsize deserialize(streams::ibinary_input_stream_t);

	protected:
		virtual void dispose() override;
		virtual bool auto_release();
		virtual bool auto_release(ushort alignment);

	public:
		ANG_DECLARE_INTERFACE();

	public:
		virtual dword add_ref() override;
		virtual dword release() override;

	public:
		pointer operator new(wsize)noexcept;
		void operator delete(pointer)noexcept;

		pointer operator new(wsize, pointer)noexcept;
		void operator delete(pointer, pointer)noexcept;

#ifdef _DEBUG
		pointer operator new(wsize, const char*, int);
		pointer operator new(wsize, word, const char*, int);
#endif//_DEBUG

	protected: //for aligned objects
		pointer operator new(wsize, ushort alignment);
		void operator delete(pointer, ushort alignment);

	protected:
		pointer operator new[](wsize)noexcept;
		void operator delete[](pointer)noexcept;

	public:

		inline bool operator == (object const& obj)const {
			return this->compare(&obj) == comparision_result::same;
		}

		inline bool operator != (object const& obj)const {
			return this->compare(&obj) != comparision_result::same;
		}

		template<typename T> typename smart_ptr_type<T>::smart_ptr_t as() {
			return  this ? interface_cast<typename smart_ptr_type<T>::type>(this) : null;
		}
		template<typename T> bool as(T*& out) {
			out = this ? interface_cast<typename smart_ptr_type<T>::type>(this) : null;
			return out != null;
		}

	};


	template<typename T1, typename T2>
	bool operator == (object_wrapper<T1> const& obj1, object_wrapper<T2> const& obj2) {
		if ((object*)obj1.get() == (object*)obj2.get())
			return true;
		if ((!obj1.get() && obj2.get()) || (obj1.get() && !obj2.get()))
			return false;
		return *obj1.get() == *obj2.get();
	}

	template<typename T1, typename T2>
	bool operator != (object_wrapper<T1> const& obj1, object_wrapper<T2> const& obj2) {
		return operator == (obj1, obj2);
	}

	
	template<typename...Ts> struct inherit_helper : false_type {};
	template<typename T, typename U, typename... Ts> struct inherit_helper<T,U,Ts...> : is_object<U> {};

	template<bool, typename...Ts > struct object_inherit;

	template<typename T, typename...Ts >
	struct object_inherit<false, T, Ts...> 
		: public object, public Ts... {
	};

	template<typename T, typename U, typename... Ts>
	struct object_inherit<true, T, U, Ts...> : public U, public Ts... {
		using object_base = U;
		template<typename...Arsg> object_inherit(Arsg&&... args)
			: U(forward<Arsg>(args)...) { }
	};

	template<typename...Ts> struct managed;
	template<typename T, typename...Ts> struct managed<T, Ts...>
		: object_inherit<inherit_helper<T, Ts...>::value, T, Ts...> {
	protected:
		using self = T;
		using base = managed<T, Ts...>;

		template<typename...Arsg>managed(Arsg&&... args)
			: object_inherit<inherit_helper<T, Ts...>::value, T, Ts...>(forward<Arsg>(args)...) { }
	};



	template<typename...Ts> using smart = managed<Ts...>;
}

#endif//__SAMRT_PTR_H__