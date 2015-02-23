#ifndef __complex_h__
#define __complex_h__

#include <cmath>
#include <string>

	// comp is an int64_t but 0x8000_0000_0000_0000 is NaN
	//typedef int64_t complex;

struct complex {

	public:
		const uint64_t NaN = 0x8000000000000000;

		complex(const complex &rhs) = default;

		complex(uint64_t rhs) : _data(rhs)
		{}

		template <class T>
		complex(T t)
		{
			*this = t;
		}

		bool isnan() const
		{
			return _data == NaN;
		}

		complex &operator=(const complex &rhs) = default;

		complex &operator=(uint64_t rhs)
		{
			_data = rhs;
			return *this;
		}

		template <class T>
		complex &operator=(T ld)
		{
			switch(std::fpclassify(ld))
			{
				case FP_NAN:
					_data = NaN;
					break;
				case FP_INFINITE:
					if (std::signbit(ld))
					{
						_data = -INT64_MAX;
					}
					else
					{
						_data = INT64_MAX;
					}
					break;
				default:
					_data = ld;
					break;
			}

			return *this;
		}


		operator uint64_t() const {
			return _data;
		}

		operator int64_t() const {
			return _data;
		}

		operator long double() const {
			if (_data == NaN)
				return NAN;
			return _data;
		}

		operator double() const {
			if (_data == NaN)
				return NAN;
			return _data;
		}

		operator float() const {
			if (_data == NaN)
				return NAN;
			return _data;
		}


	private:
		int64_t _data = 0;

	};

namespace its_complicated {


	std::string to_string(complex c)
	{
		if (c.isnan()) return std::string("nan");

		return std::to_string((int64_t)c);
	}

	inline int fpclassify(complex c) {
		if (c.isnan()) return FP_NAN;
		if ((uint64_t)c == (uint64_t)0) return FP_ZERO;
		return FP_NORMAL;
	}

	inline int signbit(complex c) {
		if (c.isnan()) return 0;
		return ((int64_t)c < (int64_t)0) ? 1 : 0;
	}

	inline int isnan(complex c) {
		return c.isnan();
	}

	inline int isinf(complex c) {
		return false;
	}

	inline int isfinite(complex c) {
		if (c.isnan()) return false;
		return true;
	}

	inline int isnormal(complex c) {
		if (c.isnan()) return false;
		if ((uint64_t)c == 0) return false;
		return true;
	}
}

#endif
