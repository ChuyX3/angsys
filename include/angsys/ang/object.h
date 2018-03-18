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
	/* class ang::object :                                            */
	/*  -> implements the base class for all library's objects        */
	/******************************************************************/
	class LINK object
		: public iobject
	{
	private:
		dword& _ref_count;

	public:
		object(bool inc_ref = false);

	protected:
		virtual~object();

		object(object &&) = delete;
		object(object const&) = delete;
		object& operator = (object &&) = delete;
		object& operator = (object const&) = delete;

	public:
		virtual comparision_result_t compare(object const* obj)const;
//		virtual string to_string()const;
//		virtual wsize serialize(streams::itext_output_stream_t)const;
//		virtual wsize serialize(streams::ibinary_output_stream_t)const;
//		virtual wsize deserialize(streams::itext_input_stream_t);
//		virtual wsize deserialize(streams::ibinary_input_stream_t);

	protected:
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

		bool operator == (object const& obj)const;

		bool operator != (object const& obj)const;

		template<typename T> typename smart_ptr_type<T>::smart_ptr_t as() {
			return  this ? dyn_cast<typename smart_ptr_type<T>::type>(this) : null;
		}
		template<typename T> bool as(T*& out) {
			out = this ? dyn_cast<typename smart_ptr_type<T>::type>(this) : null;
			return out != null;
		}

		template<typename T> typename smart_ptr_type<T>::const_smart_ptr_t as()const {
			return  this ? dyn_cast<typename smart_ptr_type<T>::const_type>(this) : null;
		}
		template<typename T> bool as(T const*& out)const {
			out = this ? dyn_cast<typename smart_ptr_type<T>::const_type>(this) : null;
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
}

#endif//__SAMRT_PTR_H__