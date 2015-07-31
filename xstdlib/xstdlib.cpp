#include <xstdlib.h>
#include <math.h>
#include <stdio.h>
#include <ctype.h>
#include <string>
#include <unistd.h>

// Template class for random number generators
class XRAND
{
protected:
	int k_iRANDMAX;
public:
	XRAND(void)
	{
		k_iRANDMAX = 0;
	}
        virtual ~XRAND(void) {;}
	virtual void Seed(int iSeed)=0;
	virtual int Generate_Random_Number(void)=0;
	virtual int GetRandInt(void)=0;
	virtual double GetRandDouble(void)=0;
	virtual double GetRandDoubleNegative(void)=0;
	int GetRandMax(void)
	{
		return k_iRANDMAX;
	}
};

// Park & Miller random number generator, from
// Numerical Recipies in c++, 2nd ed.
class XRAND_Park_Miller : public XRAND
{
private:
	int m_iSeed;
	int k_iAA;
	double k_dSCALAR;
public:
	XRAND_Park_Miller(void)
	{
		k_iRANDMAX = 2147483647;
		k_iAA = 16807;
		m_iSeed = 0x62094986;
		k_dSCALAR = 1.0 / k_iRANDMAX;
	}
	void Seed(int i_iSeed)
	{
		m_iSeed = i_iSeed;
	}
	int Generate_Random_Number(void)
	{
		int uiMult = m_iSeed * k_iAA;
		uiMult %= k_iRANDMAX;
		m_iSeed = uiMult;
		return m_iSeed;
	}
	int GetRandInt(void)
	{
		return Generate_Random_Number();
	}
	double GetRandDouble(void)
	{
		return (Generate_Random_Number() * k_dSCALAR);
	}
	double GetRandDoubleNegative(void)
	{
		return (Generate_Random_Number() * k_dSCALAR + 1.0) * 0.5;
	}
};

// Subractive Random number generator, from Knuth;
// Based on Numerical Recipes in c++, 2nd ed
// with modifications to reduce memory footprint
class XRAND_Knuth : public XRAND
{
private:
	int	m_iIdx_Next;
	int m_iIdx_Next_Offset;
	int	m_iDataset[55];

	int k_iMSEED;
	double k_dSCALAR;
	
public:
	XRAND_Knuth(void)
	{
		k_iRANDMAX = 1000000000;
		k_iMSEED = 161803398;
		k_dSCALAR = 1.0 / k_iRANDMAX;
		Seed(-1);
	}
	void Seed(int iSeed)
	{
		int iMJ = abs(k_iMSEED - abs(iSeed));
		int iMK;
		unsigned int uiI, uiII, uiK;
		
		iMJ %= k_iRANDMAX;
		m_iDataset[54] = iMJ;
		iMK = 1;
		for (uiI = 0; uiI < 54; uiI++)
		{
			uiII = (21 * (uiI + 1)) % 55; // 'random' staring value... uhh... sure
			m_iDataset[uiII - 1] = iMK;
			iMK = iMJ - iMK;
			if (iMK < 0) iMK += k_iRANDMAX;
			iMJ = m_iDataset[uiII - 1];
		}
		for (uiK = 0; uiK < 4; uiK++)
		{
			for (uiI = 0; uiI < 55; uiI++)
			{
				m_iDataset[uiI] -= m_iDataset[((uiI + 31) % 55)];
				if(m_iDataset[uiI] < 0) m_iDataset[uiI] += k_iRANDMAX;
			}
		}
		m_iIdx_Next = -1;
		m_iIdx_Next_Offset = 30;
		
	}
	int Generate_Random_Number(void)
	{
		m_iIdx_Next++;
		if (m_iIdx_Next == 55)
			m_iIdx_Next = 0;
		m_iIdx_Next_Offset++;
		if (m_iIdx_Next_Offset == 55)
			m_iIdx_Next_Offset = 0;
		int iMJ = m_iDataset[m_iIdx_Next] - m_iDataset[m_iIdx_Next_Offset];
		if (iMJ < 0) iMJ += k_iRANDMAX;
		m_iDataset[m_iIdx_Next] = iMJ;
		return iMJ;
	}
	int GetRandInt(void)
	{
		return Generate_Random_Number();
	}
	double GetRandDouble(void)
	{
		return Generate_Random_Number() * k_dSCALAR;
	}
	double GetRandDoubleNegative(void)
	{
		return (Generate_Random_Number() * k_dSCALAR - 0.5) * 2.0;
	}

	
};


