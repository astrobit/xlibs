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
	unsigned int k_iRANDMAX;
	unsigned int	m_iA;
	unsigned int	m_iC;
	unsigned int m_iSm1;
protected:
	void Set_A(unsigned int i_iA) {m_iA = i_iA;}
	void Set_C(unsigned int i_iC) {m_iC = i_iC;}
public:
	LCG(void)
	{
		m_iA = 0;
		m_iC = 0;
	}
	LCG(unsigned int i_iA, unsigned int i_iC, unsigned int i_iM)
	{
		m_iA = i_iA;
		m_iC = i_iC;
		k_iRANDMAX = i_iM;
	}
	inline int Get_Rand_Max(void) {return k_iRANDMAX;}
	inline void Seed(int i_iSeed) {m_iSm1 = i_iSeed % k_iRANDMAX;}
	inline unsigned int Generate_Random_Number(void) { unsigned int iS = (m_iA * m_iSm1 + m_iC) % k_iRANDMAX; m_iSm1 = iS; return iS;}
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
	unsigned int k_iRANDMAX;
	bool		m_bM_prime;
	unsigned int	m_iA;
	unsigned int m_iSm1;
protected:
	void Set_A(unsigned int i_iA) {m_iA = i_iA;}
public:
	MLCG(void)
	{
		m_iA = 0;
	}
	MLCG(unsigned int i_iA, unsigned int i_iM)
	{
		m_iA = i_iA;
		k_iRANDMAX = i_iM;
		m_bM_prime = Test_Prime(i_iM);
		if (!m_bM_prime)
			printf("Warning: non-prime %u chosen as M for an MLCG\n",i_iM);
//		for (unsigned int uiI = 0; uiI < 
	}
	inline int Get_Rand_Max(void) {return k_iRANDMAX;}
	inline unsigned int Seed(unsigned int i_iSeed)
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
		return m_iSm1;
	}
	inline unsigned int Generate_Random_Number(void) { unsigned int iS = (m_iA * m_iSm1) % k_iRANDMAX; m_iSm1 = iS; return iS;}
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
void Test_PM(int i_iSeed)
{
	MLCG	cLCG(16807,2147483647);
	int iCurr;
	int iSeed_Norm = i_iSeed % cLCG.Get_Rand_Max();
	if (iSeed_Norm < 0)
		iSeed_Norm += cLCG.Get_Rand_Max();
	unsigned long long llPeriod_Count =  0;
	iSeed_Norm = cLCG.Seed(iSeed_Norm);
	printf("Using seed %u\n",iSeed_Norm);
	timespec tTime_Last;
	clock_gettime(CLOCK_MONOTONIC_RAW,&tTime_Last);
	printf("%i\n",iSeed_Norm);
	//iCurr = iSeed_Norm - 1;
	do
	{
		llPeriod_Count++;
		iCurr = cLCG.Generate_Random_Number();
//		printf("%i\n",iCurr);
		if (iCurr == 0)
			printf("\033[0;31mFAIL\033[0m\n");
	}
	while (iCurr != 0 && iSeed_Norm != iCurr);
	timespec tTime_Curr;
	clock_gettime(CLOCK_MONOTONIC_RAW,&tTime_Curr);
	double	dTime = (tTime_Curr.tv_sec - tTime_Last.tv_sec) + (tTime_Curr.tv_nsec - tTime_Last.tv_nsec) * 1.0e-9;
	double	dIter_Time = dTime / llPeriod_Count;
	printf("Period %llu\n",llPeriod_Count);
	printf("Time per iteration = %.3e\n",dIter_Time);
}

void Test(int i_iSeed)
{
	int iCurr;
	unsigned long long llPeriod_Count =  0;
	int iSeed_Norm = i_iSeed % xrandmax();
	if (iSeed_Norm < 0)
		iSeed_Norm += xrandmax();
	iSeed_Norm = xsrand(iSeed_Norm);
	timespec tTime_Last;
	clock_gettime(CLOCK_MONOTONIC_RAW,&tTime_Last);
	do
	{
		llPeriod_Count++;
		iCurr = xrand();
		if (llPeriod_Count % 1000000 == 0)
			printf("%llu %i\n",llPeriod_Count/1000000,iCurr);
	} while (iSeed_Norm != iCurr);

	timespec tTime_Curr;
	clock_gettime(CLOCK_MONOTONIC_RAW,&tTime_Curr);
	double	dTime = (tTime_Curr.tv_sec - tTime_Last.tv_sec) + (tTime_Curr.tv_nsec - tTime_Last.tv_nsec) * 1.0e-9;
	double	dIter_Time = dTime / llPeriod_Count;
	printf("Period %llu\n",llPeriod_Count);
	printf("Time per iteration = %.3e\n",dIter_Time);
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
	printf("PM test\n");
	Test_PM(iSeed);
	printf("Park & Miller\n");
	xrand_Set_Type(XRT_PM);
	unsigned int uiFails = 0;
	for (unsigned int uiI = 0; uiI < 1000; uiI++)
	{
		unsigned int uiX = Get_Rand_Seed();
		unsigned int uiiSqX = isqrt(uiX);
		unsigned int uiSqiSqX = uiiSqX * uiiSqX;
		unsigned int uiSqiSqXm1 = (uiiSqX - 1) * (uiiSqX - 1);
		unsigned int uiSqiSqXp1 = (uiiSqX + 1) * (uiiSqX + 1);
		unsigned int uiDiff = uiSqiSqX - uiX;
		unsigned int uiDiffm1 = uiSqiSqXm1 - uiX;
		unsigned int uiDiffp1 = uiSqiSqXp1 - uiX;
		printf("%u: %u %i %i %i",uiX,uiiSqX,uiDiffm1,uiDiff,uiDiffp1);
		if ((int)uiDiff > 0) {uiFails++; printf("...\033[0;31mFAIL\033[0m");}
		printf("\n");
	}
	printf("%i failures\n",uiFails);
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