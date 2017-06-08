#include <cstdio>
#include <inttypes.h>
#include <iostream>
#include <cmath>
#include <cfloat>
#include <limits>
#include <vector>
#include <xextprec.h>

#include <iomanip>



void printbit(uint8_t i_cVal)
{
	if (i_cVal == 0)
		printf("0");
	else
		printf("1");
}

void pbin(void * i_lpdData, size_t tN_Bytes)
{
	for (size_t tI = 0; tI < tN_Bytes; tI++)
	{
		printf("--------");
	}
	printf("\n");
	uint8_t * lpcData = (uint8_t *)i_lpdData;
	for (size_t tI = 0; tI < tN_Bytes; tI++)
	{
		printbit(lpcData[tN_Bytes - tI - 1] & 0x80);
		printbit(lpcData[tN_Bytes - tI - 1] & 0x40);
		printbit(lpcData[tN_Bytes - tI - 1] & 0x20);
		printbit(lpcData[tN_Bytes - tI - 1] & 0x10);
		printbit(lpcData[tN_Bytes - tI - 1] & 0x08);
		printbit(lpcData[tN_Bytes - tI - 1] & 0x04);
		printbit(lpcData[tN_Bytes - tI - 1] & 0x02);
		printbit(lpcData[tN_Bytes - tI - 1] & 0x01);

	}
	printf("\n");
/*	for (size_t tI = 0; tI < tN_Bytes; tI++)
	{
		printf("76543210");
	}
	printf("\n");
	for (size_t tI = 0; tI < tN_Bytes; tI++)
	{
		for (size_t tJ = 0; tJ < 8; tJ++)
			printf("%x",tI % 16);
	}
	printf("\n");
	for (size_t tI = 0; tI < tN_Bytes; tI++)
	{
		printf("--------");
	}
	printf("\n");*/

}
template<typename T>  void pbin(const T & i_ldData)
{
	pbin((void *)&i_ldData,sizeof(T));
}

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


void mantest(const long double & i_ldLD)
{
	int iExp;
	long double ldMan = std::frexp(i_ldLD,&iExp);
	long double ldInt = std::ldexp(ldMan,64);
	uint64_t m_nMantissa = (uint64_t)ldInt;

	uint64_t * lptData = (uint64_t *) &i_ldLD;
	m_nMantissa = lptData[0];
	int64_t m_nExponent = (lptData[1] & 0x7fff) - 16382;
	uint64_t m_nSign = (lptData[1] & 0x8000) >> 15;

	
	std::cout << i_ldLD << " " << ldMan << " " << iExp << " " << m_nExponent << " " << m_nSign << std::endl;
	
//	pbin(m_nExponent);
//	pbin(m_nSign);
}