// Subractive Random number generator, from L'Ecuyer;
// Based on Numerical Recipes in c++, 2nd ed
// with modifications to allow for integer value return
class XRAND_LEcuyer : public XRAND
{
private:
	int k_iIM1;
	int k_iIM2;
	int k_iIA1;
	int k_iIA2;
	int k_iIQ1;
	int k_iIQ2;
	int k_iIR1;
	int k_iIR2;
	int k_iNTAB;
	int k_iIMM1;
	int k_iNDIV;
	double k_dEps;
	int m_iSeed_2;
	int m_iY;
	int * m_iIV;
	int m_iSeed;
	double k_dSCALAR;


	void Copy(const XRAND_LEcuyer & i_cRHO)
	{
		k_iIM1 = i_cRHO.k_iIM1;
		k_iIM2 = i_cRHO.k_iIM2;
		k_iIA1 = i_cRHO.k_iIA1;
		k_iIA2 = i_cRHO.k_iIA2;
		k_iIQ1 = i_cRHO.k_iIQ1;
		k_iIQ2 = i_cRHO.k_iIQ2;
		k_iIR1 = i_cRHO.k_iIR1;
		k_iIR2 = i_cRHO.k_iIR2;
		k_iNTAB = i_cRHO.k_iNTAB;
		k_iIMM1 = i_cRHO.k_iIMM1;
		k_iNDIV = i_cRHO.k_iNDIV;
		k_dEps = i_cRHO.k_dEps;
		m_iSeed_2 = i_cRHO.m_iSeed_2;
		m_iY = i_cRHO.m_iY;
		m_iSeed = i_cRHO.m_iSeed;
		k_dSCALAR = i_cRHO.k_dSCALAR;

		if (m_iIV != NULL)
			delete [] m_iIV;
		m_iIV = new int[k_iNDIV];
		memcpy(m_iIV,i_cRHO.m_iIV,k_iNDIV * sizeof(int));
	}
public:
	XRAND_LEcuyer(void)
	{
		k_iIM1 = 2147483563;
		k_iRANDMAX = k_iIM1;
		k_iIM2 = 2147483399;
		k_iIA1 = 40014;
		k_iIA2 = 40692;
		k_iIQ1 = 53668;
		k_iIQ2 = 52774;
		k_iIR1 = 12211;
		k_iIR2 = 3791;
		k_iNTAB = 32;
		k_iIMM1 = k_iIM1 - 1;
		k_iNDIV = 1 + k_iIMM1 / k_iNTAB;
		m_iSeed_2 = 123456789;
		m_iY = 0;
		m_iIV = new int[k_iNDIV];
		k_dEps = 3.0e-16;
		k_dSCALAR = 1.0 / k_iRANDMAX;
		Seed(-1);
	}
	XRAND_LEcuyer(const XRAND_LEcuyer & i_cRHO)
	{
		// don't know why it would be copied like this, but just in case
		m_iIV = NULL;
		Copy(i_cRHO);
	}
	XRAND_LEcuyer & operator =(const XRAND_LEcuyer &i_cRHO)
	{
		Copy(i_cRHO);
		return *this;
	}
	~XRAND_LEcuyer(void)
	{
		delete [] m_iIV;
	}

