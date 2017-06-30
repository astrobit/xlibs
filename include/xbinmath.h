#pragma once
#include <cmath>
#include <cstdint>
#include <cstring>
namespace xbinarymath64
{
	// adders (binary)
	bool overflow_adder_64(uint64_t i_tA, uint64_t i_tB, uint64_t &o_tResult);
	bool overflow_adder_n(const uint64_t *i_tA, const uint64_t *i_tB, size_t i_tNum_Bits, uint64_t *&o_tResult);

	// negator (unary)
	void negate(uint64_t * io_tA, size_t i_tNum_Bits);

	// multipliers and squarers
	uint64_t significand_square_normal_64(uint64_t i_tA, bool &o_bOverflow);
	void significand_multiply_64(uint64_t i_tA, uint64_t i_tB, bool &o_bOverflow, size_t &o_tUnderflow, uint64_t & o_tResult_Hi, uint64_t & o_tResult_Lo);
	void significand_multiply_n(const uint64_t * i_tA, const uint64_t * i_tB, size_t i_tNum_Bits, bool &o_bOverflow, size_t &o_tUnderflow, uint64_t *& o_tResult_Hi, uint64_t *& o_tResult_Lo);

	// dividers (binary)
	void significand_divider_64(uint64_t i_tA, uint64_t i_tB, uint64_t &o_tResult, uint64_t & o_tRemainder, int64_t & o_tResult_Exponent);

	// comparators (unary)
	bool greater_than_n(const uint64_t *i_tA, const uint64_t * i_tB, size_t i_tNum_Bits);
	bool not_equal_to_n(const uint64_t *i_tA, const uint64_t * i_tB, size_t i_tNum_Bits);
	bool equal_to_n(const uint64_t *i_tA, const uint64_t * i_tB, size_t i_tNum_Bits);
	bool less_than_or_equal_to_n(const uint64_t *i_tA, const uint64_t * i_tB, size_t i_tNum_Bits);
	bool less_than_n(const uint64_t *i_tA, const uint64_t * i_tB, size_t i_tNum_Bits);
	bool greater_than_or_equal_to_n(const uint64_t *i_tA, const uint64_t * i_tB, size_t i_tNum_Bits);


}
