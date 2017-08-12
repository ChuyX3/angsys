/*********************************************************************************************************************/
/*   File Name: angsys.h                                                                                             */
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


#ifndef __ANGSYS_H__
#error ...
#elif !defined __ANG_BUFFER_H__
#define __ANG_BUFFER_H__

namespace ang
{
	ANG_INTERFACE(ibuffer);

	class buffer;
	class static_buffer;
	typedef object_wrapper<buffer> buffer_t;
	typedef object_wrapper<static_buffer> static_buffer_t;

	ANG_BEGIN_INTERFACE(LINK, ibuffer)
		visible vcall pointer buffer_ptr()const pure;
		visible vcall wsize buffer_size()const pure;
		visible vcall wsize mem_copy(wsize, pointer) pure;
		visible vcall pointer map_buffer(windex, wsize) pure;
		visible vcall void unmap_buffer(pointer, wsize) pure;
		visible vcall bool can_realloc_buffer()const pure;
		visible vcall bool realloc_buffer(wsize) pure;
	ANG_END_INTERFACE();

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

	public: //overrides
		ANG_DECLARE_INTERFACE();

		virtual pointer buffer_ptr()const override;
		virtual wsize buffer_size()const override;
		virtual wsize mem_copy(wsize, pointer) override;
		virtual pointer map_buffer(windex, wsize) override;
		virtual void unmap_buffer(pointer, wsize) override;
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
		void operator delete(pointer, ulong64, word ALIGMENT = 16U)throw();

	private:
		pointer operator new(wsize)throw();
		void operator delete(pointer)throw();

	public:
		aligned_buffer();

	public: //overrides
		ANG_DECLARE_INTERFACE();

		virtual pointer buffer_ptr()const override;
		virtual wsize buffer_size()const override;
		virtual wsize mem_copy(wsize, pointer) override;
		virtual pointer map_buffer(windex, wsize) override;
		virtual void unmap_buffer(pointer, wsize) override;
		virtual bool can_realloc_buffer()const override;
		virtual bool realloc_buffer(wsize) override;

	protected:
		virtual bool auto_release()override;

	protected:
		virtual ~aligned_buffer();
	};


	class LINK static_buffer
		: public ibuffer
	{
	private:
		wsize _size;
		pointer _memory;

	private:
		pointer operator new(wsize)throw();
		void operator delete(void*)throw();

	public:
		static_buffer();
		static_buffer(pointer mem, wsize size);
		virtual ~static_buffer();

	public: //overrides
		ANG_DECLARE_INTERFACE();

		void set(pointer, wsize);

		virtual pointer buffer_ptr()const override;
		virtual wsize buffer_size()const override;
		virtual wsize mem_copy(wsize, pointer) override;
		virtual pointer map_buffer(windex, wsize) override;
		virtual void unmap_buffer(pointer, wsize) override;
		virtual bool can_realloc_buffer()const override;
		virtual bool realloc_buffer(wsize) override;
	};

}


#endif//__ANG_BUFFER_H__