int main(int iA, char * lpcC[])
{
	printf("%i %i %i %i %i\n",sizeof(float),sizeof(double),sizeof(long double),sizeof(long double *),LDBL_MAX_EXP);
	printf("%.18Le %.18Le\n",LDBL_EPSILON,EDBL_EPSILON.unload());

	expdouble xdExp(4611686018427387904.);
	expdouble xdLn210(std::log(10.0) / std::log(2.0));
	//xdExp /= 
	long double dM1 = -1;
	long double dp25 = 0.25;
	long double dp5 = 0.5;
	long double d1 = 1.;
	long double d2 = 2.;
	long double d3 = 3.;
	long double d8 = 8.;
	long double dthird = 1.0;
	dthird /= 3.0;
	long double dp4 = 0.4;
	long double dPinf = 1.0;
	dPinf /= 0.0;
	long double dMinf = -1.0;
	dMinf /= 0.0;
	long double dpnan = std::nan("");
	uint64_t tA = 0x8000000000000000;

	int iExp;
	std::frexp(dp25,&iExp);
	std::cout << iExp << std::endl;
	expdouble xdA(dp25);
	std::cout << xdA.exponent() << std::endl;
	std::frexp(d8,&iExp);
	std::cout << iExp << std::endl;
	expdouble xdB(d8);
	std::cout << xdB.exponent() << std::endl;

	std::cout << xdA.unload() << std::endl;
	std::cout << xdB.unload() << std::endl;

	int64_t tExp;
	std::cout << xdA.frexp(tExp).unload() << std::endl;
	std::cout << std::frexp(dp25,&iExp) << std::endl;
	std::cout << xdB.frexp(tExp).unload() << std::endl;
	std::cout << std::frexp(d8,&iExp) << std::endl;

//	xdA *= 0.5;
	std::cout << "-1 " << xdA.unload() << " " << xdA.frexp(tExp).unload() << " " << xdA.exponent() << std::endl;
	for (unsigned int uiI = 0; uiI < 20000; uiI++)
	{
		xdA *= -0.5;
		if (uiI % 10000 == 0)
			std::cout << uiI << " " << xdA.unload() << " " << xdA.frexp(tExp).unload() << " " << xdA.exponent() << std::endl;
	}
	std::cout << "20000 " << xdA.unload() << " " << xdA.frexp(tExp).unload() << " " << xdA.exponent() << std::endl;
	for (unsigned int uiI = 0; uiI < 20000; uiI++)
	{
		xdA *= -2.0;
		if (uiI % 10000 == 0)
			std::cout << uiI << " " << xdA.unload() << " " << xdA.frexp(tExp).unload() << " " << xdA.exponent() << std::endl;
	}
	std::cout << "20000 " << " " << xdA.unload() << " " << xdA.frexp(tExp).unload() << " " << xdA.exponent() << std::endl;

	expdouble xdThird(dthird);
	std::cout << xdThird.unload() << " " << xdThird.frexp(tExp).unload() << " " << xdThird.exponent() << std::endl;
	xdThird *= -xdThird;
	std::cout << xdThird.unload() << " " << xdThird.frexp(tExp).unload() << " " << xdThird.exponent() << std::endl;
	xdThird *= xdThird;
	std::cout << xdThird.unload() << " " << xdThird.frexp(tExp).unload() << " " << xdThird.exponent() << std::endl;

	std::string szThird = xdThird.get_b10_value(10,true);
	std::cout << szThird << std::endl;
	long double dA = 9.6;
	std::cout << expdouble(9.6).get_b10_value(10,true) << std::endl;


	if (expdouble(-1) < expdouble(0))
		std::cout << "good -1 < 0" << std::endl;
	if (expdouble(-1) < expdouble(1))
		std::cout << "good -1 < 1" << std::endl;
	if (expdouble(0) < expdouble(1))
		std::cout << "good 0 < 1" << std::endl;
	if (expdouble(-1) < expdouble(-0.1))
		std::cout << "good -1 < -0.1" << std::endl;
	if (expdouble(0.1) < expdouble(1))
		std::cout << "good 0.1 < 1.0" << std::endl;

	if (expdouble(0) < expdouble(-1))
		std::cout << "bad 0 < -1" << std::endl;
	if (expdouble(1) < expdouble(-1))
		std::cout << "bad 1 < -1" << std::endl;
	if (expdouble(1) < expdouble(0))
		std::cout << "bad 1 < 0" << std::endl;
	if (expdouble(-0.1) < expdouble(-1))
		std::cout << "bad -0.1 < -1" << std::endl;
	if (expdouble(1.0) < expdouble(0.1))
		std::cout << "bad 1.0 < 0.1" << std::endl;


	if (expdouble(-1) > expdouble(0))
		std::cout << "bad -1 > 0" << std::endl;
	if (expdouble(-1) > expdouble(1))
		std::cout << "bad -1 > 1" << std::endl;
	if (expdouble(0) > expdouble(1))
		std::cout << "bad 0 > 1" << std::endl;
	if (expdouble(-1) > expdouble(-0.1))
		std::cout << "bad -1 > -0.1" << std::endl;
	if (expdouble(0.1) > expdouble(1))
		std::cout << "bad 0.1 > 1.0" << std::endl;

	if (expdouble(0) > expdouble(-1))
		std::cout << "good 0 > -1" << std::endl;
	if (expdouble(1) > expdouble(-1))
		std::cout << "good 1 > -1" << std::endl;
	if (expdouble(1) > expdouble(0))
		std::cout << "good 1 > 0" << std::endl;
	if (expdouble(-0.1) > expdouble(-1))
		std::cout << "good -0.1 > -1" << std::endl;
	if (expdouble(1.0) > expdouble(0.1))
		std::cout << "good 1.0 > 0.1" << std::endl;

	if (expdouble(-1.0) == expdouble(-1.0))
		std::cout << "good -1 = -1" << std::endl;
	if (expdouble(1.0) == expdouble(1.0))
		std::cout << "good 1 = 1" << std::endl;
	if (expdouble(0.0) == expdouble(0.0))
		std::cout << "good 0 = 0" << std::endl;
	if (expdouble(-1.0) == expdouble(1.0))
		std::cout << "bad -1 = 1" << std::endl;
	if (expdouble(1.0) == expdouble(-1.0))
		std::cout << "bad 1 = -1" << std::endl;
	if (expdouble(0.0) == expdouble(1.0))
		std::cout << "bad 0 = 1" << std::endl;

	if (expdouble(-1.0) != expdouble(-1.0))
		std::cout << "bad -1 != -1" << std::endl;
	if (expdouble(1.0) != expdouble(1.0))
		std::cout << "bad 1 != 1" << std::endl;
	if (expdouble(0.0) != expdouble(0.0))
		std::cout << "bad 0 != 0" << std::endl;
	if (expdouble(-1.0) != expdouble(1.0))
		std::cout << "good -1 != 1" << std::endl;
	if (expdouble(1.0) != expdouble(-1.0))
		std::cout << "good 1 != -1" << std::endl;
	if (expdouble(0.0) != expdouble(1.0))
		std::cout << "good 0 != 1" << std::endl;

	std::cout << EDBL_MAX.get_b10_value(20,true);
	std::cout << " " << EDBL_MIN.get_b10_value(20,true) << std::endl;
	std::cout << std::scientific << xdThird << std::endl;

	expdouble xdTest(expdouble(1.0) / expdouble(5.0));
	expdouble xdThird2(expdouble(1.0) / expdouble(7.0));
//	expdouble xdThird2(0.125+0.0625);
	std::cout << xdThird2 << std::endl;
	std::cout << "next" << std::endl;
	size_t tI = 0;
	for (; tI < 10; tI++)
	{	
		xdTest /= xdThird2;
		std::cout << tI << " " << xdTest << std::endl;
	}

	unsigned char chA = 2;
	unsigned char chB = 3;
	unsigned char chAMB = chA - chB;
	pbin(chA);pbin(chB);pbin(chAMB);

	expdouble xdZero(0.0);
	expdouble xdAdd = xdZero + 12.0;
	std::cout << "zero addition:" << std::endl;
	std::cout << "12: " << xdAdd << std::endl;
	xdAdd = xdZero - 12.0;
	std::cout << "-12: " << xdAdd << std::endl;
	xdAdd = 12 + xdZero;
	std::cout << "12: " << xdAdd << std::endl;
	xdAdd = 12 - xdZero;
	std::cout << "12: " << xdAdd << std::endl;


	expdouble xdTwo(2.0);
	expdouble xdSmTwo(2.0e-40);
	expdouble xdThree(3.0);
	expdouble xdM2(-2.0);
	long double ldSmTwo = 2.0e-40;
	std::cout << xdSmTwo.exponent() << std::endl;
	pbin(xdSmTwo.true_exponent());
	pbin(ldSmTwo);
	std::cout << "addition:" << std::endl;
	std::cout << std::scientific << 2e-40 << ": " << (xdSmTwo) << std::endl;
	std::cout << std::scientific << -2 << ": " << (-xdTwo) << std::endl;
	std::cout << std::scientific << -3 << ": " << (-xdThree) << std::endl;
	std::cout << std::scientific << -2e-40 << ": " << (-xdSmTwo) << std::endl;
	std::cout << std::scientific << 2 << ": " << (-xdM2) << std::endl;
	std::cout << std::scientific << 1 << ": " << (xdThree - xdTwo) << std::endl;
	std::cout << std::scientific << 0 << ": " << (xdThree - xdThree) << std::endl;
	std::cout << std::scientific << 0 << ": " << (xdTwo - xdTwo) << std::endl;
	std::cout << std::scientific << 3 << ": " << (xdSmTwo + xdThree) << std::endl;
	std::cout << std::scientific << 3 << ": " << (xdThree + xdSmTwo) << std::endl;
	std::cout << std::scientific << 6 << ": " << (xdThree + xdThree) << std::endl;
	std::cout << std::scientific << 4e-40 << ": " << (xdSmTwo + xdSmTwo) << std::endl;
	std::cout << std::scientific << 0 << ": " << (xdSmTwo - xdSmTwo) << std::endl;
	std::cout << std::scientific << -4 << ": " << (xdM2 + xdM2) << std::endl;
	std::cout << std::scientific << 1 << ": " << (xdThree + xdM2) << std::endl;
	std::cout << std::scientific << 6 << ": " << (xdThree + xdThree) << std::endl;
	std::cout << std::scientific << -4 << ": " << (xdM2 + xdM2) << std::endl;
	std::cout << std::scientific << 0 << ": " << (xdM2 - xdM2) << std::endl;

	expdouble xdTwoload;
	xdTwoload.load((int64_t)2);
	expdouble xdThreeload;
	xdThreeload.load((int64_t)3);
	std::cout << "int load:" << std::endl;
	std::cout << std::fixed << 2.0 << ": " << (xdTwoload) << std::endl;
	std::cout << std::fixed << 3.0 << ": " << (xdThreeload) << std::endl;

	expdouble xdTwopttwo(8.2);
	expdouble xdBitpttwo(92837539804734.2);
	expdouble xdBitpttwofloor = xdBitpttwo.floor();
	//pbin(92837539804734);
	std::cout << "floor:" << std::endl;
	std::cout << std::fixed << 2.0 << ": " << (xdTwo.floor()) << std::endl;
	std::cout << std::fixed << 8.0 << ": " << (xdTwopttwo.floor()) << std::endl;
	std::cout << std::fixed << 92837539804734 << ": " << (xdBitpttwo.floor()) << std::endl;
	std::cout << std::fixed << 92837539804734 << ": " << (xdBitpttwofloor) << std::endl;
	std::cout << std::scientific << 0 << ": " << (xdSmTwo.floor()) << std::endl;
	//pbin(xdBitpttwo.mantissa());
	//pbin(xdBitpttwo.floor().mantissa());
	//pbin(xdBitpttwofloor.mantissa());
	std::cout << expdouble(1.75).exponent() << std::endl;
	xdZero = expdouble(0);
	if (xdZero.iszero())
		std::cout << "zero is zero" << std::endl;
	else
	{
		std::cout << "zero:" << std::endl;
		pbin(xdZero.mantissa());
		pbin(xdZero.true_exponent());
	}

	std::cout << "log2:" << std::endl;
	std::cout << std::fixed << 1.0 << ": " << (xdTwo.log2()) << std::endl;
	std::cout << std::fixed << std::log2(8.2)<< ": " << (xdTwopttwo.log2()) << std::endl;
	std::cout << std::fixed << "inf" << ": " << (xdZero.log2()) << std::endl;
	std::cout << std::fixed << "nan" << ": " << ((-xdTwo).log2()) << std::endl;

	for (long double ldI = 1.0; ldI < 5.0; ldI += 1.0)
	{
		expdouble xdTest(ldI);
		expdouble xdRes = xdTest.exp();
		long double ldRes = xdRes.unload();
		uint64_t * lpiRes = (uint64_t *)&ldRes;
		long double ldExp = std::exp(ldI);
		uint64_t * lpiExp = (uint64_t *)&ldExp;

		std::cout << std::hex << xdRes.mantissa() << " " << lpiRes[0] << " " << lpiExp[0] << std::endl;
		std::cout << ldI << " " << std::setprecision(19) << ldRes << " " << std::exp(ldI) << " " << (ldI * EDBL_LOG2_e).unload() << std::endl;
	}


	return 0;
}
