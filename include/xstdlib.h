#pragma once
#include <stdlib.h>
#include <cstring>
#include <stdarg.h>
#include <wctype.h>
#include <wchar.h>
#include <string>

// test if a number is prime
bool xTest_Prime(unsigned int i_uiI);


// Random number generator types: 
// PM = Park & Miller (Numerical Recipes ran0)
// LE = L'Euyer (Numerical Recipes ran1)
// K = Knuth  (Numerical Recipes ran3)

enum XRAND_TYPE {XRT_PM,XRT_LE,XRT_K,XRT_KM}; 

void		xrand_Set_Type(XRAND_TYPE eType); // select random number generator
XRAND_TYPE	xrand_Get_Type(void); // get random number generator type

unsigned int	xsrand(unsigned int iSeed); //seed the random number generator
unsigned int	xrsrand(void); // randomly seed the random number generator
unsigned int		xrand(void); // generate a random number between -(XRANDMAX - 1) and (XRANDMAX - 1)
double	xrand_d(void); // generate number between 0 and 1
double	xrand_dm(void); // genearte number between -1 and 1
void	xrand_gauss_bm(double & o_dY1,double & o_dY2, const double &i_dSigma_Y1 = 1.0, const double &i_dSigma_Y2 = 1.0); // generates two gaussian random numbers with mean = 0 and standard deviation 1
double	xrand_gauss(const double & i_dMean = 0.0, const double &i_dStDev = 1.0); // generate a single gaussian random number with given mean and standard deviation
unsigned int		xrandmax(void);

#define XRANDMAX xrandmax() // legacy definition 

double XRoundNearest(const double & i_dValue, const double &i_dRound_To);
double XRoundUp(const double & i_dValue, const double &i_dRound_To);
double XRoundDown(const double & i_dValue, const double &i_dRound_To);

// Round an unsigned integer to the nearest power of 2 (i.e. 1->1, 2->2, 3->4)
void XRound2(unsigned int &i_uiValue);
// round an unsigned integer to nearest multiple of 4096
void XRound4k(unsigned int &i_uiValue);


class XSTRING
{
private:
	wchar_t *		m_lpszString; // not necessarily stored as char, just a placeholder for actual data.
	char *			m_lpszCharString;
	unsigned int	m_uiCurrent_Length;
	unsigned int	m_uiAllocated_Length;
	unsigned int	m_uiCharStringLength;

	void Initialize(void)
	{
		m_lpszString = NULL; 
		m_uiAllocated_Length = 0;
		m_uiCurrent_Length = 0;
		m_uiCharStringLength = 0;
		m_lpszCharString = NULL;
	}
	void Deallocate(void)
	{
		if (m_lpszString)
			delete [] m_lpszString; 
		if (m_lpszCharString)
			delete [] m_lpszCharString; 
		Initialize();
	}

	void GenerateCharString(void)
	{
		if (m_uiCurrent_Length > 0)
		{
			if (m_uiCurrent_Length > m_uiCharStringLength)
			{
				unsigned int uiPhysical_Length = m_uiCurrent_Length;
				if (m_lpszCharString)
					delete [] m_lpszCharString;
				if (uiPhysical_Length > 4096) // larger than a page, round up to nearest page
					XRound4k(uiPhysical_Length);
				else // less than a page, round up to multiple of 2.
					XRound2(uiPhysical_Length);
				m_uiCharStringLength = uiPhysical_Length;

				m_lpszCharString = new char[m_uiCharStringLength];
				if (m_lpszCharString)
					memset(m_lpszCharString,0,uiPhysical_Length);
				else
					m_uiAllocated_Length = 0;
			}
			for (unsigned int uiI = 0; uiI < m_uiCurrent_Length; uiI++)
			{
				m_lpszCharString[uiI] = (char)m_lpszString[uiI];
			}
		}
	}

