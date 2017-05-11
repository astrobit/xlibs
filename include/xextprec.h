#pragma once
#include <string>
#include <cmath>
#include <cfloat>

////////////////////////////////////////////////////////////////////////////////
//
// logfloat extended precision floating point class
//
// The logfloat class has the basic functionality of floating point operations,
// but internally stores the data as the natural log of the value.
// This allows a much wider range of data, but increases the complexity
// of all addition and subtraction operations significantly
// Thus, expect this class to be slow, but capable of handling numbers much, much
// larger than the normal range of long double. Spcifically, it can handle
// numbers in the range -<long double maximum> < ln (abs(value)) < <long double maximum>
//
// inf and NaN are detected and signalled. There is special handling of zeros.
// sign of all numbers is preserved, including 0.
// there are four ways to retived the stored value: 
//
//		Get_LD: retrieve value as a long double. If the value is greater than
//				the maximum representable long double, the maximum value is returned
//				There is an optional argument to a boolean pointer that signals true
//				if there is overflow.
//		Get_Value: retrieve the internally stored natural log of the value. an optional argument
//				signals true if the value (not the log of the value) is negative. If the value is NaN or inf,
//				the return value is appropriately signalled.
//		Get_Value_b10: retrieve the internally log base 10 of the value. an optional argument
//				signals true if the value (not the log of the value) is negative. If the value is NaN or inf,
//				the return value is appropriately signalled.
//		Get_Value_String: retrieve a string with the base 10 representation of the number. For the time being,
//				it accepts only a simple format specifier allowing either floating point (equivalent to "%f") or 
//				exponential notation (equivalent to "%e%). inf and NaN will be reported as usual,
//				0 is reported as simply "0" or "-0".
//
// License: LGPL 3.0. Full license included below
// Copyright: (c) 2017 Brian W. Mulligan
// This work is entirely self contained, and was not written using any existing code libraries or derived from the work of any person, living or dead.
// History:
//      5 May 2017 : Created by Brian W. Mulligan
//
class logfloat
{
private:
	bool m_bSign;
	bool m_bZero;
	long double m_dValue;
	bool m_bInf;
	bool m_bNaN;
	inline void zero(void){m_dValue = -LDBL_MAX; m_bSign = false; m_bZero = true;m_bNaN = false; m_bInf = false;}
public:
	inline logfloat(void){zero();}
	inline logfloat(const long double & i_dValue)
	{
		m_bNaN = std::isnan(i_dValue);
		m_bInf = std::isinf(i_dValue);
		m_bSign = (i_dValue < 0);
		if (!m_bNaN && !m_bInf)
		{
			m_bZero = (i_dValue == 0.0);
			if (!m_bZero)
				m_dValue = log(fabs(i_dValue));
			else
				m_dValue = -LDBL_MAX;
		}
	}


	inline logfloat operator *=(const logfloat &i_cRHO)
	{
		if (i_cRHO.m_bNaN)
			m_bNaN = true;
		else if (i_cRHO.m_bInf)
		{
			m_bInf = true;
			m_bSign ^= i_cRHO.m_bSign;
		}
		else if (!m_bNaN && !m_bInf)
		{
			if (!i_cRHO.m_bZero && !m_bZero && !m_bNaN && !m_bInf)
			{
				m_dValue += i_cRHO.m_dValue; 
				m_bSign ^= i_cRHO.m_bSign;
			}
			else
				zero();
		}
		return *this;
	}
	inline logfloat operator /=(const logfloat &i_cRHO)
	{
		if (i_cRHO.m_bNaN)
			m_bNaN = true;
		else if (i_cRHO.m_bInf)
		{
			zero();
		}
		else if (i_cRHO.m_bZero)
		{
			m_bInf = true;
			m_bSign ^= i_cRHO.m_bSign;
		}
		else if (!m_bZero && !m_bNaN && !m_bInf)
		{
			m_dValue -= i_cRHO.m_dValue; 
			m_bSign ^= i_cRHO.m_bSign;
		}
		return *this;
	}
	inline logfloat& operator +=(const logfloat &i_cRHO)
	{
		if (!m_bNaN && !m_bInf)
		{
			if (i_cRHO.m_bNaN || i_cRHO.m_bInf)
				*this = i_cRHO;
			else if (!m_bZero || !i_cRHO.m_bZero)
			{
				if (m_bZero)
					*this = i_cRHO;
				else if (!i_cRHO.m_bZero)
				{
					if (m_dValue > i_cRHO.m_dValue)
					{
						if ((m_dValue - i_cRHO.m_dValue) <= LDBL_MAX_EXP)
						{
							long double dValA = exp(m_dValue - i_cRHO.m_dValue);
							if (m_bSign)
								dValA *= (long double)-1.0;
							if (i_cRHO.m_bSign)
								dValA -= (long double)1.0;
							else
								dValA += (long double)1.0;
							logfloat cA(dValA);
							if (!cA.m_bZero)
								cA.m_dValue += i_cRHO.m_dValue;
							*this = cA;
						}
					}
					else// if (m_dValue <= i_cRHO.m_dValue)
					{
						if ((i_cRHO.m_dValue - m_dValue) > LDBL_MAX_EXP)
							*this = i_cRHO;
						else
						{
							long double dValA = exp(i_cRHO.m_dValue - m_dValue);
							if (i_cRHO.m_bSign)
								dValA *= (long double)-1.0;
							if (m_bSign)
								dValA -= (long double)1.0;
							else
								dValA += (long double)1.0;
							logfloat cA(dValA);
							if (!cA.m_bZero)
								cA.m_dValue += m_dValue;
							*this = cA;
						}
					}
				}
			}
		}
		return *this;
	}
	inline logfloat operator -=(const logfloat &i_cRHO)
	{
		logfloat cNeg = i_cRHO;
		cNeg.m_bSign = !cNeg.m_bSign;
		return *this += cNeg;
	}

	inline logfloat operator +(const logfloat &i_cRHO) const
	{
		logfloat cRet(*this);
		cRet += i_cRHO;
		return cRet;
	}

	inline logfloat operator -(const logfloat &i_cRHO) const
	{
		logfloat cNeg = i_cRHO;
		cNeg.m_bSign = !cNeg.m_bSign;
		logfloat cRet(*this);
		cRet += i_cRHO;
		return cRet;
	}
	inline logfloat operator *(const logfloat &i_cRHO)
	{
		logfloat cRet(*this);
		cRet *= i_cRHO;
		return cRet;
	}
	inline logfloat operator /(const logfloat &i_cRHO)
	{
		logfloat cRet(*this);
		cRet /= i_cRHO;
		return cRet;
	}
	inline logfloat operator-(void)
	{
		logfloat cRet(*this);
		if (!m_bNaN)
			cRet.m_bSign = !m_bSign;
		return cRet;
	}
	
	inline bool isnan(void){return m_bNaN;}
	inline bool isinf(void){return m_bInf;}

