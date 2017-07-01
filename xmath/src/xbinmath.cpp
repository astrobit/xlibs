#include <xbinmath.h>
#include <functional>
#include <vector>
#include <map>
#include <xstdlib.h>
#include <iostream>
bool g_bRegArchInit = false;
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
c_regarch	regarch;

class registers
{
public:
private:
	size_t		tWord_Bits;
	size_t		tWord_Size;
public:
	xbinarymath::regtype * 	rRegister_A_4b;
	xbinarymath::regtype * 	rRegister_B_4b;
	xbinarymath::regtype * 	rRegister_C_4b;
	size_t * 	rRegister_Size_4b;

	size_t		tNW_m_1;
	size_t 		tNum_Words;
	size_t		tDbl_Num_Words;
	size_t		tQuad_Num_Words;
	size_t 		tReg_Bits;
	size_t		tReg_Alloc_Size;
	xbinarymath::regtype 	tDrop_Mask;
	size_t 		tDrop_Bits;
	xbinarymath::regtype 	tInv_Drop_Mask;
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
		if (!g_bRegArchInit)
			regarch.initialize();

		rRegister_A_4b = nullptr;
		rRegister_B_4b = nullptr;
		rRegister_C_4b = nullptr;
		rRegister_Size_4b = nullptr;

		tNW_m_1 = 0;
		tUser_Bits = 0;
		tNum_Words = 0;
		tDbl_Num_Words = 0;
		tReg_Bits = 0;
		tDrop_Mask = regarch.mask_all;
		tDrop_Bits = regarch.num_bits;
		tShift_Bits = 0;
		tInv_Drop_Mask = 0;
		tWord_Size = sizeof(xbinarymath::regtype);
		tWord_Bits = regarch.num_bits;
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
			if (!g_bRegArchInit)
				regarch.initialize();
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
			rRegister_A_4b = new xbinarymath::regtype [tQuad_Num_Words];
			rRegister_B_4b = new xbinarymath::regtype [tQuad_Num_Words];
			rRegister_C_4b = new xbinarymath::regtype [tQuad_Num_Words];
			tReg_Alloc_Size = tWord_Size * tQuad_Num_Words;
		}
	}
};

std::map<size_t, registers> g_mapRegisters;

bool xbinarymath::overflow_adder_arch(xbinarymath::regtype i_tA, xbinarymath::regtype i_tB, xbinarymath::regtype &o_tResult)
{
	if (!g_bRegArchInit)
		regarch.initialize();
	xbinarymath::regtype tMask = ~regarch.mask_hi_bit;
	bool bOverflow = (i_tA & i_tB & regarch.mask_hi_bit) != 0; // guaranteed overflow
	o_tResult = (i_tA & tMask) + (i_tB & tMask);
	bool bSecondary_Overflow = !bOverflow && ((i_tA | i_tB) & o_tResult & regarch.mask_hi_bit) != 0;
	if (bSecondary_Overflow)
		o_tResult &= tMask; // clear high order bit
	else if (!bOverflow)
		o_tResult |= (i_tA | i_tB) & regarch.mask_hi_bit;
	return (bOverflow || bSecondary_Overflow);
}
bool xbinarymath::overflow_adder_n(const xbinarymath::regtype *i_tA, const xbinarymath::regtype *i_tB, size_t i_tNum_Bits, xbinarymath::regtype *&o_tResult)
{
	bool bCarry = false;
	g_mapRegisters[i_tNum_Bits].set_register_size(i_tNum_Bits);
	const registers &cRegister (g_mapRegisters[i_tNum_Bits]);
	if (i_tNum_Bits <= cRegister.reg_size())
	{
		if (o_tResult == nullptr)
			o_tResult = new xbinarymath::regtype [1];
		bCarry = overflow_adder_arch(i_tA[0],i_tB[0],o_tResult[0]);
		if (i_tNum_Bits < cRegister.reg_size())
		{
			o_tResult[0] &= cRegister.tInv_Drop_Mask;
		}
	}
	else
	{
		if (o_tResult == nullptr)
			o_tResult = new xbinarymath::regtype [cRegister.tNum_Words];

		xbinarymath::regtype tMask = ~regarch.mask_hi_bit;
		size_t tNW_m_1 = cRegister.tNum_Words - 1;
		for (size_t tI = 0; tI < cRegister.tNum_Words; tI++)
		{
			size_t tIdx = tNW_m_1 - tI;
		
			if (bCarry)
			{
				if (i_tA[tIdx] == regarch.mask_all)
				{
					bCarry = true;
					o_tResult[tIdx] = i_tB[tIdx];
				}
				else
				{
					bCarry = overflow_adder_arch(i_tA[tIdx] + 1, i_tB[tIdx], o_tResult[tIdx]);
				}
			}
			else if (tI == 0 && i_tNum_Bits < cRegister.tReg_Bits) // mask low order bits to make sure that there isn't junk that will get incorrectly added into the result
				bCarry = overflow_adder_arch((i_tA[tIdx]) & cRegister.tInv_Drop_Mask, (i_tB[tIdx] & cRegister.tInv_Drop_Mask), o_tResult[tIdx]);
			else
				bCarry = overflow_adder_arch(i_tA[tIdx], i_tB[tIdx], o_tResult[tIdx]);
		}
	}
	return bCarry;
}

