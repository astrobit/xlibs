#include <xstdlib.h>
#include <math.h>
#include <stdio.h>
#include <ctype.h>
#include <string>
#include <unistd.h>


std::string xconsole::foreground_black		= "\033[30m";
std::string xconsole::foreground_red		= "\033[31m";
std::string xconsole::foreground_green		= "\033[32m";
std::string xconsole::foreground_blue		= "\033[34m";
std::string xconsole::foreground_yellow		= "\033[33m";
std::string xconsole::foreground_cyan 		= "\033[36m";
std::string xconsole::foreground_magenta	= "\033[35m";
std::string xconsole::foreground_gray		= "\033[37m";
std::string xconsole::foreground_grey		= "\033[37m";
std::string xconsole::foreground_white		= "\033[37m";

std::string xconsole::background_black		= "\033[40m";
std::string xconsole::background_red		= "\033[41m";
std::string xconsole::background_green		= "\033[42m";
std::string xconsole::background_blue		= "\033[44m";
std::string xconsole::background_yellow		= "\033[43m";
std::string xconsole::background_cyan 		= "\033[46m";
std::string xconsole::background_magenta	= "\033[45m";
std::string xconsole::background_gray		= "\033[47m";
std::string xconsole::background_grey		= "\033[47m";
std::string xconsole::background_white		= "\033[47m";

std::string xconsole::reset					= "\033[0m";
std::string xconsole::bold					= "\033[1m";
std::string xconsole::faint					= "\033[2m";
std::string xconsole::italic				= "\033[3m";
std::string xconsole::underline				= "\033[4m";
std::string xconsole::blink_slow			= "\033[5m";
std::string xconsole::blink_fast			= "\033[6m";
std::string xconsole::invert				= "\033[7m";


unsigned int XRAND_get_sys_rand(void)
{
	unsigned int uiSeed;
	FILE* urandom = fopen("/dev/urandom", "r");
	fread(&uiSeed, sizeof(uiSeed), 1, urandom);
	fclose(urandom);
	return uiSeed;
}

unsigned int XRAND_isqrt(unsigned int i_uiI)
{
	unsigned int uiX = 2;
	if (i_uiI == 0)
		uiX = 0;
	else if (i_uiI == 1)
		uiX = 1;
	else
	{
		unsigned int uiX_l = -1;
		unsigned int uiX_l2;
		do
		{
			uiX_l2 = uiX_l;
			uiX_l = uiX;
			uiX = (uiX + i_uiI / uiX) >> 1;
		}
		while (uiX != uiX_l && uiX != uiX_l2);
		// if i+1 is a perfect square, the sequence doesn't converge; need to check for repeating sequence (isqrt(n) .. isqrt(n+1) ... isqrt(n))
		if (uiX == uiX_l2 && uiX_l < uiX)
			uiX = uiX_l;
	}
	return uiX;
}

bool xTest_Prime(unsigned int i_uiI)
{
	// slow but effective prime number test
	// returns true if the input is prime
	// 1 is NOT a prime number
	bool bPrime = (i_uiI > 2 && (i_uiI & 0x01));
	if (i_uiI == 2)
		bPrime = true;
	else
	{
		unsigned int iSqI = XRAND_isqrt(i_uiI);
		for (unsigned int uiJ = 3; uiJ <= iSqI && bPrime; uiJ+=2)
		{
			bPrime = ((i_uiI % uiJ) != 0);
		}
	}
	return bPrime;
}
// Template class for random number generators
class XRAND
{
private:
	unsigned long long k_iRANDMAX;
	double	k_dSCALAR;
protected:
	void SetRandMax(unsigned int i_iRand_Max)
	{
		k_iRANDMAX = i_iRand_Max;
		k_dSCALAR = 1.0 / i_iRand_Max;
	}

public:
	XRAND(void)
	{
		k_iRANDMAX = 0;
		k_dSCALAR = 0.0;
	}
	XRAND(unsigned int i_iRand_Max)
	{
		SetRandMax(i_iRand_Max);
	}
	virtual ~XRAND(void) {;}
	virtual unsigned int Seed(unsigned int iSeed)=0;
	virtual unsigned int Generate_Random_Number(void)=0;
	unsigned int GetRandInt(void) {return Generate_Random_Number();}
//	virtual double GetRandDouble(void)=0;
//	virtual double GetRandDoubleNegative(void)=0;
	inline unsigned int GetRandMax(void) const
	{
		return (unsigned int)k_iRANDMAX;
	}
	inline double GetRandDouble(void)
	{
		return Generate_Random_Number() * k_dSCALAR; // 0 ... ~0.99..
	}
	inline double GetRandDoubleNegative(void)
	{
		return (Generate_Random_Number() * k_dSCALAR - 0.5) * 2.0; // -0.5 ... 0.499...
	}
};


// Linear congruential generator
// This is a generic implementation of an LCG:
// a random number generator of form
// s_i = (a s_{i-1} + d) modulo m
class XRAND_LCG : public XRAND
{
private:
	unsigned long long	m_iA;
	unsigned long long m_iD;
	unsigned long long m_iSm1;
protected:
	void Set_A(unsigned int i_iA) {m_iA = i_iA;}
	void Set_D(unsigned int i_iD) {m_iD = i_iD;}
public:
	XRAND_LCG(void)
	{
		m_iA = 0;
		m_iD = 0;
	}
	XRAND_LCG(unsigned int i_iA, unsigned int i_iD, unsigned int i_iM) : XRAND(i_iM)
	{
		m_iA = i_iA;
		m_iD = i_iD;
	}
	inline unsigned int Get_A(void) const {return m_iA;}
	inline unsigned int Get_D(void) const {return m_iD;}
	inline unsigned int Seed(unsigned int i_iSeed) {m_iSm1 = i_iSeed; return (unsigned int)m_iSm1;}
	inline unsigned int Generate_Random_Number(void) { long long iS = (m_iA * m_iSm1 + m_iD) % XRAND::GetRandMax(); m_iSm1 = iS; return (unsigned int)iS;}
	inline operator unsigned int(void) {return m_iSm1;}
};
// Multiplicitave linear congruential generator
// This is identical to the LCG above, but with d = 0
// This is the more common implementation of an LCG

class XRAND_MLCG : public XRAND
{
private:
	unsigned long long	m_iA;
	unsigned long long m_iSm1;
	bool m_bM_prime;
protected:
	void Set_A(unsigned int i_iA) {m_iA = i_iA;}
public:
	XRAND_MLCG(void)
	{
		m_iA = 0;
		m_iSm1 = 1;
		m_bM_prime = false;
	}
	XRAND_MLCG(unsigned int i_iA, unsigned int i_iM) : XRAND(i_iM)
	{
		m_iA = i_iA;
		m_bM_prime = xTest_Prime(i_iM);
		if (!m_bM_prime)
			fprintf(stderr,"\033[0;31mWarning\033[0m: non-prime %u chosen as M for an MLCG\n",i_iM);

	}
	inline unsigned int Seed(unsigned int i_iSeed)
	{
		i_iSeed %= XRAND::GetRandMax();
		while (i_iSeed == 0) // MLCG must have a non-zero value for s
		{
			i_iSeed = XRAND_get_sys_rand() % XRAND::GetRandMax();
		}
		if (i_iSeed < 0)
			i_iSeed += XRAND::GetRandMax();
		if (!m_bM_prime && !xTest_Prime(i_iSeed))
		{
			// the seed and m must be co-prime.  If this is not the case, reseed
			// do this by searching for the nearest prime number above the current value
			while (!xTest_Prime(i_iSeed))
				i_iSeed++;
		}
		m_iSm1 = i_iSeed;
		return (unsigned int)m_iSm1;
	}
	inline unsigned int Generate_Random_Number(void) { int iS = (m_iA * m_iSm1) % XRAND::GetRandMax(); m_iSm1 = iS; return iS;}
	inline operator unsigned int(void) {return m_iSm1;}
};

// Park & Miller random number generator
// Park, Stephen K. and Miller, Keith W. 1988 Communications of the ACM 31:1192
// simple MLCG with suggested a and m.
class XRAND_Park_Miller : public XRAND_MLCG
{
public:
	XRAND_Park_Miller(void) : XRAND_MLCG(16807,2147483647)
	{
//		XRAND::SetRandMax(2147483647);
//		XRAND_MLCG::Set_A(16807);
//		Seed(0x62094986);
	}
};

// From L'Ecuyer, Pierre 1988  Communications of the ACM 31:742
// The L'Ecuyer prng uses a linear combination of independant MCLG;  
// this has the advantage of increasing the period to be the least common
// multiple of periods of the constituent MCLGs
class XRAND_LEcuyer : public XRAND
{
private:
	XRAND_MLCG	m_cMLCG1;
	XRAND_MLCG	m_cMLCG2;

public:
	XRAND_LEcuyer(void) : m_cMLCG1(40014,2147483563), m_cMLCG2(40692,2147483399)
	{
		SetRandMax(m_cMLCG1.GetRandMax() - 1);
	}

