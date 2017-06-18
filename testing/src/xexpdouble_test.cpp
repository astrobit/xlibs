#include <cstdio>
#include <inttypes.h>
#include <iostream>
#include <cmath>
#include <cfloat>
#include <limits>
#include <vector>
#include <xextprec.h>
#include <xstdlib.h>
#include <iomanip>


FILE * g_fileRand = nullptr;
long double ld_load_rand(void)
{
	long double ldRes = 0;
	if (g_fileRand == nullptr)
		g_fileRand = fopen("/dev/random","rb");
	if (g_fileRand != nullptr)
	{
		uint8_t lpcData[10];
		fread(lpcData,10,1,g_fileRand);
		uint64_t * lpD = (uint64_t *)&ldRes;
		lpD[0] = 0;
		lpD[1] = 0;
		for (uint32_t tI = 0; tI < 8; tI++)
		{
			uint64_t tTemp = lpcData[tI];
			tTemp <<= (56 - (tI << 3));
			lpD[0] |= tTemp;
		}
		lpD[0] |= 0x8000000000000000;
		for (uint32_t tI = 0; tI < 2; tI++)
		{
			uint64_t tTemp = lpcData[tI + 8];
			tTemp <<= (8 - (tI << 3));
			lpD[1] |= tTemp;
		}
	}
	return ldRes;
}
long double ld_load_rand_exp(void) // generates numbers with a more limited ranges that are assured to be safe for exp(x)
{
	long double ldRes = 0;
	if (g_fileRand == nullptr)
		g_fileRand = fopen("/dev/random","rb");
	if (g_fileRand != nullptr)
	{
		uint8_t lpcData[9];
		fread(lpcData,9,1,g_fileRand);
		uint64_t * lpD = (uint64_t *)&ldRes;
		lpD[0] = 0;
		lpD[1] = 0;
		for (uint32_t tI = 0; tI < 8; tI++)
		{
			uint64_t tTemp = lpcData[tI];
			tTemp <<= (56 - (tI << 3));
			lpD[0] |= tTemp;
		}
		lpD[0] |= 0x8000000000000000;
		lpD[1] = 0x03FFF;
		std::cout << ldRes << std::endl;
		int iExp = lpcData[8] & 0x2f; 
		std::cout << iExp << std::endl;
		iExp -= 24;
		std::cout << iExp << std::endl;
		ldRes = std::ldexp(ldRes,iExp);
		if (lpcData[8] & 0x80) // make negative
		{
			lpD[1] |= 0x8000;
		}
		std::cout << ldRes << " " << iExp << std::endl;
		
	}
	return ldRes;
}
bool mult_test(long double & i_LHO, long double & i_RHO, bool &o_bTest_Valid)
{
	long double ldRes = i_LHO * i_RHO;
	expdouble xRes = expdouble(i_LHO) * expdouble(i_RHO);
	long double ldErr = std::fabs(1.0 - xRes.unload() / ldRes);
	o_bTest_Valid = !std::isinf(ldRes) && !std::isnan(ldRes) && ldRes != 0.0; // the probability of LHO = 0 or RHO = 0 is small, so assume that if res = 0, we have fallen outside of the limits of long dobule
//	bool bSuccess = (ldErr < std::ldexp(1.0,-63)) && o_bTest_Valid;
	bool bSuccess = (ldErr == 0.0 || (std::log2(ldErr) < -62.0)) && o_bTest_Valid;
	std::cout << std::log2(ldErr) << " (" << ldErr << ")";
//	std::cout << ldRes << " " << std::log2(ldErr) << " (" << ldErr << ")";
//	std::cout << expdouble(i_LHO).unload() << " * " << expdouble(i_RHO).unload() << " = " << xRes.unload() << " (" << ldRes << ") " << ldErr;
	return bSuccess;
}

bool add_test(long double & i_LHO, long double & i_RHO, bool &o_bTest_Valid)
{
	long double ldRes = i_LHO + i_RHO;
	expdouble xRes = expdouble(i_LHO) + expdouble(i_RHO);
	long double ldErr = std::fabs(1.0 - xRes.unload() / ldRes);
	o_bTest_Valid = !std::isinf(ldRes) && !std::isnan(ldRes);
	bool bSuccess = (ldErr == 0.0 || (std::log2(ldErr) < -62.0)) && o_bTest_Valid;
	std::cout << std::log2(ldErr) << " (" << ldErr << ")";

	return bSuccess;
}

