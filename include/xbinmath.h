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