	inline bool operator ==(const logfloat &  i_cRHO)
	{
		return (m_bNaN || i_cRHO.m_bNaN ||
			(m_bInf && i_cRHO.m_bInf && m_bSign == i_cRHO.m_bSign) ||
			(!m_bNaN && !m_bInf && !i_cRHO.m_bNaN && !i_cRHO.m_bInf && 
				((m_bZero && i_cRHO.m_bZero) ||
				 (!m_bZero && !i_cRHO.m_bZero && m_bSign == i_cRHO.m_bSign && m_dValue == i_cRHO.m_dValue))));
	}
	inline bool operator >(const logfloat &  i_cRHO)
	{
		return (m_bNaN || i_cRHO.m_bNaN ||
			(m_bInf && i_cRHO.m_bInf && !m_bSign && i_cRHO.m_bSign) ||
			(!m_bNaN && !m_bInf && !i_cRHO.m_bNaN && !i_cRHO.m_bInf && 
				((!m_bSign && i_cRHO.m_bZero) ||
				(m_bZero && i_cRHO.m_bSign && !i_cRHO.m_bZero) ||
				(!m_bZero && !i_cRHO.m_bZero && !m_bSign && i_cRHO.m_bSign) ||
				(!m_bZero && !i_cRHO.m_bZero && !m_bSign && !i_cRHO.m_bSign && m_dValue > i_cRHO.m_dValue) ||
				(!m_bZero && !i_cRHO.m_bZero && m_bSign && i_cRHO.m_bSign && m_dValue < i_cRHO.m_dValue))));
	}
	inline bool operator <(const logfloat &  i_cRHO)
	{
		return (m_bNaN || i_cRHO.m_bNaN ||
			(m_bInf && i_cRHO.m_bInf && m_bSign && !i_cRHO.m_bSign) ||
			(!m_bNaN && !m_bInf && !i_cRHO.m_bNaN && !i_cRHO.m_bInf && 
				((m_bSign && i_cRHO.m_bZero) ||
				(m_bZero && !i_cRHO.m_bSign && !i_cRHO.m_bZero) ||
				(!m_bZero && !i_cRHO.m_bZero && m_bSign && !i_cRHO.m_bSign) ||
				(!m_bZero && !i_cRHO.m_bZero && !m_bSign && !i_cRHO.m_bSign && m_dValue < i_cRHO.m_dValue) ||
				(!m_bZero && !i_cRHO.m_bZero && m_bSign && i_cRHO.m_bSign && m_dValue > i_cRHO.m_dValue))));
	}
	inline bool operator !=(const logfloat &  i_cRHO)
	{
		return (m_bNaN || i_cRHO.m_bNaN || !(*this == i_cRHO));
	}
	inline bool operator >=(const logfloat &  i_cRHO)
	{
		return (m_bNaN || i_cRHO.m_bNaN || !(*this < i_cRHO));
	}
	inline bool operator <=(const logfloat &  i_cRHO)
	{
		return (m_bNaN || i_cRHO.m_bNaN || !(*this > i_cRHO));
	}
	// get the value and a normal long double with an optional flag to indicate values above or below representable range of a long double. NaN or Inf will be signalled if appropriate. Zero not indicated.
	long double Get_LD(bool * o_lpbOverflow = nullptr)
	{
		long double ldRet = 0.0;
		if (o_lpbOverflow != nullptr)
			o_lpbOverflow[0] = false;
		if (m_bNaN)
			ldRet = nanl("");
		else if (m_bInf && m_bSign)
			ldRet = -INFINITY;
		else if (m_bInf && !m_bSign)
			ldRet = INFINITY;
		else if (!m_bZero)
		{
			if (m_dValue < LDBL_MAX_EXP)
				ldRet = exp(m_dValue);
			else
			{
				if (o_lpbOverflow != nullptr)
					o_lpbOverflow[0] = true;
				ldRet = LDBL_MAX;
			}
			if (m_bSign)
				ldRet *= -1.0;
		}
		return ldRet;
	}
	// get the exponent value with an optional sign flag. NaN or Inf will be signalled if appropriate. Zero not indicated.
	long double Get_Value(bool * o_lpbSign = nullptr)
	{
		long double ldRet = 0.0;
		if (o_lpbSign)
			o_lpbSign[0] = false;
		if (m_bNaN)
			ldRet = nanl("");
		else if (m_bInf && m_bSign)
		{
			ldRet = INFINITY;
			if (o_lpbSign)
				o_lpbSign[0] = true;
		}
		else if (m_bInf && !m_bSign)
			ldRet = INFINITY;
		else if (!m_bZero)
		{
			ldRet = m_dValue;
			if (o_lpbSign)
				o_lpbSign[0] = true;
		}
		return ldRet;
	}
	long double Get_Value_b10(bool * o_lpbSign = nullptr)
	{
		static long double dLogE = log10(exp(1.0));
		long double ldRet = 0.0;
		if (o_lpbSign)
			o_lpbSign[0] = false;
		if (m_bNaN)
			ldRet = nanl("");
		else if (m_bInf && m_bSign)
		{
			ldRet = INFINITY;
			if (o_lpbSign)
				o_lpbSign[0] = true;
		}
		else if (m_bInf && !m_bSign)
			ldRet = INFINITY;
		else if (!m_bZero)
		{
			ldRet = m_dValue * dLogE;
			if (o_lpbSign)
				o_lpbSign[0] = true;
		}
		return ldRet;
	}
	enum format{form_float,form_exponent};
	std::string Get_Value_String(format i_eFormat)
	{
		static long double dLogE = log10(exp(1.0));
		std::string szRet;
		if (m_bNaN)
			szRet = "nan";
		else if (m_bInf && m_bSign)
			szRet = "-inf";
		else if (m_bInf && !m_bSign)
			szRet = "inf";
		if (m_bZero && !m_bSign)
			szRet = "0";
		else if (m_bZero && m_bSign)
			szRet = "-0";
		else
		{
			long double ldY_f = m_dValue * dLogE;
			long double dLogA = fmod(ldY_f,1.0);
			if (fabs(dLogA - 1.00) < 0.00001)
				dLogA -= 1.0;
			else if (dLogA < 0.0)
				dLogA += 1.0;

			long double dY = ldY_f - dLogA;
			long double dA = pow(10.0,dLogA);
			if (m_bSign)
				dA *= -1.0;
			
			std::string szMan = std::to_string(dA);
			if (i_eFormat == form_exponent)
			{
				std::string szMan = std::to_string(dA);
				std::string szExp = std::to_string((int)fabs(dY));
				if (dY < 0.0)
					szRet = szMan + "e-" + szExp;
				else
					szRet = szMan + "e+" + szExp;
			}
			else
			{
				std::string szMan = std::to_string(fabs(dA));
				if (dA < 0.0)
					szRet.push_back('-');
				int iDecimal_Shift = (int)dY;
				if (iDecimal_Shift < 0)
				{
					szRet.push_back('0');
					szRet.push_back('.');
					while (iDecimal_Shift < -1)
					{
						szRet.push_back('0');
						iDecimal_Shift++;
					}
					for (auto itI = szMan.begin(); itI != szMan.end(); itI++)
					{
						if (*itI != '.')
							szRet.push_back(*itI);
					}
					while (szRet.back() == '0')
						szRet.pop_back();
				}
				else
				{
					szRet.push_back(szMan[0]);
					for (size_t tI = 2; tI < szMan.size(); tI++)
					{
						if (iDecimal_Shift == 0)
							szRet.push_back('.');
						iDecimal_Shift--;
						szRet.push_back(szMan[tI]);
					}
					while (szRet.back() == '0')
						szRet.pop_back();
					while (szRet.back() == '.')
						szRet.pop_back();
				}
			}
		}
		return szRet;
	}	

};



////////////////////////////////////////////////////////////////////////////////
//
// expdouble extended precision floating point class
//
// The expdouble class is a software implementation of 128 bit floating point
// with 64 mantissa bits, 1 sign bit, and 63 exponent bits. It is intended to be
// (but is not yet) compliant with the IEEE 754-2008 floating point standard.
// At present, the +, -, and * operators are defined. The division operator is
// not yet defined (coming soon!). Some standard functions are defined, most are
// not. NaN and infinity are included, but aren't fully working in all 
// circumstances.
//
// In order to extract the data in a printable format, there is a function
//
//	std::string get_b10_value(size_t i_nNum_Digits, bool i_bEngineering_Form)
//
// that returns a string with the value in base 10. The Num Digits input specifies
// how many digits after the decimal point to include. i_bEngineering_Form specifies
// that the value should be returned in the form m.mmmmmme+xxxxxxx
// where m.mmmmm is the mantissa and xxxxx is the exponent.
//
// There is also a << operator defined that works with the std::ostream. This
// operator works using the get_b10_value function
// Note: the get_b10_value will try to expand really big numbers. If you
// try to print EDBL_MAX, expect it to take a REALLY long time.

//
// License: LGPL 3.0. Full license included below
// Copyright: (c) 2017 Brian W. Mulligan
// This work is entirely self contained, and was not written using any existing code libraries or derived from the work of any person, living or dead.
// History:
//      11 May 2017 : Created by Brian W. Mulligan
//