	void Allocate(unsigned int i_uiLength) // desired string length, not memory size
	{
		if (i_uiLength > m_uiAllocated_Length)
		{
			unsigned int uiPhysical_Length = i_uiLength * sizeof(wchar_t);
			if (m_lpszString)
				delete [] m_lpszString;
			if (uiPhysical_Length > 4096) // larger than a page, round up to nearest page
				XRound4k(uiPhysical_Length);
			else // less than a page, round up to multiple of 2.
				XRound2(uiPhysical_Length);
			m_uiAllocated_Length = uiPhysical_Length / sizeof(wchar_t);

			m_lpszString = new wchar_t[m_uiAllocated_Length];
			if (m_lpszString)
				memset(m_lpszString,0,uiPhysical_Length);
			else
				m_uiAllocated_Length = 0;
		}
	}
	// copy a standard string
	void	Copy(const char * i_lpszString) 
	{
		if (i_lpszString)
		{
			unsigned int uiStringLen = strlen(i_lpszString);
			Allocate(uiStringLen + 1);
			for (unsigned int uiI = 0; uiI < uiStringLen; uiI++)
			{
				m_lpszString[uiI] = (wchar_t)i_lpszString[uiI];
			}
			m_uiCurrent_Length = uiStringLen;
		}
		else
		{
			memset(m_lpszString,0,m_uiCurrent_Length * sizeof(wchar_t));
			m_uiCurrent_Length = 0;
		}
	}
	// copy a wide string
	void	Copy(const wchar_t * i_lpszString) 
	{
		if (i_lpszString)
		{
			unsigned int uiStringLen = wcslen(i_lpszString);
			Allocate(uiStringLen + 1); // allocate a string
			wcscpy(m_lpszString,i_lpszString);
		}
		else
		{
			memset(m_lpszString,0,m_uiCurrent_Length * sizeof(wchar_t));
			m_uiCurrent_Length = 0;
		}
	}
	// copy from string class
	void	Copy(const XSTRING &i_cCopy_From)
	{
		if (i_cCopy_From.m_lpszString)
		{
			Allocate(i_cCopy_From.m_uiCurrent_Length + 1); // allocate a string
			wcscpy(m_lpszString,i_cCopy_From.m_lpszString);
		}
		else
		{
			memset(m_lpszString,0,m_uiCurrent_Length * sizeof(wchar_t));
			m_uiCurrent_Length = 0;
		}
	}

public:
	XSTRING & operator = (const XSTRING &i_sRHO)
	{
		Copy(i_sRHO);
		return *this;
	}
	XSTRING & operator = (const char * &i_sRHO)
	{
		Copy(i_sRHO);
		return *this;
	}
	XSTRING & operator = (const wchar_t * &i_sRHO)
	{
		Copy(i_sRHO);
		return *this;
	}
	const char * GetCharString(void) {GenerateCharString(); return m_lpszCharString;}
	const wchar_t * GetWideString(void) const {return m_lpszString;}
	int	Compare(const XSTRING & i_cRHO, unsigned int i_uiLen_To_Cmp = 0)
	{
		int iRet = -1;
		if (m_lpszString && i_cRHO.m_lpszString)
		{
			if (i_uiLen_To_Cmp == 0)
				iRet = wcscmp(m_lpszString,i_cRHO.m_lpszString);
			else
				iRet = wcsncmp(m_lpszString,i_cRHO.m_lpszString,i_uiLen_To_Cmp);
		}
		return iRet;
	}
	unsigned int GetLength(void) const {return m_uiCurrent_Length;}
	bool	operator < (const XSTRING &i_cRHO) { return (Compare(i_cRHO) < 0);}
	bool	operator <= (const XSTRING &i_cRHO) { return (Compare(i_cRHO) <= 0);}
	bool	operator > (const XSTRING &i_cRHO) { return (Compare(i_cRHO) > 0);}
	bool	operator >= (const XSTRING &i_cRHO) { return (Compare(i_cRHO) >= 0);}
	bool	operator == (const XSTRING &i_cRHO) { return (Compare(i_cRHO) == 0);}
	bool	operator != (const XSTRING &i_cRHO) { return (Compare(i_cRHO) != 0);}
	wchar_t operator [] (unsigned int i_uiI) const{wchar_t cRet = 0; if (i_uiI < m_uiCurrent_Length) cRet = m_lpszString[i_uiI]; return cRet;}
	XSTRING(void){Initialize();}
	XSTRING(const char * i_lpszInit_String){Initialize();Copy(i_lpszInit_String);}
	XSTRING(const wchar_t * i_lpszInit_String){Initialize();Copy(i_lpszInit_String);}
	XSTRING(const XSTRING &i_cInit_Name){Initialize();Copy(i_cInit_Name.m_lpszString);}
	~XSTRING(void){Deallocate();}

};

bool xIsANumber(const char * lpString);
bool xIsAScientificNumber(const char * i_lpszString);
bool xIsWhitespace(const char * lpString);
char * xPassWhitespace(const char * lpString);
char * xPassNumber(const char * lpString);
char * xPassSeparator(const char * lpString);
char * xPassSeparator(const char * lpString, char chSeparator);
char * xPassString(const char * lpString,char chString_Identifier);

double	xGetNumberFromEndOfString(const char * i_lpszString);
char *	xGetLastWordInString(const char * i_lpszString);

