//
#include <xstdlib.h>
#include <time.h>
#include <cstdio>
#include <vector>

class XTIMER
{
private:
	timespec m_tTime_Start;
	timespec m_tTime_Stop;
public:
	void Start(void)
	{
		clock_gettime(CLOCK_MONOTONIC_RAW,&m_tTime_Start);
	}
	void Stop(void)
	{
		clock_gettime(CLOCK_MONOTONIC_RAW,&m_tTime_Stop);
	}
	double	Get_Time_Seconds(void)
	{
		return (m_tTime_Stop.tv_sec - m_tTime_Start.tv_sec) + (m_tTime_Stop.tv_nsec - m_tTime_Start.tv_nsec) * 1.0e-9;
	}
};

unsigned int Get_Rand_Seed(void)
{
	// get a random seed
	unsigned int iSeed;
	FILE* urandom = fopen("/dev/urandom", "r");
	fread(&iSeed, sizeof(iSeed), 1, urandom);
	fclose(urandom);
	return iSeed;
}

class LCG
{
public:
	unsigned long long k_iRANDMAX;
	unsigned long long	m_iA;
	unsigned long long	m_iC;
	unsigned long long m_iSm1;
protected:
	void Set_A(unsigned long long i_iA) {m_iA = i_iA;}
	void Set_C(unsigned long long i_iC) {m_iC = i_iC;}
public:
	LCG(void)
	{
		m_iA = 0;
		m_iC = 0;
	}
	LCG(unsigned long long i_iA, unsigned long long i_iC, unsigned long long i_iM)
	{
		m_iA = i_iA;
		m_iC = i_iC;
		k_iRANDMAX = i_iM;
	}
	inline int Get_Rand_Max(void) {return (unsigned int)k_iRANDMAX;}
	inline unsigned int Seed(int i_iSeed) {m_iSm1 = i_iSeed % k_iRANDMAX; return (unsigned int)m_iSm1;}
	inline unsigned int Generate_Random_Number(void) { unsigned long long iS = (m_iA * m_iSm1 + m_iC) % k_iRANDMAX; m_iSm1 = iS; return (unsigned int)iS;}
	//operator int (void) {return m_iSm1;}
};

extern unsigned int XRAND_isqrt(unsigned int i_uiI);

bool Test_Prime(unsigned int i_uiI)
{
	return xTest_Prime(i_uiI);
}
std::vector<unsigned int> g_vPrimes;

bool Test_Prime_Fast(unsigned int i_uiI)
{
	bool bFail = false;
	unsigned int uiSqrtI = XRAND_isqrt(i_uiI);
	for (unsigned int uiJ = 0; uiJ < g_vPrimes.size() && g_vPrimes[uiJ] <= uiSqrtI && !bFail; uiJ++)
	{
		bFail = (i_uiI % g_vPrimes[uiJ]) == 0;
	}
	return !bFail;
}
void Init_Primes(void)
{
	g_vPrimes.push_back(2); // trivially add 2
//	for (unsigned int uiI = 3; uiI < (unsigned int)(-1); uiI+=2)
	for (unsigned int uiI = 3; uiI < (unsigned int)(0xffffff); uiI+=2)
//	for (unsigned int uiI = 3; uiI < 10000; uiI+=2)
	{
		bool bPrime = Test_Prime_Fast(uiI);
		if (bPrime)
			g_vPrimes.push_back(uiI);
	}	
}