class expdouble
{
private:
	uint64_t m_nMantissa;
	int64_t m_nExponent;
	
	char extract_digit(long double & io_lpdDigit)
	{
		char chRet = '0';
		int iValue = fabs(io_lpdDigit);
		if (iValue < 10)
		{
			chRet += iValue;
			io_lpdDigit -= iValue;
		}
		return chRet;
	}
public:
	void load(const long double & i_ldData)
	{
		// warning: this method does not use the ldexp and frexp functions that are part of the std c++ math library
		// it is therefore architecture dependant. It probably doesn't work on anything other than x64 arch.
		uint64_t * lptData = (uint64_t *) &i_ldData;
		//short sExp = (lptData[1] & 0x7fff);
		m_nMantissa = lptData[0];
		if (lptData[1] & 0x4000)
			m_nExponent = (lptData[1] & 0x3fff) | 0x4000000000000000;
		else if ((lptData[1] & 0x7fff) != 0)
			m_nExponent = (lptData[1] & 0x7fff) | 0x3fffffffffffc000;
		else
			m_nExponent = 0;
		uint64_t nSign = (lptData[1] & 0x8000) << 48;
		m_nExponent |= nSign;
	}
	long double unload(void) const
	{
		long double ldRet;
		if (exponent() >= LDBL_MIN_EXP  && exponent() <= LDBL_MAX_EXP)
		{
			uint64_t nSign_Bit = m_nExponent & (0x8000000000000000);
			uint64_t nSign = (nSign_Bit >> 48);
			uint64_t * lptData = (uint64_t *) &ldRet;
			lptData[0] = m_nMantissa;
			int64_t tExp_Mod = m_nExponent & 0x7fffffffffffffff;
			//tExp_Mod += 16382;
			if (tExp_Mod & 0x4000000000000000)
				lptData[1] = (m_nExponent & 0x3fff) | 0x4000 | nSign;
			else
				lptData[1] = (m_nExponent & 0x3fff) | nSign;
		}
		else if (m_nExponent < LDBL_MIN_EXP)
			ldRet = 0.0; // underflow
		else // exponent > 16384 -- overflow
		{
			uint64_t nSign_Bit = m_nExponent & (0x8000000000000000);
			ldRet = std::numeric_limits<long double>::infinity();
			if (nSign_Bit == 0x8000000000000000)
				ldRet *= -1.0;
		}

		return ldRet;
	}
	expdouble(void){};
	expdouble(const long double & i_ldLD){load(i_ldLD);};
	expdouble(const uint64_t i_nMantissa,const uint64_t i_nExponent, bool i_bSign_Negative)
	{
		m_nMantissa = i_nMantissa;
		m_nExponent = i_nExponent & 0x7fffffffffffffff; // clear sign bit
		if (i_bSign_Negative)
			m_nExponent |= 0x8000000000000000; // set sign bit
	}
	inline void nan(void) // quiet NaN
	{
		m_nExponent = (m_nExponent & 0x8000000000000000) | 0x7fffffffffffffff; // preserve sign bit, signal infinity or nan
		m_nMantissa = 0xc000000000000000; // quiet NaN
	}
	inline void infinity(void)
	{
		m_nExponent = (m_nExponent & 0x8000000000000000) | 0x7fffffffffffffff; // preserve sign bit, signal infinity or nan
		m_nMantissa = 0x8000000000000000; // infinity
	}
	inline bool iszero(void) const
	{
		return (m_nExponent == 0 || m_nExponent == 0x8000000000000000) && m_nMantissa == 0;
	}
	inline bool isnan(void) const
	{
		return ((m_nExponent & 0x7fffffffffffffff) == 0x7fffffffffffffff) && (m_nMantissa == 0xc000000000000000);
	}
	inline bool isinf(void) const
	{
		return ((m_nExponent & 0x7fffffffffffffff) == 0x7fffffffffffffff) && (m_nMantissa == 0x8000000000000000);
	}
	inline bool isfinite(void) const
	{
		return ((m_nExponent & 0x7fffffffffffffff) ^ 0x7fffffffffffffff) != 0;
	}
	inline bool isnormal(void) const
	{
		//pbin(m_nMantissa);
		return (iszero() || (isfinite() && ((m_nMantissa & 0x8000000000000000) == 0x8000000000000000)));
	}
	inline bool signbit(void) const
	{
		return ((m_nExponent & 0x8000000000000000) == 0x8000000000000000);
	}
	inline expdouble abs(void) const
	{
		expdouble xdRet(*this);
		xdRet.m_nExponent ^= (xdRet.m_nExponent & 0x8000000000000000); // clear sign bit
		return xdRet;
	}
	inline expdouble copySign(const expdouble &i_cRHO ) const
	{
		expdouble xdRet(*this);
		xdRet.m_nExponent ^= (xdRet.m_nExponent & 0x8000000000000000); // clear sign bit
		xdRet.m_nExponent |= (i_cRHO.m_nExponent & 0x8000000000000000); // set sign bit from RHO
		return xdRet;
	}
	inline bool is754version2008(void) const
	{
		return false; // hopefully this will become true, but for the moment it is a non-conforming implementation 
	}
	inline bool is754version1985(void) const
	{
		return false; // hopefully this will become true, but for the moment it is a non-conforming implementation
	}

	inline int fpclassify(void) const
	{
		int iRet;
		if ((m_nExponent & 0x7fffffffffffffff) == 0x7fffffffffffffff)
		{
			if (m_nMantissa == 0x8000000000000000)
				iRet = FP_INFINITE;
			else
				iRet = FP_NAN;
		}
		else if (m_nExponent == 0 && m_nMantissa == 0)
			iRet = FP_ZERO;
		else if (m_nMantissa == 0x8000000000000000)
			iRet = FP_NORMAL;
		else
			iRet = FP_SUBNORMAL;
		return iRet;
	}
	inline	int64_t exponent(void) const
	{
		return (m_nExponent & 0x7fffffffffffffff) - 4611686018427387902; // no sign bit
	}
	inline	uint64_t mantissa(void) const
	{
		return m_nMantissa;
	}
	inline expdouble frexp(int64_t * o_lptExp) const
	{
		expdouble xdRet(*this);
		if (o_lptExp != nullptr)
			o_lptExp[0] = exponent();
		uint64_t tExp_Temp = xdRet.m_nExponent & 0x7fffffffffffffff;
		tExp_Temp -= exponent();
		xdRet.m_nExponent = (tExp_Temp & 0x7fffffffffffffff) | (xdRet.m_nExponent & 0x8000000000000000);
		return xdRet;
	}
	inline expdouble frexp(int64_t &o_tExp) const
	{
		return (frexp(&o_tExp));
	}
	inline expdouble operator -(void) const
	{
		expdouble xdRet(*this);
		xdRet.m_nExponent ^= 0x8000000000000000; // flip sign bit
		return xdRet;
	}
	inline expdouble & operator =(const long double & i_cRHO)
	{
		load(i_cRHO);
		return *this;
	}
	