bool exp_test(long double & i_LHO, bool & o_bTest_Valid)
{
	long double ldLHO_Mod = std::log(fabs(i_LHO));
	long double ldRes = std::exp(ldLHO_Mod);
	expdouble xRes = expdouble(ldLHO_Mod).exp();
	long double ldErr = std::fabs(1.0 - xRes.unload() / ldRes);
	o_bTest_Valid = !std::isinf(ldRes) && !std::isnan(ldRes) && ldRes != 0.0 && ldRes != 1.0;
	bool bSuccess = (ldErr == 0.0 || (std::log2(ldErr) < -58.0)) && o_bTest_Valid;
	if (!bSuccess && o_bTest_Valid)
	{
		std::cout << std::endl;
		uint64_t * tPtr = (uint64_t *)&ldRes;
		uint64_t tDiff = (tPtr[0] ^ xRes.mantissa());
		xstdlib::pbin(tDiff);
	}
	std::cout << ldRes << " " << xRes.unload() << " " << std::log2(ldErr) << " (" << ldErr << ")";
	return bSuccess;
}

bool exp2_test(long double & i_LHO, bool & o_bTest_Valid)
{
	long double ldLHO_Mod = std::log2(fabs(i_LHO));
	long double ldRes = std::exp2(ldLHO_Mod);
	expdouble xRes = expdouble(ldLHO_Mod).exp2();
	long double ldErr = std::fabs(1.0 - xRes.unload() / ldRes);
	o_bTest_Valid = !std::isinf(ldRes) && !std::isnan(ldRes) && ldRes != 0.0 && ldRes != 1.0;
	bool bSuccess = (ldErr == 0.0 || (std::log2(ldErr) < -58.0)) && o_bTest_Valid;
	if (!bSuccess && o_bTest_Valid)
	{
		std::cout << std::endl;
		uint64_t * tPtr = (uint64_t *)&ldRes;
		uint64_t tDiff = (tPtr[0] ^ xRes.mantissa());
		xstdlib::pbin(tDiff);
	}
	std::cout << ldRes << " " << xRes.unload() << " " << std::log2(ldErr) << " (" << ldErr << ")";
	return bSuccess;
}

bool log2_test(long double & i_LHO, bool & o_bTest_Valid)
{
	long double ldAbs_LHO = std::fabs(i_LHO);
	long double ldRes = std::log2(ldAbs_LHO);
	expdouble xRes = expdouble(ldAbs_LHO).log2();
	long double ldErr = std::fabs(1.0 - xRes.unload() / ldRes);
	o_bTest_Valid = !std::isinf(ldRes) && !std::isnan(ldRes);
	bool bSuccess = (ldErr == 0.0 || (std::log2(ldErr) < -62.0)) && o_bTest_Valid;
	if (!bSuccess && o_bTest_Valid)
	{
		std::cout << std::endl;
		uint64_t * tPtr = (uint64_t *)&ldRes;
		uint64_t tDiff = (tPtr[0] ^ xRes.mantissa());
		xstdlib::pbin(tDiff);
	}
	std::cout << ldRes << " " << xRes.unload() << " " << std::log2(ldErr) << " (" << ldErr << ")";
	return bSuccess;
}

typedef bool binarytestfunc(long double & i_LHO, long double & i_RHO, bool &o_bTest_Valid);
typedef bool unarytestfunc(long double & i_LHO, bool &o_bTest_Valid);

bool Perform_Test(binarytestfunc & func, long double & i_LHO, long double & i_RHO, const std::string & i_szTest_Description, bool &o_bValid)
{
	bool bValid;
	std::cout << std::scientific;
	std::cout << i_szTest_Description << " (" << i_LHO << ", " << i_RHO << ") ~ ";
	bool bSuccess = func(i_LHO,i_RHO,bValid);
	if (bValid)
	{
		bSuccess &= func(i_RHO,i_LHO,bValid);
		if (!bValid)
			std::cout << " : " << xconsole::foreground_yellow << "INVALID" << xconsole::reset;
		else if (bSuccess)
			std::cout << " : " << xconsole::foreground_green << "PASS" << xconsole::reset;
		else
			std::cout << " : " << xconsole::foreground_red << "FAIL" << xconsole::reset;
	}
	else
		std::cout << " : " << xconsole::foreground_yellow << "INVALID" << xconsole::reset;
	std::cout << std::endl;
	o_bValid = bValid;
	return bSuccess;
}

