#include <string.h>
#include <xlinalgep.h>

// class containing an n-vector
// default constructor
xvector_ep::xvector_ep(void)
{
	m_lpdValues = nullptr;
	m_uiN = 0;
	m_uiN_Alloc = 0;
}
// constructor specifying size only
xvector_ep::xvector_ep(size_t i_uiN)
{
	m_lpdValues = new expdouble[i_uiN];
	m_uiN = i_uiN;
	m_uiN_Alloc = i_uiN;
}
// copy constructor
xvector_ep::xvector_ep(const xvector_ep &i_vRHO)
{
	m_lpdValues = nullptr;
	m_uiN = 0;
	m_uiN_Alloc = 0;
	Copy(i_vRHO);
}
// copy constructor
xvector_ep::xvector_ep(const std::vector<expdouble> &i_vRHO)
{
	m_lpdValues = nullptr;
	m_uiN = 0;
	m_uiN_Alloc = 0;
	Copy(i_vRHO);
}
// copy constructor
xvector_ep::xvector_ep(const std::vector<long double> &i_vRHO)
{
	m_lpdValues = nullptr;
	m_uiN = 0;
	m_uiN_Alloc = 0;
	Copy(i_vRHO);
}
// copy constructor
xvector_ep::xvector_ep(const std::vector<double> &i_vRHO)
{
	m_lpdValues = nullptr;
	m_uiN = 0;
	m_uiN_Alloc = 0;
	Copy(i_vRHO);
}
// copy constructor
xvector_ep::xvector_ep(const std::vector<float> &i_vRHO)
{
	m_lpdValues = nullptr;
	m_uiN = 0;
	m_uiN_Alloc = 0;
	Copy(i_vRHO);
}
// copy constructor
xvector_ep::xvector_ep(const std::vector<int64_t> &i_vRHO)
{
	m_lpdValues = nullptr;
	m_uiN = 0;
	m_uiN_Alloc = 0;
	Copy(i_vRHO);
}
xvector_ep::xvector_ep(const std::vector<uint64_t> &i_vRHO)
{
	m_lpdValues = nullptr;
	m_uiN = 0;
	m_uiN_Alloc = 0;
	Copy(i_vRHO);
}
xvector_ep::xvector_ep(const std::vector<int32_t> &i_vRHO)
{
	m_lpdValues = nullptr;
	m_uiN = 0;
	m_uiN_Alloc = 0;
	Copy(i_vRHO);
}
xvector_ep::xvector_ep(const std::vector<uint32_t> &i_vRHO)
{
	m_lpdValues = nullptr;
	m_uiN = 0;
	m_uiN_Alloc = 0;
	Copy(i_vRHO);
}
xvector_ep::xvector_ep(const std::vector<int16_t> &i_vRHO)
{
	m_lpdValues = nullptr;
	m_uiN = 0;
	m_uiN_Alloc = 0;
	Copy(i_vRHO);
}
xvector_ep::xvector_ep(const std::vector<uint16_t> &i_vRHO)
{
	m_lpdValues = nullptr;
	m_uiN = 0;
	m_uiN_Alloc = 0;
	Copy(i_vRHO);
}
xvector_ep::xvector_ep(const std::vector<int8_t> &i_vRHO)
{
	m_lpdValues = nullptr;
	m_uiN = 0;
	m_uiN_Alloc = 0;
	Copy(i_vRHO);
}
xvector_ep::xvector_ep(const std::vector<uint8_t> &i_vRHO)
{
	m_lpdValues = nullptr;
	m_uiN = 0;
	m_uiN_Alloc = 0;
	Copy(i_vRHO);
}
	// destructor
xvector_ep::~xvector_ep(void)
{
	if (m_lpdValues)
		delete [] m_lpdValues;
	m_lpdValues = nullptr;
	m_uiN_Alloc = 0;
	m_uiN = 0;
}

