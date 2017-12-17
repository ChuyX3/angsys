/*********************************************************************************************************************/
/*   File Name: angsys.hpp                                                                                             */
/*   Autor: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                    */
/*   File description: this file declare all functions and clases for dynamic type naming and types conversions.     */
/*   Also it defines useful macros for the use of this functions and clases.                                         */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/


#ifndef __ANGSYS_HPP__
#error ...
#elif !defined __ANG_BUFFER_HPP__
#define __ANG_BUFFER_HPP__

namespace ang
{

	class buffer;
	class buffer_view;
	typedef object_wrapper<buffer> buffer_t;
	typedef object_wrapper<buffer_view> buffer_view_t;

	template<> class LINK intf_wrapper<ibuffer>
	{
	public:
		typedef ibuffer	type;

	protected:
		ibuffer* _ptr;

	public:
		intf_wrapper();
		intf_wrapper(ibuffer*);
		intf_wrapper(ibuffer_t &&);
		intf_wrapper(ibuffer_t const&);
		intf_wrapper(nullptr_t const&);
		~intf_wrapper();
	public:
		void clean();
		bool is_empty()const;
		ibuffer* get(void)const;
		void set(ibuffer*);
		ibuffer ** addres_of(void);

	public:
		ibuffer_t& operator = (ibuffer*);
		ibuffer_t& operator = (ibuffer_t &&);
		ibuffer_t& operator = (ibuffer_t const&);

		ibuffer_ptr_t operator & (void);
		ibuffer * operator -> (void);
		ibuffer const* operator -> (void)const;
		operator ibuffer * (void);
		operator ibuffer const* (void)const;
	};


	class LINK buffer
		: public object
		, public ibuffer
	{
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

		virtual text::encoding_t encoding()const override;
		virtual pointer buffer_ptr()const override;
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

		virtual text::encoding_t encoding()const override;
		virtual pointer buffer_ptr()const override;
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
		pointer buffer_ptr()const override;
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
		ANG_DECLARE_INLINE_INTERFACE();

		virtual text::encoding_t encoding()const override;
		virtual pointer buffer_ptr()const override;
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
		ANG_DECLARE_INLINE_INTERFACE();

		virtual text::encoding_t encoding()const override;
		virtual pointer buffer_ptr()const override;
		virtual wsize buffer_size()const override;
		virtual wsize mem_copy(wsize, pointer, text::encoding_t = text::encoding::binary) override;
		virtual ibuffer_view_t map_buffer(windex, wsize) override;
		virtual bool unmap_buffer(ibuffer_view_t&, wsize) override;
		virtual bool can_realloc_buffer()const override;
		virtual bool realloc_buffer(wsize) override;
	};
}


#endif//__ANG_BUFFER_HPP__