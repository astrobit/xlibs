#include <string.h>
#include <xlinalg.h>

// class containing an n-vector
// default constructor
xvector::xvector(void)
{
	m_lpdValues = NULL;
	m_uiN = 0;
	m_uiN_Alloc = 0;
}
// constructor specifying size only
xvector::xvector(unsigned int i_uiN)
{
	m_lpdValues = new double[i_uiN];
	m_uiN = i_uiN;
	m_uiN_Alloc = i_uiN;
}
// copy constructor
xvector::xvector(const xvector &i_vRHO)
{
	m_lpdValues = NULL;
	m_uiN = 0;
	m_uiN_Alloc = 0;
	Copy(i_vRHO);
}
// copy constructor
xvector::xvector(const std::vector<double> &i_vRHO)
{
	m_lpdValues = NULL;
	m_uiN = 0;
	m_uiN_Alloc = 0;
	Copy(i_vRHO);
}
	// destructor
xvector::~xvector(void)
{
	if (m_lpdValues)
		delete [] m_lpdValues;
	m_lpdValues = NULL;
	m_uiN_Alloc = 0;
	m_uiN = 0;
}

// set size of vector
void	xvector::Set_Size(unsigned int i_uiN)
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
void	xvector::Set(unsigned int i_uiRow, const double &i_dValue)
{
	if (m_lpdValues && i_uiRow < m_uiN)
	{
		m_lpdValues[i_uiRow] = i_dValue;
	}
}
// get a vector element
double	xvector::Get(unsigned int i_uiRow) const
{
	double	dRet = 0.0;
	if (m_lpdValues && i_uiRow < m_uiN)
		dRet =  m_lpdValues[i_uiRow];
	return dRet;
}
// swap elements
void	xvector::Swap_Rows(unsigned int i_uiRow_1, unsigned int i_uiRow_2)
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
void	xvector::Print(FILE * fileOut) const
{
	for (unsigned int uiRow = 0; uiRow < m_uiN; uiRow++)
	{
		if (!fileOut)
			fileOut = stdout;
		fprintf(fileOut,"%u,%f\n",uiRow,m_lpdValues[uiRow]);
		fflush(fileOut);
	}
}

// Perform dot product
double	xvector::Dot(const xvector & i_vRHO) const
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
xvector xvector::Cross3d(const xvector & i_vRHO) const
{
	xvector vRet;
	if (m_uiN == i_vRHO.m_uiN && m_uiN == 3)
	{
		vRet.Set_Size(3);
		vRet.m_lpdValues[0] =   m_lpdValues[1] * i_vRHO.m_lpdValues[2] - m_lpdValues[2] * i_vRHO.m_lpdValues[1];
		vRet.m_lpdValues[1] = - m_lpdValues[0] * i_vRHO.m_lpdValues[2] + m_lpdValues[2] * i_vRHO.m_lpdValues[0];
		vRet.m_lpdValues[2] =   m_lpdValues[0] * i_vRHO.m_lpdValues[1] - m_lpdValues[1] * i_vRHO.m_lpdValues[0];
	}
	return vRet;
}

double xvector::Cross2d(const xvector & i_vRHO) const
{
	double	dRet=0.0;
	if (m_uiN == i_vRHO.m_uiN && m_uiN == 2)
	{
		dRet = m_lpdValues[0] * i_vRHO.m_lpdValues[1] - m_lpdValues[1] * i_vRHO.m_lpdValues[0];
	}
	return dRet;
}

