#include <string.h>
#include <xlinalg.h>

// class containing an n-vector
// default constructor
XVECTOR::XVECTOR(void)
{
	m_lpdValues = NULL;
	m_uiN = 0;
	m_uiN_Alloc = 0;
}
// constructor specifying size only
XVECTOR::XVECTOR(unsigned int i_uiN)
{
	m_lpdValues = new double[i_uiN];
	m_uiN = i_uiN;
	m_uiN_Alloc = i_uiN;
}
// copy constructor
XVECTOR::XVECTOR(const XVECTOR &i_vRHO)
{
	m_lpdValues = NULL;
	m_uiN_Alloc = 0;
	Copy(i_vRHO);
}
// copy constructor
/*XVECTOR::XVECTOR(const std::vector &i_vRHO);
{
	m_lpdValues = NULL;
	m_uiN_Alloc = 0;
	Copy(i_vRHO);
}*/
// set size of vector
void	XVECTOR::Set_Size(unsigned int i_uiN)
{
	if (i_uiN >= m_uiN_Alloc)
	{
		if (m_lpdValues)
			delete [] m_lpdValues;
		m_uiN_Alloc = m_uiN = 0;
		m_lpdValues = new double[i_uiN];
		m_uiN_Alloc = m_uiN = i_uiN;
	}
	else
		m_uiN = i_uiN;
	memset(m_lpdValues,0,sizeof(double) * m_uiN);
}
// set a vector element
void	XVECTOR::Set(unsigned int i_uiRow, const double &i_dValue)
{
	if (m_lpdValues && i_uiRow < m_uiN)
	{
		m_lpdValues[i_uiRow] = i_dValue;
	}
}
// get a vector element
double	XVECTOR::Get(unsigned int i_uiRow) const
{
	double	dRet = 0.0;
	if (m_lpdValues && i_uiRow < m_uiN)
		dRet =  m_lpdValues[i_uiRow];
	return dRet;
}
// swap elements
void	XVECTOR::Swap_Rows(unsigned int i_uiRow_1, unsigned int i_uiRow_2)
{
	double	dTemp;
	if (m_lpdValues && i_uiRow_1 < m_uiN && i_uiRow_2 < m_uiN)
	{
		dTemp = m_lpdValues[i_uiRow_1];
		m_lpdValues[i_uiRow_1] = m_lpdValues[i_uiRow_2];
		m_lpdValues[i_uiRow_2] = dTemp;
	}
}
// print the matrix to file or console
void	XVECTOR::Print(FILE * fileOut) const
{
	for (unsigned int uiRow = 0; uiRow < m_uiN; uiRow++)
	{
		if (fileOut)
			fprintf(fileOut,"%u,%f\n",uiRow,m_lpdValues[uiRow]);
		else
			printf("%u,%f\n",uiRow,m_lpdValues[uiRow]);
	}
}

// Perform dot product
double	XVECTOR::Dot(const XVECTOR & i_vRHO) const
{
	double dRet = 0.0;
	if (m_uiN == i_vRHO.m_uiN)
	{
		for (unsigned int uiI = 0; uiI < m_uiN; uiI++)
			dRet += m_lpdValues[uiI] * i_vRHO.m_lpdValues[uiI];
	}
	return dRet;
}

// Perform Cross product
XVECTOR XVECTOR::Cross3d(const XVECTOR & i_vRHO) const
{
	XVECTOR vRet;
	if (m_uiN == i_vRHO.m_uiN && m_uiN == 3)
	{
		vRet.Set_Size(3);
		vRet.m_lpdValues[0] =   m_lpdValues[1] * i_vRHO.m_lpdValues[2] - m_lpdValues[2] * i_vRHO.m_lpdValues[1];
		vRet.m_lpdValues[1] = - m_lpdValues[0] * i_vRHO.m_lpdValues[2] + m_lpdValues[2] * i_vRHO.m_lpdValues[0];
		vRet.m_lpdValues[2] =   m_lpdValues[0] * i_vRHO.m_lpdValues[1] - m_lpdValues[1] * i_vRHO.m_lpdValues[0];
	}
	return vRet;
}

