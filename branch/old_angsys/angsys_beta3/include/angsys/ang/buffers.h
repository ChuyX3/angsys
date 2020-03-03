#ifndef __ANGSYS_H__
#elif !defined __BUFFERS_H__
#define __BUFFERS_H__

namespace ang
{

	class LINK buffer final
		: public smart<buffer, ibuffer>
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

	class LINK aligned_buffer final
		: public smart<aligned_buffer, ibuffer>
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

	class LINK buffer_view final
		: public smart<buffer_view, ibuffer_view>
	{
	private:
		windex m_start;
		wsize m_size;
		ibuffer_t m_buffer;

	public:
		buffer_view();
		buffer_view(ibuffer_t, windex, wsize);

	public: //overrides
		ANG_DECLARE_INTERFACE();

		void set(ibuffer_t, windex, wsize);

		ibuffer_t parent()const;
		virtual text::encoding_t encoding()const override;
		virtual bool is_readonly()const override;
		virtual pointer buffer_ptr() override;
		virtual const_pointer buffer_ptr()const override;
		wsize buffer_size()const override;

	private:
		~buffer_view();
	};

	template<wsize _SIZE>
	class stack_buffer final
		: public smart<stack_buffer<_SIZE>, ibuffer>
	{
	private:
		static constexpr wsize SIZE = _SIZE;
		byte m_memory[SIZE];

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

	class LINK buffer_wrapper final
		: public auto_self<buffer_wrapper, ibuffer>
	{
	private:
		wsize m_size;
		pointer m_ptr;

	private:
		pointer operator new(wsize)throw();
		void operator delete(void*)throw();

	public:
		buffer_wrapper(pointer ptr, wsize sz);
		virtual ~buffer_wrapper();

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