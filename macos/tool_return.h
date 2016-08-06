#ifndef __tool_return__
#define __tool_return__

#include "errors.h"
#include <utility>

namespace MacOS {


	template<class T>
	class tool_return;


	template<class T>
	struct tool_return_type { typedef tool_return<T> type; };

	template<class T>
	struct tool_return_type<tool_return<T>> { typedef tool_return<T> type; };






	namespace internal {

		class tool_return_base
		{
			protected:
			macos_error _error = noErr;

			constexpr tool_return_base() = default;

			constexpr tool_return_base(macos_error error) : _error(error)
			{}

			public:

			constexpr macos_error error() const { return _error; }

			constexpr explicit operator bool() const { return !_error; }

			template<class... Args>
			void throw_macos_error(Args&&... args) const
			{
				if (_error) MacOS::throw_macos_error(_error, std::forward<Args>(args)...);

			}

		};
	} // namespace


	template<class T>
	class tool_return : public internal::tool_return_base
	{
		private:

		T _value = T();

		tool_return() = delete;

		operator T() const
		{
			return _value;
		}

		public:

		typedef T value_type;

		tool_return(const T &value) : _value(value)
		{}

		tool_return(T &&value) : _value(std::forward<T>(value))
		{}


		tool_return(macos_error error) : tool_return_base(error)
		{}


		tool_return &operator=(const T &value)
		{
			_value = value;
			_error = 0;
			return *this;
		}

		tool_return &operator=(T &&value)
		{
			_value = std::forward<T>(value);
			_error = 0;
			return *this;
		}


		tool_return &operator=(macos_error error)
		{
			_value = T();
			_error = error;
			return *this;
		}


		constexpr const T* operator->() const
		{
			return &_value;
		}


		constexpr const T& operator *() const
		{
			return _value;
		}


		T value() const
		{
			return _value;
		}

		template<class U>
		T value_or(U&& u) const
		{
			if (_error) return std::forward<U>(u);
			return _value;
		}

		template<class U>
		T &&value_or(U&& u) &&
		{
			if (_error) return std::forward<U>(u);
			return std::move(_value);
		}

		template<class... Args>
		T value_or_throw(Args&&... args) const
		{
			if (_error) throw_macos_error(std::forward<Args>(args)...);

			return _value;
		}

		template<class F, typename RT = typename std::result_of<F(tool_return<T>)>::type>
		void then(F &&f, typename std::enable_if<std::is_void<RT>::value>::type* = 0) {
			f(std::move(*this));
		}

		template<class F, typename RT = typename std::result_of<F(tool_return<T>)>::type>
		RT then(F &&f, typename std::enable_if<!std::is_void<RT>::value>::type* = 0) {
			return f(std::move(*this));
		}

		template<class F, typename RT = typename std::result_of<F(value_type)>::type>
		typename tool_return_type<RT>::type
		map(F &&f, typename std::enable_if<std::is_void<RT>::value>::type* = 0) {
			if (_error) return _error;
			f(std::move(_value));
			//return tool_return<void>();
			return noErr;
		}		

		template<class F, typename RT = typename std::result_of<F(value_type)>::type>
		typename tool_return_type<RT>::type
		map(F &&f, typename std::enable_if<!std::is_void<RT>::value>::type* = 0) {
			if (_error) return _error;
			return f(std::move(_value));
		}

	};

	template<>
	class tool_return<void> : public internal::tool_return_base
	{
		public:

		typedef void value_type;

		tool_return() = default;

		tool_return(macos_error error) : tool_return_base(error)
		{}

		tool_return &operator=(macos_error error)
		{
			_error = error;
			return *this;
		}
	};



	template<>
	class tool_return<macos_error> : public internal::tool_return_base
	{
		public:

		typedef void value_type;

		tool_return() = default;

		tool_return(macos_error error) : tool_return_base(error)
		{}

		tool_return &operator=(macos_error error)
		{
			_error = error;
			return *this;
		}
	};



} // namespace
#endif