	unsigned int Seed(unsigned int i_iSeed)
	{
		unsigned int iSa = i_iSeed >> 1;
		unsigned int iSb = i_iSeed - iSa;
		m_cMLCG1.Seed(iSa);
		m_cMLCG2.Seed(iSb);
		return (m_cMLCG1 - m_cMLCG2);
	}
	unsigned int Generate_Random_Number(void)
	{
		// See source, eq. 12
		int iZ = m_cMLCG1.Generate_Random_Number() - m_cMLCG2.Generate_Random_Number();
		iZ %= XRAND::GetRandMax();
		return iZ;
	}
};

// From Knuth, Donald E. 1997 The Art of Computer Programming, vol. 2 (p33)
// Algorithm M; we use a specific implementation with k = 64, and using the 
// suggested MLCGs of L'Eculyer (above)
class XRAND_Knuth_M : public XRAND
{
private:
	XRAND_MLCG	m_cMLCG1;
	XRAND_MLCG	m_cMLCG2;

	unsigned int	m_iDataset[64];

	void Generate_Table(void)
	{
		for (unsigned int uiI = 0; uiI < 64; uiI++)
			m_iDataset[uiI] = m_cMLCG1.Generate_Random_Number();
	}
public:
	XRAND_Knuth_M(void) : m_cMLCG1(40014,2147483563), m_cMLCG2(40692,2147483399)
	{
		SetRandMax(m_cMLCG1.GetRandMax());
		Generate_Table();
	}

	unsigned int  Seed(unsigned int i_iSeed)
	{
		unsigned int iSa = i_iSeed >> 1;
		unsigned int iSb = i_iSeed - iSa;
		m_cMLCG1.Seed(iSa);
		m_cMLCG2.Seed(iSb);
		Generate_Table();
		unsigned int iY = m_cMLCG2;
		unsigned int iJ = (64 * iY) / m_cMLCG2.GetRandMax();
		unsigned int iRet = m_iDataset[iJ];
		return iRet;
	}

	unsigned int Generate_Random_Number(void)
	{
		unsigned int iY = m_cMLCG2.Generate_Random_Number();
		unsigned int iJ = (64 * iY) / m_cMLCG2.GetRandMax();
		unsigned int iRet = m_iDataset[iJ];
		m_iDataset[iJ] = m_cMLCG1.Generate_Random_Number();
		return iRet;
	}
};

// From Knuth, Donald E. 1997 The Art of Computer Programming, vol. 2 (p34)
// Algorithm B; we use a specific implementation with k = 64, and using the 
// suggested MLCGs of Park & Miller (above)
class XRAND_Knuth_B : public XRAND
{
private:
	XRAND_Park_Miller	m_cMLCG_X;

	unsigned int	m_iDataset[64];
	unsigned int m_iY;

	void Generate_Table(void)
	{
		for (unsigned int uiI = 0; uiI < 64; uiI++)
			m_iDataset[uiI] = m_cMLCG_X.Generate_Random_Number();
		m_iY = m_cMLCG_X.Generate_Random_Number();
	}

public:
	XRAND_Knuth_B(void)
	{
		SetRandMax(m_cMLCG_X.GetRandMax());
		Generate_Table();
	}

	unsigned int Seed(unsigned int i_iSeed)
	{
		m_cMLCG_X.Seed(i_iSeed);
		Generate_Table();
		int iJ = (64 * m_iY) / m_cMLCG_X.GetRandMax();
		m_iY = m_iDataset[iJ];
		m_iDataset[iJ] = m_cMLCG_X.Generate_Random_Number();
		return m_iY;
	}

	unsigned int Generate_Random_Number(void)
	{
		int iJ = (64 * m_iY) / m_cMLCG_X.GetRandMax();
		m_iY = m_iDataset[iJ];
		m_iDataset[iJ] = m_cMLCG_X.Generate_Random_Number();
		return m_iY;
	}
};



XRAND_TYPE	g_eXRand_Selected_Type = XRT_KM;

XRAND_Park_Miller 	g_xrand_Park_Miller;
XRAND_Knuth_M 		g_xrand_Knuth_M;
XRAND_Knuth_B 		g_xrand_Knuth_B;
XRAND_LEcuyer 		g_xrand_LEcuyer;
XRAND *				g_lpxrand_Selected = &g_xrand_Knuth_M;

void		xrand_Set_Type(XRAND_TYPE i_eType) // select random number generator
{
	switch (i_eType)
	{
	case XRT_PM:
	default:
		g_eXRand_Selected_Type = XRT_PM;
		g_lpxrand_Selected = &g_xrand_Park_Miller;
		break;
	case XRT_LE:
		g_eXRand_Selected_Type = XRT_LE;
		g_lpxrand_Selected = &g_xrand_LEcuyer;
		break;
	case XRT_KM:
		g_eXRand_Selected_Type = XRT_KM;
		g_lpxrand_Selected = &g_xrand_Knuth_M;
		break;
	case XRT_K:
		g_eXRand_Selected_Type = XRT_K;
		g_lpxrand_Selected = &g_xrand_Knuth_B;
		break;
	}

}
XRAND_TYPE	xrand_Get_Type(void) // get random number generator type
{
	return g_eXRand_Selected_Type;
}

unsigned int			xrandmax(void)
{
	int iRet = 0;
	if (g_lpxrand_Selected)
		iRet = g_lpxrand_Selected->GetRandMax();
	return iRet;

}

unsigned int	xsrand(unsigned int i_iSeed)
{
	unsigned int uiRet = i_iSeed;
	if (g_lpxrand_Selected)
		uiRet = g_lpxrand_Selected->Seed(i_iSeed);
	return uiRet;
}

unsigned int	xrsrand(void)
{
	unsigned int uiSeed;
        size_t iJunk;
	unsigned int uiRet;
	FILE* urandom = fopen("/dev/urandom", "r");
	iJunk = fread(&uiSeed, sizeof(uiSeed), 1, urandom);
	fclose(urandom);
	if (g_lpxrand_Selected)
		uiRet = g_lpxrand_Selected->Seed(uiSeed);
	return uiRet;
}

unsigned int	xrand(void) // generate random integer number.  Depending on the generator the number may be allowed to be negative.
{
	int iRet = -1;
	if (g_lpxrand_Selected)
		iRet = g_lpxrand_Selected->GetRandInt();
	return iRet;
}

double xrand_d(void) // generate number between 0 and 1
{
	double dRet = -1.0;
	if (g_lpxrand_Selected)
		dRet = g_lpxrand_Selected->GetRandDouble();
	return dRet;

}
double xrand_dm(void) // genearte number between -1 and 1
{
	double dRet = -1.0;
	if (g_lpxrand_Selected)
		dRet = g_lpxrand_Selected->GetRandDoubleNegative();
	return dRet;
}
/*double	xrand_gauss(const double & i_dMean, const double &i_dStDev,unsigned int i_uiN)
{
	double dSum = 0.0;
	for (unsigned int uiJ = 0; uiJ < i_uiN; uiJ++)
		dSum += xrand_d();
	double dT = (dSum / i_uiN - 0.5) * sqrt(12. * i_uiN) * i_dStDev + i_dMean;
	return dT;
}*/
// General 2 random numbers with a gaussian distribution using a polar Box-Mueller method
void	xrand_gauss_bm(double & o_dY1,double & o_dY2, const double &i_dSigma_Y1, const double &i_dSigma_Y2)
{
	double dX1,dX2,dW;

	do
	{
		dX1 = 2.0 * xrand_d() - 1.0;
		dX2 = 2.0 * xrand_d() - 1.0;
		dW = dX1 * dX1 + dX2 * dX2;
	} while ( dW >= 1.0 );

	dW = sqrt( (-2.0 * log( dW ) ) / dW );
	o_dY1 = dX1 * dW * i_dSigma_Y1;
	o_dY2 = dX2 * dW * i_dSigma_Y2;
	
/*	double dX1 = xrand_d();
	double dX2 = xrand_d();
	o_dY1 = sqrt(-2.0 * log(dX1)) * cos(dX2 * 2.0 * 3.1415926535897932384626433832795) * i_dSigma_Y1;
	o_dY2 = sqrt(-2.0 * log(dX1)) * sin(dX2 * 2.0 * 3.1415926535897932384626433832795) * i_dSigma_Y2;*/
}
bool	g_bXrandGauss_Hold = false;
double	g_dXrandGauss_Data = 0.0;
double	xrand_gauss(const double & i_dMean, const double &i_dStDev)
{
	double	dRet = g_dXrandGauss_Data;
	if (!g_bXrandGauss_Hold)
	{
		xrand_gauss_bm(dRet,g_dXrandGauss_Data);
	}
	g_bXrandGauss_Hold = !g_bXrandGauss_Hold;
	dRet *= i_dStDev;
	dRet += i_dMean;
	return dRet;
}

bool xIsANumber(const char * lpString)
{
	return (lpString && lpString[0] != 0 && ((lpString[0] >= '0' && lpString[0] <= '9') || lpString[0] == '-' || lpString[0] == '.' || lpString[0] == 'e' || lpString[0] == 'E' || lpString[0] == '+'));
}
bool xIsWhitespace(const char * lpString)
{
	return (lpString && lpString[0] != 0 && (lpString[0] ==' ' || lpString[0] == '\t'));
}

