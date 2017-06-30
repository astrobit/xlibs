#include <xbinmath.h>
#include <functional>
#include <vector>
#include <map>
#include <xstdlib.h>
#include <iostream>
namespace regarch
{
	typedef uint64_t	regtype;
	regtype mask_all = 0xffffffffffffffff;
	regtype mask_low_half = 0xffffffff;
	regtype mask_hi_half = 0xffffffff00000000;
	regtype mask_hi_bit = 0x8000000000000000;
	regtype mask_mid_bit = 0x80000000;
	regtype mask_mid_bit_m1 = 0x40000000;
	size_t 	num_bits = 64;
	size_t	half_bits = 32;
	size_t	half_bits_plus_1 = 33;
	size_t	half_bits_minus_1 = 31;
};

class registers
{
public:
private:
	size_t		tWord_Bits;
	size_t		tWord_Size;
public:
	regarch::regtype * 	rRegister_A_4b;
	regarch::regtype * 	rRegister_B_4b;
	regarch::regtype * 	rRegister_C_4b;
	size_t * 	rRegister_Size_4b;

	size_t		tNW_m_1;
	size_t 		tNum_Words;
	size_t		tDbl_Num_Words;
	size_t		tQuad_Num_Words;
	size_t 		tReg_Bits;
	size_t		tReg_Alloc_Size;
	regarch::regtype 	tDrop_Mask;
	size_t 		tDrop_Bits;
	regarch::regtype 	tInv_Drop_Mask;
	size_t		tShift_Bits;

	size_t		tUser_Bits;

	void zero_register_A(void) const
	{
		memset(rRegister_A_4b,0,tReg_Alloc_Size);
	}
	void zero_register_B(void) const
	{
		memset(rRegister_B_4b,0,tReg_Alloc_Size);
	}
	void zero_register_C(void) const
	{
		memset(rRegister_C_4b,0,tReg_Alloc_Size);
	}
	void load_register_A(const void * i_lpData, size_t i_tSize_Mult) const // assume copy register words, multiplier 
	{
		if (i_lpData != nullptr && (tNum_Words * i_tSize_Mult) <= tReg_Alloc_Size)
			std::memcpy(rRegister_A_4b,i_lpData,tNum_Words * i_tSize_Mult * tWord_Size);
	}
	void load_register_B(const void * i_lpData, size_t i_tSize_Mult) const // assume copy register words, multiplier 
	{
		if (i_lpData != nullptr && (tNum_Words * i_tSize_Mult) <= tReg_Alloc_Size)
		{
//			std::cout << "copy " << tNum_Words << " " << i_tSize_Mult << " " << tWord_Size << std::endl;
			std::memcpy(rRegister_B_4b,i_lpData,tNum_Words * i_tSize_Mult * tWord_Size);
		}
	}
	void load_register_C(const void * i_lpData, size_t i_tSize_Mult) const // assume copy register words, multiplier 
	{
		if (i_lpData != nullptr && (tNum_Words * i_tSize_Mult) <= tReg_Alloc_Size)
			std::memcpy(rRegister_C_4b,i_lpData,tNum_Words * i_tSize_Mult * tWord_Size);
	}
	registers(void)
	{
		rRegister_A_4b = nullptr;
		rRegister_B_4b = nullptr;
		rRegister_C_4b = nullptr;
		rRegister_Size_4b = nullptr;

		tNW_m_1 = 0;
		tUser_Bits = 0;
		tNum_Words = 0;
		tDbl_Num_Words = 0;
		tReg_Bits = 0;
		tDrop_Mask = 0xffffffffffffffff;
		tDrop_Bits = 64;
		tShift_Bits = 0;
		tInv_Drop_Mask = 0;
		tWord_Size = sizeof(regarch::regtype);
		tWord_Bits = tWord_Size * 8;
	}
	inline size_t word_size(void) const
	{
		return tWord_Size;
	}
	inline size_t reg_size(void) const
	{
		return tWord_Bits;
	}
	void set_register_size(size_t i_tNum_Bits)
	{
		if (i_tNum_Bits != tUser_Bits)
		{
			tUser_Bits = i_tNum_Bits;
			tNum_Words = i_tNum_Bits / tWord_Bits;
			tReg_Bits = tNum_Words * tWord_Bits;
			if (i_tNum_Bits > tReg_Bits)
			{
				tNum_Words++;
				tReg_Bits += tWord_Bits;
			}
			tNW_m_1 = tNum_Words - 1;
			tDbl_Num_Words = tNum_Words << 1;
			tQuad_Num_Words = tDbl_Num_Words << 1;
			tDrop_Bits = (tWord_Bits - (i_tNum_Bits % tWord_Bits)) % tWord_Bits;
			tShift_Bits = i_tNum_Bits % tWord_Bits;
			//	size_t tShift_Bits = i_tNum_Bits % cRegister.reg_size();
			tDrop_Mask = 0;
			for (size_t tI = 0; tI < tDrop_Bits; tI++)
			{
				tDrop_Mask <<= 1;
				tDrop_Mask |= 1;
			}
			tInv_Drop_Mask = ~tDrop_Mask;

			if (rRegister_Size_4b != nullptr)
				delete [] rRegister_Size_4b;
			if (rRegister_A_4b != nullptr)
				delete [] rRegister_A_4b;
			if (rRegister_B_4b != nullptr)
				delete [] rRegister_B_4b;
			if (rRegister_C_4b != nullptr)
				delete [] rRegister_C_4b;

			// allocate registers as quad size
			rRegister_Size_4b = new size_t [tQuad_Num_Words];
			rRegister_A_4b = new regarch::regtype [tQuad_Num_Words];
			rRegister_B_4b = new regarch::regtype [tQuad_Num_Words];
			rRegister_C_4b = new regarch::regtype [tQuad_Num_Words];
			tReg_Alloc_Size = tWord_Size * tQuad_Num_Words;
		}
	}
};

