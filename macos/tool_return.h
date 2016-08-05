#ifndef __tool_return__
#define __tool_return__

#include "errors.h"
#include <utility>

namespace MacOS {

	namespace internal {

		class tool_return_base
		{
			protected:
			macos_error _error = noErr;

			tool_return_base() = default;

			tool_return_base(macos_error error) : _error(error)
			{}

			public:

			macos_error error() const
			{
				return _error;
			}


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

		tool_return(T value) : _value(value)
		{}

		tool_return(macos_error error) : tool_return_base(error)
		{}


		tool_return &operator=(T value)
		{
			_value = value;
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
			if (_error) return u;
			return _value;
		}

		template<class... Args>
		T value_or_throw(Args&&... args) const
		{
			if (_error) throw_macos_error(std::forward<Args>(args)...);

			return _value;
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


} // namespace
#endif