char * xPassWhitespace(const char * lpString)
{
	while (xIsWhitespace(lpString))
		lpString++;
	return (char*)lpString;
}
char * xPassNumber(const char * lpString)
{
	while (xIsANumber(lpString))
		lpString++;
	return (char*)lpString;
}
char * xPassSeparator(const char * lpString)
{
	while (lpString && lpString[0] != 0 && 
		(lpString[0] == ' ' || lpString[0] == '\t' || lpString[0] == ','))
		lpString++;
	return (char*)lpString;
}
char * xPassSeparator(const char * lpString, char chSeparator)
{
	while (lpString && lpString[0] != 0 && 
		(lpString[0] == chSeparator))
		lpString++;
	return (char*)lpString;
}
char * xPassString(const char * lpString,char chString_Identifier)
{
	if (lpString[0] == chString_Identifier)
	{
		lpString++;
		while (lpString && lpString[0] != 0 && 
			(lpString[0] != chString_Identifier))
			lpString++;
		if (lpString[0] == chString_Identifier)
			lpString++;
	}
	return (char*)lpString;
}

unsigned long long XMin_ui64(unsigned int i_uiNum_Parameters,...)
{
	unsigned long long tMin;
	unsigned int uiI;
	va_list	lpvaArg_Ptr;
	va_start(lpvaArg_Ptr,i_uiNum_Parameters);
	tMin = va_arg(lpvaArg_Ptr,unsigned long long);
	for (uiI = 1; uiI < i_uiNum_Parameters; uiI++)
	{
		unsigned long long tX = va_arg(lpvaArg_Ptr,unsigned long long);
		if (tX < tMin)
			tMin = tX;
	}	va_end(lpvaArg_Ptr);
	return tMin;
}

long long XMin_i64(unsigned int i_uiNum_Parameters,...)
{
	long long tMin;
	unsigned int uiI;
	va_list	lpvaArg_Ptr;
	va_start(lpvaArg_Ptr,i_uiNum_Parameters);
	tMin = va_arg(lpvaArg_Ptr,long long);
	for (uiI = 1; uiI < i_uiNum_Parameters; uiI++)
	{
		long long tX = va_arg(lpvaArg_Ptr,long long);
		if (tX < tMin)
			tMin = tX;
	}	va_end(lpvaArg_Ptr);
	return tMin;
}


unsigned int XMin_uiv(unsigned int i_uiNum_Parameters,va_list	lpvaArg_Ptr)
{
	unsigned int tMin;
	unsigned int uiI;
	tMin = va_arg(lpvaArg_Ptr,unsigned int);
	for (uiI = 1; uiI < i_uiNum_Parameters; uiI++)
	{
		unsigned int tX = va_arg(lpvaArg_Ptr,unsigned int);
		if (tX < tMin)
			tMin = tX;
	}
	return tMin;
}

int XMin_iv(unsigned int i_uiNum_Parameters,va_list	lpvaArg_Ptr)
{
	int tMin;
	unsigned int uiI;
	tMin = va_arg(lpvaArg_Ptr,int);
	for (uiI = 1; uiI < i_uiNum_Parameters; uiI++)
	{
		int tX = va_arg(lpvaArg_Ptr,int);
		if (tX < tMin)
			tMin = tX;
	}
	return tMin;
}
double XMin_dv(unsigned int i_uiNum_Parameters,va_list	lpvaArg_Ptr)
{
	double tMin;
	unsigned int uiI;
	tMin = va_arg(lpvaArg_Ptr,double);
	for (uiI = 1; uiI < i_uiNum_Parameters; uiI++)
	{
		double tX = va_arg(lpvaArg_Ptr,double);
		if (tX < tMin)
			tMin = tX;
	}
	return tMin;
}

unsigned int XMin_ui(unsigned int i_uiNum_Parameters,...)
{
	unsigned int tMin;
	va_list	lpvaArg_Ptr;
	va_start(lpvaArg_Ptr,i_uiNum_Parameters);
	tMin = XMin_uiv(i_uiNum_Parameters,lpvaArg_Ptr);
	va_end(lpvaArg_Ptr);
	return tMin;
}

int XMin_i(unsigned int i_uiNum_Parameters,...)
{
	int tMin;
	va_list	lpvaArg_Ptr;
	va_start(lpvaArg_Ptr,i_uiNum_Parameters);
	tMin = XMin_iv(i_uiNum_Parameters,lpvaArg_Ptr);
	va_end(lpvaArg_Ptr);
	return tMin;
}

unsigned short XMin_us(unsigned int i_uiNum_Parameters,...)
{
	unsigned short tMin;
	va_list	lpvaArg_Ptr;
	va_start(lpvaArg_Ptr,i_uiNum_Parameters);
	tMin = XMin_uiv(i_uiNum_Parameters,lpvaArg_Ptr);
	va_end(lpvaArg_Ptr);
	return tMin;
}

short XMin_s(unsigned int i_uiNum_Parameters,...)
{
	short tMin;
	va_list	lpvaArg_Ptr;
	va_start(lpvaArg_Ptr,i_uiNum_Parameters);
	tMin = XMin_iv(i_uiNum_Parameters,lpvaArg_Ptr);
	va_end(lpvaArg_Ptr);
	return tMin;
}

unsigned char XMin_uc(unsigned int i_uiNum_Parameters,...)
{
	unsigned char tMin;
	va_list	lpvaArg_Ptr;
	va_start(lpvaArg_Ptr,i_uiNum_Parameters);
	tMin = XMin_uiv(i_uiNum_Parameters,lpvaArg_Ptr);
	va_end(lpvaArg_Ptr);
	return tMin;
}

char XMin_c(unsigned int i_uiNum_Parameters,...)
{
	char tMin;
	va_list	lpvaArg_Ptr;
	va_start(lpvaArg_Ptr,i_uiNum_Parameters);
	tMin = XMin_iv(i_uiNum_Parameters,lpvaArg_Ptr);
	va_end(lpvaArg_Ptr);
	return tMin;
}

double XMin_d(unsigned int i_uiNum_Parameters,...)
{
	double tMin;
	va_list	lpvaArg_Ptr;
	va_start(lpvaArg_Ptr,i_uiNum_Parameters);
	tMin = XMin_dv(i_uiNum_Parameters,lpvaArg_Ptr);
	va_end(lpvaArg_Ptr);
	return tMin;
}

float XMin_f(unsigned int i_uiNum_Parameters,...)
{
	float tMin;
	va_list	lpvaArg_Ptr;
	va_start(lpvaArg_Ptr,i_uiNum_Parameters);
	tMin = XMin_dv(i_uiNum_Parameters,lpvaArg_Ptr);
	va_end(lpvaArg_Ptr);
	return tMin;
}

unsigned long long XMax_ui64(unsigned int i_uiNum_Parameters,...)
{
	unsigned long long tMax;
	unsigned int uiI;
	va_list	lpvaArg_Ptr;
	va_start(lpvaArg_Ptr,i_uiNum_Parameters);
	tMax = va_arg(lpvaArg_Ptr,unsigned long long);
	for (uiI = 1; uiI < i_uiNum_Parameters; uiI++)
	{
		unsigned long long tX = va_arg(lpvaArg_Ptr,unsigned long long);
		if (tX > tMax)
			tMax = tX;
	}
	va_end(lpvaArg_Ptr);
	return tMax;
}

long long XMax_i64(unsigned int i_uiNum_Parameters,...)
{
	long long tMax;
	unsigned int uiI;
	va_list	lpvaArg_Ptr;
	va_start(lpvaArg_Ptr,i_uiNum_Parameters);
	tMax = va_arg(lpvaArg_Ptr,long long);
	for (uiI = 1; uiI < i_uiNum_Parameters; uiI++)
	{
		long long tX = va_arg(lpvaArg_Ptr,long long);
		if (tX > tMax)
			tMax = tX;
	}
	va_end(lpvaArg_Ptr);
	return tMax;
}

unsigned int XMax_uiv(unsigned int i_uiNum_Parameters,va_list	lpvaArg_Ptr)
{
	unsigned int tMax;
	unsigned int uiI;
	tMax = va_arg(lpvaArg_Ptr,unsigned int);
	for (uiI = 1; uiI < i_uiNum_Parameters; uiI++)
	{
		unsigned int tX = va_arg(lpvaArg_Ptr,unsigned int);
		if (tX > tMax)
			tMax = tX;
	}
	return tMax;
}

int XMax_iv(unsigned int i_uiNum_Parameters,va_list	lpvaArg_Ptr)
{
	int tMax;
	unsigned int uiI;
	tMax = va_arg(lpvaArg_Ptr,int);
	for (uiI = 1; uiI < i_uiNum_Parameters; uiI++)
	{
		int tX = va_arg(lpvaArg_Ptr,int);
		if (tX > tMax)
			tMax = tX;
	}
	return tMax;
}