std::map<size_t, registers> g_mapRegisters;

bool xbinarymath64::overflow_adder_64(uint64_t i_tA, uint64_t i_tB, uint64_t &o_tResult)
{
	uint64_t tMask = ~regarch::mask_hi_bit;
	bool bOverflow = (i_tA & i_tB & regarch::mask_hi_bit) != 0; // guaranteed overflow
	o_tResult = (i_tA & tMask) + (i_tB & tMask);
	bool bSecondary_Overflow = !bOverflow && ((i_tA | i_tB) & o_tResult & regarch::mask_hi_bit) != 0;
	if (bSecondary_Overflow)
		o_tResult &= tMask; // clear high order bit
	else if (!bOverflow)
		o_tResult |= (i_tA | i_tB) & regarch::mask_hi_bit;
	return (bOverflow || bSecondary_Overflow);
}
bool xbinarymath64::overflow_adder_n(const uint64_t *i_tA, const uint64_t *i_tB, size_t i_tNum_Bits, uint64_t *&o_tResult)
{
	bool bCarry = false;
	g_mapRegisters[i_tNum_Bits].set_register_size(i_tNum_Bits);
	const registers &cRegister (g_mapRegisters[i_tNum_Bits]);
	if (i_tNum_Bits <= cRegister.reg_size())
	{
		if (o_tResult == nullptr)
			o_tResult = new uint64_t [1];
		bCarry = overflow_adder_64(i_tA[0],i_tB[0],o_tResult[0]);
		if (i_tNum_Bits < cRegister.reg_size())
		{
			o_tResult[0] &= cRegister.tInv_Drop_Mask;
		}
	}
	else
	{
		if (o_tResult == nullptr)
			o_tResult = new uint64_t [cRegister.tNum_Words];

		uint64_t tMask = ~regarch::mask_hi_bit;
		size_t tNW_m_1 = cRegister.tNum_Words - 1;
		for (size_t tI = 0; tI < cRegister.tNum_Words; tI++)
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
			else if (tI == 0 && i_tNum_Bits < cRegister.tReg_Bits) // mask low order bits to make sure that there isn't junk that will get incorrectly added into the result
				bCarry = overflow_adder_64((i_tA[tIdx]) & cRegister.tInv_Drop_Mask, (i_tB[tIdx] & cRegister.tInv_Drop_Mask), o_tResult[tIdx]);
			else
				bCarry = overflow_adder_64(i_tA[tIdx], i_tB[tIdx], o_tResult[tIdx]);
		}
	}
	return bCarry;
}