	inline expdouble operator *(const expdouble & i_cRHO) const
	{
		expdouble xdRet(*this);
		return (xdRet *= i_cRHO);
	}
	expdouble & operator *=(const expdouble & i_cRHO)
	{
		if (isnan() || i_cRHO.isnan())
			nan();
		else if (isinf() || i_cRHO.isinf())
		{
			if (isinf() && i_cRHO.m_nMantissa == 0 && i_cRHO.m_nExponent == 0) // infinity * 0 = NaN
				nan();
			else if (i_cRHO.isinf() && m_nMantissa == 0 && m_nExponent == 0) // infinity * 0 = NaN
				nan();
			else
			{
				uint64_t tSign = (m_nExponent ^ i_cRHO.m_nExponent) & 0x8000000000000000;
				infinity();
				m_nExponent |= tSign; // allow -0
			}
		}
		else if (isnormal() && i_cRHO.isnormal())
		{
			if (iszero() || i_cRHO.iszero())
			{
				m_nMantissa = 0;
				uint64_t tSign = (m_nExponent ^ i_cRHO.m_nExponent) & 0x8000000000000000;
				m_nExponent = tSign; // allow -0
			}
			else
			{
				int64_t tExp = exponent() + i_cRHO.exponent();
				uint64_t tSign = (m_nExponent ^ i_cRHO.m_nExponent) & 0x8000000000000000;
				//pbin(tSign);
				//pbin(m_nExponent & 0x8000000000000000);
				//pbin(i_cRHO.m_nExponent & 0x8000000000000000);
				uint64_t tRHO_Low = i_cRHO.m_nMantissa & 0x0ffffffff;
				uint64_t tRHO_High = (i_cRHO.m_nMantissa & 0xffffffff00000000);
			
				uint64_t tLow = m_nMantissa & 0x0ffffffff;
				uint64_t tHigh = (m_nMantissa & 0xffffffff00000000);

				tRHO_High >>= 32;
				tHigh >>= 32;
				// multiplaction method:
				// LHO = (tHigh + tLow * 2^-32)
				// RHO = (tRHO_High + tRHO_Low * 2^-32)
				// where high and low represent 32 highest and lowest order bits, respectively
				// then 
				// ans = LHO * RHO = tHigh * tRHO_High + tHigh * tRHO_Low * 2^-32 + tLow * tRHO_High * 2^-32 + tLow * tRHO_Low * 2^-64
				// we add lowest order bits first, then shift right by 32 bits, add the next set, shift, add
				// to ensure there is no overflow, we add the highest order bits in their own register, then
				// shift right until that register is empty
				uint64_t tRes[4] = {tHigh * tRHO_High,tHigh * tRHO_Low,tLow * tRHO_High,tLow * tRHO_Low};
				uint64_t tLL = tRes[3] & 0x0ffffffff;
				uint64_t tHL = (tRes[3] >> 32); // high order bits of reg 3
				tHL += (tRes[2] & 0x0ffffffff); // add low order bits of reg 2
				tHL += (tRes[1] & 0x0ffffffff); // add low order bits of reg 1
				uint64_t tLH = (tRes[2] >> 32); // high order bits of reg 2
				tLH += (tRes[1] >> 32); // add high order bits of reg 1
				tLH += (tRes[0] & 0x0ffffffff); // add low order bits of reg 0
				tLH += (tHL >> 32); // add high order bits of lowest order registers
				tHL &= 0xffffffff; // clear high order bits of lowest order registers
				tHL <<= 32; // clear high order bits of lowest order registers and shift right
				tHL |= tLL; // add low order bits of lower order register. tHL now contains all low order data
				uint64_t tHH = (tRes[0] >> 32); // high order bits of reg 0
				tHH += (tLH >> 32); // add high order bits of lower order register
				tLH ^= (tLH & 0xffffffff00000000); // clear high order bits of lower order register
				tLH |= (tHH << 32); // place low order bits from highest order register into the high-low register
				tHH >>= 32; // low order bits no longer needed

				while (tHH != 0)
				{
					tLH >>= 1; // right shift to make space
					if (tHH & 1) // if low order bit of HH is set
						tLH |= 0x8000000000000000; // set high order bit of LH
					tExp++; // increase exponent due to shift
					tHH >>= 1; // right shift HH
				}
				while ((tLH & 0x8000000000000000) == 0)
				{
					tLH <<= 1; // left shift to make space
					if (tHL & 0x8000000000000000) // if high order bit of HL is set
						tLH |= 1; // set low order bit of LH
					tExp--; // decrease exponent due to shift
					tHL <<= 1; // shfit HL
				}

				m_nMantissa = tLH;
				m_nExponent = (tExp + 4611686018427387902) & 0x7fffffffffffffff | tSign;
			}
		}
		return *this;
	}
	inline expdouble & operator -=(const expdouble & i_cRHO)
	{
		return (*this += (-i_cRHO));
	}

	expdouble & operator +=(const expdouble & i_cRHO)
	{
		uint64_t tSignL = m_nExponent & 0x8000000000000000;
		uint64_t tSignR = i_cRHO.m_nExponent & 0x8000000000000000;
		uint64_t tExpL = m_nExponent & 0x7fffffffffffffff;
		uint64_t tExpR = i_cRHO.m_nExponent & 0x7fffffffffffffff;
		if (tExpL & 0x4000000000000000)
			tExpL |= 0x8000000000000000;
		if (tExpR & 0x4000000000000000)
			tExpR |= 0x8000000000000000;

		int64_t iExp_Diff = tExpL - tExpR;
		if (iExp_Diff < -63) // too big a difference - just copy the RHO
		{
			return (*this = i_cRHO);
		}
		else if (iExp_Diff < 63) // valid range
		{
			if (iExp_Diff >= 0)
			{
				uint64_t tManRHO = i_cRHO.m_nMantissa >> iExp_Diff;
				if (tSignL == tSignR)
					m_nMantissa += tManRHO;
				else
					m_nMantissa -= tManRHO;
			}
			else
			{
				uint64_t tMan = m_nMantissa >> (-iExp_Diff);
				if (tSignL == tSignR)
					m_nMantissa = i_cRHO.m_nMantissa + tMan;
				else
					m_nMantissa = i_cRHO.m_nMantissa - tMan;
				m_nExponent = tExpR;
			}

				
		}
		return *this;
		
		
	}

	inline expdouble operator +(const expdouble & i_cRHO) const
	{
		expdouble cRet(*this);
		return (cRet += i_cRHO);
	}
	inline expdouble operator -(const expdouble & i_cRHO) const
	{
		expdouble cRet(*this);
		return (cRet -= i_cRHO);
	}

	std::string get_b10_value(size_t i_nNum_Digits, bool i_bEngineering_Form) const
	{
		std::string szRet;
		if (isinf())
		{
			if (signbit())
				szRet.push_back('-');
			szRet += "inf";
		}
		else if (isnan())
			szRet = "nan";
		else if (iszero())
		{
			if (signbit())
				szRet.push_back('-');
			szRet.push_back('0');
			if (i_nNum_Digits > 0)
				szRet.push_back('.');
			for (size_t tI = 0; tI < i_nNum_Digits; tI++)
				szRet.push_back('0');
		}
		else
		{
			long double ldMantissa = frexp(nullptr).unload();
			if (ldMantissa < 0.0)
				ldMantissa *= -1.0;
			long double ldExponent = exponent() * log10(2.0) + log10(ldMantissa);
			long double ldValue = ldExponent - floor(ldExponent);		
			ldExponent -= ldValue;
			long double ldB10 = pow(10.0,ldValue);

			std::vector<char> vDigits;
			std::vector<char> vDecimal_Digits;
			bool bRound = true;
			if (!i_bEngineering_Form)
			{
				if (ldExponent < 0.0)
				{
					vDigits.push_back('0');
					ldExponent += 1.0;
					while (ldExponent < 0.0)
					{
						if (vDecimal_Digits.size() < i_nNum_Digits)
							vDecimal_Digits.push_back('0');
						ldExponent += 1.0;
					}
					bRound = (vDecimal_Digits.size() < i_nNum_Digits);
					while (vDecimal_Digits.size() < i_nNum_Digits)
					{
						vDecimal_Digits.push_back(extract_digit(ldB10));
						ldB10 *= 10.0;
					}
				}
				else
				{
					while (ldExponent >= 0.0)
					{
						vDigits.push_back(extract_digit(ldB10));
						ldB10 *= 10.0;
						ldExponent -= 1.0;
					}
					while (vDecimal_Digits.size() < i_nNum_Digits)
					{
						vDecimal_Digits.push_back(extract_digit(ldB10));
						ldB10 *= 10.0;
					}
				}
			}
			else
			{				
				vDigits.push_back(extract_digit(ldB10));
				ldB10 *= 10.0;

				while (vDecimal_Digits.size() < i_nNum_Digits)
				{
					vDecimal_Digits.push_back(extract_digit(ldB10));
					ldB10 *= 10.0;
				}
			}
			if (bRound)
			{
				if (ldB10 > 5.0) // need to round up
				{
					if (vDecimal_Digits.size() > 0)
					{
						size_t tI = vDecimal_Digits.size() - 1;
						while (tI > 0 && tI < vDecimal_Digits.size() && vDecimal_Digits[tI] == '9')
						{
							vDecimal_Digits[tI] = '0';
							tI--;
						}
						if (tI < vDecimal_Digits.size())
							vDecimal_Digits[tI]++;
						else
						{
							size_t tJ = vDigits.size() - 1;
							while (tJ > 0 && tJ < vDigits.size() && vDigits[tJ] == '9')
							{
								vDigits[tJ] = '0';
								tJ--;
							}
							if (tJ < vDecimal_Digits.size())
								vDigits[tJ]++;
							else
							{
								vDecimal_Digits.emplace(vDecimal_Digits.begin(),vDigits.back()); // add last digit to the decimals
								vDigits.pop_back();
								ldExponent += 1.0;
								vDigits.push_back('1');
							}
						}
					}
					else
					{
						size_t tJ = vDigits.size() - 1;
						while (tJ > 0 && tJ < vDigits.size() && vDigits[tJ] == '9')
						{
							vDigits[tJ] = '0';
							tJ--;
						}
						if (tJ < vDecimal_Digits.size())
							vDigits[tJ]++;
						else
						{
							vDecimal_Digits.emplace(vDecimal_Digits.begin(),vDigits.back()); // add last digit to the decimals
							vDigits.pop_back();
							ldExponent += 1.0;
							vDigits.push_back('1');
						}
					}
				}
			}
			if (signbit())
				szRet.push_back('-');
			for (size_t tI = 0; tI < vDigits.size(); tI++)
				szRet.push_back(vDigits[tI]);
			if (vDecimal_Digits.size() > 0)
				szRet.push_back('.');
			for (size_t tI = 0; tI < vDecimal_Digits.size(); tI++)
				szRet.push_back(vDecimal_Digits[tI]);
			if (i_bEngineering_Form)
			{
				szRet.push_back('e');
				expdouble xdExp(ldExponent);
				if (!xdExp.signbit())
					szRet.push_back('+');
				std::string szExp = xdExp.get_b10_value(0,false);
				for (size_t tI = 0; tI < szExp.size(); tI++)
				{
					szRet.push_back(szExp[tI]);
				}
			}
		}
		return szRet;
	}