double XMax_dv(unsigned int i_uiNum_Parameters,va_list	lpvaArg_Ptr)
{
	double tMax;
	unsigned int uiI;
	tMax = va_arg(lpvaArg_Ptr,double);
	for (uiI = 1; uiI < i_uiNum_Parameters; uiI++)
	{
		double tX = va_arg(lpvaArg_Ptr,double);
		if (tX > tMax)
			tMax = tX;
	}
	return tMax;
}
unsigned int XMax_ui(unsigned int i_uiNum_Parameters,...)
{
	unsigned int tMax;
	va_list	lpvaArg_Ptr;
	va_start(lpvaArg_Ptr,i_uiNum_Parameters);
	tMax = XMax_uiv(i_uiNum_Parameters,lpvaArg_Ptr);
	va_end(lpvaArg_Ptr);
	return tMax;
}

int XMax_i(unsigned int i_uiNum_Parameters,...)
{
	int tMax;
	va_list	lpvaArg_Ptr;
	va_start(lpvaArg_Ptr,i_uiNum_Parameters);
	tMax = XMax_iv(i_uiNum_Parameters,lpvaArg_Ptr);
	va_end(lpvaArg_Ptr);

	return tMax;
}

unsigned short XMax_us(unsigned int i_uiNum_Parameters,...)
{
	unsigned short tMax;
	va_list	lpvaArg_Ptr;
	va_start(lpvaArg_Ptr,i_uiNum_Parameters);
	tMax = XMax_uiv(i_uiNum_Parameters,lpvaArg_Ptr);
	va_end(lpvaArg_Ptr);

	return tMax;
}

short XMax_s(unsigned int i_uiNum_Parameters,...)
{
	short tMax;
	va_list	lpvaArg_Ptr;
	va_start(lpvaArg_Ptr,i_uiNum_Parameters);
	tMax = XMax_iv(i_uiNum_Parameters,lpvaArg_Ptr);
	va_end(lpvaArg_Ptr);

	return tMax;
}

unsigned char XMax_uc(unsigned int i_uiNum_Parameters,...)
{
	unsigned char tMax;
	va_list	lpvaArg_Ptr;
	va_start(lpvaArg_Ptr,i_uiNum_Parameters);
	tMax = XMax_uiv(i_uiNum_Parameters,lpvaArg_Ptr);
	va_end(lpvaArg_Ptr);

	return tMax;
}

char XMax_c(unsigned int i_uiNum_Parameters,...)
{
	char tMax;
	va_list	lpvaArg_Ptr;
	va_start(lpvaArg_Ptr,i_uiNum_Parameters);
	tMax = XMax_iv(i_uiNum_Parameters,lpvaArg_Ptr);
	va_end(lpvaArg_Ptr);

	return tMax;
}

double XMax_d(unsigned int i_uiNum_Parameters,...)
{
	double tMax;
	va_list	lpvaArg_Ptr;
	va_start(lpvaArg_Ptr,i_uiNum_Parameters);
	tMax = XMax_dv(i_uiNum_Parameters,lpvaArg_Ptr);
	va_end(lpvaArg_Ptr);

	return tMax;
}

float XMax_f(unsigned int i_uiNum_Parameters,...)
{
	float tMax;
	va_list	lpvaArg_Ptr;
	va_start(lpvaArg_Ptr,i_uiNum_Parameters);
	tMax = XMax_dv(i_uiNum_Parameters,lpvaArg_Ptr);
	va_end(lpvaArg_Ptr);

	return tMax;
}

void XRound2(unsigned int &i_uiValue)
{
	if ((i_uiValue & 0x080000000) && (i_uiValue & 0x07fffffff))
	{
		i_uiValue = 0; // overflow
	}
	else if (i_uiValue & 0x07fffffff)
	{
		unsigned int uiCtr = 0;
		while (!(i_uiValue & 0x080000000))
		{
			uiCtr++;
			i_uiValue <<= 1;
		}
		if (i_uiValue & 0x7fffffff) // lower order bit is set
		{
			i_uiValue &= 0x80000000; // clear lower order bits
			if (uiCtr != 0)
				uiCtr -= 1;
		}
		i_uiValue >>= uiCtr;
	}
}
void XRound4k(unsigned int &i_uiValue)
{
	if (i_uiValue & 0x0fff) // low order bits set?
	{
		i_uiValue &= ~0x0fff;
		i_uiValue += 0x01000;
	}
}



double XRoundNearest(const double & i_dValue, const double &i_dRound_To)
{
	double	dDiv = i_dValue / i_dRound_To;
	int64_t iVal = (int64_t)(dDiv);
	if (i_dValue < 0.0)
		iVal--;
	if ((dDiv - iVal) >= 0.5)
		iVal++;

	return iVal * i_dRound_To;
}

double XRoundUp(const double & i_dValue, const double &i_dRound_To)
{
	int64_t iVal = (int64_t)(i_dValue / i_dRound_To);
	if (i_dValue > 0.0)
		iVal++;
	return iVal * i_dRound_To;
}
double XRoundDown(const double & i_dValue, const double &i_dRound_To)
{
	int64_t iVal = (int64_t)(i_dValue / i_dRound_To);
	if (i_dValue < 0.0)
		iVal--;
	return iVal * i_dRound_To;
}



const char * xParse_Command_Line_Data_Ptr(int i_iArgC,const char * i_lpszArgV[],const char * i_lpszOption)
{ // if the command line option is say "data", this is what input option should be
// e.g. command data = 10
// if option is -data, (e.g. command -data 10) then pass "-data" as option input
// = sign is optional (-data=10, -data = 10, -data 10 will all be read)
	unsigned int uiI;
	const char * lpszRet = NULL;
	if (i_iArgC > 1 && i_lpszOption != NULL && i_lpszArgV != NULL)
	{
		size_t iOption_Len = strlen(i_lpszOption);
		for (uiI = 1; uiI < (unsigned int)i_iArgC && lpszRet == NULL; uiI++) // start from 1 - 0 is the command
		{
			if (strncmp(i_lpszArgV[uiI],i_lpszOption,iOption_Len) == 0)
			{
				const char * lpszCursor = i_lpszArgV[uiI];
				lpszCursor += iOption_Len; // advance past option string
				if (lpszCursor[0] == '=')
				{
					lpszCursor++;
					if (lpszCursor[0] == 0 && (uiI < (unsigned int)(i_iArgC - 1)))
					{
						lpszCursor = i_lpszArgV[uiI + 1];
					}
				}
				else if (lpszCursor[0] == 0 && uiI < (unsigned int)(i_iArgC - 1))
				{
					lpszCursor = i_lpszArgV[uiI + 1];
					if (lpszCursor[0] == '=')
					{
						lpszCursor++;
						if (lpszCursor[0] == 0 && (uiI < (unsigned int)(i_iArgC - 1)))
							lpszCursor = i_lpszArgV[uiI + 2];
					}
				}
				else
					continue;
				lpszRet = lpszCursor;
			}
		}
	}
	return lpszRet;
}

double xParse_Command_Line_Dbl(int iArgC,const char * lpszArgV[],const char * i_lpszOption, const double &dDefault_Value)
{ // if the command line option is say "data", this is what input option should be
// e.g. command data = 10
// if option is -data, (e.g. command -data 10) then pass "-data" as option input
// = sign is optional (-data=10, -data = 10, -data 10 will all be read)
	double	dRet = dDefault_Value; 
	const char * lpszCursor = xParse_Command_Line_Data_Ptr(iArgC,lpszArgV,i_lpszOption);
	if (lpszCursor)
	{
		if (xIsANumber(lpszCursor))
		{
			dRet = atof(lpszCursor);
		}
		else
		{
			printf("Command line option %s contains an invalid value ('%s').\n",i_lpszOption,lpszCursor);
		}
	}

/*	bool	bFound = false;
	unsigned int uiI;
	size_t iOption_Len = strlen(i_lpszOption);
	for (uiI = 1; uiI < iArgC && !bFound; uiI++) // start from 1 - 0 is the command
	{
		if (strncmp(lpszArgV[uiI],i_lpszOption,iOption_Len) == 0)
		{
			const char * lpszCursor = lpszArgV[uiI];
			lpszCursor += iOption_Len; // advance past option string
			if (lpszCursor[0] == '=')
			{
				lpszCursor++;
				if (lpszCursor[0] == 0 && (uiI < (iArgC - 1)))
				{
					lpszCursor = lpszArgV[uiI + 1];
				}
			}
			else if (lpszCursor[0] == 0 && uiI < (iArgC - 1))
			{
				lpszCursor = lpszArgV[uiI + 1];
				if (lpszCursor[0] == '=')
				{
					lpszCursor++;
					if (lpszCursor[0] == 0 && (uiI < (iArgC - 1)))
						lpszCursor = lpszArgV[uiI + 2];
				}
			}
			else
				continue;

		}
	}*/
	return dRet;
}