void xbinarymath64::negate(uint64_t * io_tA, size_t i_tNum_Bits)
{
	g_mapRegisters[i_tNum_Bits].set_register_size(i_tNum_Bits);
	const registers &cRegister (g_mapRegisters[i_tNum_Bits]);
	if (i_tNum_Bits <= cRegister.reg_size())
	{
		io_tA[0] &= cRegister.tInv_Drop_Mask;
		//xstdlib::pbin(cRegister.tInv_Drop_Mask);
		//xstdlib::pbin(io_tA[0]);
		io_tA[0] = ~io_tA[0];
		//xstdlib::pbin(io_tA[0]);
		io_tA[0]++;
		//xstdlib::pbin(io_tA[0]);
		io_tA[0] &= cRegister.tInv_Drop_Mask;
		//xstdlib::pbin(io_tA[0]);
	}
	else
	{
		for (size_t tI = 0; tI < cRegister.tNum_Words; tI++)
		{
			cRegister.rRegister_A_4b[tI] = ~(io_tA[tI]);
			//xstdlib::pbin(io_tA[tI]);
		}
		uint64_t tAdd = 1;

		if (i_tNum_Bits < cRegister.tReg_Bits)
		{
			tAdd <<= cRegister.tDrop_Bits;
	//	xstdlib::pbin(tAdd);
			cRegister.rRegister_A_4b[cRegister.tNW_m_1] &= cRegister.tInv_Drop_Mask; // clear low order bits
		}
	//	xstdlib::pbin(io_tA[0]);
	//	xstdlib::pbin(io_tA[1]);
	//	xstdlib::pbin(cRegister.rRegister_A_4b[0]);
	//	xstdlib::pbin(cRegister.rRegister_A_4b[1]);
		bool bCarry = overflow_adder_64(cRegister.rRegister_A_4b[cRegister.tNW_m_1], tAdd, io_tA[cRegister.tNW_m_1]);
	
	//	xstdlib::pbin(io_tA[1]);
		for (size_t tI = 1; tI < cRegister.tNum_Words; tI++)
		{
			size_t tIdx = cRegister.tNW_m_1 - tI;
			if (bCarry)
				bCarry = overflow_adder_64(cRegister.rRegister_A_4b[tIdx], 1, io_tA[tIdx]);
			else
				io_tA[tIdx] = cRegister.rRegister_A_4b[tIdx];
		}
	}
}

uint64_t xbinarymath64::significand_square_normal_64(uint64_t i_tA, bool &o_bOverflow)
{
	o_bOverflow = false;
	uint64_t tHi = (i_tA >> regarch::half_bits);
	uint64_t tLo = i_tA & regarch::mask_low_half;
	uint64_t tLL = tLo * tLo;
	uint64_t tHL = tHi * tLo;
	uint64_t tHH = tHi * tHi;
	uint64_t tLLpHL = (tLL >> regarch::half_bits_plus_1) + tHL;

	uint64_t tRet = (tLLpHL >> regarch::half_bits_minus_1) + tHH;
	o_bOverflow = ((tRet & regarch::mask_hi_bit) != 0);
	if ((tRet & regarch::mask_hi_bit) == 0)
	{
		tRet <<= 1; // 
		if (tLLpHL & regarch::mask_mid_bit_m1)
			tRet += 1;
	}
	if (tLLpHL & regarch::mask_mid_bit)
		tRet += 1;
	return tRet;
}