double XVECTOR::Cross2d(const XVECTOR & i_vRHO) const
{
	double	dRet=0.0;
	if (m_uiN == i_vRHO.m_uiN && m_uiN == 2)
	{
		dRet = m_lpdValues[0] * i_vRHO.m_lpdValues[1] - m_lpdValues[1] * i_vRHO.m_lpdValues[0];
	}
	return dRet;
}

/*
XTENSOR	XVECTOR::Cross(const XVECTOR & i_vRHO) const
{
	XTENSOR	tRet;
	if (m_uiN == i_vRHO.m_uiN)
	{
		// TBD
	}
	return tRet;
}
*/

// Aritmetic operations
XVECTOR &XVECTOR::operator =(const XVECTOR &i_vRHO)
{
	Copy(i_vRHO);
	return *this;
}
XVECTOR &XVECTOR::operator =(const std::vector<double> &i_vRHO)
{
	Set_Size(i_vRHO.size());
	for (unsigned int uiI = 0; uiI < i_vRHO.size(); uiI++)
	{
		Set(uiI,i_vRHO[uiI]);
	}
	return *this;
}
XVECTOR XVECTOR::operator +(const XVECTOR &i_vRHO) const
{
	XVECTOR	vRet;
	if (m_uiN == i_vRHO.m_uiN)
	{
		vRet.Set_Size(m_uiN);
		for (unsigned int uiI = 0; uiI < m_uiN; uiI++)
		{
			vRet.m_lpdValues[uiI] = m_lpdValues[uiI] + i_vRHO.m_lpdValues[uiI];
		}
	}
	return vRet;
}
XVECTOR &XVECTOR::operator +=(const XVECTOR &i_vRHO)
{
	if (m_uiN == i_vRHO.m_uiN)
	{
		for (unsigned int uiI = 0; uiI < m_uiN; uiI++)
		{
			m_lpdValues[uiI] += i_vRHO.m_lpdValues[uiI];
		}
	}
	return *this;
}
XVECTOR XVECTOR::operator -(const XVECTOR &i_vRHO) const
{
	XVECTOR	vRet;
	if (m_uiN == i_vRHO.m_uiN)
	{
		vRet.Set_Size(m_uiN);
		for (unsigned int uiI = 0; uiI < m_uiN; uiI++)
		{
			vRet.m_lpdValues[uiI] = m_lpdValues[uiI] - i_vRHO.m_lpdValues[uiI];
		}
	}
	return vRet;
}
XVECTOR &XVECTOR::operator -=(const XVECTOR &i_vRHO)
{
	if (m_uiN == i_vRHO.m_uiN)
	{
		for (unsigned int uiI = 0; uiI < m_uiN; uiI++)
		{
			m_lpdValues[uiI] -= i_vRHO.m_lpdValues[uiI];
		}
	}
	return *this;
}
// Scalar operatations
XVECTOR XVECTOR::operator -(void)  const
{
	XVECTOR	vRet(m_uiN);
	for (unsigned int uiI = 0; uiI < m_uiN; uiI++)
	{
		vRet.m_lpdValues[uiI] = -m_lpdValues[uiI];
	}
	return vRet;
}
XVECTOR XVECTOR::operator *(const double &i_dRHO)  const
{
	XVECTOR	vRet(m_uiN);
	for (unsigned int uiI = 0; uiI < m_uiN; uiI++)
	{
		vRet.m_lpdValues[uiI] = m_lpdValues[uiI] * i_dRHO;
	}
	return vRet;
}