	inline bool operator ==(const expdouble & i_cRHO) const
	{
		return (isnan() || i_cRHO.isnan() || 
				(iszero() && i_cRHO.iszero()) ||
				(m_nMantissa == i_cRHO.m_nMantissa && m_nExponent == i_cRHO.m_nExponent));
	}
	inline bool operator !=(const expdouble & i_cRHO) const
	{
		return (isnan() || i_cRHO.isnan() || !(i_cRHO == *this));
	}
	inline bool operator >(const expdouble & i_cRHO) const
	{
		return (isnan() || i_cRHO.isnan() || 
				(!iszero() && i_cRHO.iszero() && !signbit()) || 
				(iszero() && !i_cRHO.iszero() && i_cRHO.signbit()) || 
				(!iszero() && !signbit() && i_cRHO.signbit()) ||
				(!iszero() && !i_cRHO.iszero() && (
					(signbit() && i_cRHO.signbit() && exponent() < i_cRHO.exponent()) ||
					(!signbit() && !i_cRHO.signbit() && exponent() > i_cRHO.exponent()) ||
					(signbit() && i_cRHO.signbit() && exponent() == i_cRHO.exponent() && m_nMantissa < i_cRHO.m_nMantissa) ||
					(!signbit() && !i_cRHO.signbit() && exponent() == i_cRHO.exponent() && m_nMantissa > i_cRHO.m_nMantissa))));

	}
	inline bool operator <(const expdouble & i_cRHO) const
	{
		return (isnan() || i_cRHO.isnan() || 
				(iszero() && !i_cRHO.iszero() && !i_cRHO.signbit()) || 
				(!iszero() && signbit() && i_cRHO.iszero()) || 
				(!iszero() && signbit() && !i_cRHO.signbit()) ||
				(!iszero() && !i_cRHO.iszero() && (
					(signbit() && i_cRHO.signbit() && exponent() > i_cRHO.exponent()) ||
					(!signbit() && !i_cRHO.signbit() && exponent() < i_cRHO.exponent()) ||
					(signbit() && i_cRHO.signbit() && exponent() == i_cRHO.exponent() && m_nMantissa > i_cRHO.m_nMantissa) ||
					(!signbit() && !i_cRHO.signbit() && exponent() == i_cRHO.exponent() && m_nMantissa < i_cRHO.m_nMantissa))));

	}
	inline bool operator <=(const expdouble & i_cRHO) const
	{
		return (*this == i_cRHO) || (*this < i_cRHO);
	}
	inline bool operator >=(const expdouble & i_cRHO) const
	{
		return (*this == i_cRHO) || (*this > i_cRHO);
	}

	//@@TODO: division
	//@@TODO: exp, pow, log, sqrt
	//@@TODO: log10, log2, exp2, exp10, exp10m1, exp2m1
	//@@TODO: cos, tan, sin
	//@@TODO: atan, acos, asin, 
	//@@TODO: fused multiply add
	//@@TODO: cosh, tanh, sinh
	//@@TODO: atanh, acosh, asinh
	//@@TODO: the rest of the IEEE 754-2008 standard
	//@@TODO: correctly handle infinities and underflow in multiplication and addition
		
};
inline std::ostream & operator<<(std::ostream& io_Out, const expdouble& i_xdVal)
{
	io_Out << i_xdVal.get_b10_value(io_Out.precision(),(io_Out.flags() & io_Out.scientific) != 0);
	return io_Out;
}

expdouble EDBL_MIN(0x8000000000000000,0,false);
expdouble EDBL_MAX(0xffffffffffffffff,0x7ffffffffffffffe,false);
#define EDBL_MANT_DIG	64
#define EDBL_DIG 		18
#define EDBL_MIN_EXP	-4611686018427387901
#define EDBL_MAX_EXP	4611686018427387904
expdouble EDBL_EPSILON(0x8000000000000000,4611686018427387902 - 63,false);
#define EDBL_MIN_10_EXP -1388255822130836092 //based on EDBL_MIN
#define EDBL_MAX_10_EXP 1388255822130836092 //based on EDBL_MAX

#if 0
                    GNU GENERAL PUBLIC LICENSE
                       Version 3, 29 June 2007

 Copyright (C) 2007 Free Software Foundation, Inc. <http://fsf.org/>
 Everyone is permitted to copy and distribute verbatim copies
 of this license document, but changing it is not allowed.

                            Preamble

  The GNU General Public License is a free, copyleft license for
software and other kinds of works.

  The licenses for most software and other practical works are designed
to take away your freedom to share and change the works.  By contrast,
the GNU General Public License is intended to guarantee your freedom to
share and change all versions of a program--to make sure it remains free
software for all its users.  We, the Free Software Foundation, use the
GNU General Public License for most of our software; it applies also to
any other work released this way by its authors.  You can apply it to
your programs, too.

  When we speak of free software, we are referring to freedom, not
price.  Our General Public Licenses are designed to make sure that you
have the freedom to distribute copies of free software (and charge for
them if you wish), that you receive source code or can get it if you
want it, that you can change the software or use pieces of it in new
free programs, and that you know you can do these things.

  To protect your rights, we need to prevent others from denying you
these rights or asking you to surrender the rights.  Therefore, you have
certain responsibilities if you distribute copies of the software, or if
you modify it: responsibilities to respect the freedom of others.

  For example, if you distribute copies of such a program, whether
gratis or for a fee, you must pass on to the recipients the same
freedoms that you received.  You must make sure that they, too, receive
or can get the source code.  And you must show them these terms so they
know their rights.

  Developers that use the GNU GPL protect your rights with two steps:
(1) assert copyright on the software, and (2) offer you this License
giving you legal permission to copy, distribute and/or modify it.

  For the developers' and authors' protection, the GPL clearly explains
