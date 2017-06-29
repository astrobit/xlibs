#include <xbinmath.h>
#include <functional>
#include <vector>
#include <map>

namespace regarch64
{
	uint64_t mask_low32 = 0xffffffff;
	uint64_t mask_hi_32 = 0xffffffff00000000;
	uint64_t mask_bit_64 = 0x8000000000000000;
	uint64_t mask_bit_32 = 0x80000000;
	uint64_t mask_bit_31 = 0x40000000;
	size_t 	num_bits = 64;
	size_t	half_bits = 32;
	size_t	half_bits_plus_1 = 33;
	size_t	half_bits_minus_1 = 31;
};

bool xbinarymath64::overflow_adder_64(uint64_t i_tA, uint64_t i_tB, uint64_t &o_tResult)
{
	uint64_t tMask = ~regarch64::mask_bit_64;
	bool bOverflow = (i_tA & i_tB & regarch64::mask_bit_64) != 0; // guaranteed overflow
	o_tResult = (i_tA & tMask) + (i_tB & tMask);
	bool bSecondary_Overflow = !bOverflow && ((i_tA | i_tB) & o_tResult & regarch64::mask_bit_64) != 0;
	if (bSecondary_Overflow)
		o_tResult &= tMask; // clear high order bit
	else if (!bOverflow)
		o_tResult |= (i_tA | i_tB) & regarch64::mask_bit_64;
	return (bOverflow || bSecondary_Overflow);
}
bool xbinarymath64::overflow_adder_n(const uint64_t *i_tA, const uint64_t *i_tB, size_t i_tNum_Bits, uint64_t *&o_tResult)
{
	bool bCarry = false;
	size_t tReg_Size = sizeof(uint64_t) * 8;
	if (i_tNum_Bits <= tReg_Size)
	{
		if (o_tResult == nullptr)
			o_tResult = new uint64_t [1];
		bCarry = overflow_adder_64(i_tA[0],i_tB[0],o_tResult[0]);
		if (i_tNum_Bits < tReg_Size)
		{
			size_t tDrop_Bits = tReg_Size - i_tNum_Bits;
			uint64_t tDrop_Mask = 0;
			for (size_t tI = 0; tI < tDrop_Bits; tI++)
			{
				tDrop_Mask <<= 1;
				tDrop_Mask |= 1;
			}
			o_tResult[0] &= (~tDrop_Mask);
		}
	}
	else
	{
		size_t tNum_Words = i_tNum_Bits / tReg_Size;
		size_t tReg_Bits = tNum_Words * tReg_Size;
		if (i_tNum_Bits > tReg_Bits)
		{
			tNum_Words++;
			tReg_Bits += tReg_Size;
		}
		size_t tDrop_Bits = tReg_Size - (i_tNum_Bits % tReg_Size);
	//	size_t tShift_Bits = i_tNum_Bits % tReg_Size;
		uint64_t tDrop_Mask = 0;
		for (size_t tI = 0; tI < tDrop_Bits; tI++)
		{
			tDrop_Mask <<= 1;
			tDrop_Mask |= 1;
		}
		uint64_t tInv_Drop_Mask = ~tDrop_Mask;

		if (o_tResult == nullptr)
			o_tResult = new uint64_t [tNum_Words];

		uint64_t tMask = ~regarch64::mask_bit_64;
		size_t tNW_m_1 = tNum_Words - 1;
		for (size_t tI = 0; tI < tNum_Words; tI++)
		{
			size_t tIdx = tNW_m_1 - tI;
		
			if (bCarry)
			{
				if (i_tA[tIdx] == 0xffffffffffffffff)
				{
					bCarry = true;
					o_tResult[tIdx] = i_tB[tIdx];
				}
				else
				{
					bCarry = overflow_adder_64(i_tA[tIdx] + 1, i_tB[tIdx], o_tResult[tIdx]);
				}
			}
			else if (tI == 0 && i_tNum_Bits < tReg_Bits) // mask low order bits to make sure that there isn't junk that will get incorrectly added into the result
				bCarry = overflow_adder_64((i_tA[tIdx]) & tInv_Drop_Mask, (i_tB[tIdx] & tInv_Drop_Mask), o_tResult[tIdx]);
			else
				bCarry = overflow_adder_64(i_tA[tIdx], i_tB[tIdx], o_tResult[tIdx]);
		}
	}
	return bCarry;
}
uint64_t * g_tNegator_Register = nullptr;
size_t 	g_tNegator_Register_Alloc_Size = 0;