class MLCG
{
public:
	unsigned long long k_iRANDMAX;
	bool		m_bM_prime;
	unsigned long long	m_iA;
	unsigned long long m_iSm1;
protected:
	void Set_A(unsigned long long i_iA) {m_iA = i_iA;}
public:
	MLCG(void)
	{
		m_iA = 0;
	}
	MLCG(unsigned long long i_iA, unsigned long long i_iM)
	{
		m_iA = i_iA;
		k_iRANDMAX = i_iM;
		m_bM_prime = Test_Prime(i_iM);
		if (!m_bM_prime)
			printf("Warning: non-prime %llu chosen as M for an MLCG\n",i_iM);
//		for (unsigned int uiI = 0; uiI < 
	}
	inline int Get_Rand_Max(void) {return (unsigned int)k_iRANDMAX;}
	inline unsigned int Seed(unsigned long long i_iSeed)
	{
		i_iSeed %= k_iRANDMAX;
		while (i_iSeed == 0) // MLCG must have a non-zero value for s
		{
			i_iSeed = Get_Rand_Seed() % k_iRANDMAX;
		}
		if (i_iSeed < 0)
			i_iSeed += k_iRANDMAX;
		if (!m_bM_prime && !Test_Prime(i_iSeed))
		{
			// the seed and m must be co-prime.  If this is not the case, reseed
			// do this by searching for the nearest prime number above the current value
			while (!Test_Prime(i_iSeed))
				i_iSeed++;
		}
		m_iSm1 = i_iSeed;
		return (unsigned int)m_iSm1;
	}
	inline unsigned int Generate_Random_Number(void) { unsigned long long iS = (m_iA * m_iSm1) % k_iRANDMAX; m_iSm1 = iS; return (unsigned int)iS;}
};
void Test_Basic(int i_iSeed)
{
	LCG	cLCG(1,3,7);
	int iCurr;
	int iSeed_Norm = i_iSeed % cLCG.Get_Rand_Max();
	if (iSeed_Norm < 0)
		iSeed_Norm += cLCG.Get_Rand_Max();
	unsigned long long llPeriod_Count =  0;
	cLCG.Seed(iSeed_Norm);
	timespec tTime_Last;
	clock_gettime(CLOCK_MONOTONIC_RAW,&tTime_Last);
	iSeed_Norm = cLCG.Generate_Random_Number();
	printf("%i\n",iSeed_Norm);
	//iCurr = iSeed_Norm - 1;
	do
	{
		llPeriod_Count++;
		iCurr = cLCG.Generate_Random_Number();
		printf("%i\n",iCurr);
	}
	while (iSeed_Norm != iCurr);
	timespec tTime_Curr;
	clock_gettime(CLOCK_MONOTONIC_RAW,&tTime_Curr);
	double	dTime = (tTime_Curr.tv_sec - tTime_Last.tv_sec) + (tTime_Curr.tv_nsec - tTime_Last.tv_nsec) * 1.0e-9;
	double	dIter_Time = dTime / llPeriod_Count;
	printf("Period %llu\n",llPeriod_Count);
	printf("Time per iteration = %.3e\n",dIter_Time);
}

typedef unsigned int (*func_seed)(unsigned int);
typedef unsigned int (*func_rand)(void);


void Test(unsigned int i_iSeed, func_seed fSeed, func_rand fRand, func_rand fMax, bool bZero_Allowed)
{
	printf("%u (%x)\n",fMax(),fMax());
	unsigned int uiMax_Period = -1;
	unsigned long long llMaxPeriod = uiMax_Period;
	unsigned int iCurr;
	unsigned int iSeed_Norm = i_iSeed % fMax();
	if (iSeed_Norm < 0)
		iSeed_Norm += fMax();
	unsigned long long llPeriod_Count =  0;
	unsigned long long llIters =  0;
	unsigned long long llLoop_Period_Count = 0;

	iSeed_Norm = fSeed(iSeed_Norm);
	printf("Using seed %u\n",iSeed_Norm);
	timespec tTime_Last;
	for (unsigned int uiI = 0; uiI < -1; uiI++) if (fRand() > fMax()) printf("\033[0;31mFAIL\033[0m\n");

	iSeed_Norm = fRand();
	printf("%u\n",iSeed_Norm);
	clock_gettime(CLOCK_MONOTONIC_RAW,&tTime_Last);
	//iCurr = iSeed_Norm - 1;
	do
	{
		llPeriod_Count++;
		llIters++;
		iCurr = fRand();
	}
	while ((bZero_Allowed || iCurr != 0) && iSeed_Norm != iCurr && llPeriod_Count < llMaxPeriod);
	iSeed_Norm = fRand();
	do
	{
		llIters++;
		llLoop_Period_Count++;
		iCurr = fRand();
	}
	while (iCurr != 0 && iSeed_Norm != iCurr && llLoop_Period_Count < llMaxPeriod);

	timespec tTime_Curr;
	clock_gettime(CLOCK_MONOTONIC_RAW,&tTime_Curr);
	double	dTime = (tTime_Curr.tv_sec - tTime_Last.tv_sec) + (tTime_Curr.tv_nsec - tTime_Last.tv_nsec) * 1.0e-9;
	double	dIter_Time = dTime / llIters;
	printf("Period %llu\n",llPeriod_Count);
	printf("Time per iteration = %.3e\n",dIter_Time);
	fflush(stdout);

	printf("Loop test....");
	if (llLoop_Period_Count < llPeriod_Count)
		printf("Loop period %llu.\n",llLoop_Period_Count);
	else
		printf("No loop.\n");
}