that there is no warranty for this free software.  For both users' and
authors' sake, the GPL requires that modified versions be marked as
changed, so that their problems will not be attributed erroneously to
authors of previous versions.

  Some devices are designed to deny users access to install or run
modified versions of the software inside them, although the manufacturer
can do so.  This is fundamentally incompatible with the aim of
protecting users' freedom to change the software.  The systematic
pattern of such abuse occurs in the area of products for individuals to
use, which is precisely where it is most unacceptable.  Therefore, we
have designed this version of the GPL to prohibit the practice for those
products.  If such problems arise substantially in other domains, we
stand ready to extend this provision to those domains in future versions
of the GPL, as needed to protect the freedom of users.

  Finally, every program is threatened constantly by software patents.
States should not allow patents to restrict development and use of
software on general-purpose computers, but in those that do, we wish to
avoid the special danger that patents applied to a free program could
make it effectively proprietary.  To prevent this, the GPL assures that
patents cannot be used to render the program non-free.

  The precise terms and conditions for copying, distribution and
modification follow.

                       TERMS AND CONDITIONS

  0. Definitions.

  "This License" refers to version 3 of the GNU General Public License.

  "Copyright" also means copyright-like laws that apply to other kinds of
works, such as semiconductor masks.

  "The Program" refers to any copyrightable work licensed under this
License.  Each licensee is addressed as "you".  "Licensees" and
"recipients" may be individuals or organizations.

  To "modify" a work means to copy from or adapt all or part of the work
in a fashion requiring copyright permission, other than the making of an
exact copy.  The resulting work is called a "modified version" of the
earlier work or a work "based on" the earlier work.

  A "covered work" means either the unmodified Program or a work based
on the Program.

  To "propagate" a work means to do anything with it that, without
permission, would make you directly or secondarily liable for
infringement under applicable copyright law, except executing it on a
computer or modifying a private copy.  Propagation includes copying,
distribution (with or without modification), making available to the
public, and in some countries other activities as well.

  To "convey" a work means any kind of propagation that enables other
parties to make or receive copies.  Mere interaction with a user through
a computer network, with no transfer of a copy, is not conveying.

  An interactive user interface displays "Appropriate Legal Notices"
to the extent that it includes a convenient and prominently visible
feature that (1) displays an appropriate copyright notice, and (2)
tells the user that there is no warranty for the work (except to the
extent that warranties are provided), that licensees may convey the
work under this License, and how to view a copy of this License.  If
the interface presents a list of user commands or options, such as a
menu, a prominent item in the list meets this criterion.

  1. Source Code.

  The "source code" for a work means the preferred form of the work
for making modifications to it.  "Object code" means any non-source
form of a work.

  A "Standard Interface" means an interface that either is an official
standard defined by a recognized standards body, or, in the case of
interfaces specified for a particular programming language, one that
is widely used among developers working in that language.

  The "System Libraries" of an executable work include anything, other
than the work as a whole, that (a) is included in the normal form of
packaging a Major Component, but which is not part of that Major
Component, and (b) serves only to enable use of the work with that
Major Component, or to implement a Standard Interface for which an
implementation is available to the public in source code form.  A
"Major Component", in this context, means a major essential component
(kernel, window system, and so on) of the specific operating system
(if any) on which the executable work runs, or a compiler used to
produce the work, or an object code interpreter used to run it.

  The "Corresponding Source" for a work in object code form means all
the source code needed to generate, install, and (for an executable
work) run the object code and to modify the work, including scripts to
control those activities.  However, it does not include the work's
System Libraries, or general-purpose tools or generally available free
programs which are used unmodified in performing those activities but
which are not part of the work.  For example, Corresponding Source
includes interface definition files associated with source files for
the work, and the source code for shared libraries and dynamically
linked subprograms that the work is specifically designed to require,
such as by intimate data communication or control flow between those
subprograms and other parts of the work.

  The Corresponding Source need not include anything that users
can regenerate automatically from other parts of the Corresponding
Source.

  The Corresponding Source for a work in source code form is that
same work.

  2. Basic Permissions.

  All rights granted under this License are granted for the term of
copyright on the Program, and are irrevocable provided the stated
conditions are met.  This License explicitly affirms your unlimited
permission to run the unmodified Program.  The output from running a
covered work is covered by this License only if the output, given its
content, constitutes a covered work.  This License acknowledges your
rights of fair use or other equivalent, as provided by copyright law.

  You may make, run and propagate covered works that you do not
convey, without conditions so long as your license otherwise remains
in force.  You may convey covered works to others for the sole purpose
of having them make modifications exclusively for you, or provide you
with facilities for running those works, provided that you comply with
the terms of this License in conveying all material for which you do
not control copyright.  Those thus making or running the covered works
for you must do so exclusively on your behalf, under your direction
and control, on terms that prohibit them from making any copies of
your copyrighted material outside their relationship with you.

  Conveying under any other circumstances is permitted solely under
the conditions stated below.  Sublicensing is not allowed; section 10
makes it unnecessary.

  3. Protecting Users' Legal Rights From Anti-Circumvention Law.

  No covered work shall be deemed part of an effective technological
measure under any applicable law fulfilling obligations under article
11 of the WIPO copyright treaty adopted on 20 December 1996, or
similar laws prohibiting or restricting circumvention of such
measures.

  When you convey a covered work, you waive any legal power to forbid
circumvention of technological measures to the extent such circumvention
is effected by exercising rights under this License with respect to
the covered work, and you disclaim any intention to limit operation or
modification of the work as a means of enforcing, against the work's
users, your or third parties' legal rights to forbid circumvention of
technological measures.

  4. Conveying Verbatim Copies.

  You may convey verbatim copies of the Program's source code as you
receive it, in any medium, provided that you conspicuously and
appropriately publish on each copy an appropriate copyright notice;
keep intact all notices stating that this License and any
non-permissive terms added in accord with section 7 apply to the code;
keep intact all notices of the absence of any warranty; and give all
recipients a copy of this License along with the Program.

  You may charge any price or no price for each copy that you convey,
and you may offer support or warranty protection for a fee.

  5. Conveying Modified Source Versions.

  You may convey a work based on the Program, or the modifications to
produce it from the Program, in the form of source code under the
terms of section 4, provided that you also meet all of these conditions:

    a) The work must carry prominent notices stating that you modified
    it, and giving a relevant date.

    b) The work must carry prominent notices stating that it is
    released under this License and any conditions added under section
    7.  This requirement modifies the requirement in section 4 to
    "keep intact all notices".

    c) You must license the entire work, as a whole, under this
    License to anyone who comes into possession of a copy.  This
    License will therefore apply, along with any applicable section 7
    additional terms, to the whole of the work, and all its parts,
    regardless of how they are packaged.  This License gives no
    permission to license the work in any other way, but it does not
    invalidate such permission if you have separately received it.

    d) If the work has interactive user interfaces, each must display
    Appropriate Legal Notices; however, if the Program has interactive
    interfaces that do not display Appropriate Legal Notices, your
    work need not make them do so.

  A compilation of a covered work with other separate and independent
works, which are not by their nature extensions of the covered work,
and which are not combined with it such as to form a larger program,
in or on a volume of a storage or distribution medium, is called an
"aggregate" if the compilation and its resulting copyright are not
used to limit the access or legal rights of the compilation's users
beyond what the individual works permit.  Inclusion of a covered work
in an aggregate does not cause this License to apply to the other
parts of the aggregate.

  6. Conveying Non-Source Forms.

  You may convey a covered work in object code form under the terms