void xbinarymath64::significand_multiply_64(uint64_t i_tA, uint64_t i_tB, bool &o_bOverflow, size_t &o_tUnderflow, uint64_t & o_tResult_Hi, uint64_t & o_tResult_Lo)
{
	if (i_tA != 0 && i_tB != 0)
	{
		uint64_t tHiA = (i_tA >> regarch::half_bits);
		uint64_t tLoA = i_tA & regarch::mask_low_half;
		uint64_t tHiB = (i_tB >> regarch::half_bits);
		uint64_t tLoB = i_tB & regarch::mask_low_half;
		uint64_t tLL = tLoA * tLoB;
		uint64_t tHaLb = tHiA * tLoB;
		uint64_t tHbLa = tHiB * tLoA;
		uint64_t tHH = tHiA * tHiB;

		uint64_t tLLh = (tLL >> regarch::half_bits) & regarch::mask_low_half;
		uint64_t tLLl = tLL & regarch::mask_low_half;

		uint64_t tHaLbh = (tHaLb >> regarch::half_bits) & regarch::mask_low_half;
		uint64_t tHaLbl = tHaLb & regarch::mask_low_half;

		uint64_t tLaHbh = (tHbLa >> regarch::half_bits) & regarch::mask_low_half;
		uint64_t tLaHbl = tHbLa & regarch::mask_low_half;

		o_tResult_Hi = tLLh + tHaLbl + tLaHbl;
	//	std::cout<< "a";xstdlib::pbin(o_tResult_Hi);
		o_tResult_Lo = (o_tResult_Hi & regarch::mask_low_half);
		o_tResult_Lo <<= regarch::half_bits;
		o_tResult_Lo |= tLLl;
	//	std::cout<< "b";xstdlib::pbin(o_tResult_Lo);
		o_tResult_Hi >>= regarch::half_bits;
		o_tResult_Hi += tHaLbh + tLaHbh + tHH;
	//	std::cout<< "c";xstdlib::pbin(o_tResult_Hi);
		o_bOverflow = (o_tResult_Hi & regarch::mask_hi_bit) != 0;
		o_tUnderflow = 0;
		if (!o_bOverflow)
		{
			o_tUnderflow = -1; // first shift is not considered an underflow, so first shift results in underflow = 0.
			do
			{
				o_tUnderflow++;
				o_tResult_Hi <<= 1;
				if ((o_tResult_Lo & regarch::mask_hi_bit) != 0)
				{
					o_tResult_Hi |= 1;
				}
				o_tResult_Lo <<= 1;
	//			std::cout<< "D";xstdlib::pbin(o_tResult_Hi);
	//			std::cout<< "E";xstdlib::pbin(o_tResult_Lo);
			} while ((o_tResult_Hi & regarch::mask_hi_bit) == 0);

		}
	}
	else
	{
		o_bOverflow = false;
		o_tUnderflow = 0;
		o_tResult_Hi = o_tResult_Lo = 0;
	}
}