void xbinarymath64::negate(uint64_t * io_tA, size_t i_tNum_Bits)
{
	size_t tReg_Size = sizeof(uint64_t) * 8;
	if (i_tNum_Bits == tReg_Size)
	{
		io_tA[0] = ~io_tA[0];
		io_tA[0]++;
	}
	else
	{
		size_t tNum_Words = i_tNum_Bits / tReg_Size;
		size_t tReg_Bits = tNum_Words * tReg_Size;
		if (i_tNum_Bits > tReg_Bits)
		{
			tNum_Words++;
			tReg_Bits += tReg_Size;
		}
		size_t tDrop_Bits = tReg_Size - (i_tNum_Bits % tReg_Size);
		uint64_t tDrop_Mask = 0;
		for (size_t tI = 0; tI < tDrop_Bits; tI++)
		{
			tDrop_Mask <<= 1;
			tDrop_Mask |= 1;
		}
		uint64_t tInv_Drop_Mask = ~tDrop_Mask;
		if (g_tNegator_Register_Alloc_Size < tNum_Words)
			g_tNegator_Register = new uint64_t [tNum_Words];

		for (size_t tI = 0; tI < tNum_Words; tI++)
		{
			g_tNegator_Register[tI] = ~(io_tA[tI]);
		}
		bool bCarry;
		uint64_t tAdd = 1;
		size_t tNW_m_1 = tNum_Words - 1;

		if (i_tNum_Bits < tReg_Bits)
		{
			tAdd <<= tDrop_Bits;
	//	xstdlib::pbin(tAdd);
	//	xstdlib::pbin(tInv_Drop_Mask);
			g_tNegator_Register[tNum_Words - 1] &= tInv_Drop_Mask; // clear low order bits
		}
	//	xstdlib::pbin(io_tA[0]);
	//	xstdlib::pbin(io_tA[1]);
	//	xstdlib::pbin(g_tNegator_Register[0]);
	//	xstdlib::pbin(g_tNegator_Register[1]);
		bCarry = overflow_adder_64(g_tNegator_Register[tNW_m_1], tAdd, io_tA[tNW_m_1]);
	
	//	xstdlib::pbin(io_tA[1]);
		for (size_t tI = 1; tI < tNum_Words; tI++)
		{
			size_t tIdx = tNW_m_1 - tI;
			if (bCarry)
				bCarry = overflow_adder_64(g_tNegator_Register[tIdx], 1, io_tA[tIdx]);
			else
				io_tA[tIdx] = g_tNegator_Register[tIdx];
		}
	}
}

uint64_t xbinarymath64::significand_square_normal_64(uint64_t i_tA, bool &o_bOverflow)
{
	o_bOverflow = false;
	uint64_t tHi = (i_tA >> regarch64::half_bits);
	uint64_t tLo = i_tA & regarch64::mask_low32;
	uint64_t tLL = tLo * tLo;
	uint64_t tHL = tHi * tLo;
	uint64_t tHH = tHi * tHi;
	uint64_t tLLpHL = (tLL >> regarch64::half_bits_plus_1) + tHL;

	uint64_t tRet = (tLLpHL >> regarch64::half_bits_minus_1) + tHH;
	o_bOverflow = ((tRet & regarch64::mask_bit_64) != 0);
	if ((tRet & regarch64::mask_bit_64) == 0)
	{
		tRet <<= 1; // 
		if (tLLpHL & regarch64::mask_bit_31)
			tRet += 1;
	}
	if (tLLpHL & regarch64::mask_bit_32)
		tRet += 1;
	return tRet;
}