of sections 4 and 5, provided that you also convey the
machine-readable Corresponding Source under the terms of this License,
in one of these ways:

    a) Convey the object code in, or embodied in, a physical product
    (including a physical distribution medium), accompanied by the
    Corresponding Source fixed on a durable physical medium
    customarily used for software interchange.

    b) Convey the object code in, or embodied in, a physical product
    (including a physical distribution medium), accompanied by a
    written offer, valid for at least three years and valid for as
    long as you offer spare parts or customer support for that product
    model, to give anyone who possesses the object code either (1) a
    copy of the Corresponding Source for all the software in the
    product that is covered by this License, on a durable physical
    medium customarily used for software interchange, for a price no
    more than your reasonable cost of physically performing this
    conveying of source, or (2) access to copy the
    Corresponding Source from a network server at no charge.

    c) Convey individual copies of the object code with a copy of the
    written offer to provide the Corresponding Source.  This
    alternative is allowed only occasionally and noncommercially, and
    only if you received the object code with such an offer, in accord
    with subsection 6b.

    d) Convey the object code by offering access from a designated
    place (gratis or for a charge), and offer equivalent access to the
    Corresponding Source in the same way through the same place at no
    further charge.  You need not require recipients to copy the
    Corresponding Source along with the object code.  If the place to
    copy the object code is a network server, the Corresponding Source
    may be on a different server (operated by you or a third party)
    that supports equivalent copying facilities, provided you maintain
    clear directions next to the object code saying where to find the
    Corresponding Source.  Regardless of what server hosts the
    Corresponding Source, you remain obligated to ensure that it is
    available for as long as needed to satisfy these requirements.

    e) Convey the object code using peer-to-peer transmission, provided
    you inform other peers where the object code and Corresponding
    Source of the work are being offered to the general public at no
    charge under subsection 6d.

  A separable portion of the object code, whose source code is excluded
from the Corresponding Source as a System Library, need not be
included in conveying the object code work.

  A "User Product" is either (1) a "consumer product", which means any
tangible personal property which is normally used for personal, family,
or household purposes, or (2) anything designed or sold for incorporation
into a dwelling.  In determining whether a product is a consumer product,
doubtful cases shall be resolved in favor of coverage.  For a particular
product received by a particular user, "normally used" refers to a
typical or common use of that class of product, regardless of the status
of the particular user or of the way in which the particular user
actually uses, or expects or is expected to use, the product.  A product
is a consumer product regardless of whether the product has substantial
commercial, industrial or non-consumer uses, unless such uses represent
the only significant mode of use of the product.

  "Installation Information" for a User Product means any methods,
procedures, authorization keys, or other information required to install
and execute modified versions of a covered work in that User Product from
a modified version of its Corresponding Source.  The information must
suffice to ensure that the continued functioning of the modified object
code is in no case prevented or interfered with solely because
modification has been made.

  If you convey an object code work under this section in, or with, or
specifically for use in, a User Product, and the conveying occurs as
part of a transaction in which the right of possession and use of the
User Product is transferred to the recipient in perpetuity or for a
fixed term (regardless of how the transaction is characterized), the
Corresponding Source conveyed under this section must be accompanied
by the Installation Information.  But this requirement does not apply
if neither you nor any third party retains the ability to install
modified object code on the User Product (for example, the work has
been installed in ROM).

  The requirement to provide Installation Information does not include a
requirement to continue to provide support service, warranty, or updates
for a work that has been modified or installed by the recipient, or for
the User Product in which it has been modified or installed.  Access to a
network may be denied when the modification itself materially and
adversely affects the operation of the network or violates the rules and
protocols for communication across the network.

  Corresponding Source conveyed, and Installation Information provided,
in accord with this section must be in a format that is publicly
documented (and with an implementation available to the public in
source code form), and must require no special password or key for
unpacking, reading or copying.

  7. Additional Terms.

  "Additional permissions" are terms that supplement the terms of this
License by making exceptions from one or more of its conditions.
Additional permissions that are applicable to the entire Program shall
be treated as though they were included in this License, to the extent
that they are valid under applicable law.  If additional permissions
apply only to part of the Program, that part may be used separately
under those permissions, but the entire Program remains governed by
this License without regard to the additional permissions.

  When you convey a copy of a covered work, you may at your option
remove any additional permissions from that copy, or from any part of
it.  (Additional permissions may be written to require their own
removal in certain cases when you modify the work.)  You may place
additional permissions on material, added by you to a covered work,
for which you have or can give appropriate copyright permission.

  Notwithstanding any other provision of this License, for material you
add to a covered work, you may (if authorized by the copyright holders of
that material) supplement the terms of this License with terms:

    a) Disclaiming warranty or limiting liability differently from the
    terms of sections 15 and 16 of this License; or

    b) Requiring preservation of specified reasonable legal notices or
    author attributions in that material or in the Appropriate Legal
    Notices displayed by works containing it; or

    c) Prohibiting misrepresentation of the origin of that material, or
    requiring that modified versions of such material be marked in
    reasonable ways as different from the original version; or

    d) Limiting the use for publicity purposes of names of licensors or
    authors of the material; or

    e) Declining to grant rights under trademark law for use of some
    trade names, trademarks, or service marks; or

    f) Requiring indemnification of licensors and authors of that
    material by anyone who conveys the material (or modified versions of
    it) with contractual assumptions of liability to the recipient, for
    any liability that these contractual assumptions directly impose on
    those licensors and authors.

  All other non-permissive additional terms are considered "further
restrictions" within the meaning of section 10.  If the Program as you
received it, or any part of it, contains a notice stating that it is
governed by this License along with a term that is a further
restriction, you may remove that term.  If a license document contains
a further restriction but permits relicensing or conveying under this
License, you may add to a covered work material governed by the terms
of that license document, provided that the further restriction does
not survive such relicensing or conveying.

  If you add terms to a covered work in accord with this section, you
must place, in the relevant source files, a statement of the
additional terms that apply to those files, or a notice indicating
where to find the applicable terms.

  Additional terms, permissive or non-permissive, may be stated in the
form of a separately written license, or stated as exceptions;
the above requirements apply either way.

  8. Termination.

  You may not propagate or modify a covered work except as expressly
provided under this License.  Any attempt otherwise to propagate or
modify it is void, and will automatically terminate your rights under
this License (including any patent licenses granted under the third
paragraph of section 11).

  However, if you cease all violation of this License, then your
license from a particular copyright holder is reinstated (a)
provisionally, unless and until the copyright holder explicitly and
finally terminates your license, and (b) permanently, if the copyright
holder fails to notify you of the violation by some reasonable means
prior to 60 days after the cessation.

  Moreover, your license from a particular copyright holder is
reinstated permanently if the copyright holder notifies you of the
violation by some reasonable means, this is the first time you have
received notice of violation of this License (for any work) from that
copyright holder, and you cure the violation prior to 30 days after
your receipt of the notice.

  Termination of your rights under this section does not terminate the
licenses of parties who have received copies or rights from you under
this License.  If your rights have been terminated and not permanently
reinstated, you do not qualify to receive new licenses for the same
material under section 10.

  9. Acceptance Not Required for Having Copies.

  You are not required to accept this License in order to receive or
run a copy of the Program.  Ancillary propagation of a covered work
occurring solely as a consequence of using peer-to-peer transmission
to receive a copy likewise does not require acceptance.  However,
nothing other than this License grants you permission to propagate or
modify any covered work.  These actions infringe copyright if you do
not accept this License.  Therefore, by modifying or propagating a
covered work, you indicate your acceptance of this License to do so.

  10. Automatic Licensing of Downstream Recipients.

  Each time you convey a covered work, the recipient automatically
receives a license from the original licensors, to run, modify and
propagate that work, subject to this License.  You are not responsible
for enforcing compliance by third parties with this License.

  An "entity transaction" is a transaction transferring control of an
organization, or substantially all assets of one, or subdividing an
organization, or merging organizations.  If propagation of a covered
work results from an entity transaction, each party to that
transaction who receives a copy of the work also receives whatever
licenses to the work the party's predecessor in interest had or could
give under the previous paragraph, plus a right to possession of the
Corresponding Source of the work from the predecessor in interest, if
the predecessor has it or can get it with reasonable efforts.

  You may not impose any further restrictions on the exercise of the
rights granted or affirmed under this License.  For example, you may
not impose a license fee, royalty, or other charge for exercise of
rights granted under this License, and you may not initiate litigation
(including a cross-claim or counterclaim in a lawsuit) alleging that
any patent claim is infringed by making, using, selling, offering for
sale, or importing the Program or any portion of it.

  11. Patents.

  A "contributor" is a copyright holder who authorizes use under this
