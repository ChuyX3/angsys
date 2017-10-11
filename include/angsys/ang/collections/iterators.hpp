#ifndef __ANG_COLLECTIONS_HPP__
#elif !defined __ANG_COLLECTIONS_ITERATORS_HPP__
#define __ANG_COLLECTIONS_ITERATORS_HPP__

namespace ang
{
	namespace collections
	{
		template<class T> class base_iterator
		{
		protected:
			ienum<T>* _parent;
			position_t _current;
			wsize _offset;
			wsize _unused;

		public:
			base_iterator(base_iterator const&);
			base_iterator(ienum<T>* p = null, position_t cur = null, wsize offset = 0U);
			~base_iterator();

		public: //Overrides
			void clean();

		public: //Methods
			ienum<T>* parent()const;
			void parent(ienum<T>*);
			position_t current()const;
			void current(position_t);
			wsize offset()const;
			void offset(wsize);
			bool is_valid()const;

			base_iterator& operator = (const base_iterator&);

			bool operator ==  (const base_iterator& it)const {
				return it._current == _current && it._offset == _offset;
			}
			bool operator !=  (const base_iterator& it)const {
				return it._current != _current || it._offset != _offset;
			}
			bool operator >=  (const base_iterator& it)const {
				if (_current == it._current)
					return _offset >= it._offset;
				else
					return _current > _current;
			}
			bool operator <=  (const base_iterator& it)const {
				if (_current == it._current)
					return _offset <= it._offset;
				else
					return _current < _current;
			}
			bool operator >  (const base_iterator& it)const {
				if (_current == it._current)
					return _offset > it._offset;
				else
					return _current > _current;
			}
			bool operator <  (const base_iterator& it)const {
				if (_current == it._current)
					return _offset < it._offset;
				else
					return _current < _current;
			}
		};

		template<class T> class iterator : public base_iterator<T>
		{
		public:
			iterator(base_iterator<T> const& it) : base_iterator<T>(it) {}
			iterator(ienum<T>* p = null, position_t cur = null, wsize offset = 0U)
				: base_iterator<T>(p, cur, offset) {
			}

			iterator& operator = (const base_iterator<T>& it) {
				base_iterator<T>::operator = (it);
				return*this;
			}

			operator T*()const;
			T* operator -> ()const;
			T& operator * ()const;
		};

		template<class T> class iterator<const T> : public base_iterator<T>
		{
		public:
			iterator(base_iterator<T> const& it) : base_iterator<T>(it) {}
			iterator(ienum<T>* p = null, position_t cur = null, wsize offset = 0U)
				: base_iterator<T>(p, cur, offset) {
			}

			iterator& operator = (const base_iterator<T>& it) {
				base_iterator<T>::operator = (it);
				return*this;
			}

			operator T const*()const;
			T const* operator -> ()const;
			T const& operator * ()const;
		};

		template<class T>
		class forward_iterator : public iterator<T>
		{
		public:
			forward_iterator(base_iterator<T> const& it) : iterator<T>(it) {}
			forward_iterator(ienum<T>* p = null, position_t cur = null, wsize offset = 0U)
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
			forward_iterator(ienum<T>* p = null, position_t cur = null, wsize offset = 0U)
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
			backward_iterator(ienum<T>* p = null, position_t cur = null, wsize offset = 0U)
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
			backward_iterator(ienum<T>* p = null, position_t cur = null, wsize offset = 0U)
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
}//ang


#endif //__ANG_COLLECTIONS_ITERATORS_HPP__