	void Seed(int i_iSeed)
	{
		int iJ,iK;

		if (i_iSeed < 0)
			i_iSeed = -i_iSeed;
		if (i_iSeed == 0)
			i_iSeed = 1;
		m_iSeed = m_iSeed_2 = i_iSeed;
		for (iJ = k_iNTAB + 7; iJ >= 0; iJ--)
		{
			iK = m_iSeed / k_iIQ1;
			m_iSeed = k_iIA1 * (m_iSeed - iK * k_iIQ1) - iK * k_iIR1;
			if (m_iSeed < 0) m_iSeed += k_iIM1;
			if (iJ < k_iNTAB) m_iIV[iJ] = m_iSeed;
		}
		m_iY = m_iIV[0];
	}
	int Generate_Random_Number(void)
	{
		int iK;
		int iJ;
		int iRet = k_iRANDMAX;

		iK = m_iSeed / k_iIQ1;
		m_iSeed = k_iIA1 * (m_iSeed - iK * k_iIQ1) - iK * k_iIR1;
		if (m_iSeed < 0) m_iSeed += k_iIM1;
		iK = m_iSeed_2 / k_iIQ2;
		m_iSeed_2 = k_iIA2 * (m_iSeed_2 - iK * k_iIQ2) - iK * k_iIR2;
		if (m_iSeed_2 < 0) m_iSeed_2 += k_iIM2;
		iJ = m_iY / k_iNDIV;
		m_iY = m_iIV[iJ] - m_iSeed_2;
		m_iIV[iJ] = m_iSeed;
		if (m_iY < 1) m_iY += k_iIMM1;
		if (m_iY <= k_iRANDMAX)
			iRet = m_iY;
		return iRet;

	}
	int GetRandInt(void)
	{
		return Generate_Random_Number();
	}
	double GetRandDouble(void)
	{
		return Generate_Random_Number() * k_dSCALAR;
	}
	double GetRandDoubleNegative(void)
	{
		return (Generate_Random_Number() * k_dSCALAR - 0.5) * 2.0;
	}
};


XRAND_TYPE	g_eXRand_Selected_Type = XRT_PM;

XRAND_Park_Miller 	g_xrand_Park_Miller;
XRAND_Knuth 		g_xrand_Knuth;
XRAND_LEcuyer 		g_xrand_LEcuyer;
XRAND *				g_lpxrand_Selected = &g_xrand_Park_Miller;

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
	case XRT_K:
		g_eXRand_Selected_Type = XRT_K;
		g_lpxrand_Selected = &g_xrand_Knuth;
		break;
	}

}
XRAND_TYPE	xrand_Get_Type(void) // get random number generator type
{
	return g_eXRand_Selected_Type;
}

int			xrandmax(void)
{
	int iRet = 0;
	if (g_lpxrand_Selected)
		iRet = g_lpxrand_Selected->GetRandMax();
	return iRet;

}

void	xsrand(int i_iSeed)
{
	if (g_lpxrand_Selected)
		g_lpxrand_Selected->Seed(i_iSeed);
}

void	xrsrand(void)
{
	unsigned int uiSeed;
        size_t iJunk;
	FILE* urandom = fopen("/dev/urandom", "r");
	iJunk = fread(&uiSeed, sizeof(uiSeed), 1, urandom);
	fclose(urandom);
	if (g_lpxrand_Selected)
		g_lpxrand_Selected->Seed(uiSeed);
}

int	xrand(void) // generate random integer number.  Depending on the generator the number may be allowed to be negative.
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

char * xPassWhitespace(char * lpString)
{
	while (xIsWhitespace(lpString))
		lpString++;
	return lpString;
}
char * xPassNumber(char * lpString)
{
	while (xIsANumber(lpString))
		lpString++;
	return lpString;
}
char * xPassSeparator(char * lpString)
{
	while (lpString && lpString[0] != 0 && 
		(lpString[0] == ' ' || lpString[0] == '\t' || lpString[0] == ','))
		lpString++;
	return lpString;
}
char * xPassSeparator(char * lpString, char chSeparator)
{
	while (lpString && lpString[0] != 0 && 
		(lpString[0] == chSeparator))
		lpString++;
	return lpString;
}
char * xPassString(char * lpString,char chString_Identifier)
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
	return lpString;
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