bool xParse_Command_Line_Bool(int iArgC,const char * lpszArgV[],const char * i_lpszOption, const double &bDefault_Value)
{ // if the command line option is say "data", this is what input option should be
// e.g. command data = 10
// if option is -data, (e.g. command -data 10) then pass "-data" as option input
// = sign is optional (-data=10, -data = 10, -data 10 will all be read)
	bool	bRet = bDefault_Value; 

	const char * lpszCursor = xParse_Command_Line_Data_Ptr(iArgC,lpszArgV,i_lpszOption);
	if (lpszCursor)
	{
			char lpszTemp[7];
			lpszTemp[6] = 0;
			lpszTemp[0] = lpszCursor[0] > 0 ? tolower(lpszCursor[0]) : 0;
			lpszTemp[1] = lpszCursor[1] > 0 ? tolower(lpszCursor[1]) : 0;
			lpszTemp[2] = lpszCursor[2] > 0 ? tolower(lpszCursor[2]) : 0;
			lpszTemp[3] = lpszCursor[3] > 0 ? tolower(lpszCursor[3]) : 0;
			lpszTemp[4] = lpszCursor[4] > 0 ? tolower(lpszCursor[4]) : 0;
			lpszTemp[5] = lpszCursor[5] > 0 ? tolower(lpszCursor[5]) : 0;
			if ((strcmp(lpszTemp,"yes") == 0) || (strcmp(lpszTemp,"true") == 0) || (strcmp(lpszTemp,".true.") == 0) || (strcmp(lpszTemp,"y") == 0))
			{
				bRet = true;
			}
			else if ((strcmp(lpszTemp,"no") == 0) || (strcmp(lpszTemp,"false") == 0) || (strcmp(lpszTemp,".false.") == 0) || (strcmp(lpszTemp,"n") == 0))
			{
				bRet = false;
			}
			else
			{
				printf("Command line option %s contains an invalid value ('%s').\n",i_lpszOption,lpszCursor);
			}
	}

/*

	bool	bFound = false;
	unsigned int uiI;
	size_t iOption_Len = strlen(i_lpszOption);
	char lpszTemp[7];
	for (uiI = 1; uiI < iArgC && !bFound; uiI++) // start from 1 - 0 is the command
	{
		if (strncmp(lpszArgV[uiI],i_lpszOption,iOption_Len) == 0)
		{
			const char * lpszCursor = lpszArgV[uiI];
			lpszCursor += iOption_Len; // advance past option string
			if (lpszCursor[0] == '=')
			{
				lpszCursor++;
				if (lpszCursor[0] == 0 && (uiI < (iArgC - 1)))
				{
					lpszCursor = lpszArgV[uiI + 1];
				}
			}
			else if (lpszCursor[0] == 0 && uiI < (iArgC - 1))
			{
				lpszCursor = lpszArgV[uiI + 1];
				if (lpszCursor[0] == '=')
				{
					lpszCursor++;
					if (lpszCursor[0] == 0 && (uiI < (iArgC - 1)))
						lpszCursor = lpszArgV[uiI + 2];
				}
			}
			else
				continue;

			lpszTemp[6] = 0;
			lpszTemp[0] = tolower(lpszCursor[0]);
			lpszTemp[1] = tolower(lpszCursor[1]);
			lpszTemp[2] = tolower(lpszCursor[2]);
			lpszTemp[3] = tolower(lpszCursor[3]);
			lpszTemp[4] = tolower(lpszCursor[4]);
			lpszTemp[5] = tolower(lpszCursor[5]);
			if ((strcmp(lpszTemp,"yes") == 0) || (strcmp(lpszTemp,"true") == 0) || (strcmp(lpszTemp,".true.") == 0) || (strcmp(lpszTemp,"y") == 0))
			{
				bRet = true;
				bFound = true;
			}
			else if ((strcmp(lpszTemp,"no") == 0) || (strcmp(lpszTemp,"false") == 0) || (strcmp(lpszTemp,".false.") == 0) || (strcmp(lpszTemp,"n") == 0))
			{
				bRet = false;
				bFound = true;
			}
			else
			{
				printf("Command line option %s contains an invalid value ('%s').\n",i_lpszOption,lpszCursor);
			}
		}
	}*/
	return bRet;
}

unsigned int xParse_Command_Line_UInt(int iArgC,const char * lpszArgV[],const char * i_lpszOption, unsigned int i_uiDefault_Value)
{ // if the command line option is say "data", this is what input option should be
// e.g. command data = 10
// if option is -data, (e.g. command -data 10) then pass "-data" as option input
// = sign is optional (-data=10, -data = 10, -data 10 will all be read)
	unsigned int	uiRet = i_uiDefault_Value; 
	const char * lpszCursor = xParse_Command_Line_Data_Ptr(iArgC,lpszArgV,i_lpszOption);
	if (lpszCursor)
	{
			if (xIsANumber(lpszCursor))
			{
				uiRet = atoi(lpszCursor);
			}
			else
			{
				printf("Command line option %s contains an invalid value ('%s').\n",i_lpszOption,lpszCursor);
			}
	}

/*	bool	bFound = false;
	unsigned int uiI;
	size_t iOption_Len = strlen(i_lpszOption);
	for (uiI = 1; uiI < iArgC && !bFound; uiI++) // start from 1 - 0 is the command
	{
		if (strncmp(lpszArgV[uiI],i_lpszOption,iOption_Len) == 0)
		{
			const char * lpszCursor = lpszArgV[uiI];
			lpszCursor += iOption_Len; // advance past option string
			if (lpszCursor[0] == '=')
			{
				lpszCursor++;
				if (lpszCursor[0] == 0 && (uiI < (iArgC - 1)))
				{
					lpszCursor = lpszArgV[uiI + 1];
				}
			}
			else if (lpszCursor[0] == 0 && uiI < (iArgC - 1))
			{
				lpszCursor = lpszArgV[uiI + 1];
				if (lpszCursor[0] == '=')
				{
					lpszCursor++;
					if (lpszCursor[0] == 0 && (uiI < (iArgC - 1)))
						lpszCursor = lpszArgV[uiI + 2];
				}
			}
			else
				continue;

			if (xIsANumber(lpszCursor))
			{
				uiRet = atoi(lpszCursor);
				bFound = true;
			}
			else
			{
				printf("Command line option %s contains an invalid value ('%s').\n",i_lpszOption,lpszCursor);
			}
		}
	}*/
	return uiRet;
}


int xParse_Command_Line_Int(int iArgC,const char * lpszArgV[],const char * i_lpszOption, int i_iDefault_Value)
{ // if the command line option is say "data", this is what input option should be
// e.g. command data = 10
// if option is -data, (e.g. command -data 10) then pass "-data" as option input
// = sign is optional (-data=10, -data = 10, -data 10 will all be read)
	int	iRet = i_iDefault_Value; 
	const char * lpszCursor = xParse_Command_Line_Data_Ptr(iArgC,lpszArgV,i_lpszOption);
	if (lpszCursor)
	{
			if (xIsANumber(lpszCursor))
			{
				iRet = atoi(lpszCursor);
			}
			else
			{
				printf("Command line option %s contains an invalid value ('%s').\n",i_lpszOption,lpszCursor);
			}
	}

/*	bool	bFound = false;
	unsigned int uiI;
	size_t iOption_Len = strlen(i_lpszOption);
	for (uiI = 1; uiI < iArgC && !bFound; uiI++) // start from 1 - 0 is the command
	{
		if (strncmp(lpszArgV[uiI],i_lpszOption,iOption_Len) == 0)
		{
			const char * lpszCursor = lpszArgV[uiI];
			lpszCursor += iOption_Len; // advance past option string
			if (lpszCursor[0] == '=')
			{
				lpszCursor++;
				if (lpszCursor[0] == 0 && (uiI < (iArgC - 1)))
				{
					lpszCursor = lpszArgV[uiI + 1];
				}
			}
			else if (lpszCursor[0] == 0 && uiI < (iArgC - 1))
			{
				lpszCursor = lpszArgV[uiI + 1];
				if (lpszCursor[0] == '=')
				{
					lpszCursor++;
					if (lpszCursor[0] == 0 && (uiI < (iArgC - 1)))
						lpszCursor = lpszArgV[uiI + 2];
				}
			}
			else
				continue;

			if (xIsANumber(lpszCursor))
			{
				uiRet = atoi(lpszCursor);
				bFound = true;
			}
			else
			{
				printf("Command line option %s contains an invalid value ('%s').\n",i_lpszOption,lpszCursor);
			}
		}
	}*/
	return iRet;
}