void xbinarymath64::significand_multiply_64(uint64_t i_tA, uint64_t i_tB, bool &o_bOverflow, size_t &o_tUnderflow, uint64_t & o_tResult_Hi, uint64_t & o_tResult_Lo)
{
	if (i_tA != 0 && i_tB != 0)
	{
		uint64_t tHiA = (i_tA >> regarch64::half_bits);
		uint64_t tLoA = i_tA & regarch64::mask_low32;
		uint64_t tHiB = (i_tB >> regarch64::half_bits);
		uint64_t tLoB = i_tB & regarch64::mask_low32;
		uint64_t tLL = tLoA * tLoB;
		uint64_t tHaLb = tHiA * tLoB;
		uint64_t tHbLa = tHiB * tLoA;
		uint64_t tHH = tHiA * tHiB;

		uint64_t tLLh = (tLL >> regarch64::half_bits) & regarch64::mask_low32;
		uint64_t tLLl = tLL & regarch64::mask_low32;

		uint64_t tHaLbh = (tHaLb >> regarch64::half_bits) & regarch64::mask_low32;
		uint64_t tHaLbl = tHaLb & regarch64::mask_low32;

		uint64_t tLaHbh = (tHbLa >> regarch64::half_bits) & regarch64::mask_low32;
		uint64_t tLaHbl = tHbLa & regarch64::mask_low32;

		o_tResult_Hi = tLLh + tHaLbl + tLaHbl;
	//	std::cout<< "a";xstdlib::pbin(o_tResult_Hi);
		o_tResult_Lo = (o_tResult_Hi & regarch64::mask_low32);
		o_tResult_Lo <<= regarch64::half_bits;
		o_tResult_Lo |= tLLl;
	//	std::cout<< "b";xstdlib::pbin(o_tResult_Lo);
		o_tResult_Hi >>= regarch64::half_bits;
		o_tResult_Hi += tHaLbh + tLaHbh + tHH;
	//	std::cout<< "c";xstdlib::pbin(o_tResult_Hi);
		o_bOverflow = (o_tResult_Hi & regarch64::mask_bit_64) != 0;
		o_tUnderflow = 0;
		if (!o_bOverflow)
		{
			o_tUnderflow = -1; // first shift is not considered an underflow, so first shift results in underflow = 0.
			do
			{
				o_tUnderflow++;
				o_tResult_Hi <<= 1;
				if ((o_tResult_Lo & regarch64::mask_bit_64) != 0)
				{
					o_tResult_Hi |= 1;
				}
				o_tResult_Lo <<= 1;
	//			std::cout<< "D";xstdlib::pbin(o_tResult_Hi);
	//			std::cout<< "E";xstdlib::pbin(o_tResult_Lo);
			} while ((o_tResult_Hi & regarch64::mask_bit_64) == 0);

		}
	}
	else
	{
		o_bOverflow = false;
		o_tUnderflow = 0;
		o_tResult_Hi = o_tResult_Lo = 0;
	}
}


uint64_t * g_mmHalf_Registers = nullptr;
uint64_t * g_mmResult_Registers = nullptr;
size_t * g_mmHalf_Shift_Registers = nullptr;

size_t 	g_mmRegister_Alloc_Size = 0;