void xbinarymath64::significand_multiply_n(const uint64_t * i_tA, const uint64_t * i_tB, size_t i_tNum_Bits, bool &o_bOverflow, size_t &o_tUnderflow, uint64_t *& o_tResult_Hi, uint64_t *& o_tResult_Lo)
{
	g_mapRegisters[i_tNum_Bits].set_register_size(i_tNum_Bits);
	const registers &cRegister (g_mapRegisters[i_tNum_Bits]);
	if (i_tNum_Bits <= cRegister.reg_size())
	{
		if (o_tResult_Hi == nullptr)
			o_tResult_Hi = new uint64_t [1];
		if (o_tResult_Lo == nullptr)
			o_tResult_Lo = new uint64_t [1];
		significand_multiply_64(i_tA[0],i_tB[0],o_bOverflow,o_tUnderflow,o_tResult_Hi[0],o_tResult_Lo[0]);
		if (i_tNum_Bits < cRegister.reg_size())
		{
			uint64_t tShift_Data = o_tResult_Hi[0] & cRegister.tDrop_Mask;
			o_tResult_Hi[0] &= cRegister.tInv_Drop_Mask;
			o_tResult_Lo[0] >>= cRegister.tDrop_Bits;
			tShift_Data <<= (cRegister.reg_size() - cRegister.tDrop_Bits);
			o_tResult_Lo[0] |= tShift_Data;
			o_tResult_Lo[0] &= cRegister.tInv_Drop_Mask;
		}
	}
	else
	{
		if (o_tResult_Hi == nullptr)
			o_tResult_Hi = new uint64_t[cRegister.tNum_Words];
		if (o_tResult_Lo == nullptr)
			o_tResult_Lo = new uint64_t[cRegister.tNum_Words];
		bool bZero_A = true;
		for (size_t tI = 0; tI < cRegister.tNum_Words - 1 && bZero_A; tI++)
		{
		
			bZero_A = (i_tA[tI] == 0);
		}
		bZero_A &= ((i_tA[cRegister.tNum_Words - 1] & cRegister.tInv_Drop_Mask) == 0);
		bool bZero_B = true;
		for (size_t tI = 0; tI < cRegister.tNum_Words - 1 && bZero_B; tI++)
		{
		
			bZero_B = (i_tB[tI] == 0);
		}
		bZero_B &= ((i_tB[cRegister.tNum_Words - 1] & cRegister.tInv_Drop_Mask) == 0);
		if (!bZero_A && !bZero_B)
		{

			//first created registers containing the upper and lower halfs of each segment
			for (size_t tI = 0; tI < cRegister.tNum_Words; tI++)
			{
				size_t tI_64 = tI * 2;
				size_t tI_2 = 2 * tI;
				size_t tI_2_p_Dbl_Num_Words = tI_2 + cRegister.tDbl_Num_Words;

				cRegister.rRegister_A_4b[tI_2] = (i_tA[tI] >> regarch::half_bits);
				cRegister.rRegister_A_4b[tI_2 + 1] = i_tA[tI] & regarch::mask_low_half;
				cRegister.rRegister_Size_4b[tI_2] = tI_64;
				cRegister.rRegister_Size_4b[tI_2 + 1] = tI_64 + 1;

				cRegister.rRegister_A_4b[tI_2_p_Dbl_Num_Words] = (i_tB[tI] >> regarch::half_bits);
				cRegister.rRegister_A_4b[tI_2_p_Dbl_Num_Words + 1] = i_tB[tI] & regarch::mask_low_half;
				cRegister.rRegister_Size_4b[tI_2_p_Dbl_Num_Words] = tI_64;
				cRegister.rRegister_Size_4b[tI_2_p_Dbl_Num_Words + 1] = tI_64 + 1;
			}
			std::map<size_t, std::vector<uint64_t>> mAdder_Map;
			//multiply all of the half registers together
			for (size_t tI = 0; tI < cRegister.tDbl_Num_Words; tI++)
			{
				for (size_t tJ = 0; tJ < cRegister.tDbl_Num_Words; tJ++)
				{
					uint64_t tR = cRegister.rRegister_A_4b[tI] * cRegister.rRegister_A_4b[tJ + cRegister.tDbl_Num_Words];
					if (tI == tJ && tI == 0)
						mAdder_Map[0].push_back(tR);
					else
					{
						size_t tHalf_Shifts = cRegister.rRegister_Size_4b[tI] + cRegister.rRegister_Size_4b[tJ + cRegister.tDbl_Num_Words];
						mAdder_Map[tHalf_Shifts + 1].push_back(tR & regarch::mask_low_half);
						mAdder_Map[tHalf_Shifts].push_back(tR >> regarch::half_bits);
					}
				}
			}
			cRegister.zero_register_B();

			for (auto iterI = mAdder_Map.rbegin(); iterI != mAdder_Map.rend(); iterI++)
			{
				if (iterI != mAdder_Map.rbegin() && iterI->first != 0)
				{
					// shift result
					cRegister.rRegister_B_4b[cRegister.tDbl_Num_Words - 1] >>= regarch::half_bits;
					for (size_t tI = 1; tI < cRegister.tDbl_Num_Words; tI++)
					{
						size_t tIdx = cRegister.tDbl_Num_Words - tI;
						uint64_t tTemp = cRegister.rRegister_A_4b[tIdx - 1] & regarch::mask_low_half;
						tTemp <<= regarch::half_bits;
						cRegister.rRegister_A_4b[tIdx] |= tTemp;
						cRegister.rRegister_B_4b[tIdx - 1] >>= regarch::half_bits;
		//				xstdlib::pbin(g_mmResult_Registers[0]);
		//				xstdlib::pbin(g_mmResult_Registers[1]);
					}
				}
				// add components
				for (auto iterJ = iterI->second.begin(); iterJ != iterI->second.end(); iterJ++)
				{
					cRegister.rRegister_B_4b[0] += *iterJ;
				}
		//		std::cout << iterI->first << " " << iterI->second.size() << " ";
		//		xstdlib::pbin(g_mmResult_Registers[0]);
			}
			o_bOverflow = (cRegister.rRegister_B_4b[0] & regarch::mask_hi_bit) != 0;
			o_tUnderflow = 0;
			if (!o_bOverflow)
			{
				o_tUnderflow = -1; // first shift is not considered an underflow, so first shift results in underflow = 0.
				do
				{
					o_tUnderflow++;
					size_t tMax = cRegister.tDbl_Num_Words - 1;
					for (size_t tI = 0; tI < cRegister.tDbl_Num_Words; tI++)
					{
						cRegister.rRegister_B_4b[tI] <<= 1;
						if (tI < tMax && (cRegister.rRegister_B_4b[tI + 1] & regarch::mask_hi_bit) != 0)
						{
							cRegister.rRegister_B_4b[tI] |= 1;
						}
					}
				} while ((cRegister.rRegister_B_4b[0] & regarch::mask_hi_bit) == 0);
			}
			for(size_t tI = 0; tI < cRegister.tNum_Words; tI++)
			{
				o_tResult_Hi[tI] = cRegister.rRegister_B_4b[tI];
				if (i_tNum_Bits == cRegister.tReg_Bits)
					o_tResult_Lo[tI] = cRegister.rRegister_B_4b[tI + cRegister.tNum_Words];
			}
			if (i_tNum_Bits != cRegister.tReg_Bits)
			{
		
				uint64_t tDrop_Data = 0;
				tDrop_Data = o_tResult_Hi[cRegister.tNum_Words - 1] & cRegister.tDrop_Mask;
				o_tResult_Hi[cRegister.tNum_Words - 1] &= cRegister.tInv_Drop_Mask;
				for (size_t tI = 0; tI < cRegister.tNum_Words; tI++)
				{
					o_tResult_Lo[tI] = tDrop_Data;
					o_tResult_Lo[tI] <<= cRegister.tShift_Bits;
					tDrop_Data = cRegister.rRegister_B_4b[tI + cRegister.tNum_Words] & cRegister.tInv_Drop_Mask;
					tDrop_Data >>= tDrop_Data;
					o_tResult_Lo[tI] |= tDrop_Data;
					tDrop_Data = cRegister.rRegister_B_4b[tI + cRegister.tNum_Words] & cRegister.tInv_Drop_Mask;
				}
			}
		}
		else
		{
			o_bOverflow = false;
			o_tUnderflow = 0;
			for (size_t tI = 0; tI < cRegister.tNum_Words; tI++)
			{
				o_tResult_Hi[tI] = o_tResult_Lo[tI] = 0;
			}
		}
	}
}



