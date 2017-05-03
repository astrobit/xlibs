#include <string.h>
#include <xlinalg.h>

// class containing an n-vector
// default constructor
xindex_vector::xindex_vector(void)
{
	m_lpuiValues = NULL;
	m_uiN = 0;
	m_uiN_Alloc = 0;
}
// constructor specifying size only
xindex_vector::xindex_vector(unsigned int i_uiN)
{
	m_lpuiValues = new unsigned int[i_uiN];
	m_uiN = i_uiN;
	m_uiN_Alloc = i_uiN;
}
// copy constructor
xindex_vector::xindex_vector(const xindex_vector &i_cRHO)
{
	m_uiN_Alloc = 0;
	m_lpuiValues = NULL;
	Copy(i_cRHO);
}
// destructor
xindex_vector::~xindex_vector(void)
{
	m_uiN_Alloc = 0;
	m_uiN = 0;
	if (m_lpuiValues)
		delete [] m_lpuiValues;
	m_lpuiValues = NULL;
}
// set size of vector
void	xindex_vector::Set_Size(unsigned int i_uiN)
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
void	xindex_vector::Set(unsigned int i_uiRow, const unsigned int &i_uiValue)
{
	if (m_lpuiValues && i_uiRow < m_uiN)
	{
		m_lpuiValues[i_uiRow] = i_uiValue;
	}
}
// get a vector element
unsigned int	xindex_vector::Get(unsigned int i_uiRow)
{
	unsigned int	uiRet = 0;
	if (m_lpuiValues && i_uiRow < m_uiN)
		uiRet =  m_lpuiValues[i_uiRow];
	return uiRet;
}
// swap elements
void	xindex_vector::Swap_Rows(unsigned int i_uiRow_1, unsigned int i_uiRow_2)
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
void	xindex_vector::Print(FILE * fileOut)
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
xindex_vector &xindex_vector::operator =(const xindex_vector &i_vRHO)
{
	Copy(i_vRHO);
	return *this;
}

xindex_vector xindex_vector::operator +(const xindex_vector &i_vRHO) const
{
	xindex_vector	vRet;
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
xindex_vector &xindex_vector::operator +=(const xindex_vector &i_vRHO)
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
xindex_vector xindex_vector::operator -(const xindex_vector &i_vRHO) const
{
	xindex_vector	vRet;
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
xindex_vector &xindex_vector::operator -=(const xindex_vector &i_vRHO)
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
xindex_vector xindex_vector::operator -(void)  const
{
	xindex_vector	vRet(m_uiN);
	for (unsigned int uiI = 0; uiI < m_uiN; uiI++)
	{
		vRet.m_lpuiValues[uiI] = -m_lpuiValues[uiI];
	}
	return vRet;
}
// Boolean operations
bool	xindex_vector::operator==(const xindex_vector &i_vRHO)  const
{
	bool bRet = m_uiN == i_vRHO.m_uiN;
	for (unsigned int uiI = 0; uiI < m_uiN && bRet; uiI++)
	{
		bRet &= (m_lpuiValues[uiI] == i_vRHO.m_lpuiValues[uiI]);
	}
	return bRet;
}
bool	xindex_vector::operator!=(const xindex_vector &i_vRHO)  const
{
	bool bRet = m_uiN == i_vRHO.m_uiN;
	for (unsigned int uiI = 0; uiI < m_uiN && bRet; uiI++)
	{
		bRet &= (m_lpuiValues[uiI] == i_vRHO.m_lpuiValues[uiI]);
	}
	return !bRet;
}

void xindex_vector::Copy(const xindex_vector &i_vRHO)
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
void xindex_vector::Zero(void)
{
	if (m_uiN_Alloc > 0 && m_lpuiValues)
		memset(m_lpuiValues,0,sizeof(unsigned int) * m_uiN_Alloc);
}


