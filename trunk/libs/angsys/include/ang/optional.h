#ifndef __COFFE_H__
#error ...
#elif !defined __COFFE_OPTIONAL_H__
#define __COFFE_OPTIONAL_H__

namespace coffe
{

	template<typename T, coffe::smart_type = smart_ptr_type<T>::smart_type>
	struct optional
	{
		using type = typename smart_ptr_type<T>::type;
		using type_ptr = typename smart_ptr_type<T>::smart_ptr_t;

		optional()
			: m_error(error_code::success)
			, m_result() {
		}
		optional(type_ptr&& value)
			: m_error(error_code::success)
			, m_result(coffe::move(value)) {
		}
		optional(type_ptr const& value)
			: m_error(error_code::success)
			, m_result(value) {
		}
		optional(const error& err)
			: m_error(err)
			, m_result() {
		}
		optional(optional && op)
			: m_error(coffe::move(op.m_error))
			, m_result(coffe::move(op.m_result)) {
		}
		optional(optional const& op)
			: m_error(op.m_error)
			, m_result(op.m_result) {
		}
		~optional() {

		}

		void reset() {
			m_error = error_code::success;
		}

		bool failed()const {
			return m_error.code() != error_code::success;
		}

		bool successed()const {
			return m_error.code() == error_code::success;
		}

		coffe::error const& error()const {
			return m_error;
		}

		type_ptr get()const {
			if (failed())
				throw_exception(error());
			return m_result;
		}

		type* operator ->()const {
			return get().get();
		}

		explicit operator type* ()const {
			return get().get();
		}


		operator bool()const {
			return successed();
		}

		optional& operator = (optional && op)
		{
			if (this != &op)
			{
				reset();
				m_error = coffe::move(op.m_error);
				m_result = coffe::move(op.m_result);
			}
			return*this;
		}

		optional& operator = (optional const& op)
		{
			m_error = op.m_error;
			m_result = op.m_result;
			return*this;
		}

	private:
		mutable coffe::error m_error;
		mutable type_ptr m_result;
	};

	template<typename T>
	struct optional<T, coffe::smart_type::none>
	{
		using type = typename remove_const<typename remove_reference<T>::type>::type;

		optional()
			: m_error(error_code::success)
			, m_result() {
			if constexpr (is_trivially_constructible<type>::value) {
				memset(&m_result, 0, size_of<type>());
			}
		}
		optional(type&& value)
			: m_error(error_code::success)
			, m_result(coffe::move(value)) {
		}
		optional(type const& value)
			: m_error(error_code::success)
			, m_result(value) {
		}
		optional(const error& err)
			: m_error(err)
			, m_result() {
			if constexpr (is_trivially_constructible<type>::value) {
				memset(&m_result, 0, size_of<type>());
			}
		}
		optional(optional && op)
			: m_error(coffe::move(op.m_error))
			, m_result(coffe::move(op.m_result)) {
		}
		optional(optional const& op)
			: m_error(op.m_error)
			, m_result(op.m_result) {
		}
		~optional() {

		}

		void reset() {
			m_error = error_code::success;
		}

		bool failed()const {
			return m_error.code() != error_code::success;
		}

		bool successed()const {
			return m_error.code() == error_code::success;
		}

		coffe::error const& error()const {
			return m_error;
		}

		type& get()const {
			if (failed())
				throw_exception(error());
			return m_result;
		}

		type* operator ->()const {
			return &get();
		}

		explicit operator type& ()const {
			return &get();
		}

		operator bool()const {
			return successed();
		}

		optional& operator = (optional && op)
		{
			if (this != &op)
			{
				reset();
				m_error = coffe::move(op.m_error);
				m_result = coffe::move(op.m_result);
			}
			return*this;
		}

		optional& operator = (optional const& op)
		{
			m_error = op.m_error;
			m_result = op.m_result;
			return*this;
		}

	private:
		mutable coffe::error m_error;
		mutable type m_result;
	};

	template<typename T>
	struct optional<intf_wrapper<T>, coffe::smart_type::none>
	{
		using type = typename remove_const<typename remove_reference<T>::type>::type;
		using type_ptr = intf_wrapper<T>;

		optional()
			: m_error(error_code::success)
			, m_result() {
		}
		optional(type_ptr&& value)
			: m_error(error_code::success)
			, m_result(coffe::move(value)) {
		}
		optional(type_ptr const& value)
			: m_error(error_code::success)
			, m_result(value) {
		}
		optional(const error& err)
			: m_error(err)
			, m_result() {
		}
		optional(optional && op)
			: m_error(coffe::move(op.m_error))
			, m_result(coffe::move(op.m_result)) {
		}
		optional(optional const& op)
			: m_error(op.m_error)
			, m_result(op.m_result) {
		}
		~optional() {

		}