void xParse_Command_Line_String(int iArgC,const char * lpszArgV[],const char * i_lpszOption, char * o_lpszParameter, unsigned int i_uiMax_Parameter_Size, const char * i_lpszDefault_Value)
{ // if the command line option is say "data", this is what input option should be
// e.g. command data = 10
// if option is -data, (e.g. command -data 10) then pass "-data" as option input
// = sign is optional (-data=10, -data = 10, -data 10 will all be read)
	unsigned int	uiIdx = 0;
	char chTerminator = 0;

	if (i_lpszDefault_Value != NULL)
	{
		strncpy(o_lpszParameter,i_lpszDefault_Value,i_uiMax_Parameter_Size);
	}
	const char * lpszCursor = xParse_Command_Line_Data_Ptr(iArgC,lpszArgV,i_lpszOption);
	if (lpszCursor)
	{
		if (lpszCursor[0] == '\"' || lpszCursor[0] == '\'')
		{
			chTerminator = lpszCursor[0];
			lpszCursor++;
		}
		while (lpszCursor[0] && uiIdx < i_uiMax_Parameter_Size && lpszCursor[0] != 0 && lpszCursor[0] != chTerminator)
		{
			o_lpszParameter[uiIdx] = lpszCursor[0];
			uiIdx++;
			lpszCursor++;
		}
		o_lpszParameter[uiIdx] = 0;
		o_lpszParameter[i_uiMax_Parameter_Size - 1] = 0;
	}


/*	for (uiI = 1; uiI < iArgC && !bFound; uiI++) // start from 1 - 0 is the command
	{
		if (strncmp(lpszArgV[uiI],i_lpszOption,iOption_Len) == 0)
		{
			const char * lpszCursor = lpszArgV[uiI];
			lpszCursor += iOption_Len; // advance past option string
			if (lpszCursor[0] == '=')
			{
				lpszCursor++;
				if (lpszCursor[0] == 0 && (uiI < (iArgC - 1)))
				{
					lpszCursor = lpszArgV[uiI + 1];
				}
			}
			else if (lpszCursor[0] == 0 && uiI < (iArgC - 1))
			{
				lpszCursor = lpszArgV[uiI + 1];
				if (lpszCursor[0] == '=')
				{
					lpszCursor++;
					if (lpszCursor[0] == 0 && (uiI < (iArgC - 1)))
						lpszCursor = lpszArgV[uiI + 2];
				}
			}
			else
				continue;
			if (lpszCursor[0] == '\"' || lpszCursor[0] == '\'')
			{
				chTerminator = lpszCursor[0];
				lpszCursor++;
			}
			while (lpszCursor[0] && uiIdx < i_uiMax_Parameter_Size && lpszCursor[0] != 0 && lpszCursor[0] != chTerminator)
			{
				o_lpszParameter[uiIdx] = lpszCursor[0];
				uiIdx++;
				lpszCursor++;
			}
			o_lpszParameter[uiIdx] = 0;
			o_lpszParameter[i_uiMax_Parameter_Size - 1] = 0;
			bFound = true;
		}
	}*/
}
bool xParse_Command_Line_Exists(int iArgC,const char * lpszArgV[],const char * i_lpszOption)
{
	bool	bFound = false;
	unsigned int uiI;
	size_t iOption_Len = strlen(i_lpszOption);

	if (iArgC > 1)
	{
		for (uiI = 1; uiI < (unsigned int)iArgC && !bFound; uiI++) // start from 1 - 0 is the command
		{
			bFound = (strncmp(lpszArgV[uiI],i_lpszOption,iOption_Len) == 0);
			bFound &= (lpszArgV[uiI][iOption_Len] == 0 || lpszArgV[uiI][iOption_Len] == '=');
		}
	}
	return bFound;
}


void xRomanNumeralGenerator(char * lpszResult, unsigned int uiI)
{
	char * lpszCursor = lpszResult;
	while (uiI >= 1000)
	{
		lpszCursor[0] = 'M';
		uiI -= 1000;
		lpszCursor++;
	}
	if (uiI >= 400)
	{
		lpszCursor[0] = 'C';
		lpszCursor++;
		lpszCursor[0] = 'M';
		lpszCursor++;
		uiI -= 400;
	}
	else
	{
		while (uiI >= 100)
		{
			lpszCursor[0] = 'C';
			uiI -= 100;
			lpszCursor++;
		}
	}
	if (uiI >= 90)
	{
		lpszCursor[0] = 'X';
		lpszCursor++;
		lpszCursor[0] = 'C';
		lpszCursor++;
		uiI -= 90;
	}
	else if (uiI >= 50)
	{
		lpszCursor[0] = 'L';
		lpszCursor++;
		uiI -= 50;
	}
	else if (uiI >= 40)
	{
		lpszCursor[0] = 'X';
		lpszCursor++;
		lpszCursor[0] = 'L';
		lpszCursor++;
		uiI -= 40;
	}

	while (uiI >= 10)
	{
		lpszCursor[0] = 'X';
		uiI -= 10;
		lpszCursor++;
	}

	if (uiI >= 9)
	{
		lpszCursor[0] = 'I';
		lpszCursor++;
		lpszCursor[0] = 'X';
		lpszCursor++;
		uiI -= 9;
	}
	else if (uiI >= 5)
	{
		lpszCursor[0] = 'V';
		lpszCursor++;
		uiI -= 5;
	}
	else if (uiI >= 4)
	{
		lpszCursor[0] = 'I';
		lpszCursor++;
		lpszCursor[0] = 'V';
		lpszCursor++;
		uiI -= 4;
	}

	while (uiI > 0)
	{
		lpszCursor[0] = 'I';
		uiI -= 1;
		lpszCursor++;
	}
	lpszCursor[0] = 0;

}

