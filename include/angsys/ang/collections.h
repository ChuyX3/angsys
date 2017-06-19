/*********************************************************************************************************************/
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*********************************************************************************************************************/

#ifndef __ANG_COLLECTIONS_H__
#define __ANG_COLLECTIONS_H__

namespace ang
{
	namespace collections
	{
		typedef pointer position_t;

		//ANG_BEGIN_ENUM(LINK, iteration_method, byte_t)
		//	by_levels,
		//	preorder,
		//	inorder,
		//	postorder,
		//ANG_END_ENUM(iteration_method);
	
		template<class T> class iterator final
		{
		public:
			static inline type_name_t class_name();
			inline static bool is_child_of(type_name_t);

		protected:
			ienum<T>* _parent = null;
			position_t _current = null;
			uint _offset;
			uint _unused;

		public:
			iterator(iterator&&);
			iterator(const iterator&);
			iterator(uint offset); //from index
			iterator(ienum<T>* = null, position_t cur = null, uint offset = 0U);
			~iterator();

		public: //Overrides
			void clean();

		public: //Methods
			ienum<T>* parent()const;
			void parent(ienum<T>*);
			position_t current()const;
			void current(position_t);
			uint offset()const;
			void offset(uint);
			bool is_valid()const;

			iterator<T>& operator = (iterator<T>&&);
			iterator<T>& operator = (const iterator<T>&);
			iterator<T>& operator +=(int);
			iterator<T>& operator -=(int);
			explicit operator T*()const;
			const T* operator -> ()const;
			T* operator -> ();
			const T& operator * ()const;
			T& operator * ();
			iterator<T>& operator ++();//Prefix
			iterator<T>& operator --();//Prefix
			iterator<T> operator ++(int);//Postfix
			iterator<T> operator --(int);//Postfix
		
			iterator<T> operator + (int)const;
			iterator<T> operator - (int)const;

			bool operator ==  (const iterator<T>& it)const {
				return it._current == _current && it._offset == _offset;
			}
			bool operator !=  (const iterator<T>& it)const {
				return it._current != _current || it._offset != _offset;
			}
			bool operator >=  (const iterator<T>& it)const {
				if (_current == it._current)
					return _offset >= it._offset;
				else
					return _current > _current;
			}
			bool operator <=  (const iterator<T>& it)const {
				if (_current == it._current)
					return _offset <= it._offset;
				else
					return _current < _current;
			}
			bool operator >  (const iterator<T>& it)const {
				if (_current == it._current)
					return _offset > it._offset;
				else
					return _current > _current;
			}
			bool operator <  (const iterator<T>& it)const {
				if (_current == it._current)
					return _offset < it._offset;
				else
					return _current < _current;
			}
		};


		template<class T> 
		class iterator<object_wrapper<T>> final
		{
		public:
			static inline type_name_t class_name();
			inline static bool is_child_of(type_name_t);

		protected:
			ienum<object_wrapper<T>>* _parent = null;
			position_t _current = null;
			uint _offset;
			uint _unused;

		public:
			iterator(iterator&&);
			iterator(const iterator&);
			iterator(ienum<object_wrapper<T>>* = null, position_t cur = null, uint offset = 0U);
			~iterator();

		public: //Overrides
			void clean();

		public: //Methods
			ienum<object_wrapper<T>>* parent()const;
			void parent(ienum<object_wrapper<T>>*);
			position_t current()const;
			void current(position_t);
			uint offset()const;
			void offset(uint);
			bool is_valid()const;

			iterator& operator = (iterator&&);
			iterator& operator = (const iterator&);
			iterator& operator +=(int);
			iterator& operator -=(int);
			explicit operator object_wrapper<T>* ()const;
			explicit operator object_wrapper_ptr<T> ()const;
			object_wrapper<T> operator * ()const;
			object_wrapper<T>& operator * ();

			const T* operator -> ()const;
			T* operator -> ();
			
			iterator& operator ++();//Prefix
			iterator& operator --();//Prefix
			iterator operator ++(int);//Postfix
			iterator operator --(int);//Postfix

			iterator operator + (int)const;
			iterator operator - (int)const;