void xbinarymath64::significand_divider_64(uint64_t i_tA, uint64_t i_tB, uint64_t &o_tResult, uint64_t & o_tRemainder, int64_t & o_tResult_Exponent)
{
	if (i_tA == 0)
	{
		o_tResult = 0;
		o_tRemainder = 0;
	}
	else if (i_tB != 0)
	{
		bool bHigh_Bit_Flag = false;
		//uint64_t tDenomenator = i_tB;

		o_tRemainder = i_tA;
		o_tResult_Exponent = 64;//64;
		o_tResult = 0;
		if (o_tRemainder >= i_tB)
		{
			o_tResult = o_tRemainder / i_tB;
			o_tRemainder -= o_tResult * i_tB;
		}
		while ((o_tResult & regarch::mask_hi_bit) == 0 && o_tRemainder != 0)
		{
			o_tResult <<= 1;
			if (o_tRemainder >= i_tB || bHigh_Bit_Flag)
			{
				o_tRemainder -= i_tB;
				o_tResult |= 1;
			}
			o_tResult_Exponent--;
			bHigh_Bit_Flag = (o_tRemainder & regarch::mask_hi_bit) != 0;
			o_tRemainder <<= 1;
		}
		while ((o_tResult & regarch::mask_hi_bit) == 0)
		{
			o_tResult_Exponent--;
			o_tResult <<= 1;
		}
	}
}