void xbinarymath::negate(xbinarymath::regtype * io_tA, size_t i_tNum_Bits)
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
		xbinarymath::regtype tAdd = 1;

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
		bool bCarry = overflow_adder_arch(cRegister.rRegister_A_4b[cRegister.tNW_m_1], tAdd, io_tA[cRegister.tNW_m_1]);
	
	//	xstdlib::pbin(io_tA[1]);
		for (size_t tI = 1; tI < cRegister.tNum_Words; tI++)
		{
			size_t tIdx = cRegister.tNW_m_1 - tI;
			if (bCarry)
				bCarry = overflow_adder_arch(cRegister.rRegister_A_4b[tIdx], 1, io_tA[tIdx]);
			else
				io_tA[tIdx] = cRegister.rRegister_A_4b[tIdx];
		}
	}
}

xbinarymath::regtype xbinarymath::significand_square_normal_arch(xbinarymath::regtype i_tA, bool &o_bOverflow)
{
	if (!g_bRegArchInit)
		regarch.initialize();
	o_bOverflow = false;
	xbinarymath::regtype tHi = (i_tA >> regarch.half_bits);
	xbinarymath::regtype tLo = i_tA & regarch.mask_low_half;
	xbinarymath::regtype tLL = tLo * tLo;
	xbinarymath::regtype tHL = tHi * tLo;
	xbinarymath::regtype tHH = tHi * tHi;
	xbinarymath::regtype tLLpHL = (tLL >> regarch.half_bits_plus_1) + tHL;

	xbinarymath::regtype tRet = (tLLpHL >> regarch.half_bits_minus_1) + tHH;
	o_bOverflow = ((tRet & regarch.mask_hi_bit) != 0);
	if ((tRet & regarch.mask_hi_bit) == 0)
	{
		tRet <<= 1; // 
		if (tLLpHL & regarch.mask_mid_bit_m1)
			tRet += 1;
	}
	if (tLLpHL & regarch.mask_mid_bit)
		tRet += 1;
	return tRet;
}