/*
XTENSOR	xvector::Cross(const xvector & i_vRHO) const
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
xvector &xvector::operator =(const xvector &i_vRHO)
{
	Copy(i_vRHO);
	return *this;
}

xvector &xvector::operator =(const std::vector<double> &i_vRHO)
{
	Copy(i_vRHO);
//	Set_Size(i_vRHO.size());
//	for (unsigned int uiI = 0; uiI < i_vRHO.size(); uiI++)
//	{
//		Set(uiI,i_vRHO[uiI]);
//	}
	return *this;
}
xvector xvector::operator +(const xvector &i_vRHO) const
{
	xvector	vRet;
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
xvector &xvector::operator +=(const xvector &i_vRHO)
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
xvector xvector::operator -(const xvector &i_vRHO) const
{
	xvector	vRet;
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
xvector &xvector::operator -=(const xvector &i_vRHO)
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
xvector xvector::operator -(void)  const
{
	xvector	vRet(m_uiN);
	for (unsigned int uiI = 0; uiI < m_uiN; uiI++)
	{
		vRet.m_lpdValues[uiI] = -m_lpdValues[uiI];
	}
	return vRet;
}
xvector xvector::operator *(const double &i_dRHO)  const
{
	xvector	vRet(m_uiN);
	for (unsigned int uiI = 0; uiI < m_uiN; uiI++)
	{
		vRet.m_lpdValues[uiI] = m_lpdValues[uiI] * i_dRHO;
	}
	return vRet;
}

xvector &xvector::operator *=(const double &i_dRHO)
{
	for (unsigned int uiI = 0; uiI < m_uiN; uiI++)
	{
		m_lpdValues[uiI] *= i_dRHO;
	}
	return *this;
}
xvector xvector::operator /(const double &i_dRHO)  const
{
	double	d1_RHO = 1. / i_dRHO;
	xvector	vRet(m_uiN);
	for (unsigned int uiI = 0; uiI < m_uiN; uiI++)
	{
		vRet.m_lpdValues[uiI] = m_lpdValues[uiI] * d1_RHO;
	}
	return vRet;
}
xvector &xvector::operator /=(const double &i_dRHO)
{
	double	d1_RHO = 1.0 / i_dRHO;
	for (unsigned int uiI = 0; uiI < m_uiN; uiI++)
	{
		m_lpdValues[uiI] *= d1_RHO;
	}
	return *this;
}
// Boolean operations
bool	xvector::operator==(const xvector &i_vRHO)  const
{
	bool bRet = m_uiN == i_vRHO.m_uiN;
	for (unsigned int uiI = 0; uiI < m_uiN && bRet; uiI++)
	{
		bRet &= (m_lpdValues[uiI] == i_vRHO.m_lpdValues[uiI]);
	}
	return bRet;
}
bool	xvector::operator!=(const xvector &i_vRHO)  const
{
	bool bRet = m_uiN == i_vRHO.m_uiN;
	for (unsigned int uiI = 0; uiI < m_uiN && bRet; uiI++)
	{
		bRet &= (m_lpdValues[uiI] == i_vRHO.m_lpdValues[uiI]);
	}
	return !bRet;
}

void xvector::Copy(const xvector &i_vRHO)
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

void xvector::Copy(const std::vector<double> &i_vRHO)
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
		m_lpdValues[uiI] = i_vRHO[uiI];

}

xvector operator * (const double &i_dLHO, const xvector &i_vRHO)
{
	return i_vRHO * i_dLHO;
}

// Vector multiply: x_i = l_i * r_i
xvector	xvector::Vector_Multiply(const xvector & i_vRHO) const
{
	xvector vRet;
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
void xvector::Normalize(void)
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
void xvector::Zero(void)
{
	if (m_lpdValues)
	{
		memset(m_lpdValues,0,m_uiN_Alloc * sizeof(double));
	}
}

// Get the magnitude of a vector
double	xvector::Magnitude(void) const
{
	double	dMagSqr = Dot(*this);
	return sqrt(dMagSqr);
}

bool xvector::is_nan(void) const
{
	bool bRet = false;
	for (unsigned int uiI = 0; uiI < m_uiN && !bRet; uiI++)
		bRet = isnan(m_lpdValues[uiI]);
	return bRet;
}

bool xvector::is_inf(void) const
{
	bool bRet = false;
	for (unsigned int uiI = 0; uiI < m_uiN && !bRet; uiI++)
		bRet = isinf(m_lpdValues[uiI]);
	return bRet;
}

