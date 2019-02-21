#ifndef __ANGSYS_H__
#elif !defined __BUFFERS_H__
#define __BUFFERS_H__

namespace ang
{
	namespace text
	{
		struct LINK encoding_t : public ang::value<encoding>
		{
			static encoding_t parse(cstr_t);
			static encoding_t parse(cwstr_t);
			static encoding_t parse(cmstr_t);
			static rtti_t const& class_info();
			encoding_t() : value(default_value<type>::value) {}
			encoding_t(type const& v) : value(v) {}
			encoding_t(encoding_t const& v) : value(v) {}
			encoding_t(type && v) : value(ang::forward<type>(v)) { }
			encoding_t(encoding_t && v) : value(ang::forward<value>(v)) { }
			ang::wstring to_string()const;
			encoding_t& operator = (type const& v) { get() = v; return*this; }
			encoding_t& operator = (encoding_t const& v) { get() = v.get(); return*this; }
			encoding_t& operator = (type && v) { get() = ang::move(v); v = default_value<type>::value; return*this; }
			encoding_t& operator = (encoding_t && v) { get() = ang::move(v.get()); v.set(default_value<type>::value); return*this; }
			friend inline bool operator == (encoding_t const& a1, encoding_t const& a2) { return a1.get() == a2.get(); }
			friend inline bool operator != (encoding_t const& a1, encoding_t const& a2) { return a1.get() != a2.get(); }
			friend inline bool operator >= (encoding_t const& a1, encoding_t const& a2) { return a1.get() >= a2.get(); }
			friend inline bool operator <= (encoding_t const& a1, encoding_t const& a2) { return a1.get() <= a2.get(); }
			friend inline bool operator > (encoding_t const& a1, encoding_t const& a2) { return a1.get() > a2.get(); }
			friend inline bool operator < (encoding_t const& a1, encoding_t const& a2) { return a1.get() < a2.get(); }
			friend inline bool operator == (encoding_t const& a1, encoding a2) { return a1.get() == a2; }
			friend inline bool operator == (encoding a1, encoding_t const& a2) { return a1 == a2.get(); }
			friend inline bool operator != (encoding_t const& a1, encoding a2) { return a1.get() != a2; }
			friend inline bool operator != (encoding a1, encoding_t const& a2) { return a1 != a2.get(); }
			friend inline bool operator >= (encoding_t const& a1, encoding a2) { return a1.get() >= a2; }
			friend inline bool operator >= (encoding a1, encoding_t const& a2) { return a1 >= a2.get(); }
			friend inline bool operator <= (encoding_t const& a1, encoding a2) { return a1.get() <= a2; }
			friend inline bool operator <= (encoding a1, encoding_t const& a2) { return a1 <= a2.get(); }
			friend inline bool operator > (encoding_t const& a1, encoding a2) { return a1.get() > a2; }
			friend inline bool operator > (encoding a1, encoding_t const& a2) { return a1 > a2.get(); }
			friend inline bool operator < (encoding_t const& a1, encoding a2) { return a1.get() < a2; }
			friend inline bool operator < (encoding a1, encoding_t const& a2) { return a1 < a2.get(); }
		};
	}


	ang_interface(ibuffer);
	ang_interface(ibuffer_view);

	ang_object(buffer);
	ang_object(aligned_buffer);
	ang_object(buffer_view);

	ang_begin_interface(LINK ibuffer_view)
		visible vcall bool is_readonly()const pure;
		visible vcall pointer buffer_ptr() pure;
		visible vcall const_pointer buffer_ptr()const pure;
		visible vcall wsize buffer_size()const pure;
	ang_end_interface();

	ANG_INTF_WRAPPER_DECLARATION(LINK, ibuffer_view);

	ang_begin_interface(LINK ibuffer, ibuffer_view)
		visible vcall text::encoding_t encoding()const pure;
		visible vcall wsize mem_copy(wsize, pointer, text::encoding_t = text::encoding::binary) pure;
		visible vcall ibuffer_view_t map_buffer(windex, wsize) pure;
		visible vcall bool unmap_buffer(ibuffer_view_t&, wsize used) pure;
		visible vcall bool can_realloc_buffer()const pure;
		visible vcall bool realloc_buffer(wsize) pure;
	ang_end_interface();

	ANG_INTF_WRAPPER_DECLARATION(LINK, ibuffer);