			bool operator ==  (const iterator& it)const {
				return it._current == _current && it._offset == _offset;
			}
			bool operator !=  (const iterator& it)const {
				return it._current != _current || it._offset != _offset;
			}
			bool operator >=  (const iterator& it)const {
				if (_current == it._current)
					return _offset >= it._offset;
				else
					return _current > _current;
			}
			bool operator <=  (const iterator& it)const {
				if (_current == it._current)
					return _offset <= it._offset;
				else
					return _current < _current;
			}
			bool operator >  (const iterator& it)const {
				if (_current == it._current)
					return _offset > it._offset;
				else
					return _current > _current;
			}
			bool operator <  (const iterator& it)const {
				if (_current == it._current)
					return _offset < it._offset;
				else
					return _current < _current;
			}
		};


		template<class K, class T>
		class pair
		{
		public:
			typedef	pair<K, T>		self_t;
			typedef T					type;
			typedef K					key;

		protected:
			key			_key;
			type		_value;

		public:
			inline static type_name_t class_name();
			inline static bool is_child_of(type_name_t);

			pair() :_value(), _key() { }
			pair(key k) :_value(), _key(ang::move(k)) { }
			pair(key k, type data) : _key(ang::move(k)), _value(ang::move(data)) { }
			pair(const self_t& key) : _key(key._key), _value(key._value) { }
			pair(self_t&& key) : _key(ang::move(key._key)), _value(ang::move(key._value)) { }

			const type& value()const { return _value; }
			void value(type value) { _value = ang::move(value); }
			type& value() { return _value; }
			const key& key_value()const { return _key; }
			void key_value(key value) { _key = ang::move(value); }
			key& key_value() { return _key; }

			self_t& operator = (const self_t&value) {
				_key = value._key;
				_value = value._value;
				return*this;
			}
			self_t& operator = (self_t&&value) {
				_key = ang::move(value._key);
				_value = ang::move(value._value);
				return*this;
			}
			self_t& operator = (key k) {
				_key = ang::move(k);
				return*this;
			}

			friend inline bool operator == (const self_t& value1, const self_t& value2) { return value1._key == value2._key; }
			friend inline bool operator == (const self_t& value1, const key& value2) { return value1._key == value2; }
			friend inline bool operator == (const key& value1, const self_t& value2) { return value1 == value2._key; }
			friend inline bool operator != (const self_t& value1, const self_t& value2) { return value1._key != value2._key; }
			friend inline bool operator != (const self_t& value1, const key& value2) { return value1._key != value2; }
			friend inline bool operator != (const key& value1, const self_t& value2) { return value1 != value2._key; }
			friend inline bool operator >= (const self_t& value1, const self_t& value2) { return value1._key >= value2._key; }
			friend inline bool operator >= (const self_t& value1, const key& value2) { return value1._key >= value2; }
			friend inline bool operator >= (const key& value1, const self_t& value2) { return value1 >= value2._key; }
			friend inline bool operator <= (const self_t& value1, const self_t& value2) { return value1._key <= value2._key; }
			friend inline bool operator <= (const self_t& value1, const key& value2) { return value1._key <= value2; }
			friend inline bool operator <= (const key& value1, const self_t& value2) { return value1 <= value2._key; }
			friend inline bool operator > (const self_t& value1, const self_t& value2) { return value1._key > value2._key; }
			friend inline bool operator > (const self_t& value1, const key& value2) { return value1._key > value2; }
			friend inline bool operator > (const key& value1, const self_t& value2) { return value1 > value2._key; }
			friend inline bool operator < (const self_t& value1, const self_t& value2) { return value1._key < value2._key; }
			friend inline bool operator < (const self_t& value1, const key& value2) { return value1._key < value2; }
			friend inline bool operator < (const key& value1, const self_t& value2) { return value1 < value2._key; }
		};

	}//collections

	template<class T, typename F>
	static void foreach(collections::ienum<T> const* store, F func) {
		for (collections::iterator<T> it = store->begin(); it.is_valid(); ++it)
			func((T&)*it);
	}
}//ang


#endif//__ANG_COLLECTIONS_H__