void xbinarymath::significand_multiply_arch(xbinarymath::regtype i_tA, xbinarymath::regtype i_tB, bool &o_bOverflow, size_t &o_tUnderflow, xbinarymath::regtype & o_tResult_Hi, xbinarymath::regtype & o_tResult_Lo)
{
	if (!g_bRegArchInit)
		regarch.initialize();
	if (i_tA != 0 && i_tB != 0)
	{
		xbinarymath::regtype tHiA = (i_tA >> regarch.half_bits);
		xbinarymath::regtype tLoA = i_tA & regarch.mask_low_half;
		xbinarymath::regtype tHiB = (i_tB >> regarch.half_bits);
		xbinarymath::regtype tLoB = i_tB & regarch.mask_low_half;
		xbinarymath::regtype tLL = tLoA * tLoB;
		xbinarymath::regtype tHaLb = tHiA * tLoB;
		xbinarymath::regtype tHbLa = tHiB * tLoA;
		xbinarymath::regtype tHH = tHiA * tHiB;

		xbinarymath::regtype tLLh = (tLL >> regarch.half_bits) & regarch.mask_low_half;
		xbinarymath::regtype tLLl = tLL & regarch.mask_low_half;

		xbinarymath::regtype tHaLbh = (tHaLb >> regarch.half_bits) & regarch.mask_low_half;
		xbinarymath::regtype tHaLbl = tHaLb & regarch.mask_low_half;

		xbinarymath::regtype tLaHbh = (tHbLa >> regarch.half_bits) & regarch.mask_low_half;
		xbinarymath::regtype tLaHbl = tHbLa & regarch.mask_low_half;

		o_tResult_Hi = tLLh + tHaLbl + tLaHbl;
	//	std::cout<< "a";xstdlib::pbin(o_tResult_Hi);
		o_tResult_Lo = (o_tResult_Hi & regarch.mask_low_half);
		o_tResult_Lo <<= regarch.half_bits;
		o_tResult_Lo |= tLLl;
	//	std::cout<< "b";xstdlib::pbin(o_tResult_Lo);
		o_tResult_Hi >>= regarch.half_bits;
		o_tResult_Hi += tHaLbh + tLaHbh + tHH;
	//	std::cout<< "c";xstdlib::pbin(o_tResult_Hi);
		o_bOverflow = (o_tResult_Hi & regarch.mask_hi_bit) != 0;
		o_tUnderflow = 0;
		if (!o_bOverflow)
		{
			o_tUnderflow = -1; // first shift is not considered an underflow, so first shift results in underflow = 0.
			do
			{
				o_tUnderflow++;
				o_tResult_Hi <<= 1;
				if ((o_tResult_Lo & regarch.mask_hi_bit) != 0)
				{
					o_tResult_Hi |= 1;
				}
				o_tResult_Lo <<= 1;
	//			std::cout<< "D";xstdlib::pbin(o_tResult_Hi);
	//			std::cout<< "E";xstdlib::pbin(o_tResult_Lo);
			} while ((o_tResult_Hi & regarch.mask_hi_bit) == 0);

		}
	}
	else
	{
		o_bOverflow = false;
		o_tUnderflow = 0;
		o_tResult_Hi = o_tResult_Lo = 0;
	}
}