License of the Program or a work on which the Program is based.  The
work thus licensed is called the contributor's "contributor version".

  A contributor's "essential patent claims" are all patent claims
owned or controlled by the contributor, whether already acquired or
hereafter acquired, that would be infringed by some manner, permitted
by this License, of making, using, or selling its contributor version,
but do not include claims that would be infringed only as a
consequence of further modification of the contributor version.  For
purposes of this definition, "control" includes the right to grant
patent sublicenses in a manner consistent with the requirements of
this License.

  Each contributor grants you a non-exclusive, worldwide, royalty-free
patent license under the contributor's essential patent claims, to
make, use, sell, offer for sale, import and otherwise run, modify and
propagate the contents of its contributor version.

  In the following three paragraphs, a "patent license" is any express
agreement or commitment, however denominated, not to enforce a patent
(such as an express permission to practice a patent or covenant not to
sue for patent infringement).  To "grant" such a patent license to a
party means to make such an agreement or commitment not to enforce a
patent against the party.

  If you convey a covered work, knowingly relying on a patent license,
and the Corresponding Source of the work is not available for anyone
to copy, free of charge and under the terms of this License, through a
publicly available network server or other readily accessible means,
then you must either (1) cause the Corresponding Source to be so
available, or (2) arrange to deprive yourself of the benefit of the
patent license for this particular work, or (3) arrange, in a manner
consistent with the requirements of this License, to extend the patent
license to downstream recipients.  "Knowingly relying" means you have
actual knowledge that, but for the patent license, your conveying the
covered work in a country, or your recipient's use of the covered work
in a country, would infringe one or more identifiable patents in that
country that you have reason to believe are valid.

  If, pursuant to or in connection with a single transaction or
arrangement, you convey, or propagate by procuring conveyance of, a
covered work, and grant a patent license to some of the parties
receiving the covered work authorizing them to use, propagate, modify
or convey a specific copy of the covered work, then the patent license
you grant is automatically extended to all recipients of the covered
work and works based on it.

  A patent license is "discriminatory" if it does not include within
the scope of its coverage, prohibits the exercise of, or is
conditioned on the non-exercise of one or more of the rights that are
specifically granted under this License.  You may not convey a covered
work if you are a party to an arrangement with a third party that is
in the business of distributing software, under which you make payment
to the third party based on the extent of your activity of conveying
the work, and under which the third party grants, to any of the
parties who would receive the covered work from you, a discriminatory
patent license (a) in connection with copies of the covered work
conveyed by you (or copies made from those copies), or (b) primarily
for and in connection with specific products or compilations that
contain the covered work, unless you entered into that arrangement,
or that patent license was granted, prior to 28 March 2007.

  Nothing in this License shall be construed as excluding or limiting
any implied license or other defenses to infringement that may
otherwise be available to you under applicable patent law.

  12. No Surrender of Others' Freedom.

  If conditions are imposed on you (whether by court order, agreement or
otherwise) that contradict the conditions of this License, they do not
excuse you from the conditions of this License.  If you cannot convey a
covered work so as to satisfy simultaneously your obligations under this
License and any other pertinent obligations, then as a consequence you may
not convey it at all.  For example, if you agree to terms that obligate you
to collect a royalty for further conveying from those to whom you convey
the Program, the only way you could satisfy both those terms and this
License would be to refrain entirely from conveying the Program.

  13. Use with the GNU Affero General Public License.

  Notwithstanding any other provision of this License, you have
permission to link or combine any covered work with a work licensed
under version 3 of the GNU Affero General Public License into a single
combined work, and to convey the resulting work.  The terms of this
License will continue to apply to the part which is the covered work,
but the special requirements of the GNU Affero General Public License,
section 13, concerning interaction through a network will apply to the
combination as such.

  14. Revised Versions of this License.

  The Free Software Foundation may publish revised and/or new versions of
the GNU General Public License from time to time.  Such new versions will
be similar in spirit to the present version, but may differ in detail to
address new problems or concerns.

  Each version is given a distinguishing version number.  If the
Program specifies that a certain numbered version of the GNU General
Public License "or any later version" applies to it, you have the
option of following the terms and conditions either of that numbered
version or of any later version published by the Free Software
Foundation.  If the Program does not specify a version number of the
GNU General Public License, you may choose any version ever published
by the Free Software Foundation.

  If the Program specifies that a proxy can decide which future
versions of the GNU General Public License can be used, that proxy's
public statement of acceptance of a version permanently authorizes you
to choose that version for the Program.

  Later license versions may give you additional or different
permissions.  However, no additional obligations are imposed on any
author or copyright holder as a result of your choosing to follow a
later version.

  15. Disclaimer of Warranty.

  THERE IS NO WARRANTY FOR THE PROGRAM, TO THE EXTENT PERMITTED BY
APPLICABLE LAW.  EXCEPT WHEN OTHERWISE STATED IN WRITING THE COPYRIGHT
HOLDERS AND/OR OTHER PARTIES PROVIDE THE PROGRAM "AS IS" WITHOUT WARRANTY
OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE.  THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE PROGRAM
IS WITH YOU.  SHOULD THE PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF
ALL NECESSARY SERVICING, REPAIR OR CORRECTION.

  16. Limitation of Liability.

  IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN WRITING
WILL ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MODIFIES AND/OR CONVEYS
THE PROGRAM AS PERMITTED ABOVE, BE LIABLE TO YOU FOR DAMAGES, INCLUDING ANY
GENERAL, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING OUT OF THE
USE OR INABILITY TO USE THE PROGRAM (INCLUDING BUT NOT LIMITED TO LOSS OF
DATA OR DATA BEING RENDERED INACCURATE OR LOSSES SUSTAINED BY YOU OR THIRD
PARTIES OR A FAILURE OF THE PROGRAM TO OPERATE WITH ANY OTHER PROGRAMS),
EVEN IF SUCH HOLDER OR OTHER PARTY HAS BEEN ADVISED OF THE POSSIBILITY OF
SUCH DAMAGES.

  17. Interpretation of Sections 15 and 16.

  If the disclaimer of warranty and limitation of liability provided
above cannot be given local legal effect according to their terms,
reviewing courts shall apply local law that most closely approximates
an absolute waiver of all civil liability in connection with the
Program, unless a warranty or assumption of liability accompanies a
copy of the Program in return for a fee.

                     END OF TERMS AND CONDITIONS

            How to Apply These Terms to Your New Programs

  If you develop a new program, and you want it to be of the greatest
possible use to the public, the best way to achieve this is to make it
free software which everyone can redistribute and change under these terms.

  To do so, attach the following notices to the program.  It is safest
to attach them to the start of each source file to most effectively
state the exclusion of warranty; and each file should have at least
the "copyright" line and a pointer to where the full notice is found.

    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

Also add information on how to contact you by electronic and paper mail.

  If the program does terminal interaction, make it output a short
notice like this when it starts in an interactive mode:

    <program>  Copyright (C) <year>  <name of author>
    This program comes with ABSOLUTELY NO WARRANTY; for details type `show w'.
    This is free software, and you are welcome to redistribute it
    under certain conditions; type `show c' for details.

The hypothetical commands `show w' and `show c' should show the appropriate
parts of the General Public License.  Of course, your program's commands
might be different; for a GUI interface, you would use an "about box".

  You should also get your employer (if you work as a programmer) or school,
if any, to sign a "copyright disclaimer" for the program, if necessary.
For more information on this, and how to apply and follow the GNU GPL, see
<http://www.gnu.org/licenses/>.

  The GNU General Public License does not permit incorporating your program
into proprietary programs.  If your program is a subroutine library, you
may consider it more useful to permit linking proprietary applications with
the library.  If this is what you want to do, use the GNU Lesser General
Public License instead of this License.  But first, please read
<http://www.gnu.org/philosophy/why-not-lgpl.html>.
#endif