bool Perform_Test(unarytestfunc & func, long double & i_LHO, const std::string & i_szTest_Description, bool &o_bValid)
{
	bool bValid;
	std::cout << std::scientific;
	std::cout << i_szTest_Description << " (" << i_LHO << ") ~ ";
	bool bSuccess = func(i_LHO,bValid);
	if (!bValid)
		std::cout << " : " << xconsole::foreground_yellow << "INVALID" << xconsole::reset;
	else if (bSuccess)
		std::cout << " : " << xconsole::foreground_green << "PASS" << xconsole::reset;
	else
		std::cout << " : " << xconsole::foreground_red << "FAIL" << xconsole::reset;
	std::cout << std::endl;
	o_bValid = bValid;
	return bSuccess;
}

int main(int iA, char * lpcC[])
{
	bool bSuccess, bValid;
	long double ldSet[5] = {ld_load_rand(),ld_load_rand(),ld_load_rand(),ld_load_rand(),ld_load_rand()};
	long double ldExp_Safe_Set[5] = {ld_load_rand_exp(),ld_load_rand_exp(),ld_load_rand_exp(),ld_load_rand_exp(),ld_load_rand_exp()};
	size_t uiValid_Test_Count[5] = {0,0,0};
	size_t uiSuccess_Test_Count[5] = {0,0,0};
	for (unsigned int uiI = 0; uiI < 5; uiI++)
	{
		for (unsigned int uiJ = 0; uiJ < 5; uiJ++)
		{
			bSuccess = Perform_Test(mult_test,ldSet[uiI],ldSet[uiJ],std::string("a * b"),bValid);
			if (bValid)
				uiValid_Test_Count[0]++;
			if (bSuccess)
				uiSuccess_Test_Count[0]++;
			bSuccess = Perform_Test(mult_test,ldExp_Safe_Set[uiI],ldExp_Safe_Set[uiJ],std::string("a * b"),bValid);
			if (bValid)
				uiValid_Test_Count[0]++;
			if (bSuccess)
				uiSuccess_Test_Count[0]++;
		}
	}
	for (unsigned int uiI = 0; uiI < 5; uiI++)
	{
		for (unsigned int uiJ = 0; uiJ < 5; uiJ++)
		{
			bSuccess = Perform_Test(add_test,ldSet[uiI],ldSet[uiJ],std::string("a + b"),bValid);
			if (bValid)
				uiValid_Test_Count[1]++;
			if (bSuccess)
				uiSuccess_Test_Count[1]++;
			bSuccess = Perform_Test(add_test,ldExp_Safe_Set[uiI],ldExp_Safe_Set[uiJ],std::string("a + b"),bValid);
			if (bValid)
				uiValid_Test_Count[1]++;
			if (bSuccess)
				uiSuccess_Test_Count[1]++;
		}
	}
	for (unsigned int uiI = 0; uiI < 5; uiI++)
	{
		bSuccess = Perform_Test(exp_test,ldExp_Safe_Set[uiI],std::string("exp(a)"),bValid);
		if (bValid)
			uiValid_Test_Count[2]++;
		if (bSuccess)
			uiSuccess_Test_Count[2]++;
	}
	for (unsigned int uiI = 0; uiI < 5; uiI++)
	{
		bSuccess = Perform_Test(exp2_test,ldExp_Safe_Set[uiI],std::string("exp2(a)"),bValid);
		if (bValid)
			uiValid_Test_Count[3]++;
		if (bSuccess)
			uiSuccess_Test_Count[3]++;
	}
	for (unsigned int uiI = 0; uiI < 5; uiI++)
	{
		bSuccess = Perform_Test(log2_test,ldSet[uiI],std::string("log2(a)"),bValid);
		if (bValid)
			uiValid_Test_Count[4]++;
		if (bSuccess)
			uiSuccess_Test_Count[4]++;
		bSuccess = Perform_Test(log2_test,ldExp_Safe_Set[uiI],std::string("log2(a)"),bValid);
		if (bValid)
			uiValid_Test_Count[4]++;
		if (bSuccess)
			uiSuccess_Test_Count[4]++;
	}
	std::cout << "Summary:" << std::endl;
	for (size_t tI = 0; tI < 5; tI++)
	{
		std::cout << "\t";
		switch (tI)
		{
		case 0:
			std::cout << "*";
			break;
		case 1:
			std::cout << "+";
			break;
		case 2:
			std::cout << "exp";
			break;
		case 3:
			std::cout << "exp2";
			break;
		case 4:
			std::cout << "log2";
			break;
		}
		std::cout << ": ";
		if (uiSuccess_Test_Count[tI] == uiValid_Test_Count[tI])
			std::cout << xconsole::foreground_green << "PASS" << xconsole::reset;
		else
			std::cout << xconsole::foreground_red << "FAIL" << xconsole::reset;

		std::cout << " " << uiSuccess_Test_Count[tI] << "/" << uiValid_Test_Count[tI] << std::endl;
	}
	return 0;

	//@@TODO: write all of this as a series of tests with PASS/FAIL information
	long double da = 33333333.3333333333333333333;
	long double db = 33333333.0;
	expdouble xxxx(2.2);
	std::cout << xxxx.intpart().unload() << std::endl;
	std::cout << xxxx.fracpart().unload() << std::endl;
	xxxx.load(da);
	std::cout << xxxx.intpart().unload() << std::endl;
	std::cout << xxxx.fracpart().unload() << std::endl;
	xxxx.load(db);
	std::cout << xxxx.intpart().unload() << std::endl;
	std::cout << xxxx.fracpart().unload() << std::endl;

	std::cout << "2.0 " << (expdouble(1.0).exp2()).unload() << std::endl;
	std::cout << "4.0 " << (expdouble(2.0).exp2()).unload() << std::endl;
	std::cout << "1.41 " << (expdouble(0.5).exp2()).unload() << std::endl;
	std::cout << std::exp2((long double)0.3) << " "  << (expdouble(0.3).exp2()).unload() << std::endl;
	std::cout << "0.5 " << (expdouble(-1.0).exp2()).unload() << std::endl;
	std::cout << std::exp2((long double)-1.5) << " " << (expdouble(-1.5).exp2()).unload() << std::endl;

	std::cout << (expdouble(1.0) / expdouble(3.0)).unload() << std::endl;
	std::cout << (expdouble(1.0) / expdouble(2.0)).unload() << std::endl;
	std::cout << (expdouble(4.4) / expdouble(2.0)).unload() << std::endl;
	std::cout << ((expdouble(1.0) / expdouble(3.0)) * expdouble(3.0)).unload() << std::endl;
	std::cout << ((expdouble(1.0) / expdouble(2.0)) * expdouble(2.0)).unload() << std::endl;
	std::cout << ((expdouble(4.4) / expdouble(2.0)) * expdouble(2.0)).unload() << std::endl;
	std::cout << 2.0 << ": " << expdouble(2.0) << std::endl;
	std::cout << 20.0 << ": " << expdouble(20.0) << std::endl;
	std::cout << 0.2 << ": " << expdouble(0.2) << std::endl;
	std::cout << 2.0e-40 << ": " << expdouble(2.0e-40) << std::endl;
	std::cout << std::scientific << 2.0e-40 << ": " << expdouble(2.0e-40) << std::fixed << std::endl;
//	return 0;

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
	xstdlib::pbin(chA);xstdlib::pbin(chB);xstdlib::pbin(chAMB);

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
	xstdlib::pbin(xdSmTwo.true_exponent());
	xstdlib::pbin(ldSmTwo);
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
		xstdlib::pbin(xdZero.mantissa());
		xstdlib::pbin(xdZero.true_exponent());
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

	expdouble xd0(0.0);
	expdouble xd1(1.0);
	expdouble xd2(2.0);
	expdouble xd3(3.0);
	expdouble xd4(4.0);
	expdouble xd5(5.0);
	expdouble xd6(6.0);
	expdouble xd7(7.0);
	expdouble xd8(8.0);
	expdouble xd9(9.0);

	std::cout << xd0.extract_digit() + '0';
	std::cout << xd1.extract_digit() + '0';
	std::cout << xd2.extract_digit() + '0';
	std::cout << xd3.extract_digit() + '0';
	std::cout << xd4.extract_digit() + '0';
	std::cout << xd5.extract_digit() + '0';
	std::cout << xd6.extract_digit() + '0';
	std::cout << xd7.extract_digit() + '0';
	std::cout << xd8.extract_digit() + '0';
	std::cout << xd9.extract_digit() + '0';
	std::cout << std::endl;


	return 0;
}