void xbinarymath::significand_multiply_n(const xbinarymath::regtype * i_tA, const xbinarymath::regtype * i_tB, size_t i_tNum_Bits, bool &o_bOverflow, size_t &o_tUnderflow, xbinarymath::regtype *& o_tResult_Hi, xbinarymath::regtype *& o_tResult_Lo)
{
	g_mapRegisters[i_tNum_Bits].set_register_size(i_tNum_Bits);
	const registers &cRegister (g_mapRegisters[i_tNum_Bits]);
	if (i_tNum_Bits <= cRegister.reg_size())
	{
		if (o_tResult_Hi == nullptr)
			o_tResult_Hi = new xbinarymath::regtype [1];
		if (o_tResult_Lo == nullptr)
			o_tResult_Lo = new xbinarymath::regtype [1];
		significand_multiply_arch(i_tA[0],i_tB[0],o_bOverflow,o_tUnderflow,o_tResult_Hi[0],o_tResult_Lo[0]);
		if (i_tNum_Bits < cRegister.reg_size())
		{
			xbinarymath::regtype tShift_Data = o_tResult_Hi[0] & cRegister.tDrop_Mask;
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
			o_tResult_Hi = new xbinarymath::regtype[cRegister.tNum_Words];
		if (o_tResult_Lo == nullptr)
			o_tResult_Lo = new xbinarymath::regtype[cRegister.tNum_Words];
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

				cRegister.rRegister_A_4b[tI_2] = (i_tA[tI] >> regarch.half_bits);
				cRegister.rRegister_A_4b[tI_2 + 1] = i_tA[tI] & regarch.mask_low_half;
				cRegister.rRegister_Size_4b[tI_2] = tI_64;
				cRegister.rRegister_Size_4b[tI_2 + 1] = tI_64 + 1;

				cRegister.rRegister_A_4b[tI_2_p_Dbl_Num_Words] = (i_tB[tI] >> regarch.half_bits);
				cRegister.rRegister_A_4b[tI_2_p_Dbl_Num_Words + 1] = i_tB[tI] & regarch.mask_low_half;
				cRegister.rRegister_Size_4b[tI_2_p_Dbl_Num_Words] = tI_64;
				cRegister.rRegister_Size_4b[tI_2_p_Dbl_Num_Words + 1] = tI_64 + 1;
			}
			std::map<size_t, std::vector<xbinarymath::regtype>> mAdder_Map;
			//multiply all of the half registers together
			for (size_t tI = 0; tI < cRegister.tDbl_Num_Words; tI++)
			{
				for (size_t tJ = 0; tJ < cRegister.tDbl_Num_Words; tJ++)
				{
					xbinarymath::regtype tR = cRegister.rRegister_A_4b[tI] * cRegister.rRegister_A_4b[tJ + cRegister.tDbl_Num_Words];
					if (tI == tJ && tI == 0)
						mAdder_Map[0].push_back(tR);
					else
					{
						size_t tHalf_Shifts = cRegister.rRegister_Size_4b[tI] + cRegister.rRegister_Size_4b[tJ + cRegister.tDbl_Num_Words];
						mAdder_Map[tHalf_Shifts + 1].push_back(tR & regarch.mask_low_half);
						mAdder_Map[tHalf_Shifts].push_back(tR >> regarch.half_bits);
					}
				}
			}
			cRegister.zero_register_B();

			for (auto iterI = mAdder_Map.rbegin(); iterI != mAdder_Map.rend(); iterI++)
			{
				if (iterI != mAdder_Map.rbegin() && iterI->first != 0)
				{
					// shift result
					cRegister.rRegister_B_4b[cRegister.tDbl_Num_Words - 1] >>= regarch.half_bits;
					for (size_t tI = 1; tI < cRegister.tDbl_Num_Words; tI++)
					{
						size_t tIdx = cRegister.tDbl_Num_Words - tI;
						xbinarymath::regtype tTemp = cRegister.rRegister_A_4b[tIdx - 1] & regarch.mask_low_half;
						tTemp <<= regarch.half_bits;
						cRegister.rRegister_A_4b[tIdx] |= tTemp;
						cRegister.rRegister_B_4b[tIdx - 1] >>= regarch.half_bits;
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
			o_bOverflow = (cRegister.rRegister_B_4b[0] & regarch.mask_hi_bit) != 0;
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
						if (tI < tMax && (cRegister.rRegister_B_4b[tI + 1] & regarch.mask_hi_bit) != 0)
						{
							cRegister.rRegister_B_4b[tI] |= 1;
						}
					}
				} while ((cRegister.rRegister_B_4b[0] & regarch.mask_hi_bit) == 0);
			}
			for(size_t tI = 0; tI < cRegister.tNum_Words; tI++)
			{
				o_tResult_Hi[tI] = cRegister.rRegister_B_4b[tI];
				if (i_tNum_Bits == cRegister.tReg_Bits)
					o_tResult_Lo[tI] = cRegister.rRegister_B_4b[tI + cRegister.tNum_Words];
			}
			if (i_tNum_Bits != cRegister.tReg_Bits)
			{
		
				xbinarymath::regtype tDrop_Data = 0;
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



void xbinarymath::significand_divider_arch(xbinarymath::regtype i_tA, xbinarymath::regtype i_tB, xbinarymath::regtype &o_tResult, xbinarymath::regtype & o_tRemainder, xbinarymath::exptype & o_tResult_Exponent, xbinarymath::exptype & o_tRemainder_Exponent)
{
	if (!g_bRegArchInit)
		regarch.initialize();
	if (i_tA == 0)
	{
		o_tResult = 0;
		o_tRemainder = 0;
		o_tResult_Exponent = 0;
		o_tRemainder_Exponent = 0;
	}
	else if (i_tB != 0)
	{
		bool bHigh_Bit_Flag = false;
		//xbinarymath::regtype tDenomenator = i_tB;

		o_tRemainder = i_tA;
		o_tResult_Exponent = regarch.num_bits + 1;//64;
		o_tResult = 0;
		while ((i_tB & regarch.mask_hi_bit) == 0)
		{
			i_tB <<= 1;
			o_tResult_Exponent++;
		}
		while ((o_tRemainder & regarch.mask_hi_bit) == 0)
		{
			o_tRemainder <<= 1;
			o_tResult_Exponent--;
		}
		// invert B
		i_tB = ~i_tB;
		i_tB++;

		while ((o_tResult & regarch.mask_hi_bit) == 0 && (o_tRemainder != 0 || bHigh_Bit_Flag))
		{
			o_tResult <<= 1;
			xbinarymath::regtype tTest = o_tRemainder + i_tB;
			if ((tTest & regarch.mask_hi_bit) == 0 || bHigh_Bit_Flag)
			{
				o_tRemainder = tTest;
				o_tResult |= 1;
			}
			o_tResult_Exponent--;
			bHigh_Bit_Flag = (o_tRemainder & regarch.mask_hi_bit) != 0;
			o_tRemainder <<= 1;
		}
		if (bHigh_Bit_Flag)
		{
			o_tRemainder >>= 1;
			o_tRemainder |= regarch.mask_hi_bit;
		}
		while ((o_tResult & regarch.mask_hi_bit) == 0)
		{
			o_tResult_Exponent--;
			o_tResult <<= 1;
		}
		o_tRemainder_Exponent = o_tResult_Exponent - 64;

	}
}

bool xbinarymath::greater_than_n(const xbinarymath::regtype *i_tA, const xbinarymath::regtype * i_tB, size_t i_tNum_Bits)
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

	bool bGreater_Than = (cRegister.rRegister_C_4b[0] & regarch.mask_hi_bit) == 0;
	bool bEqual = true;
	for (size_t tI = 0; tI < cRegister.tNum_Words && bEqual; tI++)
	{
		bEqual = (cRegister.rRegister_C_4b[tI] == 0);
	}
		
	return bGreater_Than && !bEqual;
}
bool xbinarymath::greater_than_or_equal_to_n(const xbinarymath::regtype *i_tA, const xbinarymath::regtype * i_tB, size_t i_tNum_Bits)
{
	g_mapRegisters[i_tNum_Bits].set_register_size(i_tNum_Bits);
	registers &cRegister (g_mapRegisters[i_tNum_Bits]);

	cRegister.load_register_B(i_tB,1);
	negate(cRegister.rRegister_B_4b,i_tNum_Bits);
	overflow_adder_n(i_tA,cRegister.rRegister_B_4b,i_tNum_Bits,cRegister.rRegister_C_4b);
	bool bGreater_Than = (cRegister.rRegister_C_4b[0] & regarch.mask_hi_bit) == 0;
	//xstdlib::pbin(cRegister.rRegister_C_4b[0]);
	// the equality test below is unnecssary -- if the high bit of C is not set, then either all words are 0 (a == b) or a > b
//	bool bEqual = true;
//	for (size_t tI = 0; tI < cRegister.tNum_Words && bEqual; tI++)
//	{
//		bEqual = (cRegister.rRegister_C_4b[tI] == 0);
//	}
		
	return bGreater_Than;// || bEqual;
}
bool xbinarymath::less_than_n(const xbinarymath::regtype *i_tA, const xbinarymath::regtype * i_tB, size_t i_tNum_Bits)
{
	return !greater_than_or_equal_to_n(i_tA,i_tB,i_tNum_Bits);
}
bool xbinarymath::less_than_or_equal_to_n(const xbinarymath::regtype *i_tA, const xbinarymath::regtype * i_tB, size_t i_tNum_Bits)
{
	return !greater_than_n(i_tA,i_tB,i_tNum_Bits);
}
bool xbinarymath::equal_to_n(const xbinarymath::regtype *i_tA, const xbinarymath::regtype * i_tB, size_t i_tNum_Bits)
{
	g_mapRegisters[i_tNum_Bits].set_register_size(i_tNum_Bits);
	registers &cRegister (g_mapRegisters[i_tNum_Bits]);

	cRegister.load_register_B(i_tB,1);
	//xstdlib::pbin(i_tB[0]);
	//if (i_tNum_Bits > 64)
	//	xstdlib::pbin(i_tB[1]);
	//xstdlib::pbin(cRegister.rRegister_B_4b[0]);
	//xstdlib::pbin(cRegister.rRegister_B_4b[1]);
	xbinarymath::negate(cRegister.rRegister_B_4b,i_tNum_Bits);
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
bool xbinarymath::not_equal_to_n(const xbinarymath::regtype *i_tA, const xbinarymath::regtype * i_tB, size_t i_tNum_Bits)
{
	return !equal_to_n(i_tA,i_tB,i_tNum_Bits);
}

void xbinarymath::shift_left_n(xbinarymath::regtype *i_tA,size_t i_tNum_Bits, size_t i_tShift)
{
	g_mapRegisters[i_tNum_Bits].set_register_size(i_tNum_Bits);
	registers &cRegister (g_mapRegisters[i_tNum_Bits]);
	size_t tWord_Size = sizeof(xbinarymath::regtype) * 8;
	if (i_tShift > i_tNum_Bits)
	{
		memset(i_tA,0,cRegister.tNum_Words * sizeof(xbinarymath::regtype));
	}
	else if (i_tNum_Bits <= tWord_Size)
	{
		i_tA[0] <<= i_tShift;
		i_tA[0] &= cRegister.tInv_Drop_Mask; // probably not necessary, but clear low bits
	}
	else
	{
		size_t tWord_Shift = i_tShift / tWord_Size;
		if (tWord_Shift > 0)
		{
			for (size_t tI = 0; tI < (cRegister.tNum_Words - tWord_Shift); tI++)
			{
				i_tA[tI] = i_tA[tI + tWord_Shift];
			}
			for (size_t tI = (cRegister.tNum_Words - tWord_Shift); tI < cRegister.tNum_Words; tI++)
				i_tA[tI] = 0;

			i_tShift %= tWord_Size;
		}
		if (i_tShift != 0)
		{
			size_t tNWm1 = cRegister.tNum_Words - 1;
			size_t tShift_Up = tWord_Size - i_tShift;
			for (size_t tI = 0; tI < cRegister.tNum_Words; tI++)
			{
				i_tA[tI] <<= i_tShift;
				if (tI != tNWm1)
				{
					xbinarymath::regtype tShift_Bits = (i_tA[tI + 1] >> tShift_Up);
					i_tA[tI] |= tShift_Bits;
				}
			}
		}
	}
}
void xbinarymath::shift_right_n(xbinarymath::regtype *i_tA,size_t i_tNum_Bits, size_t i_tShift)
{
	g_mapRegisters[i_tNum_Bits].set_register_size(i_tNum_Bits);
	registers &cRegister (g_mapRegisters[i_tNum_Bits]);
	if (i_tShift > i_tNum_Bits)
	{
		memset(i_tA,0,cRegister.tNum_Words * sizeof(xbinarymath::regtype));
	}
	else if (i_tNum_Bits <= cRegister.reg_size())
	{
		i_tA[0] >>= i_tShift;
		i_tA[0] &= cRegister.tInv_Drop_Mask; // probably not necessary, but clear low bits
	}
	else
	{
		size_t tWord_Shift = i_tShift / regarch.num_bits;
		if (tWord_Shift > 0)
		{
			for (size_t tI = tWord_Shift; tI < cRegister.tNum_Words; tI++)
			{
				i_tA[tI] = i_tA[tI - tWord_Shift];
			}
			for (size_t tI = 0; tI < tWord_Shift; tI++)
				i_tA[tI] = 0;

			i_tShift %= regarch.num_bits;
		}
		if (i_tShift != 0)
		{
			size_t tNWm1 = cRegister.tNum_Words - 1;
			size_t tShift_Up = regarch.num_bits - i_tShift;
			for (size_t tI = 0; tI < cRegister.tNum_Words; tI++)
			{
				size_t tIdx = tNWm1 - tI;
				i_tA[tIdx] >>= i_tShift;
				if (tI != tNWm1)
				{
					xbinarymath::regtype tShift_Bits = (i_tA[tIdx - 1] << tShift_Up);
					i_tA[tIdx] |= tShift_Bits;
				}
			}
			i_tA[cRegister.tNum_Words - 1] &= cRegister.tInv_Drop_Mask; // clear low bits
		}
	}
}

void xbinarymath::significand_divider_n(const xbinarymath::regtype *i_tA, const xbinarymath::regtype * i_tB, size_t i_tNum_Bits, xbinarymath::regtype *&o_tResult, xbinarymath::regtype *& o_tRemainder, xbinarymath::exptype & o_tResult_Exponent, xbinarymath::exptype & o_tRemainder_Exponent)
{
	g_mapRegisters[i_tNum_Bits].set_register_size(i_tNum_Bits);
	registers &cRegister (g_mapRegisters[i_tNum_Bits]);
	if (o_tResult == nullptr)
		o_tResult = new xbinarymath::regtype [cRegister.tNum_Words];
	if (o_tRemainder == nullptr)
		o_tRemainder = new xbinarymath::regtype [cRegister.tNum_Words];
	if (i_tNum_Bits <= cRegister.reg_size())
	{

		cRegister.load_register_B(i_tA,1);
		cRegister.load_register_C(i_tB,1);

		significand_divider_arch(cRegister.rRegister_B_4b[0] & cRegister.tInv_Drop_Mask,cRegister.rRegister_C_4b[0] & cRegister.tInv_Drop_Mask,o_tResult[0],o_tRemainder[0],o_tResult_Exponent,o_tRemainder_Exponent);
	}
	else
	{
		bool bA_Zero = true;
		for (size_t tI = 0; tI < cRegister.tNum_Words && bA_Zero; tI++)
			bA_Zero &= (i_tA[tI] == 0);
		bool bB_Zero = true;
		for (size_t tI = 0; tI < cRegister.tNum_Words && bB_Zero; tI++)
			bB_Zero &= (i_tB[tI] == 0);
		o_tRemainder_Exponent = 0;
		memset(o_tResult,0,cRegister.tNum_Words * sizeof(xbinarymath::regtype));
		memset(o_tRemainder,0,cRegister.tNum_Words * sizeof(xbinarymath::regtype));
		if (!(bA_Zero || bB_Zero))
		{
			bool bHigh_Bit_Flag = false;
//			xbinarymath::regtype tLow_Bit = 1;
//			tLow_Bit <<= (i_tNum_Bits % (sizeof(xbinarymath::regtype) * 8));
			//xbinarymath::regtype tDenomenator = i_tB;

			cRegister.load_register_C(i_tB,1);
			cRegister.rRegister_C_4b[cRegister.tNum_Words - 1] &= cRegister.tInv_Drop_Mask;

			memcpy(o_tRemainder,i_tA,cRegister.tNum_Words * sizeof(xbinarymath::regtype));
			o_tRemainder[cRegister.tNum_Words - 1] &= cRegister.tInv_Drop_Mask;

			o_tResult_Exponent = i_tNum_Bits + 1;

			bool bRemainder_Zero = false;
			while ((o_tRemainder[0] & regarch.mask_hi_bit) == 0) // remainder >= B && remainder != 0
			{
				shift_left_n(o_tRemainder,i_tNum_Bits,1);
				o_tResult_Exponent--;
			}
//			std::cout << "here1" << std::endl;
			cRegister.load_register_B(i_tB,1);
			while ((cRegister.rRegister_B_4b[0] & regarch.mask_hi_bit) == 0) // remainder >= B && remainder != 0
			{
				shift_left_n(cRegister.rRegister_B_4b,i_tNum_Bits,1);
				o_tResult_Exponent++;
			}
			//std::cout << o_tResult_Exponent << std::endl;
			//std::cout << "here2" << std::endl;
			//xstdlib::pbin(cRegister.rRegister_B_4b[0]);
			//xstdlib::pbin(cRegister.rRegister_B_4b[1]);
			negate(cRegister.rRegister_B_4b,i_tNum_Bits);
			//xstdlib::pbin(cRegister.rRegister_B_4b[0]);
			//xstdlib::pbin(cRegister.rRegister_B_4b[1]);
//			xstdlib::pbin(o_tRemainder[0]);
//			xstdlib::pbin(o_tRemainder[1]);
			while ((o_tResult[0] & regarch.mask_hi_bit) == 0 && (!bRemainder_Zero || bHigh_Bit_Flag))
			{
				//xstdlib::pbin(o_tRemainder[0]);
//				xstdlib::pbin(o_tRemainder[1]);
				shift_left_n(o_tResult,i_tNum_Bits,1);
				overflow_adder_n(o_tRemainder,cRegister.rRegister_B_4b,i_tNum_Bits,cRegister.rRegister_A_4b);
				if (bHigh_Bit_Flag || (cRegister.rRegister_A_4b[0] & regarch.mask_hi_bit) == 0) // remainder >= B 
				{
					memcpy(o_tRemainder,cRegister.rRegister_A_4b,cRegister.tNum_Words * sizeof(xbinarymath::regtype));
					o_tResult[cRegister.tNum_Words - 1] |= 1;
					//xstdlib::pbin(o_tRemainder[0]);
				}
				o_tResult_Exponent--;
				bHigh_Bit_Flag = (o_tRemainder[0] & regarch.mask_hi_bit) != 0;
				shift_left_n(o_tRemainder,i_tNum_Bits,1);
				bRemainder_Zero = true;
				for (size_t tI = 0; tI < cRegister.tNum_Words && bRemainder_Zero; tI++)
					bRemainder_Zero &= (o_tRemainder[tI] == 0);
			}
			if (bHigh_Bit_Flag)
			{
				shift_right_n(o_tRemainder,i_tNum_Bits,1);
				o_tRemainder[0] |= regarch.mask_hi_bit;
			}

			while ((o_tResult[0] & regarch.mask_hi_bit) == 0)
			{
				o_tResult_Exponent--;
				shift_left_n(o_tResult,i_tNum_Bits,1);
			}
			o_tRemainder_Exponent = o_tResult_Exponent - i_tNum_Bits;
		}
	}
}