bool xbinarymath64::greater_than_n(const uint64_t *i_tA, const uint64_t * i_tB, size_t i_tNum_Bits)
{
	g_mapRegisters[i_tNum_Bits].set_register_size(i_tNum_Bits);
	registers &cRegister (g_mapRegisters[i_tNum_Bits]);

	cRegister.load_register_B(i_tB,1);
	negate(cRegister.rRegister_B_4b,i_tNum_Bits);
//	xstdlib::pbin(cRegister.rRegister_B_4b[0]);
//	xstdlib::pbin(cRegister.rRegister_B_4b[1]);
//	xstdlib::pbin(i_tA[0]);
//	xstdlib::pbin(i_tA[1]);
	overflow_adder_n(i_tA,cRegister.rRegister_B_4b,i_tNum_Bits,cRegister.rRegister_C_4b);
//	xstdlib::pbin(cRegister.rRegister_C_4b[0]);
//	xstdlib::pbin(cRegister.rRegister_C_4b[1]);

	bool bGreater_Than = (cRegister.rRegister_C_4b[0] & regarch::mask_hi_bit) == 0;
	bool bEqual = true;
	for (size_t tI = 0; tI < cRegister.tNum_Words && bEqual; tI++)
	{
		bEqual = (cRegister.rRegister_C_4b[tI] == 0);
	}
		
	return bGreater_Than && !bEqual;
}
bool xbinarymath64::greater_than_or_equal_to_n(const uint64_t *i_tA, const uint64_t * i_tB, size_t i_tNum_Bits)
{
	g_mapRegisters[i_tNum_Bits].set_register_size(i_tNum_Bits);
	registers &cRegister (g_mapRegisters[i_tNum_Bits]);

	cRegister.load_register_B(i_tB,1);
	negate(cRegister.rRegister_B_4b,i_tNum_Bits);
	overflow_adder_n(i_tA,cRegister.rRegister_B_4b,i_tNum_Bits,cRegister.rRegister_C_4b);
	bool bGreater_Than = (cRegister.rRegister_C_4b[0] & regarch::mask_hi_bit) == 0;
	//xstdlib::pbin(cRegister.rRegister_C_4b[0]);
	bool bEqual = true;
	for (size_t tI = 0; tI < cRegister.tNum_Words && bEqual; tI++)
	{
		bEqual = (cRegister.rRegister_C_4b[tI] == 0);
	}
		
	return bGreater_Than || bEqual;
}
bool xbinarymath64::less_than_n(const uint64_t *i_tA, const uint64_t * i_tB, size_t i_tNum_Bits)
{
	return !greater_than_or_equal_to_n(i_tA,i_tB,i_tNum_Bits);
}
bool xbinarymath64::less_than_or_equal_to_n(const uint64_t *i_tA, const uint64_t * i_tB, size_t i_tNum_Bits)
{
	return !greater_than_n(i_tA,i_tB,i_tNum_Bits);
}
bool xbinarymath64::equal_to_n(const uint64_t *i_tA, const uint64_t * i_tB, size_t i_tNum_Bits)
{
	g_mapRegisters[i_tNum_Bits].set_register_size(i_tNum_Bits);
	registers &cRegister (g_mapRegisters[i_tNum_Bits]);

	cRegister.load_register_B(i_tB,1);
	//xstdlib::pbin(i_tB[0]);
	//if (i_tNum_Bits > 64)
	//	xstdlib::pbin(i_tB[1]);
	//xstdlib::pbin(cRegister.rRegister_B_4b[0]);
	//xstdlib::pbin(cRegister.rRegister_B_4b[1]);
	xbinarymath64::negate(cRegister.rRegister_B_4b,i_tNum_Bits);
	//xstdlib::pbin(cRegister.rRegister_B_4b[0]);
	//xstdlib::pbin(cRegister.rRegister_B_4b[1]);
	overflow_adder_n(i_tA,cRegister.rRegister_B_4b,i_tNum_Bits,cRegister.rRegister_C_4b);
	//xstdlib::pbin(cRegister.rRegister_C_4b[0]);
	//xstdlib::pbin(cRegister.rRegister_C_4b[1]);
	bool bEqual = true;
	for (size_t tI = 0; tI < cRegister.tNum_Words && bEqual; tI++)
	{
		bEqual = (cRegister.rRegister_C_4b[tI] == 0);
	}
		
	return bEqual;
}
bool xbinarymath64::not_equal_to_n(const uint64_t *i_tA, const uint64_t * i_tB, size_t i_tNum_Bits)
{
	return !equal_to_n(i_tA,i_tB,i_tNum_Bits);
}