void xbinarymath64::significand_multiply_n(const uint64_t * i_tA, const uint64_t * i_tB, size_t i_tNum_Bits, bool &o_bOverflow, size_t &o_tUnderflow, uint64_t *& o_tResult_Hi, uint64_t *& o_tResult_Lo)
{
	size_t tReg_Size = sizeof(uint64_t) * 8;
	if (i_tNum_Bits <= tReg_Size)
	{
		if (o_tResult_Hi == nullptr)
			o_tResult_Hi = new uint64_t [1];
		if (o_tResult_Lo == nullptr)
			o_tResult_Lo = new uint64_t [1];
		significand_multiply_64(i_tA[0],i_tB[0],o_bOverflow,o_tUnderflow,o_tResult_Hi[0],o_tResult_Lo[0]);
		if (i_tNum_Bits < tReg_Size)
		{
			size_t tDrop_Bits = tReg_Size - i_tNum_Bits;
			uint64_t tDrop_Mask = 0;
			for (size_t tI = 0; tI < tDrop_Bits; tI++)
			{
				tDrop_Mask <<= 1;
				tDrop_Mask |= 1;
			}
			uint64_t tShift_Data = o_tResult_Hi[0] & tDrop_Mask;
			o_tResult_Hi[0] &= (~tDrop_Mask);
			o_tResult_Lo[0] >>= tDrop_Bits;
			tShift_Data <<= (tReg_Size - tDrop_Bits);
			o_tResult_Lo[0] |= tShift_Data;
			o_tResult_Lo[0] &= (~tDrop_Mask);
		}
	}
	else
	{
		size_t tNum_Words = i_tNum_Bits / tReg_Size;
		size_t tReg_Bits = tNum_Words * tReg_Size;
		if (i_tNum_Bits > tReg_Bits)
		{
			tNum_Words++;
			tReg_Bits += tReg_Size;
		}
		size_t tDrop_Bits = tReg_Size - (i_tNum_Bits % tReg_Size);
		size_t tShift_Bits = i_tNum_Bits % tReg_Size;
		uint64_t tDrop_Mask = 0;
		for (size_t tI = 0; tI < tDrop_Bits; tI++)
		{
			tDrop_Mask <<= 1;
			tDrop_Mask |= 1;
		}
		uint64_t tInv_Drop_Mask = ~tDrop_Mask;

		size_t tDbl_Num_Words = 2 * tNum_Words;

		if (o_tResult_Hi == nullptr)
			o_tResult_Hi = new uint64_t[tNum_Words];
		if (o_tResult_Lo == nullptr)
			o_tResult_Lo = new uint64_t[tNum_Words];
		bool bZero_A = true;
		for (size_t tI = 0; tI < tNum_Words - 1 && bZero_A; tI++)
		{
		
			bZero_A = (i_tA[tI] == 0);
		}
		bZero_A &= ((i_tA[tNum_Words - 1] & tInv_Drop_Mask) == 0);
		bool bZero_B = true;
		for (size_t tI = 0; tI < tNum_Words - 1 && bZero_B; tI++)
		{
		
			bZero_B = (i_tB[tI] == 0);
		}
		bZero_B &= ((i_tB[tNum_Words - 1] & tInv_Drop_Mask) == 0);
		if (!bZero_A && !bZero_B)
		{

			if (g_mmRegister_Alloc_Size != i_tNum_Bits)
			{
				if (g_mmHalf_Registers != nullptr)
					delete [] g_mmHalf_Registers;
				if (g_mmHalf_Shift_Registers != nullptr)
					delete [] g_mmHalf_Shift_Registers;
				if (g_mmResult_Registers != nullptr)
					delete [] g_mmResult_Registers;

				//size_t tHalf_Regs = tNum_Words * 4;
				g_mmHalf_Shift_Registers = new size_t [tDbl_Num_Words * 2];
				g_mmHalf_Registers = new uint64_t [tDbl_Num_Words * 2];
				g_mmRegister_Alloc_Size = i_tNum_Bits;
				g_mmResult_Registers = new uint64_t [tDbl_Num_Words];
			}
			//first created registers containing the upper and lower halfs of each segment
			for (size_t tI = 0; tI < tNum_Words; tI++)
			{
				size_t tI_64 = tI * 2;
				size_t tI_2 = 2 * tI;
				size_t tI_2_p_Dbl_Num_Words = tI_2 + tDbl_Num_Words;

				g_mmHalf_Registers[tI_2] = (i_tA[tI] >> regarch64::half_bits);
				g_mmHalf_Registers[tI_2 + 1] = i_tA[tI] & regarch64::mask_low32;
				g_mmHalf_Shift_Registers[tI_2] = tI_64;
				g_mmHalf_Shift_Registers[tI_2 + 1] = tI_64 + 1;

				g_mmHalf_Registers[tI_2_p_Dbl_Num_Words] = (i_tB[tI] >> regarch64::half_bits);
				g_mmHalf_Registers[tI_2_p_Dbl_Num_Words + 1] = i_tB[tI] & regarch64::mask_low32;
				g_mmHalf_Shift_Registers[tI_2_p_Dbl_Num_Words] = tI_64;
				g_mmHalf_Shift_Registers[tI_2_p_Dbl_Num_Words + 1] = tI_64 + 1;
			}
			std::map<size_t, std::vector<uint64_t>> mAdder_Map;
			//multiply all of the half registers together
			for (size_t tI = 0; tI < tDbl_Num_Words; tI++)
			{
				for (size_t tJ = 0; tJ < tDbl_Num_Words; tJ++)
				{
					uint64_t tR = g_mmHalf_Registers[tI] * g_mmHalf_Registers[tJ + tDbl_Num_Words];
					if (tI == tJ && tI == 0)
						mAdder_Map[0].push_back(tR);
					else
					{
						size_t tHalf_Shifts = g_mmHalf_Shift_Registers[tI] + g_mmHalf_Shift_Registers[tJ + tDbl_Num_Words];
						mAdder_Map[tHalf_Shifts + 1].push_back(tR & regarch64::mask_low32);
						mAdder_Map[tHalf_Shifts].push_back(tR >> regarch64::half_bits);
					}
				}
			}
			memset(g_mmResult_Registers,0,sizeof(uint64_t) * tNum_Words);

			for (auto iterI = mAdder_Map.rbegin(); iterI != mAdder_Map.rend(); iterI++)
			{
				if (iterI != mAdder_Map.rbegin() && iterI->first != 0)
				{
					// shift result
					g_mmResult_Registers[tDbl_Num_Words - 1] >>= regarch64::half_bits;
					for (size_t tI = 1; tI <= (tDbl_Num_Words - 1); tI++)
					{
						size_t tIdx = tDbl_Num_Words - tI;
						uint64_t tTemp = g_mmResult_Registers[tIdx - 1] & regarch64::mask_low32;
						tTemp <<= regarch64::half_bits;
						g_mmResult_Registers[tIdx] |= tTemp;
						g_mmResult_Registers[tIdx - 1] >>= regarch64::half_bits;
		//				xstdlib::pbin(g_mmResult_Registers[0]);
		//				xstdlib::pbin(g_mmResult_Registers[1]);
					}
				}
				// add components
				for (auto iterJ = iterI->second.begin(); iterJ != iterI->second.end(); iterJ++)
				{
					g_mmResult_Registers[0] += *iterJ;
				}
		//		std::cout << iterI->first << " " << iterI->second.size() << " ";
		//		xstdlib::pbin(g_mmResult_Registers[0]);
			}
			o_bOverflow = (g_mmResult_Registers[0] & regarch64::mask_bit_64) != 0;
			o_tUnderflow = 0;
			if (!o_bOverflow)
			{
				o_tUnderflow = -1; // first shift is not considered an underflow, so first shift results in underflow = 0.
				do
				{
					o_tUnderflow++;
					size_t tMax = tDbl_Num_Words - 1;
					for (size_t tI = 0; tI <= tMax; tI++)
					{
						g_mmResult_Registers[tI] <<= 1;
						if (tI < tMax && (g_mmResult_Registers[tI + 1] & regarch64::mask_bit_64) != 0)
						{
							g_mmResult_Registers[tI] |= 1;
						}
					}
				} while ((g_mmResult_Registers[0] & regarch64::mask_bit_64) == 0);
			}
			for(size_t tI = 0; tI < tNum_Words; tI++)
			{
				o_tResult_Hi[tI] = g_mmResult_Registers[tI];
				if (i_tNum_Bits == tReg_Bits)
					o_tResult_Lo[tI] = g_mmResult_Registers[tI + tNum_Words];
			}
			if (i_tNum_Bits != tReg_Bits)
			{
		
				uint64_t tDrop_Data = 0;
				tDrop_Data = o_tResult_Hi[tNum_Words - 1] & tDrop_Mask;
				o_tResult_Hi[tNum_Words - 1] &= tInv_Drop_Mask;
				for (size_t tI = 0; tI < tNum_Words; tI++)
				{
					o_tResult_Lo[tI] = tDrop_Data;
					o_tResult_Lo[tI] <<= tShift_Bits;
					tDrop_Data = g_mmResult_Registers[tI + tNum_Words] & tInv_Drop_Mask;
					tDrop_Data >>= tDrop_Data;
					o_tResult_Lo[tI] |= tDrop_Data;
					tDrop_Data = g_mmResult_Registers[tI + tNum_Words] & tInv_Drop_Mask;
				}
			}
		}
		else
		{
			o_bOverflow = false;
			o_tUnderflow = 0;
			for (size_t tI = 0; tI < tNum_Words; tI++)
			{
				o_tResult_Hi[tI] = o_tResult_Lo[tI] = 0;
			}
		}
	}
}