// set size of vector
void	xvector_ep::Set_Size(size_t i_uiN)
{
	if (i_uiN >= m_uiN_Alloc)
	{
		if (m_lpdValues)
			delete [] m_lpdValues;
		m_uiN_Alloc = m_uiN = 0;
		m_lpdValues = new expdouble[i_uiN];
		m_uiN_Alloc = m_uiN = i_uiN;
	}
	else
		m_uiN = i_uiN;
	memset(m_lpdValues,0,sizeof(expdouble) * m_uiN);
}
// set a vector element
void	xvector_ep::Set(size_t i_uiRow, const expdouble &i_dValue)
{
	if (m_lpdValues && i_uiRow < m_uiN)
	{
		m_lpdValues[i_uiRow] = i_dValue;
	}
}
// get a vector element
expdouble	xvector_ep::Get(size_t i_uiRow) const
{
	expdouble	dRet = 0.0;
	if (m_lpdValues && i_uiRow < m_uiN)
		dRet =  m_lpdValues[i_uiRow];
	return dRet;
}
// swap elements
void	xvector_ep::Swap_Rows(size_t i_uiRow_1, size_t i_uiRow_2)
{
	expdouble	dTemp;
	if (m_lpdValues && i_uiRow_1 < m_uiN && i_uiRow_2 < m_uiN)
	{
		dTemp = m_lpdValues[i_uiRow_1];
		m_lpdValues[i_uiRow_1] = m_lpdValues[i_uiRow_2];
		m_lpdValues[i_uiRow_2] = dTemp;
	}
}
// print the matrix to file or console
void	xvector_ep::Print(FILE * fileOut) const
{
	for (size_t uiRow = 0; uiRow < m_uiN; uiRow++)
	{
		if (!fileOut)
			fileOut = stdout;
		fprintf(fileOut,"%u,%s\n",uiRow,m_lpdValues[uiRow].get_b10_value(6,true).c_str());
		fflush(fileOut);
	}
}

// Perform dot product
expdouble	xvector_ep::Dot(const xvector_ep & i_vRHO) const
{
	expdouble dRet = 0.0;
	if (m_uiN == i_vRHO.m_uiN)
	{

//#pragma omp parallel for reduction(+:dRet) //@@TODO: since we are using expdouble we can't use built in reduction in OpenMP, so need to develop a method to do this
		for (size_t uiI = 0; uiI < m_uiN; uiI++)
			dRet += m_lpdValues[uiI] * i_vRHO.m_lpdValues[uiI];
	}
	return dRet;
}

// Perform Cross product
xvector_ep xvector_ep::Cross3d(const xvector_ep & i_vRHO) const
{
	xvector_ep vRet;
	if (m_uiN == i_vRHO.m_uiN && m_uiN == 3)
	{
		vRet.Set_Size(3);
		vRet.m_lpdValues[0] =   m_lpdValues[1] * i_vRHO.m_lpdValues[2] - m_lpdValues[2] * i_vRHO.m_lpdValues[1];
		vRet.m_lpdValues[1] = - m_lpdValues[0] * i_vRHO.m_lpdValues[2] + m_lpdValues[2] * i_vRHO.m_lpdValues[0];
		vRet.m_lpdValues[2] =   m_lpdValues[0] * i_vRHO.m_lpdValues[1] - m_lpdValues[1] * i_vRHO.m_lpdValues[0];
	}
	return vRet;
}

expdouble xvector_ep::Cross2d(const xvector_ep & i_vRHO) const
{
	expdouble	dRet=0.0;
	if (m_uiN == i_vRHO.m_uiN && m_uiN == 2)
	{
		dRet = m_lpdValues[0] * i_vRHO.m_lpdValues[1] - m_lpdValues[1] * i_vRHO.m_lpdValues[0];
	}
	return dRet;
}