LCG	g_cLCG(2147483629,2147483587,(1<<31) - 1); // Rtl uniform
unsigned int SeedLCG(unsigned int i_iSeed)
{
	return g_cLCG.Seed(i_iSeed);
}
unsigned int RandLCG(void)
{
	return g_cLCG.Generate_Random_Number();
}
unsigned int RandMaxLCG(void)
{
	return g_cLCG.Get_Rand_Max();
}
void Test_LCG(unsigned int i_iSeed, bool bZero_Allowed)
{
	Test(i_iSeed,SeedLCG,RandLCG,RandMaxLCG,bZero_Allowed);
}

unsigned int isqrt(unsigned int i_uiI)
{
	unsigned int uiX = 2;
	if (i_uiI == 0)
		uiX = 0;
	else if (i_uiI == 1)
		uiX = 1;
	else
	{
		unsigned int uiX_l;
		do
		{
			uiX_l = uiX;
			uiX = (uiX + i_uiI / uiX) >> 1;
		}
		while (uiX != uiX_l);
	}
	return uiX;
}


int main(int i_iArg_Count, const char * i_lpszArg_Values[])
{
	XTIMER	cTimer;
	//this may not be the best way to perform the test in terms of repeatability, but 
	int iSeed = (int)Get_Rand_Seed();
//	int iSeed = 0xb12f213a;//Get_Rand_Seed();
	printf("Seed = %i\n",iSeed);
//	cTimer.Start();
//	Init_Primes();
//	cTimer.Stop();
//	printf("Prime initialization time = %.2f sec.\n",cTimer.Get_Time_Seconds());
	printf("Test algorithm test\n");
	fflush(stdout);
	Test_Basic(iSeed);
	fflush(stdout);
	printf("PM test\n");
	Test_LCG(iSeed,true);
	fflush(stdout);
	printf("Park & Miller\n");
	xrand_Set_Type(XRT_PM);
	Test(iSeed,xsrand,xrand,xrandmax,false);
	fflush(stdout);
	printf("L'Eculyer\n");
	xrand_Set_Type(XRT_LE);
	Test(iSeed,xsrand,xrand,xrandmax,true);
	fflush(stdout);
	printf("Knuth Algorithm B\n");
	xrand_Set_Type(XRT_K);
	Test(iSeed,xsrand,xrand,xrandmax,true);
	fflush(stdout);
	printf("Knuth Algorithm M\n");
	xrand_Set_Type(XRT_KM);
	Test(iSeed,xsrand,xrand,xrandmax,true);
	fflush(stdout);

	unsigned int uiM = 0, uiA = 0, uiC = 0;
	for (unsigned int uiI = -1; uiI > 0 && uiC == 0; uiI--)
	{
		if (Test_Prime(uiI))
		{
			if (uiM == 0)
				uiM = uiI;
			else if (uiA == 0)
				uiA = uiI;
			else if (uiC == 0)
				uiC = uiI;
			uiI >>= 1;
		}
	}
	g_cLCG = LCG(uiA,uiC,uiM);
	printf("Generated test (%u s + %u) %% %u\n",uiA,uiC,uiM);
	Test_LCG(iSeed,true);

//	for (unsigned int uiI = 2; uiI < 1024; uiI++)
//	{
//		printf("%i : %i\n",uiI,XRAND_isqrt(uiI));
//		bool bTest = Test_Prime(uiI);
//		if (bTest)
//		printf("%i\n",uiI);
//	}
//	for (unsigned int uiI = 0; uiI < g_vPrimes.size(); uiI++)
//	{
//		printf("%u\n",g_vPrimes[uiI]);
//	}
//	printf("Prime count = %u\n",g_vPrimes.size());
//	Test(iSeed);
	
//	xrand_Set_Type(XRT_LE);
//	printf("L'Eculyer\n");
//	Test(iSeed);
	
//	xrand_Set_Type(XRT_KM);
//	printf("Knuth Algorithm M\n");
//	Test(iSeed);

//	xrand_Set_Type(XRT_K);
//	printf("Knuth Algorithm B\n");
//	Test(iSeed);
}
