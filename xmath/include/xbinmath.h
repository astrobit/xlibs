#pragma once
#include <cmath>
#include <cstdint>
#include <cstring>
namespace xbinarymath
{
// Note: I think this is a more or less comprehensive list of the compiler macros for 64-bit processors. This uses 64-bit registers for 64-bit processors, and 32-bit registers for all other processors (I'm assuming no one is going to try to use this on an 8-bit or 16-bit machine. If you do, you'll just need to add appropriate macro checks and make the typedefs (u)int16(8)_t for 16-bit(8-bit) processors
#if defined(__amd64__) || defined (__amd64) || defined (__x86_64__) || defined(__x86_64) || defined(_M_X64) || defined(_M_AMD64) || defined(__aarch64__) ||defined(__ia64__) || defined(_IA64) || defined(__IA64__) || defined(__ia64) || defined(_M_IA64) || defined(_M_IA64) || defined(_M_IA64) || defined(__itanium__) || defined(__powerpc64__) || defined(__ppc64__) || defined(__PPC64__) || defined(_ARCH_PPC64)

	typedef uint64_t	regtype;
	typedef int64_t		exptype;
#else
	typedef uint32_t	regtype;
	typedef int32_t		exptype;
#endif

	extern bool g_bRegArchInit;
	class c_regarch
	{
	public:
		xbinarymath::regtype mask_all;// = 0xffffffffffffffff;
		xbinarymath::regtype mask_low_half;// = 0xffffffff;
		xbinarymath::regtype mask_hi_half;// = 0xffffffff00000000;
		xbinarymath::regtype mask_hi_bit;// = 0x8000000000000000;
		xbinarymath::regtype mask_mid_bit;// = 0x80000000;
		xbinarymath::regtype mask_mid_bit_m1;// = 0x40000000;
		size_t 	num_bits;
		size_t	half_bits;
		size_t	half_bits_plus_1;
		size_t	half_bits_minus_1;


	
		void initialize(void)
		{
			num_bits = sizeof(xbinarymath::regtype) * 8;
			half_bits = num_bits >> 1;
			half_bits_plus_1 = half_bits + 1;
			half_bits_minus_1 = half_bits - 1;

			mask_all = 1;
			mask_hi_half = 1;
			mask_low_half = 1;

			mask_hi_bit = 1;
			mask_mid_bit = 1;
			mask_mid_bit_m1 = 1;
			for (size_t tI = 0; tI < num_bits; tI++)
			{
				mask_all <<= 1;
				mask_hi_half <<= 1;
				if (tI < half_bits_minus_1)
					mask_low_half <<= 1;
				if (tI < (num_bits - 1))
					mask_hi_bit <<= 1;
				if (tI < half_bits_minus_1)
					mask_mid_bit <<= 1;
				if (tI < (half_bits_minus_1 - 1))
					mask_mid_bit_m1 <<= 1;

				mask_all |= 1;
				if (tI < half_bits)
					mask_hi_half |= 1;
				mask_low_half |= 1;
			}		
			g_bRegArchInit  = true;

	//		std::cout << "regarch: " << num_bits << " " << half_bits << " " << half_bits_plus_1 << " " << half_bits_minus_1 << std::endl;
	//		xstdlib::pbin(mask_all);
	//		xstdlib::pbin(mask_hi_half);
	//		xstdlib::pbin(mask_low_half);
	//		xstdlib::pbin(mask_mid_bit_m1);
	//		xstdlib::pbin(mask_mid_bit);
	//		xstdlib::pbin(mask_hi_bit);
	//		xstdlib::pbin(0x1248124812481248);


		}
		c_regarch(void) {initialize();}

	};

	extern c_regarch	regarch;
	// adders (binary)
	bool overflow_adder_arch(regtype i_tA, regtype i_tB, regtype &o_tResult);
	bool overflow_adder_n(const regtype *i_tA, const regtype *i_tB, size_t i_tNum_Bits, regtype *&o_tResult);

	// negator (unary)
	void negate(regtype * io_tA, size_t i_tNum_Bits);

	// multipliers and squarers
	regtype significand_square_normal_arch(regtype i_tA, bool &o_bOverflow);
	void significand_multiply_arch(regtype i_tA, regtype i_tB, bool &o_bOverflow, size_t &o_tUnderflow, regtype & o_tResult_Hi, regtype & o_tResult_Lo);
	void significand_multiply_n(const regtype * i_tA, const regtype * i_tB, size_t i_tNum_Bits, bool &o_bOverflow, size_t &o_tUnderflow, regtype *& o_tResult_Hi, regtype *& o_tResult_Lo);

	// dividers (binary)
	void significand_divider_arch(regtype i_tA, regtype i_tB, regtype &o_tResult, regtype & o_tRemainder, exptype & o_tResult_Exponent, exptype & o_tRemainder_Exponent);
	void significand_divider_n(const regtype *i_tA, const regtype * i_tB, size_t i_tNum_Bits, regtype *&o_tResult, regtype *& o_tRemainder, exptype & o_tResult_Exponent, exptype & o_tRemainder_Exponent);

	// comparators (unary)
	bool greater_than_n(const regtype *i_tA, const regtype * i_tB, size_t i_tNum_Bits);
	bool not_equal_to_n(const regtype *i_tA, const regtype * i_tB, size_t i_tNum_Bits);
	bool equal_to_n(const regtype *i_tA, const regtype * i_tB, size_t i_tNum_Bits);
	bool less_than_or_equal_to_n(const regtype *i_tA, const regtype * i_tB, size_t i_tNum_Bits);
	bool less_than_n(const regtype *i_tA, const regtype * i_tB, size_t i_tNum_Bits);
	bool greater_than_or_equal_to_n(const regtype *i_tA, const regtype * i_tB, size_t i_tNum_Bits);

	// bit operators
	void shift_left_n(regtype *i_tA,size_t i_tNum_Bits, size_t i_tShift);
	void shift_right_n(regtype *i_tA,size_t i_tNum_Bits, size_t i_tShift);


}