	class LINK buffer
		: public object
		, public ibuffer
	{
	public:
		static buffer_t new_buffer(wsize);

	public:
		pointer operator new (wsize, ulong64)throw();
		void operator delete(pointer, ulong64)throw();

	private:
		pointer operator new(wsize)throw();
		void operator delete(pointer)throw();

	public:
		buffer();
		buffer(wsize, pointer);

	public: //overrides
		ANG_DECLARE_INTERFACE();
		virtual bool is_readonly()const override;
		virtual text::encoding_t encoding()const override;
		virtual pointer buffer_ptr() override;
		virtual const_pointer buffer_ptr()const override;
		virtual wsize buffer_size()const override;
		virtual wsize mem_copy(wsize, pointer, text::encoding_t = text::encoding::binary) override;
		virtual ibuffer_view_t map_buffer(windex, wsize) override;
		virtual bool unmap_buffer(ibuffer_view_t&, wsize) override;
		virtual bool can_realloc_buffer()const override;
		virtual bool realloc_buffer(wsize) override;

	protected:
		virtual ~buffer();
	};

	class LINK aligned_buffer
		: public object
		, public ibuffer
	{
	public:
		pointer operator new (wsize, ulong64, word ALIGMENT = 16U)throw();
		void operator delete(pointer, ulong64, word ALIGMENT)throw();

	private:
		pointer operator new(wsize)throw();
		void operator delete(pointer)throw();

	public:
		aligned_buffer();

	public: //overrides
		ANG_DECLARE_INTERFACE();

		virtual bool is_readonly()const override;
		virtual text::encoding_t encoding()const override;
		virtual pointer buffer_ptr() override;
		virtual const_pointer buffer_ptr()const override;
		virtual wsize buffer_size()const override;
		virtual wsize mem_copy(wsize, pointer, text::encoding_t = text::encoding::binary) override;
		virtual ibuffer_view_t map_buffer(windex, wsize) override;
		virtual bool unmap_buffer(ibuffer_view_t&, wsize) override;
		virtual bool can_realloc_buffer()const override;
		virtual bool realloc_buffer(wsize) override;

	protected:
		bool auto_release()override;

	protected:
		virtual ~aligned_buffer();
	};

	class LINK buffer_view
		: public object
		, public ibuffer_view
	{
	private:
		windex _start;
		wsize _size;
		ibuffer_t _buffer;

	public:
		buffer_view();
		buffer_view(ibuffer_t, windex, wsize);

	public: //overrides
		ANG_DECLARE_INTERFACE();

		void set(ibuffer_t, windex, wsize);

		ibuffer_t parent()const;
		virtual bool is_readonly()const override;
		virtual pointer buffer_ptr() override;
		virtual const_pointer buffer_ptr()const override;
		wsize buffer_size()const override;

	private:
		~buffer_view();
	};

	template<wsize _SIZE>
	class stack_buffer
		: public ibuffer
	{
	private:
		static constexpr wsize SIZE = _SIZE;
		byte _memory[SIZE];

	private:
		pointer operator new(wsize)throw();
		void operator delete(void*)throw();

	public:
		stack_buffer();
		virtual ~stack_buffer();

	public: //overrides
		ANG_DECLARE_INTERFACE();

		virtual bool is_readonly()const override;
		virtual text::encoding_t encoding()const override;
		virtual pointer buffer_ptr() override;
		virtual const_pointer buffer_ptr()const override;
		virtual wsize buffer_size()const override;
		virtual wsize mem_copy(wsize, pointer, text::encoding_t = text::encoding::binary) override;
		virtual ibuffer_view_t map_buffer(windex, wsize) override;
		virtual bool unmap_buffer(ibuffer_view_t&, wsize) override;
		virtual bool can_realloc_buffer()const override;
		virtual bool realloc_buffer(wsize) override;
	};

	class LINK dummy_buffer
		: public ibuffer
	{
	private:
		wsize _size;
		pointer _ptr;

	private:
		pointer operator new(wsize)throw();
		void operator delete(void*)throw();

	public:
		dummy_buffer(pointer ptr, wsize sz);
		virtual ~dummy_buffer();

	public: //overrides
		ANG_DECLARE_INTERFACE();

		virtual bool is_readonly()const override;
		virtual text::encoding_t encoding()const override;
		virtual pointer buffer_ptr() override;
		virtual const_pointer buffer_ptr()const override;
		virtual wsize buffer_size()const override;
		virtual wsize mem_copy(wsize, pointer, text::encoding_t = text::encoding::binary) override;
		virtual ibuffer_view_t map_buffer(windex, wsize) override;
		virtual bool unmap_buffer(ibuffer_view_t&, wsize) override;
		virtual bool can_realloc_buffer()const override;
		virtual bool realloc_buffer(wsize) override;
	};


}

#endif//__BUFFERS_H__