/*
XTENSOR	xvector_ep::Cross(const xvector_ep & i_vRHO) const
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
xvector_ep &xvector_ep::operator =(const xvector_ep &i_vRHO)
{
	Copy(i_vRHO);
	return *this;
}

xvector_ep &xvector_ep::operator =(const std::vector<expdouble> &i_vRHO)
{
	Copy(i_vRHO);
	return *this;
}
xvector_ep &xvector_ep::operator =(const std::vector<double> &i_vRHO)
{
	Copy(i_vRHO);
	return *this;
}
xvector_ep &xvector_ep::operator =(const std::vector<long double> &i_vRHO)
{
	Copy(i_vRHO);
	return *this;
}
xvector_ep &xvector_ep::operator =(const std::vector<float> &i_vRHO)
{
	Copy(i_vRHO);
	return *this;
}
xvector_ep &xvector_ep::operator =(const std::vector<uint64_t> &i_vRHO)
{
	Copy(i_vRHO);
	return *this;
}
xvector_ep &xvector_ep::operator =(const std::vector<int64_t> &i_vRHO)
{
	Copy(i_vRHO);
	return *this;
}
xvector_ep &xvector_ep::operator =(const std::vector<uint32_t> &i_vRHO)
{
	Copy(i_vRHO);
	return *this;
}
xvector_ep &xvector_ep::operator =(const std::vector<int32_t> &i_vRHO)
{
	Copy(i_vRHO);
	return *this;
}
xvector_ep &xvector_ep::operator =(const std::vector<uint16_t> &i_vRHO)
{
	Copy(i_vRHO);
	return *this;
}
xvector_ep &xvector_ep::operator =(const std::vector<int16_t> &i_vRHO)
{
	Copy(i_vRHO);
	return *this;
}
xvector_ep &xvector_ep::operator =(const std::vector<uint8_t> &i_vRHO)
{
	Copy(i_vRHO);
	return *this;
}
xvector_ep &xvector_ep::operator =(const std::vector<int8_t> &i_vRHO)
{
	Copy(i_vRHO);
	return *this;
}

xvector_ep xvector_ep::operator +(const xvector_ep &i_vRHO) const
{
	xvector_ep	vRet;
	if (m_uiN == i_vRHO.m_uiN)
	{
		vRet.Set_Size(m_uiN);
#pragma omp for
		for (size_t uiI = 0; uiI < m_uiN; uiI++)
		{
			vRet.m_lpdValues[uiI] = m_lpdValues[uiI] + i_vRHO.m_lpdValues[uiI];
		}
	}
	return vRet;
}
xvector_ep &xvector_ep::operator +=(const xvector_ep &i_vRHO)
{
	if (m_uiN == i_vRHO.m_uiN)
	{
#pragma omp for
		for (size_t uiI = 0; uiI < m_uiN; uiI++)
		{
			m_lpdValues[uiI] += i_vRHO.m_lpdValues[uiI];
		}
	}
	return *this;
}
xvector_ep xvector_ep::operator -(const xvector_ep &i_vRHO) const
{
	xvector_ep	vRet;
	if (m_uiN == i_vRHO.m_uiN)
	{
		vRet.Set_Size(m_uiN);
#pragma omp for
		for (size_t uiI = 0; uiI < m_uiN; uiI++)
		{
			vRet.m_lpdValues[uiI] = m_lpdValues[uiI] - i_vRHO.m_lpdValues[uiI];
		}
	}
	return vRet;
}
xvector_ep &xvector_ep::operator -=(const xvector_ep &i_vRHO)
{
	if (m_uiN == i_vRHO.m_uiN)
	{
#pragma omp for
		for (size_t uiI = 0; uiI < m_uiN; uiI++)
		{
			m_lpdValues[uiI] -= i_vRHO.m_lpdValues[uiI];
		}
	}
	return *this;
}

// Scalar operatations
xvector_ep xvector_ep::operator -(void)  const
{
	xvector_ep	vRet(m_uiN);
#pragma omp for
		for (size_t uiI = 0; uiI < m_uiN; uiI++)
	{
		vRet.m_lpdValues[uiI] = -m_lpdValues[uiI];
	}
	return vRet;
}
xvector_ep xvector_ep::operator *(const expdouble &i_dRHO)  const
{
	xvector_ep	vRet(m_uiN);
#pragma omp for
		for (size_t uiI = 0; uiI < m_uiN; uiI++)
	{
		vRet.m_lpdValues[uiI] = m_lpdValues[uiI] * i_dRHO;
	}
	return vRet;
}

xvector_ep &xvector_ep::operator *=(const expdouble &i_dRHO)
{
#pragma omp for
	for (size_t uiI = 0; uiI < m_uiN; uiI++)
	{
		m_lpdValues[uiI] *= i_dRHO;
	}
	return *this;
}
xvector_ep xvector_ep::operator /(const expdouble &i_dRHO)  const
{
	expdouble	d1_RHO = expdouble(1.0) / i_dRHO;
	xvector_ep	vRet(m_uiN);
#pragma omp for
	for (size_t uiI = 0; uiI < m_uiN; uiI++)
	{
		vRet.m_lpdValues[uiI] = m_lpdValues[uiI] * d1_RHO;
	}
	return vRet;
}
xvector_ep &xvector_ep::operator /=(const expdouble &i_dRHO)
{
	expdouble	d1_RHO = expdouble(1.0) / i_dRHO;
#pragma omp for
	for (size_t uiI = 0; uiI < m_uiN; uiI++)
	{
		m_lpdValues[uiI] *= d1_RHO;
	}
	return *this;
}
// Boolean operations
bool	xvector_ep::operator==(const xvector_ep &i_vRHO)  const
{
	bool bRet = m_uiN == i_vRHO.m_uiN;
#pragma omp for
	for (size_t uiI = 0; uiI < m_uiN; uiI++)
	{
		bRet &= (m_lpdValues[uiI] == i_vRHO.m_lpdValues[uiI]);
	}
	return bRet;
}
bool	xvector_ep::operator!=(const xvector_ep &i_vRHO)  const
{
	bool bRet = m_uiN == i_vRHO.m_uiN;
#pragma omp for
	for (size_t uiI = 0; uiI < m_uiN; uiI++)
	{
		bRet &= (m_lpdValues[uiI] == i_vRHO.m_lpdValues[uiI]);
	}
	return !bRet;
}

void xvector_ep::Copy(const xvector_ep &i_vRHO)
{
	if (m_uiN_Alloc < i_vRHO.m_uiN)
	{
		if (m_lpdValues)
			delete [] m_lpdValues;
		m_lpdValues = new expdouble[i_vRHO.m_uiN];
		m_uiN_Alloc = i_vRHO.m_uiN;
	}
	m_uiN = i_vRHO.m_uiN;
	memcpy(m_lpdValues,i_vRHO.m_lpdValues, i_vRHO.m_uiN * sizeof(expdouble));
}

void xvector_ep::Copy(const std::vector<expdouble> &i_vRHO)
{
	if (m_uiN_Alloc < i_vRHO.size())
	{
		if (m_lpdValues)
			delete [] m_lpdValues;
		m_lpdValues = new expdouble[i_vRHO.size()];
		m_uiN_Alloc = i_vRHO.size();
	}
	m_uiN = i_vRHO.size();
#pragma omp for
	for (size_t uiI = 0; uiI < m_uiN; uiI++)
		m_lpdValues[uiI] = i_vRHO[uiI];

}
void xvector_ep::Copy(const std::vector<long double> &i_vRHO)
{
	if (m_uiN_Alloc < i_vRHO.size())
	{
		if (m_lpdValues)
			delete [] m_lpdValues;
		m_lpdValues = new expdouble[i_vRHO.size()];
		m_uiN_Alloc = i_vRHO.size();
	}
	m_uiN = i_vRHO.size();
#pragma omp for
	for (size_t uiI = 0; uiI < m_uiN; uiI++)
		m_lpdValues[uiI] = i_vRHO[uiI];

}
void xvector_ep::Copy(const std::vector<double> &i_vRHO)
{
	if (m_uiN_Alloc < i_vRHO.size())
	{
		if (m_lpdValues)
			delete [] m_lpdValues;
		m_lpdValues = new expdouble[i_vRHO.size()];
		m_uiN_Alloc = i_vRHO.size();
	}
	m_uiN = i_vRHO.size();
#pragma omp for
	for (size_t uiI = 0; uiI < m_uiN; uiI++)
		m_lpdValues[uiI] = i_vRHO[uiI];

}
void xvector_ep::Copy(const std::vector<float> &i_vRHO)
{
	if (m_uiN_Alloc < i_vRHO.size())
	{
		if (m_lpdValues)
			delete [] m_lpdValues;
		m_lpdValues = new expdouble[i_vRHO.size()];
		m_uiN_Alloc = i_vRHO.size();
	}
	m_uiN = i_vRHO.size();
#pragma omp for
	for (size_t uiI = 0; uiI < m_uiN; uiI++)
		m_lpdValues[uiI] = i_vRHO[uiI];

}
void xvector_ep::Copy(const std::vector<uint64_t> &i_vRHO)
{
	if (m_uiN_Alloc < i_vRHO.size())
	{
		if (m_lpdValues)
			delete [] m_lpdValues;
		m_lpdValues = new expdouble[i_vRHO.size()];
		m_uiN_Alloc = i_vRHO.size();
	}
	m_uiN = i_vRHO.size();
#pragma omp for
	for (size_t uiI = 0; uiI < m_uiN; uiI++)
		m_lpdValues[uiI] = i_vRHO[uiI];

}
void xvector_ep::Copy(const std::vector<int64_t> &i_vRHO)
{
	if (m_uiN_Alloc < i_vRHO.size())
	{
		if (m_lpdValues)
			delete [] m_lpdValues;
		m_lpdValues = new expdouble[i_vRHO.size()];
		m_uiN_Alloc = i_vRHO.size();
	}
	m_uiN = i_vRHO.size();
#pragma omp for
	for (size_t uiI = 0; uiI < m_uiN; uiI++)
		m_lpdValues[uiI] = i_vRHO[uiI];

}

void xvector_ep::Copy(const std::vector<uint32_t> &i_vRHO)
{
	if (m_uiN_Alloc < i_vRHO.size())
	{
		if (m_lpdValues)
			delete [] m_lpdValues;
		m_lpdValues = new expdouble[i_vRHO.size()];
		m_uiN_Alloc = i_vRHO.size();
	}
	m_uiN = i_vRHO.size();
#pragma omp for
	for (size_t uiI = 0; uiI < m_uiN; uiI++)
		m_lpdValues[uiI] = i_vRHO[uiI];

}
void xvector_ep::Copy(const std::vector<int32_t> &i_vRHO)
{
	if (m_uiN_Alloc < i_vRHO.size())
	{
		if (m_lpdValues)
			delete [] m_lpdValues;
		m_lpdValues = new expdouble[i_vRHO.size()];
		m_uiN_Alloc = i_vRHO.size();
	}
	m_uiN = i_vRHO.size();
#pragma omp for
	for (size_t uiI = 0; uiI < m_uiN; uiI++)
		m_lpdValues[uiI] = i_vRHO[uiI];

}
void xvector_ep::Copy(const std::vector<uint16_t> &i_vRHO)
{
	if (m_uiN_Alloc < i_vRHO.size())
	{
		if (m_lpdValues)
			delete [] m_lpdValues;
		m_lpdValues = new expdouble[i_vRHO.size()];
		m_uiN_Alloc = i_vRHO.size();
	}
	m_uiN = i_vRHO.size();
#pragma omp for
	for (size_t uiI = 0; uiI < m_uiN; uiI++)
		m_lpdValues[uiI] = i_vRHO[uiI];

}
void xvector_ep::Copy(const std::vector<int16_t> &i_vRHO)
{
	if (m_uiN_Alloc < i_vRHO.size())
	{
		if (m_lpdValues)
			delete [] m_lpdValues;
		m_lpdValues = new expdouble[i_vRHO.size()];
		m_uiN_Alloc = i_vRHO.size();
	}
	m_uiN = i_vRHO.size();
#pragma omp for
	for (size_t uiI = 0; uiI < m_uiN; uiI++)
		m_lpdValues[uiI] = i_vRHO[uiI];

}

void xvector_ep::Copy(const std::vector<uint8_t> &i_vRHO)
{
	if (m_uiN_Alloc < i_vRHO.size())
	{
		if (m_lpdValues)
			delete [] m_lpdValues;
		m_lpdValues = new expdouble[i_vRHO.size()];
		m_uiN_Alloc = i_vRHO.size();
	}
	m_uiN = i_vRHO.size();
#pragma omp for
	for (size_t uiI = 0; uiI < m_uiN; uiI++)
		m_lpdValues[uiI] = i_vRHO[uiI];

}


void xvector_ep::Copy(const std::vector<int8_t> &i_vRHO)
{
	if (m_uiN_Alloc < i_vRHO.size())
	{
		if (m_lpdValues)
			delete [] m_lpdValues;
		m_lpdValues = new expdouble[i_vRHO.size()];
		m_uiN_Alloc = i_vRHO.size();
	}
	m_uiN = i_vRHO.size();
#pragma omp for
	for (size_t uiI = 0; uiI < m_uiN; uiI++)
		m_lpdValues[uiI] = i_vRHO[uiI];

}



xvector_ep operator * (const expdouble &i_dLHO, const xvector_ep &i_vRHO)
{
	return i_vRHO * i_dLHO;
}

// Vector multiply: x_i = l_i * r_i
xvector_ep	xvector_ep::Vector_Multiply(const xvector_ep & i_vRHO) const
{
	xvector_ep vRet;
	if (m_uiN == i_vRHO.m_uiN)
	{
		vRet.Set_Size(m_uiN);
#pragma omp for
		for (size_t uiI = 0; uiI < m_uiN; uiI++)
		{
			vRet.m_lpdValues[uiI] = m_lpdValues[uiI] * i_vRHO.m_lpdValues[uiI];
		}
	}
	return vRet;
}

// Normalize the vector
void xvector_ep::Normalize(void)
{
	expdouble dMag = Magnitude();
	if (dMag != 0.0)
	{
		expdouble	dNorm = expdouble(1.0) / dMag;
#pragma omp for
		for (size_t uiI = 0; uiI < m_uiN; uiI++)
		{
			m_lpdValues[uiI] *= dNorm;
		}
	}
}
// Set all elements to zero
void xvector_ep::Zero(void)
{
	if (m_lpdValues)
	{
		memset(m_lpdValues,0,m_uiN_Alloc * sizeof(expdouble));
	}
}

// Get the magnitude of a vector
expdouble	xvector_ep::Magnitude(void) const
{
	expdouble	dMagSqr = Dot(*this);
	return dMagSqr.sqrt();
}

bool xvector_ep::is_nan(void) const
{
	bool bRet = false;
#pragma omp parallel for reduction(||:bRet)
	for (size_t uiI = 0; uiI < m_uiN; uiI++)
		bRet |= m_lpdValues[uiI].isnan();
	return bRet;
}

bool xvector_ep::is_inf(void) const
{
	bool bRet = false;
#pragma omp parallel for reduction(||:bRet) 
	for (size_t uiI = 0; uiI < m_uiN; uiI++)
		bRet |= m_lpdValues[uiI].isinf();
	return bRet;
}