void xGet_Element_Symbol(unsigned int i_uiAtomic_Number, char * o_lpszElement_Symbol) // o_lpszElement_Symbol must be of size char [3] or longer; some elements require char [4]
{
	switch (i_uiAtomic_Number)
	{
	case 1:
		strcpy(o_lpszElement_Symbol, "H");
		break;
	case 2:
		strcpy(o_lpszElement_Symbol, "He");
		break;
	case 3:
		strcpy(o_lpszElement_Symbol, "Li");
		break;
	case 4:
		strcpy(o_lpszElement_Symbol, "Be");
		break;
	case 5:
		strcpy(o_lpszElement_Symbol, "B");
		break;
	case 6:
		strcpy(o_lpszElement_Symbol, "C");
		break;
	case 7:
		strcpy(o_lpszElement_Symbol, "N");
		break;
	case 8:
		strcpy(o_lpszElement_Symbol, "O");
		break;
	case 9:
		strcpy(o_lpszElement_Symbol, "F");
		break;
	case 10:
		strcpy(o_lpszElement_Symbol, "Ne");
		break;
	case 11:
		strcpy(o_lpszElement_Symbol, "Na");
		break;
	case 12:
		strcpy(o_lpszElement_Symbol, "Mg");
		break;
	case 13:
		strcpy(o_lpszElement_Symbol, "Al");
		break;
	case 14:
		strcpy(o_lpszElement_Symbol, "Si");
		break;
	case 15:
		strcpy(o_lpszElement_Symbol, "P");
		break;
	case 16:
		strcpy(o_lpszElement_Symbol, "S");
		break;
	case 17:
		strcpy(o_lpszElement_Symbol, "Cl");
		break;
	case 18:
		strcpy(o_lpszElement_Symbol, "Ar");
		break;
	case 19:
		strcpy(o_lpszElement_Symbol, "K");
		break;
	case 20:
		strcpy(o_lpszElement_Symbol, "Ca");
		break;
	case 21:
		strcpy(o_lpszElement_Symbol, "Sc");
		break;
	case 22:
		strcpy(o_lpszElement_Symbol, "Ti");
		break;
	case 23:
		strcpy(o_lpszElement_Symbol, "V");
		break;
	case 24:
		strcpy(o_lpszElement_Symbol, "Cr");
		break;
	case 25:
		strcpy(o_lpszElement_Symbol, "Mn");
		break;
	case 26:
		strcpy(o_lpszElement_Symbol, "Fe");
		break;
	case 27:
		strcpy(o_lpszElement_Symbol, "Co");
		break;
	case 28:
		strcpy(o_lpszElement_Symbol, "Ni");
		break;
	case 29:
		strcpy(o_lpszElement_Symbol, "Cu");
		break;
	case 30:
		strcpy(o_lpszElement_Symbol, "Zn");
		break;
	case 31:
		strcpy(o_lpszElement_Symbol, "Ga");
		break;
	case 32:
		strcpy(o_lpszElement_Symbol, "Ge");
		break;
	case 33:
		strcpy(o_lpszElement_Symbol, "As");
		break;
	case 34:
		strcpy(o_lpszElement_Symbol, "Se");
		break;
	case 35:
		strcpy(o_lpszElement_Symbol, "Br");
		break;
	case 36:
		strcpy(o_lpszElement_Symbol, "Kr");
		break;
	case 37:
		strcpy(o_lpszElement_Symbol, "Rb");
		break;
	case 38:
		strcpy(o_lpszElement_Symbol, "Sr");
		break;
	case 39:
		strcpy(o_lpszElement_Symbol, "Y");
		break;
	case 40:
		strcpy(o_lpszElement_Symbol, "Zr");
		break;
	case 41:
		strcpy(o_lpszElement_Symbol, "Nb");
		break;
	case 42:
		strcpy(o_lpszElement_Symbol, "Mo");
		break;
	case 43:
		strcpy(o_lpszElement_Symbol, "Tc");
		break;
	case 44:
		strcpy(o_lpszElement_Symbol, "Ru");
		break;
	case 45:
		strcpy(o_lpszElement_Symbol, "Rh");
		break;
	case 46:
		strcpy(o_lpszElement_Symbol, "Pd");
		break;
	case 47:
		strcpy(o_lpszElement_Symbol, "Ag");
		break;
	case 48:
		strcpy(o_lpszElement_Symbol, "Cd");
		break;
	case 49:
		strcpy(o_lpszElement_Symbol, "In");
		break;
	case 50:
		strcpy(o_lpszElement_Symbol, "Sn");
		break;
	case 51:
		strcpy(o_lpszElement_Symbol, "Sb");
		break;
	case 52:
		strcpy(o_lpszElement_Symbol, "Te");
		break;
	case 53:
		strcpy(o_lpszElement_Symbol, "I");
		break;
	case 54:
		strcpy(o_lpszElement_Symbol, "Xe");
		break;
	case 55:
		strcpy(o_lpszElement_Symbol, "Cs");
		break;
	case 56:
		strcpy(o_lpszElement_Symbol, "Ba");
		break;
	case 57:
		strcpy(o_lpszElement_Symbol, "La");
		break;
	case 58:
		strcpy(o_lpszElement_Symbol, "Ce");
		break;
	case 59:
		strcpy(o_lpszElement_Symbol, "Pr");
		break;
	case 60:
		strcpy(o_lpszElement_Symbol, "Nd");
		break;
	case 61:
		strcpy(o_lpszElement_Symbol, "Pm");
		break;
	case 62:
		strcpy(o_lpszElement_Symbol, "Sm");
		break;
	case 63:
		strcpy(o_lpszElement_Symbol, "Eu");
		break;
	case 64:
		strcpy(o_lpszElement_Symbol, "Gd");
		break;
	case 65:
		strcpy(o_lpszElement_Symbol, "Tb");
		break;
	case 66:
		strcpy(o_lpszElement_Symbol, "Dy");
		break;
	case 67:
		strcpy(o_lpszElement_Symbol, "Ho");
		break;
	case 68:
		strcpy(o_lpszElement_Symbol, "Er");
		break;
	case 69:
		strcpy(o_lpszElement_Symbol, "Tm");
		break;
	case 70:
		strcpy(o_lpszElement_Symbol, "Yb");
		break;
	case 71:
		strcpy(o_lpszElement_Symbol, "Lu");
		break;
	case 72:
		strcpy(o_lpszElement_Symbol, "Hf");
		break;
	case 73:
		strcpy(o_lpszElement_Symbol, "Ta");
		break;
	case 74:
		strcpy(o_lpszElement_Symbol, "W");
		break;
	case 75:
		strcpy(o_lpszElement_Symbol, "Re");
		break;
	case 76:
		strcpy(o_lpszElement_Symbol, "Os");
		break;
	case 77:
		strcpy(o_lpszElement_Symbol, "Ir");
		break;
	case 78:
		strcpy(o_lpszElement_Symbol, "Pt");
		break;
	case 79:
		strcpy(o_lpszElement_Symbol, "Au");
		break;
	case 80:
		strcpy(o_lpszElement_Symbol, "Hg");
		break;
	case 81:
		strcpy(o_lpszElement_Symbol, "Tl");
		break;
	case 82:
		strcpy(o_lpszElement_Symbol, "Pb");
		break;
	case 83:
		strcpy(o_lpszElement_Symbol, "Bi");
		break;
	case 84:
		strcpy(o_lpszElement_Symbol, "Po");
		break;
	case 85:
		strcpy(o_lpszElement_Symbol, "At");
		break;
	case 86:
		strcpy(o_lpszElement_Symbol, "Rn");
		break;
	case 87:
		strcpy(o_lpszElement_Symbol, "Fr");
		break;
	case 88:
		strcpy(o_lpszElement_Symbol, "Ra");
		break;
	case 89:
		strcpy(o_lpszElement_Symbol, "Ac");
		break;
	case 90:
		strcpy(o_lpszElement_Symbol, "Th");
		break;
	case 91:
		strcpy(o_lpszElement_Symbol, "Pa");
		break;
	case 92:
		strcpy(o_lpszElement_Symbol, "U");
		break;
	case 93:
		strcpy(o_lpszElement_Symbol, "Np");
		break;
	case 94:
		strcpy(o_lpszElement_Symbol, "Pu");
		break;
	case 95:
		strcpy(o_lpszElement_Symbol, "Am");
		break;
	case 96:
		strcpy(o_lpszElement_Symbol, "Cm");
		break;
	case 97:
		strcpy(o_lpszElement_Symbol, "Bk");
		break;
	case 98:
		strcpy(o_lpszElement_Symbol, "Cf");
		break;
	case 99:
		strcpy(o_lpszElement_Symbol, "Es");
		break;
	case 100:
		strcpy(o_lpszElement_Symbol, "Fm");
		break;
	case 101:
		strcpy(o_lpszElement_Symbol, "Md");
		break;
	case 102:
		strcpy(o_lpszElement_Symbol, "No");
		break;
	case 103:
		strcpy(o_lpszElement_Symbol, "Lr");
		break;
	case 104:
		strcpy(o_lpszElement_Symbol, "Rf");
		break;
	case 105:
		strcpy(o_lpszElement_Symbol, "Db");
		break;
	case 106:
		strcpy(o_lpszElement_Symbol, "Sg");
		break;
	case 107:
		strcpy(o_lpszElement_Symbol, "Bh");
		break;
	case 108:
		strcpy(o_lpszElement_Symbol, "Hs");
		break;
	case 109:
		strcpy(o_lpszElement_Symbol, "Mt");
		break;
	case 110:
		strcpy(o_lpszElement_Symbol, "Ds");
		break;
	case 111:
		strcpy(o_lpszElement_Symbol, "Rg");
		break;
	case 112:
		strcpy(o_lpszElement_Symbol, "Cn");
		break;
	case 113:
		strcpy(o_lpszElement_Symbol, "Uut");
		break;
	case 114:
		strcpy(o_lpszElement_Symbol, "Fl");
		break;
	case 115:
		strcpy(o_lpszElement_Symbol, "Uup");
		break;
	case 116:
		strcpy(o_lpszElement_Symbol, "Lv");
		break;
	case 117:
		strcpy(o_lpszElement_Symbol, "Uus");
		break;
	case 118:
		strcpy(o_lpszElement_Symbol, "Uuo");
		break;
	default:
		o_lpszElement_Symbol[0] = 0;
		o_lpszElement_Symbol[1] = 0;
		break;
	}

}

class xlclALLOCATION_SIZE_DATA
{
public:
	unsigned int uiPage_Size;
	unsigned int uiAvailable_Pages;
	unsigned int uiAvailable_Memory;
	unsigned int uiWord_Size;
	unsigned int uiWord_Exponent;
	unsigned int uiWord_Remainder;
	bool bData_Initialized;

	xlclALLOCATION_SIZE_DATA(void) {bData_Initialized = false; Populate();}

	void Populate(void)
	{
		bData_Initialized = true;
		uiPage_Size = getpagesize();
		uiAvailable_Pages = sysconf(_SC_PHYS_PAGES);
		//printf("Available pages: %i.  Page size %i.\n",uiAvailable_Pages, uiPage_Size);
		uiAvailable_Memory = uiAvailable_Pages * uiPage_Size;
		uiWord_Size = sizeof(long);
		uiWord_Exponent = 0;
		uiWord_Remainder = uiWord_Size;
		while (!(uiWord_Remainder & 1))
		{
			uiWord_Remainder >>= 1;
			uiWord_Exponent++;
		}
	}
};

xlclALLOCATION_SIZE_DATA g_xlclAllocation_Size_Data;

unsigned int xlclFast_Log2_Int(unsigned int i_uiOperand)
{ // finds the minimum integer base 2 logarithm y that satisfies 2^y >= i_uiOperand
	unsigned int uiResult = 0;
	while (((unsigned int)1 << uiResult) < i_uiOperand)
		uiResult++;
	return uiResult;
}