XVECTOR &XVECTOR::operator *=(const double &i_dRHO)
{
	for (unsigned int uiI = 0; uiI < m_uiN; uiI++)
	{
		m_lpdValues[uiI] *= i_dRHO;
	}
	return *this;
}
XVECTOR XVECTOR::operator /(const double &i_dRHO)  const
{
	double	d1_RHO = 1. / i_dRHO;
	XVECTOR	vRet(m_uiN);
	for (unsigned int uiI = 0; uiI < m_uiN; uiI++)
	{
		vRet.m_lpdValues[uiI] = m_lpdValues[uiI] * d1_RHO;
	}
	return vRet;
}
XVECTOR &XVECTOR::operator /=(const double &i_dRHO)
{
	double	d1_RHO = 1.0 / i_dRHO;
	for (unsigned int uiI = 0; uiI < m_uiN; uiI++)
	{
		m_lpdValues[uiI] *= d1_RHO;
	}
	return *this;
}
// Boolean operations
bool	XVECTOR::operator==(const XVECTOR &i_vRHO)  const
{
	bool bRet = m_uiN == i_vRHO.m_uiN;
	for (unsigned int uiI = 0; uiI < m_uiN && bRet; uiI++)
	{
		bRet &= (m_lpdValues[uiI] == i_vRHO.m_lpdValues[uiI]);
	}
	return bRet;
}
bool	XVECTOR::operator!=(const XVECTOR &i_vRHO)  const
{
	bool bRet = m_uiN == i_vRHO.m_uiN;
	for (unsigned int uiI = 0; uiI < m_uiN && bRet; uiI++)
	{
		bRet &= (m_lpdValues[uiI] == i_vRHO.m_lpdValues[uiI]);
	}
	return !bRet;
}

void XVECTOR::Copy(const XVECTOR &i_vRHO)
{
	if (m_uiN_Alloc < i_vRHO.m_uiN)
	{
		if (m_lpdValues)
			delete [] m_lpdValues;
		m_lpdValues = new double[i_vRHO.m_uiN];
		m_uiN_Alloc = i_vRHO.m_uiN;
	}
	m_uiN = i_vRHO.m_uiN;
	memcpy(m_lpdValues,i_vRHO.m_lpdValues, i_vRHO.m_uiN * sizeof(double));
}

/*void XVECTOR::Copy(const std::vector &i_vRHO)
{
	if (m_uiN_Alloc < i_vRHO.size())
	{
		if (m_lpdValues)
			delete [] m_lpdValues;
		m_lpdValues = new double[i_vRHO.size()];
		m_uiN_Alloc = i_vRHO.size();
	}
	m_uiN = i_vRHO.size();
	for (unsigned int uiI = 0; uiI < m_uiN; uiI++)
		m_lpdValues[uiI] = i_vRHO.at(uiI);

}*/

XVECTOR operator * (const double &i_dLHO, const XVECTOR &i_vRHO)
{
	return i_vRHO * i_dLHO;
}

// Vector multiply: x_i = l_i * r_i
XVECTOR	XVECTOR::Vector_Multiply(const XVECTOR & i_vRHO) const
{
	XVECTOR vRet;
	if (m_uiN == i_vRHO.m_uiN)
	{
		vRet.Set_Size(m_uiN);
		for (unsigned int uiI = 0; uiI < m_uiN; uiI++)
		{
			vRet.m_lpdValues[uiI] = m_lpdValues[uiI] * i_vRHO.m_lpdValues[uiI];
		}
	}
	return vRet;
}

// Normalize the vector
void XVECTOR::Normalize(void)
{
	double dMag = Magnitude();
	if (dMag != 0.0)
	{
		double	dNorm = 1.0 / dMag;
		for (unsigned int uiI = 0; uiI < m_uiN; uiI++)
		{
			m_lpdValues[uiI] *= dNorm;
		}
	}
}
// Set all elements to zero
void XVECTOR::Zero(void)
{
	if (m_lpdValues)
	{
		memset(m_lpdValues,0,m_uiN_Alloc * sizeof(double));
	}
}

// Get the magnitude of a vector
double	XVECTOR::Magnitude(void) const
{
	double	dMagSqr = Dot(*this);
	return sqrt(dMagSqr);
}