		void reset() {
			m_error = error_code::success;
		}

		bool failed()const {
			return m_error.code() != error_code::success;
		}

		bool successed()const {
			return m_error.code() == error_code::success;
		}

		coffe::error const& error()const {
			return m_error;
		}

		type_ptr get()const {
			if (failed())
				throw_exception(error());
			return m_result;
		}

		type* operator ->()const {
			return get().get();
		}

		explicit operator type* ()const {
			return get().get();
		}


		operator bool()const {
			return successed();
		}

		optional& operator = (optional && op)
		{
			if (this != &op)
			{
				reset();
				m_error = coffe::move(op.m_error);
				m_result = coffe::move(op.m_result);
			}
			return*this;
		}

		optional& operator = (optional const& op)
		{
			m_error = op.m_error;
			m_result = op.m_result;
			return*this;
		}

	private:
		mutable coffe::error m_error;
		mutable type_ptr m_result;
	};

	template<typename T>
	struct optional<object_wrapper<T>, coffe::smart_type::none>
	{
		using type = typename remove_const<typename remove_reference<T>::type>::type;
		using type_ptr = object_wrapper<T>;

		optional()
			: m_error(error_code::success)
			, m_result() {
		}
		optional(type_ptr&& value)
			: m_error(error_code::success)
			, m_result(coffe::move(value)) {
		}
		optional(type_ptr const& value)
			: m_error(error_code::success)
			, m_result(value) {
		}
		optional(const error& err)
			: m_error(err)
			, m_result() {
		}
		optional(optional && op)
			: m_error(coffe::move(op.m_error))
			, m_result(coffe::move(op.m_result)) {
		}
		optional(optional const& op)
			: m_error(op.m_error)
			, m_result(op.m_result) {
		}
		~optional() {

		}

		void reset() {
			m_error = error_code::success;
		}

		bool failed()const {
			return m_error.code() != error_code::success;
		}

		bool successed()const {
			return m_error.code() == error_code::success;
		}

		coffe::error const& error()const {
			return m_error;
		}

		type_ptr get()const {
			if (failed())
				throw_exception(error());
			return m_result;
		}

		type* operator ->()const {
			return get().get();
		}

		explicit operator type* ()const {
			return get().get();
		}

		operator bool()const {
			return successed();
		}

		optional& operator = (optional && op)
		{
			if (this != &op)
			{
				reset();
				m_error = coffe::move(op.m_error);
				m_result = coffe::move(op.m_result);
			}
			return*this;
		}

		optional& operator = (optional const& op)
		{
			m_error = op.m_error;
			m_result = op.m_result;
			return*this;
		}

	private:
		mutable coffe::error m_error;
		mutable type_ptr m_result;
	};

	template<typename T>
	struct optional<T[], coffe::smart_type::none>
	{
		using type = array<typename remove_const<typename remove_reference<T>::type>::type>;

		optional()
			: m_error(error_code::success)
			, m_result() {
		}
		optional(type&& value)
			: m_error(error_code::success)
			, m_result(coffe::move(value)) {
		}
		optional(type const& value)
			: m_error(error_code::success)
			, m_result(value) {
		}
		optional(const error& err)
			: m_error(err)
			, m_result() {
		}
		optional(optional && op)
			: m_error(coffe::move(op.m_error))
			, m_result(coffe::move(op.m_result)) {
		}
		optional(optional const& op)
			: m_error(op.m_error)
			, m_result(op.m_result) {
		}
		~optional() {

		}

		void reset() {
			m_error = error_code::success;
		}

		bool failed()const {
			return m_error.code() != error_code::success;
		}

		bool successed()const {
			return m_error.code() == error_code::success;
		}

		coffe::error const& error()const {
			return m_error;
		}

		type& get()const {
			if (failed())
				throw_exception(error());
			return m_result;
		}

		type* operator ->()const {
			return &get();
		}

		explicit operator type& ()const {
			return &get();
		}

		operator bool()const {
			return successed();
		}

		optional& operator = (optional && op)
		{
			if (this != &op)
			{
				reset();
				m_error = coffe::move(op.m_error);
				m_result = coffe::move(op.m_result);
			}
			return*this;
		}

		optional& operator = (optional const& op)
		{
			m_error = op.m_error;
			m_result = op.m_result;
			return*this;
		}

	private:
		mutable coffe::error m_error;
		mutable type m_result;
	};

}

#endif /*__COFFE_OPTIONAL_H__*/