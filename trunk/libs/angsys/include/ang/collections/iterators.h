#ifndef __COFFE_COLLECTIONS_H__
#elif !defined __COFFE_COLLECTIONS_ITERATORS_H__
#define __COFFE_COLLECTIONS_ITERATORS_H__

namespace coffe
{
	namespace collections
	{

		template<class T> class base_iterator
		{
		protected:
			ienum<T>* m_parent;
			pointer m_current;
			wsize m_offset;
			wsize m_unused;

		public:
			base_iterator(base_iterator const&);
			base_iterator(ienum<T>* p = null, pointer cur = null, wsize offset = 0U);
			~base_iterator();

		public: //Overrides
			void clear();

		public: //Methods
			ienum<T>* parent()const;
			void parent(ienum<T>*);
			pointer current()const;
			void current(pointer);
			wsize offset()const;
			void offset(wsize);
			bool is_valid()const;

			base_iterator& operator = (const base_iterator&);

			bool operator ==  (const base_iterator& it)const {
				return it.m_current == m_current && it.m_offset == m_offset;
			}
			bool operator !=  (const base_iterator& it)const {
				return it.m_current != m_current || it.m_offset != m_offset;
			}
			bool operator >=  (const base_iterator& it)const {
				if (m_current == it.m_current)
					return m_offset >= it.m_offset;
				else
					return m_current > m_current;
			}
			bool operator <=  (const base_iterator& it)const {
				if (m_current == it.m_current)
					return m_offset <= it.m_offset;
				else
					return m_current < m_current;
			}
			bool operator >  (const base_iterator& it)const {
				if (m_current == it.m_current)
					return m_offset > it.m_offset;
				else
					return m_current > m_current;
			}
			bool operator <  (const base_iterator& it)const {
				if (m_current == it.m_current)
					return m_offset < it.m_offset;
				else
					return m_current < m_current;
			}
		};

		template<class T> class iterator : public base_iterator<T>
		{
		public:
			iterator(base_iterator<T> const& it) : base_iterator<T>(it) {}
			iterator(ienum<T>* p = null, pointer cur = null, wsize offset = 0U)
				: base_iterator<T>(p, cur, offset) {
			}

			iterator& operator = (const base_iterator<T>& it) {
				base_iterator<T>::operator = (it);
				return*this;
			}

			operator typename auto_type<T>::ptr_type()const;
			typename auto_type<T>::ptr_type operator -> ()const;
			typename auto_type<T>::ref_type operator * ()const;
		};

		template<class T> class iterator<const T> : public base_iterator<T>
		{
		public:
			iterator(base_iterator<T> const& it) : base_iterator<T>(it) {}
			iterator(ienum<T>* p = null, pointer cur = null, wsize offset = 0U)
				: base_iterator<T>(p, cur, offset) {
			}

			iterator& operator = (const base_iterator<T>& it) {
				base_iterator<T>::operator = (it);
				return*this;
			}

			operator typename auto_type<T>::const_ptr_type()const;
			typename auto_type<T>::const_ptr_type operator -> ()const;
			typename auto_type<T>::const_ref_type operator * ()const;
		};

		template<class T>
		class forward_iterator : public iterator<T>
		{
		public:
			forward_iterator(base_iterator<T> const& it) : iterator<T>(it) {}
			forward_iterator(ienum<T>* p = null, pointer cur = null, wsize offset = 0U)
				: iterator<T>(p,cur,offset) {
			}

			forward_iterator& operator +=(int);
			forward_iterator& operator -=(int);
			forward_iterator& operator ++();//Prefix
			forward_iterator& operator --();//Prefix
			forward_iterator operator ++(int);//Postfix
			forward_iterator operator --(int);//Postfix
			forward_iterator operator + (int)const;
			forward_iterator operator - (int)const;
		};

		template<class T>
		class forward_iterator<const T> : public iterator<const T>
		{
		public:
			forward_iterator(base_iterator<T> const& it) : iterator<const T>(it) {}
			forward_iterator(ienum<T>* p = null, pointer cur = null, wsize offset = 0U)
				: iterator<const T>(p, cur, offset) {
			}

			forward_iterator& operator +=(int);
			forward_iterator& operator -=(int);
			forward_iterator& operator ++();//Prefix
			forward_iterator& operator --();//Prefix
			forward_iterator operator ++(int);//Postfix
			forward_iterator operator --(int);//Postfix
			forward_iterator operator + (int)const;
			forward_iterator operator - (int)const;
		};

		template<class T>
		class backward_iterator : public iterator<T>
		{
		public:
			backward_iterator(base_iterator<T> const& it) : iterator<T>(it) {}
			backward_iterator(ienum<T>* p = null, pointer cur = null, wsize offset = 0U)
				: iterator<T>(p, cur, offset) {
			}

			backward_iterator& operator +=(int);
			backward_iterator& operator -=(int);
			backward_iterator& operator ++();//Prefix
			backward_iterator& operator --();//Prefix
			backward_iterator operator ++(int);//Postfix
			backward_iterator operator --(int);//Postfix
			backward_iterator operator + (int)const;
			backward_iterator operator - (int)const;
		};

		template<class T>
		class backward_iterator<const T> : public iterator<const T>
		{
		public:
			backward_iterator(base_iterator<T> const& it) : iterator<const T>(it) {}
			backward_iterator(ienum<T>* p = null, pointer cur = null, wsize offset = 0U)
				: iterator<const T>(p, cur, offset) {
			}

			backward_iterator& operator +=(int);
			backward_iterator& operator -=(int);
			backward_iterator& operator ++();//Prefix
			backward_iterator& operator --();//Prefix
			backward_iterator operator ++(int);//Postfix
			backward_iterator operator --(int);//Postfix
			backward_iterator operator + (int)const;
			backward_iterator operator - (int)const;
		};

	}//collections
}//coffe

COFFE_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, coffe::collections::iteration_method);

#endif //__COFFE_COLLECTIONS_ITERATORS_H__
