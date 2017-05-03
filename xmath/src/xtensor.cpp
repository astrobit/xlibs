#include <xlinalg.h>
#include <string.h>


void xtensor::Allocate(const xindex_vector	&i_uivDimensions)
{
	unsigned int uiRank = i_uivDimensions.Get_Size();
	unsigned int uiSize = 1;
	unsigned int uiSizeLast = 0;
	bool			bOverflow_Err = false;
	if (uiRank > 0)
	{
		for (unsigned uiI = 0; uiI < uiRank && !bOverflow_Err; uiI++)
		{
			uiSize *= i_uivDimensions.m_lpuiValues[uiI];
			bOverflow_Err = (uiSize < uiSizeLast);
			uiSizeLast = uiSize;
		}
		if (bOverflow_Err)
		{
			m_uiRank = 0;
			m_uiNum_Entries = 0;
		}
		else
		{
			if (uiSize > m_uiSize_Alloc)
			{
				if (m_lpdValues)
					delete [] m_lpdValues;
				m_lpdValues = new double [uiSize];
				if (!m_lpdValues)
				{
					m_uiRank = 0;
					m_uiSize_Alloc = 0;
					m_uiNum_Entries = 0;
				}
				else
					m_uiSize_Alloc = uiSize;
			}
			if (m_lpdValues)
			{
				m_uiNum_Entries = uiSize;
				m_uivDimensions = i_uivDimensions;
				m_uiRank = uiRank;
				unsigned int uiCurr = 1;
				for (unsigned int uiI = m_uiRank - 1; uiI < m_uiRank; uiI++)
				{
					m_uivIndexing_Vector.m_lpuiValues[uiI] = uiCurr;
					uiCurr *= i_uivDimensions.m_lpuiValues[uiI];
				}
			}
		}
	}
	else
	{
		m_uiRank = 0;
		m_uiNum_Entries = 0;
	}

}

void			xtensor::Copy(const xtensor &i_tRHO)
{
	// ensure allocatioj is sufficient
	Allocate(i_tRHO.m_uivDimensions);
	if (m_uiRank > 0)
		memcpy(m_lpdValues,i_tRHO.m_lpdValues,m_uiNum_Entries * sizeof(double));
}

// 
unsigned int	xtensor::ComputeIndex(const xindex_vector	&i_uivElement_Index) const
{
	unsigned int uiIndex = 0x0ffffffff;
	if (m_uiRank > 0)
	{
		bool	bValid_Index = true;
		uiIndex = 0;
		for (unsigned int uiI = 0; uiI < m_uiRank && bValid_Index; uiI++)
		{
			bValid_Index &= (i_uivElement_Index.m_lpuiValues[uiI] < m_uivDimensions.m_lpuiValues[uiI]);
			uiIndex += i_uivElement_Index.m_lpuiValues[uiI] * m_uivIndexing_Vector.m_lpuiValues[uiI];
		}
	}
	return uiIndex;
}

// default constructor
xtensor::xtensor(void)
{
	m_lpdValues = NULL;
	m_uiRank = 0;
	m_uiNum_Entries = 0;
	m_uiSize_Alloc = 0;
}
// constructor sith specified size
xtensor::xtensor(const xindex_vector &i_vDimensions)
{
	m_lpdValues = NULL;
	m_uiRank = 0;
	m_uiNum_Entries = 0;
	m_uiSize_Alloc = 0;
	Allocate(i_vDimensions);
}
// copy constructor
xtensor::xtensor(const xtensor &i_tRHO)
{
	m_lpdValues = NULL;
	m_uiRank = 0;
	m_uiNum_Entries = 0;
	m_uiSize_Alloc = 0;
	Copy(i_tRHO);
}
// destructor
xtensor::~xtensor(void)
{
	if (m_lpdValues)
		delete [] m_lpdValues;
	m_lpdValues = NULL;
	m_uiRank = 0;
	m_uivDimensions.Zero();
	m_uivIndexing_Vector.Zero();
	m_uiNum_Entries = 0;
	m_uiSize_Alloc = 0;
}

// set size of tensor
void	xtensor::Set_Size(const xindex_vector & i_uivDimensions)
{
	Allocate(i_uivDimensions);
}
// set tensor element
void	xtensor::Set(const xindex_vector & i_uivElement_Index, const double &i_dValue)
{
	unsigned int uiIndex = ComputeIndex(i_uivElement_Index);
	if (uiIndex < m_uiNum_Entries)
		m_lpdValues[uiIndex] = i_dValue;
}
// get tensor element
double	xtensor::Get(const xindex_vector & i_uivElement_Index) const
{
	double	dRet = 0.0;
	unsigned int uiIndex = ComputeIndex(i_uivElement_Index);
	if (uiIndex < m_uiNum_Entries)
		dRet = m_lpdValues[uiIndex];
	return dRet;
}
