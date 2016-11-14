
#ifndef __floating_point_h__
#define __floating_point_h__

#include <type_traits>
#include <cstdint>
#include <cmath>
#include <cstring>

//#include <inttypes.h>

//#include "endian.h"

enum class endian {
	little = 3412,
	big = 1234,
	native = little
};

namespace floating_point {

	template<size_t size>
	void reverse_bytes(void *vp) {
		char *cp = (char *)vp;
		for (size_t i = 0; i < size / 2; ++i)
			std::swap(cp[i], cp[size - i - 1]);
	}

	template<size_t size>
	void reverse_bytes_if(void *vp, std::true_type) {
		reverse_bytes<size>(vp);
	}
	template<size_t size>
	void reverse_bytes_if(void *vp, std::false_type) {
	}

#if 0
	enum classification {
		zero,
		infinite,
		quiet_nan,
		signaling_nan,
		normal,
		subnormal
	};
#endif

	namespace single_traits {
		constexpr size_t bias = 127;
		constexpr size_t exponent_bits = 8;
		constexpr size_t significand_bits = 23;
		constexpr int max_exp = 127;
		constexpr int min_exp = -127;

		constexpr uint32_t significand_mask = ((1 << significand_bits) - 1);
		constexpr uint32_t sign_bit = UINT32_C(1) << 31;
		constexpr uint32_t nan_exp = UINT32_C(255) << significand_bits;

		constexpr uint32_t quiet_nan = UINT32_C(0x02) << (significand_bits - 2);
		constexpr uint32_t signaling_nan = UINT32_C(0x01) << (significand_bits - 2);
	}

	namespace double_traits {
		constexpr size_t bias = 1023;
		constexpr size_t exponent_bits = 11;
		constexpr size_t significand_bits = 52;
		constexpr int max_exp = 1023;
		constexpr int min_exp = -1023;

		constexpr uint64_t significand_mask = ((UINT64_C(1) << significand_bits) - 1);
		constexpr uint64_t sign_bit = UINT64_C(1) << 63;
		constexpr uint64_t nan_exp = UINT64_C(2047) << significand_bits;

		constexpr uint64_t quiet_nan = UINT64_C(0x02) << (significand_bits - 2);
		constexpr uint64_t signaling_nan = UINT64_C(0x01) << (significand_bits - 2);
	}

	namespace extended_traits {

		constexpr size_t bias = 16383;
		constexpr size_t exponent_bits = 15;
		constexpr size_t significand_bits = 63; // does not include explicit 1.
		constexpr int max_exp = 16383;
		constexpr int min_exp = -16383;

		constexpr uint64_t significand_mask = ((UINT64_C(1) << significand_bits) - 1);

		constexpr uint64_t quiet_nan = UINT64_C(0x02) << (significand_bits - 2);
		constexpr uint64_t signaling_nan = UINT64_C(0x01) << (significand_bits - 2);

		constexpr uint64_t one_bit = UINT64_C(0x8000000000000000);


		// stored separately.
		constexpr uint16_t sign_bit = 0x8000;
		constexpr uint16_t nan_exp = 0x7fff;

	}

	template<size_t _size, endian _byte_order>
	struct format {
		static constexpr size_t size = _size;
		static constexpr endian byte_order = _byte_order;
	};


	class info {
	private:
		void read_single(const void *);
		void read_double(const void *);
		void read_extended(const void *);


		void write_single(void *) const;
		void write_double(void *) const;
		void write_extended(void *) const;
	public:

		bool sign = false;
		bool one = false;
		int exp = 0;
		uint64_t sig = 0; // includes explicit 1 bit, adjusted to 63 bits of fraction.

		bool nan = false;
		bool inf = false;

		//classification type = zero;

		template<class T, typename = std::enable_if<std::is_floating_point<T>::value> >
		void read(T x)
		{ read(format<sizeof(x), endian::native>{}, &x); }

		template<size_t size, endian byte_order>
		void read(format<size, byte_order>, const void *vp) {

			uint8_t buffer[size];
			static_assert(byte_order != endian::native, "byte order");

			std::memcpy(buffer, vp, size);
			reverse_bytes<size>(buffer);
			read(format<size, endian::native>{}, buffer);
		}

		void read(format<4, endian::native>, const void *vp) {
			read_single(vp);
		}


		void read(format<8, endian::native>, const void *vp) {
			read_double(vp);
		}

		void read(format<10, endian::native>, const void *vp) {
			read_extended(vp);
		}

		void read(format<12, endian::native>, const void *vp) {
			read_extended(vp);
		}

		void read(format<16, endian::native>, const void *vp) {
			read_extended(vp);
		}



		template<class T, typename = std::enable_if<std::is_floating_point<T>::value> >
		void write(T &x) const
		{ write(format<sizeof(x), endian::native>{}, &x); }

		template<size_t size, endian byte_order>
		void write(format<size, byte_order>, void *vp) const {

			uint8_t buffer[size];
			static_assert(byte_order != endian::native, "byte order");


			write(format<size, endian::native>{}, buffer);

			reverse_bytes<size>(buffer);
			std::memcpy(vp, buffer, size);
		}


		void write(format<4, endian::native>, void *vp) const {
			write_single(vp);
		}


		void write(format<8, endian::native>, void *vp) const {
			write_double(vp);
		}

		void write(format<10, endian::native>, void *vp) const {
			write_extended(vp);
		}

		void write(format<12, endian::native>, void *vp) const {
			write_extended(vp);
			std::memset((uint8_t *)vp + 10, 0, 12-10);
		}

		void write(format<16, endian::native>, void *vp) const {
			write_extended(vp);
			std::memset((uint8_t *)vp + 10, 0, 16-10);
		}





		template<class T, typename = std::enable_if<std::is_floating_point<T>::value> >
		info(T x) { read(x); }
		info() = default;





	};


/*
	std::string to_string(const info &fpi)
	{
	}
*/
	inline int fpclassify(const info &fpi) {
		if (fpi.nan) return FP_NAN;
		if (fpi.inf) return FP_INFINITE;
		if (fpi.sig == 0) return FP_ZERO;
		return fpi.sig >> 63 ? FP_NORMAL : FP_SUBNORMAL;
	}

	inline int signbit(const info &fpi) {
		return fpi.sign;
	}

	inline int isnan(const info &fpi) {
		return fpi.nan;
	}

	inline int isinf(const info &fpi) {
		return fpi.inf;
	}

	inline int isfinite(const info &fpi) {
		if (fpi.nan || fpi.inf) return false;
		return true;
	}

	inline int isnormal(const info &fpi) {
		if (fpi.nan || fpi.inf) return false;
		return fpi.sig >> 63;
	}

}

#endif
