#include <string.h>
#include <xlinalg.h>

// class containing an n-vector
// default constructor
XINDEX_VECTOR::XINDEX_VECTOR(void)
{
	m_lpuiValues = NULL;
	m_uiN = 0;
	m_uiN_Alloc = 0;
}
// constructor specifying size only
XINDEX_VECTOR::XINDEX_VECTOR(unsigned int i_uiN)
{
	m_lpuiValues = new unsigned int[i_uiN];
	m_uiN = i_uiN;
	m_uiN_Alloc = i_uiN;
}
// copy constructor
XINDEX_VECTOR::XINDEX_VECTOR(const XINDEX_VECTOR &i_cRHO)
{
	m_uiN_Alloc = 0;
	m_lpuiValues = NULL;
	Copy(i_cRHO);
}
// destructor
XINDEX_VECTOR::~XINDEX_VECTOR(void)
{
	m_uiN_Alloc = 0;
	m_uiN = 0;
	if (m_lpuiValues)
		delete [] m_lpuiValues;
	m_lpuiValues = NULL;
}
// set size of vector
void	XINDEX_VECTOR::Set_Size(unsigned int i_uiN)
{
	if (i_uiN >= m_uiN_Alloc)
	{
		if (m_lpuiValues)
			delete [] m_lpuiValues;
		m_uiN_Alloc = m_uiN = 0;
		m_lpuiValues = new unsigned int[i_uiN];
		m_uiN_Alloc = m_uiN = i_uiN;
	}
	else
		m_uiN = i_uiN;
	memset(m_lpuiValues,0,sizeof(unsigned int) * m_uiN);
}
// set a vector element
void	XINDEX_VECTOR::Set(unsigned int i_uiRow, const unsigned int &i_uiValue)
{
	if (m_lpuiValues && i_uiRow < m_uiN)
	{
		m_lpuiValues[i_uiRow] = i_uiValue;
	}
}
// get a vector element
unsigned int	XINDEX_VECTOR::Get(unsigned int i_uiRow)
{
	unsigned int	uiRet = 0;
	if (m_lpuiValues && i_uiRow < m_uiN)
		uiRet =  m_lpuiValues[i_uiRow];
	return uiRet;
}
// swap elements
void	XINDEX_VECTOR::Swap_Rows(unsigned int i_uiRow_1, unsigned int i_uiRow_2)
{
	unsigned int	uiTemp;
	if (m_lpuiValues && i_uiRow_1 < m_uiN && i_uiRow_2 < m_uiN)
	{
		uiTemp = m_lpuiValues[i_uiRow_1];
		m_lpuiValues[i_uiRow_1] = m_lpuiValues[i_uiRow_2];
		m_lpuiValues[i_uiRow_2] = uiTemp;
	}
}
// print the matrix to file or console
void	XINDEX_VECTOR::Print(FILE * fileOut)
{
	for (unsigned int uiRow = 0; uiRow < m_uiN; uiRow++)
	{
		if (fileOut)
			fprintf(fileOut,"%u,%i\n",uiRow,m_lpuiValues[uiRow]);
		else
			printf("%u,%i\n",uiRow,m_lpuiValues[uiRow]);
	}
}

// Aritmetic operations
XINDEX_VECTOR &XINDEX_VECTOR::operator =(const XINDEX_VECTOR &i_vRHO)
{
	Copy(i_vRHO);
	return *this;
}

XINDEX_VECTOR XINDEX_VECTOR::operator +(const XINDEX_VECTOR &i_vRHO) const
{
	XINDEX_VECTOR	vRet;
	if (m_uiN == i_vRHO.m_uiN)
	{
		vRet.Set_Size(m_uiN);
		for (unsigned int uiI = 0; uiI < m_uiN; uiI++)
		{
			vRet.m_lpuiValues[uiI] = m_lpuiValues[uiI] + i_vRHO.m_lpuiValues[uiI];
		}
	}
	return vRet;
}
XINDEX_VECTOR &XINDEX_VECTOR::operator +=(const XINDEX_VECTOR &i_vRHO)
{
	if (m_uiN == i_vRHO.m_uiN)
	{
		for (unsigned int uiI = 0; uiI < m_uiN; uiI++)
		{
			m_lpuiValues[uiI] += i_vRHO.m_lpuiValues[uiI];
		}
	}
	return *this;
}
XINDEX_VECTOR XINDEX_VECTOR::operator -(const XINDEX_VECTOR &i_vRHO) const
{
	XINDEX_VECTOR	vRet;
	if (m_uiN == i_vRHO.m_uiN)
	{
		vRet.Set_Size(m_uiN);
		for (unsigned int uiI = 0; uiI < m_uiN; uiI++)
		{
			vRet.m_lpuiValues[uiI] = m_lpuiValues[uiI] - i_vRHO.m_lpuiValues[uiI];
		}
	}
	return vRet;
}
XINDEX_VECTOR &XINDEX_VECTOR::operator -=(const XINDEX_VECTOR &i_vRHO)
{
	if (m_uiN == i_vRHO.m_uiN)
	{
		for (unsigned int uiI = 0; uiI < m_uiN; uiI++)
		{
			m_lpuiValues[uiI] -= i_vRHO.m_lpuiValues[uiI];
		}
	}
	return *this;
}
// Scalar operatations
XINDEX_VECTOR XINDEX_VECTOR::operator -(void)  const
{
	XINDEX_VECTOR	vRet(m_uiN);
	for (unsigned int uiI = 0; uiI < m_uiN; uiI++)
	{
		vRet.m_lpuiValues[uiI] = -m_lpuiValues[uiI];
	}
	return vRet;
}
// Boolean operations
bool	XINDEX_VECTOR::operator==(const XINDEX_VECTOR &i_vRHO)  const
{
	bool bRet = m_uiN == i_vRHO.m_uiN;
	for (unsigned int uiI = 0; uiI < m_uiN && bRet; uiI++)
	{
		bRet &= (m_lpuiValues[uiI] == i_vRHO.m_lpuiValues[uiI]);
	}
	return bRet;
}
bool	XINDEX_VECTOR::operator!=(const XINDEX_VECTOR &i_vRHO)  const
{
	bool bRet = m_uiN == i_vRHO.m_uiN;
	for (unsigned int uiI = 0; uiI < m_uiN && bRet; uiI++)
	{
		bRet &= (m_lpuiValues[uiI] == i_vRHO.m_lpuiValues[uiI]);
	}
	return !bRet;
}

void XINDEX_VECTOR::Copy(const XINDEX_VECTOR &i_vRHO)
{
	if (m_uiN_Alloc < i_vRHO.m_uiN)
	{
		if (m_lpuiValues)
			delete [] m_lpuiValues;
		m_lpuiValues = new unsigned int[i_vRHO.m_uiN];
		m_uiN_Alloc = i_vRHO.m_uiN;
	}
	m_uiN = i_vRHO.m_uiN;
	memcpy(m_lpuiValues,i_vRHO.m_lpuiValues, i_vRHO.m_uiN * sizeof(double));
}
void XINDEX_VECTOR::Zero(void)
{
	if (m_uiN_Alloc > 0 && m_lpuiValues)
		memset(m_lpuiValues,0,sizeof(unsigned int) * m_uiN_Alloc);
}