unsigned int xlclChoose_Allocation_Size_Word_Aligned(bool i_bQuiet, unsigned int i_uiNumber_Of_Elements, unsigned int i_uiElement_Size)
{ // This function chooses an allocation size that is word aligned
	unsigned int uiAllocation_Suggested = 0;
	unsigned int uiAllocation_Size_Requested = i_uiNumber_Of_Elements * i_uiElement_Size;
	g_xlclAllocation_Size_Data.Populate();
	if (i_uiElement_Size != 0)
	{
		uiAllocation_Size_Requested = i_uiNumber_Of_Elements * i_uiElement_Size;
		if (uiAllocation_Size_Requested == g_xlclAllocation_Size_Data.uiPage_Size || uiAllocation_Size_Requested == g_xlclAllocation_Size_Data.uiWord_Size)
			uiAllocation_Suggested = i_uiNumber_Of_Elements; // trivial - word or page requested
		else
		{
			// choose an allocation size that is word aligned
			unsigned int uiAllocation_2_Exponent = 0;
			unsigned int uiAllocation_Supplement = 0;
			unsigned int uiAllocation_Remainer = i_uiElement_Size * i_uiNumber_Of_Elements;
			
			// Determine the number of factors of 2 in the allocation size
			while (!(uiAllocation_Remainer & 0x01))
			{
				uiAllocation_Remainer >>= 1;
				uiAllocation_2_Exponent++;
			}
			if (uiAllocation_2_Exponent < g_xlclAllocation_Size_Data.uiWord_Exponent)
			{
				uiAllocation_Supplement = g_xlclAllocation_Size_Data.uiWord_Exponent - uiAllocation_2_Exponent;
			}
			uiAllocation_Suggested = i_uiNumber_Of_Elements << uiAllocation_Supplement;
		}
	}
	else if (!i_bQuiet)
	{
		fprintf(stderr,"xstdlib: Choose allocation size smart has been called with a 0 size element.\n");
	}
	unsigned int uiAllocation_Size_Suggested = uiAllocation_Suggested * i_uiElement_Size;

	if (!i_bQuiet && g_xlclAllocation_Size_Data.uiAvailable_Memory < uiAllocation_Size_Requested)
	{
		fprintf(stderr,"xstdlib: Choose allocation size (word aligned) has been called with a request\n");
		fprintf(stderr,"xstdlib:  for more pages than are available in memory.\n");
		fprintf(stderr,"xstdlib: Element size: %i.  Number of Elements requested: %i\n",i_uiElement_Size, i_uiNumber_Of_Elements);
		fprintf(stderr,"xstdlib: To prevent this message use xChoose_Allocation_Size_Word_Aligned_Quiet.\n"); 
	}
	if (!i_bQuiet && g_xlclAllocation_Size_Data.uiAvailable_Memory < uiAllocation_Size_Suggested)
	{
		fprintf(stderr,"xstdlib: Choose allocation size (word aligned) has suggested an allocation\n");
		fprintf(stderr,"xstdlib:  for more pages than are available in memory.\n");
		fprintf(stderr,"xstdlib: Element size: %i.  Number of Elements requested: %i\n",i_uiElement_Size, i_uiNumber_Of_Elements);
		fprintf(stderr,"xstdlib: Pages available: %i.  Page size: %i\n",g_xlclAllocation_Size_Data.uiPage_Size,g_xlclAllocation_Size_Data.uiAvailable_Pages);
		fprintf(stderr,"xstdlib: Memory available: %i.  Request size: %i\n",g_xlclAllocation_Size_Data.uiAvailable_Memory,uiAllocation_Size_Suggested);
		fprintf(stderr,"xstdlib: To prevent this message use xChoose_Allocation_Size_Word_Aligned_Quiet.\n"); 
	}
	if (i_bQuiet && i_uiElement_Size > g_xlclAllocation_Size_Data.uiPage_Size)
	{
		fprintf(stderr,"xstdlib: Choose allocation size (word aligned) has been called with an\n");
		fprintf(stderr,"xstdlib: element size > page size. (Element size: %i, Page size: %i)\n",i_uiElement_Size,g_xlclAllocation_Size_Data.uiPage_Size); 
		fprintf(stderr,"xstdlib: To prevent this message use xChoose_Allocation_Size_Word_Aligned_Quiet.\n"); 
	}
	return uiAllocation_Suggested;
}
unsigned int xChoose_Allocation_Size_Word_Aligned(unsigned int i_uiNumber_Of_Elements, unsigned int i_uiElement_Size)
{
	return xlclChoose_Allocation_Size_Word_Aligned(false,i_uiNumber_Of_Elements, i_uiElement_Size);
}
unsigned int xChoose_Allocation_Size_Word_Aligned_Quiet(unsigned int i_uiNumber_Of_Elements, unsigned int i_uiElement_Size)
{
	return xlclChoose_Allocation_Size_Word_Aligned(true,i_uiNumber_Of_Elements, i_uiElement_Size);
}

bool xIsAScientificNumber(const char * i_lpszString)
{
	return ((i_lpszString[0] >= '0' && i_lpszString[0] <= '9') || i_lpszString[0] == '.' || i_lpszString[0] == '-' || i_lpszString[0] == 'e' || i_lpszString[0] == 'E' || i_lpszString[0] == '+');
}

double	xGetNumberFromEndOfString(const char * i_lpszString)
{
	double dRet = nan("");
	if (i_lpszString)
	{
		const char * lpszCursor = i_lpszString + strlen(i_lpszString);
		while (lpszCursor[0] == 10 || lpszCursor[0] == 13 || lpszCursor[0] == 0)
			lpszCursor--; // bypass CR/LF and end of string
		if (lpszCursor >= i_lpszString) // make sure we are still within the buffer
			while (xIsAScientificNumber(lpszCursor))
				lpszCursor--;
		if (lpszCursor >= i_lpszString && xIsAScientificNumber(lpszCursor + 1))
		{
			lpszCursor++;
			dRet = atof(lpszCursor);
		}
	}
	return dRet;
}

char *	xGetLastWordInString(const char * i_lpszString)
{
	const char * lpszRet = NULL;
	if (i_lpszString)
	{
		const char * lpszCursor = i_lpszString + strlen(i_lpszString);
		while (lpszCursor[0] == 10 || lpszCursor[0] == 13 || lpszCursor[0] == 0)
			lpszCursor--; // bypass CR/LF and end of string
		if (lpszCursor >= i_lpszString) // make sure we are still within the buffer
			while (lpszCursor[0] != ' ' && lpszCursor[0] != '\t' && lpszCursor[0] != 0 && lpszCursor > i_lpszString)
				lpszCursor--;
		if (lpszCursor >= i_lpszString && lpszCursor[0] != 0)
		{
			lpszCursor++;
			lpszRet = lpszCursor;
		}
	}
	return (char *)lpszRet;
}

xstdlib::datatype xstdlib::identify_datatype(const std::string i_szString)
{
	bool bInteger = true;
	bool bFloating_Point = true;
	bool bHex = false;
	bool bOctal = false;
	bool bBinary = false;
	xstdlib::datatype eType = xstdlib::empty;
	if (!i_szString.empty())
	{
		if (i_szString == "-" || i_szString == "+" || i_szString == "b" || i_szString == "B")
		{
			bInteger = false;
			bFloating_Point = false;
		}
		else if (i_szString == "nan" || i_szString == "NAN" || i_szString == "NaN" || 
				i_szString == "inf" || i_szString == "-inf" || i_szString == "Inf" || i_szString == "-Inf" ||
				i_szString == "INF" || i_szString == "-INF")
		{ // special case to handle nan and infinity - these are considered to be floating point data
			bInteger = false;
			bFloating_Point = true;
		}
		else
		{
			auto iterI = i_szString.begin();
			// check for possibilty of this string containing a hexadecimal or octal number
			if ((*iterI) == '-' || (*iterI) == '+')
				iterI++;
			if (iterI != i_szString.end() && ((*iterI) == 'b' || (*iterI) == 'B'))
			{
				bBinary = true;
				iterI++;
			}
			else if (iterI != i_szString.end() && (*iterI) == '0')
			{
				iterI++;
				if (iterI != i_szString.end() && ((*iterI) == 'x' || (*iterI) == 'X'))
				{
					bHex = true;
					iterI++;
				}
				else if (iterI != i_szString.end() && ((*iterI) >= '0' && (*iterI) <= '9'))
				{
					bOctal = true;
					iterI++;
				}
			}

			char chLast = 0;
			bool bPast_Exponent = false;
			bool bDots = false;
			for (; iterI != i_szString.end() && bFloating_Point; iterI++)
			{
				if (bBinary && (*iterI) != '0' && (*iterI) != '1')
				{
					bBinary = bInteger = bFloating_Point = false;
				}
				else if ((*iterI) < '0' || (*iterI) > '9')
				{
					bInteger = false; // integer must contain a number 
					if (bHex)
						bFloating_Point = false;
					else if ((*iterI) == '.')
					{
						if (bDots || bPast_Exponent) // already encountered decimal point
							bFloating_Point = false;
						bDots = true;
					}
					else if ((*iterI) == '-' || (*iterI) == '+')
					{
						if (chLast != 'e' && chLast != 'E') // '-' only allowed immediate after e or E
							bFloating_Point = false;
					}
					else if ((*iterI) == 'e' || (*iterI) == 'E')
					{
						bFloating_Point = !bPast_Exponent;
						bPast_Exponent = true;
					}
					else
						bFloating_Point = false;
				}

				chLast = (*iterI);
			}
		}

		if (bBinary)
			eType = xstdlib::binary;
		else if (bHex && bInteger)
			eType = xstdlib::hex;
		else if (bOctal && bInteger)
			eType = xstdlib::octal;
		else if (bInteger)
			eType = xstdlib::integer;
		else if (bFloating_Point)
			eType = xstdlib::floating_point;
		else
		{
			if (i_szString == "true" || i_szString == "false" || i_szString == "TRUE" || i_szString == "FALSE" || i_szString == "True" || i_szString == "False" || i_szString == "t" || i_szString == "f" || i_szString == "T" || i_szString == "F")
				eType = xstdlib::logical;
			else
				eType = xstdlib::string;
		}
	}
	return eType;
}

void xstdlib::printbit(uint8_t i_cVal)
{
	if (i_cVal == 0)
		printf("0");
	else
		printf("1");
}

void xstdlib::pbin(void * i_lpdData, size_t tN_Bytes)
{
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
}