const char * xParse_Command_Line_Data_Ptr(int iArgC,const char * lpszArgV[],const char * i_lpszOption);
bool xParse_Command_Line_Exists(int iArgC,const char * lpszArgV[],const char * i_lpszOption);
double xParse_Command_Line_Dbl(int iArgC,const char * lpszArgV[],const char * i_lpszOption, const double &dDefault_Value = -1.0e300);
bool xParse_Command_Line_Bool(int iArgC,const char * lpszArgV[],const char * i_lpszOption, const double &bDefault_Value = false);
unsigned int xParse_Command_Line_UInt(int iArgC,const char * lpszArgV[],const char * i_lpszOption, unsigned int i_uiDefault_Value = -1);
int xParse_Command_Line_Int(int iArgC,const char * lpszArgV[],const char * i_lpszOption, int i_uiDefault_Value = -1);
void xParse_Command_Line_String(int iArgC,const char * lpszArgV[],const char * i_lpszOption, char * o_lpszParameter, unsigned int i_uiMax_Parameter_Size, const char * i_lpszDefault_Value = NULL);

void xRomanNumeralGenerator(char * lpszResult, unsigned int uiI);
void xGet_Element_Symbol(unsigned int i_uiAtomic_Number, char * o_lpszElement_Symbol); // o_lpszElement_Symbol must be of size char [3] or longer; some elements require char [4]

unsigned int xChoose_Allocation_Size_Word_Aligned(unsigned int i_uiNumber_Of_Elements, unsigned int i_uiElement_Size);
unsigned int xChoose_Allocation_Size_Word_Aligned_Quiet(unsigned int i_uiNumber_Of_Elements, unsigned int i_uiElement_Size);
template <typename T> unsigned int xChoose_Allocation_Size_Word_Aligned(unsigned int i_uiNumber_Of_Elements)
{
	return xChoose_Allocation_Size_Word_Aligned(i_uiNumber_Of_Elements,sizeof(T));
}
template <typename T> unsigned int xChoose_Allocation_Size_Word_Aligned_Quiet(unsigned int i_uiNumber_Of_Elements)
{
	return xChoose_Allocation_Size_Word_Aligned_Quiet(i_uiNumber_Of_Elements,sizeof(T));
}


unsigned long long XMin_ui64(unsigned int i_uiNum_Parameters,...);
long long XMin_i64(unsigned int i_uiNum_Parameters,...);
unsigned int XMin_ui(unsigned int i_uiNum_Parameters,...);
int XMin_i(unsigned int i_uiNum_Parameters,...);
unsigned short XMin_us(unsigned int i_uiNum_Parameters,...);
short XMin_s(unsigned int i_uiNum_Parameters,...);
unsigned char XMin_uc(unsigned int i_uiNum_Parameters,...);
char XMin_c(unsigned int i_uiNum_Parameters,...);
double XMin_d(unsigned int i_uiNum_Parameters,...);
float XMin_f(unsigned int i_uiNum_Parameters,...);
unsigned long long XMax_ui64(unsigned int i_uiNum_Parameters,...);
long long XMax_i64(unsigned int i_uiNum_Parameters,...);
unsigned int XMax_ui(unsigned int i_uiNum_Parameters,...);
int XMax_i(unsigned int i_uiNum_Parameters,...);
unsigned short XMax_us(unsigned int i_uiNum_Parameters,...);
short XMax_s(unsigned int i_uiNum_Parameters,...);
unsigned char XMax_uc(unsigned int i_uiNum_Parameters,...);
char XMax_c(unsigned int i_uiNum_Parameters,...);
double XMax_d(unsigned int i_uiNum_Parameters,...);
float XMax_f(unsigned int i_uiNum_Parameters,...);


namespace xconsole
{
	extern std::string foreground_black;
	extern std::string foreground_red;
	extern std::string foreground_green;
	extern std::string foreground_blue;
	extern std::string foreground_yellow;
	extern std::string foreground_cyan;
	extern std::string foreground_magenta;
	extern std::string foreground_gray;
	extern std::string foreground_grey;
	extern std::string foreground_white;

	extern std::string background_black;
	extern std::string background_red;
	extern std::string background_green;
	extern std::string background_blue;
	extern std::string background_yellow;
	extern std::string background_cyan;
	extern std::string background_magenta;
	extern std::string background_gray;
	extern std::string background_grey;
	extern std::string background_white;

	extern std::string reset;
	extern std::string bold;
	extern std::string faint;
	extern std::string italic;
	extern std::string underline;
	extern std::string blink_slow;
	extern std::string blink_fast;
	extern std::string invert;
};

namespace xstdlib
{
	enum datatype {empty, logical, hex, octal, binary, integer, floating_point, string};
	datatype identify_datatype(const std::string i_szString);



	void printbit(uint8_t i_cVal);
	void pbin(void * i_lpdData, size_t tN_Bytes);
	template<typename T>  void pbin(const T & i_ldData)
	{
		pbin((void *)&i_ldData,sizeof(T));
	}
}
