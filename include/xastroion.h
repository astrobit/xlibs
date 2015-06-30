#pragma once
#include <stdarg.h>
#include <xastro.h>

class XASTRO_ATOMIC_IONIZATION_DATA
{
public:
	unsigned int m_uiZ;
	unsigned int m_uiIonization_Energies_Count;
	double * m_lpIonization_energies_erg;
private:
	void Copy(const XASTRO_ATOMIC_IONIZATION_DATA &i_cRHO)
	{
		if (m_lpIonization_energies_erg && m_uiIonization_Energies_Count < i_cRHO.m_uiIonization_Energies_Count)
		{
			delete [] m_lpIonization_energies_erg;
			m_lpIonization_energies_erg =  NULL;
		}
		if (m_lpIonization_energies_erg == NULL)
			m_lpIonization_energies_erg = new double[i_cRHO.m_uiIonization_Energies_Count];

		memcpy(m_lpIonization_energies_erg,i_cRHO.m_lpIonization_energies_erg,i_cRHO.m_uiIonization_Energies_Count * sizeof(double));
		m_uiIonization_Energies_Count = i_cRHO.m_uiIonization_Energies_Count;
		m_uiZ = i_cRHO.m_uiZ;
		
	}
public:
	void Clean(void)
	{
		m_uiZ = 0;
		m_uiIonization_Energies_Count = 0;
		if (m_lpIonization_energies_erg)
			delete [] m_lpIonization_energies_erg;
		m_lpIonization_energies_erg = NULL;
	}
	XASTRO_ATOMIC_IONIZATION_DATA(void)
	{
		m_uiZ = 0;
		m_uiIonization_Energies_Count = 0;
		m_lpIonization_energies_erg = NULL;
	}
	~XASTRO_ATOMIC_IONIZATION_DATA(void)
	{
		Clean();
	}
	XASTRO_ATOMIC_IONIZATION_DATA(const XASTRO_ATOMIC_IONIZATION_DATA &i_cRHO)
	{
		m_uiZ = 0;
		m_uiIonization_Energies_Count = 0;
		m_lpIonization_energies_erg = NULL;
		Copy(i_cRHO);
	}
	XASTRO_ATOMIC_IONIZATION_DATA(unsigned int i_uiZ, unsigned int i_uiIonization_Energies_Count, ...)
	{
		va_list lpvaArg_Ptr;
		va_start(lpvaArg_Ptr,i_uiIonization_Energies_Count);
		m_uiZ = i_uiZ;
		m_uiIonization_Energies_Count = i_uiIonization_Energies_Count;
		m_lpIonization_energies_erg = new double[i_uiIonization_Energies_Count];
		for (unsigned int uiI = 0; uiI < i_uiIonization_Energies_Count; uiI++)
		{
			m_lpIonization_energies_erg[uiI] = va_arg(lpvaArg_Ptr,double);
		}
		va_end(lpvaArg_Ptr);
	}
	XASTRO_ATOMIC_IONIZATION_DATA & operator =(const XASTRO_ATOMIC_IONIZATION_DATA i_cRHO)
	{
		Copy(i_cRHO);
		return *this;
	}
};

class XASTRO_ATOMIC_IONIZATION_DATA_LI
{
public:
	XASTRO_ATOMIC_IONIZATION_DATA m_cData;
	XASTRO_ATOMIC_IONIZATION_DATA_LI * m_lpNext;
	XASTRO_ATOMIC_IONIZATION_DATA_LI * m_lpPrev;

	XASTRO_ATOMIC_IONIZATION_DATA_LI(void) : m_cData() {m_lpNext = m_lpPrev = NULL;}
	XASTRO_ATOMIC_IONIZATION_DATA_LI(const XASTRO_ATOMIC_IONIZATION_DATA &i_cRHO) : m_cData(i_cRHO) {m_lpNext = m_lpPrev = NULL;}
	XASTRO_ATOMIC_IONIZATION_DATA_LI(unsigned int i_uiZ, unsigned int i_uiIonization_Energies_Count, ...) : m_cData()
	{
		unsigned int uiI;
		va_list lpvaArg_Ptr;
		va_start(lpvaArg_Ptr,i_uiIonization_Energies_Count);
		m_cData.m_uiZ = i_uiZ;
		m_cData.m_uiIonization_Energies_Count = i_uiIonization_Energies_Count;
		m_cData.m_lpIonization_energies_erg = new double[m_cData.m_uiIonization_Energies_Count];
		for (uiI = 0; uiI < i_uiIonization_Energies_Count; uiI++)
		{
			m_cData.m_lpIonization_energies_erg[uiI] = va_arg(lpvaArg_Ptr,double);
		}
		va_end(lpvaArg_Ptr);

		m_lpNext = m_lpPrev = NULL;
	}
	~XASTRO_ATOMIC_IONIZATION_DATA_LI(void)
	{
		m_cData.Clean();
		m_lpNext = NULL;
		m_lpPrev = NULL;
	}
		
	XASTRO_ATOMIC_IONIZATION_DATA_LI & operator =(const XASTRO_ATOMIC_IONIZATION_DATA_LI i_cRHO)
	{
		m_cData = i_cRHO.m_cData;
		return *this;
	}
};

class XASTRO_ATOMIC_IONIZATION_DATA_LIST
{
private:
	unsigned int				m_uiNum_List_Items;
	unsigned int *				m_uiQuick_Find_Z;
	XASTRO_ATOMIC_IONIZATION_DATA **	m_lpcQuick_Find_Data;

	
	XASTRO_ATOMIC_IONIZATION_DATA_LI * m_lpHead;
	XASTRO_ATOMIC_IONIZATION_DATA_LI * m_lpTail;
	
	void Initialize(void);

public:
	XASTRO_ATOMIC_IONIZATION_DATA_LIST(void)
	{
		m_lpHead = NULL;
		m_lpTail = NULL;
		m_uiNum_List_Items = 0;
		m_uiQuick_Find_Z = NULL;
		m_lpcQuick_Find_Data = NULL;
		Initialize();
	}
	~XASTRO_ATOMIC_IONIZATION_DATA_LIST(void)
	{
		XASTRO_ATOMIC_IONIZATION_DATA_LI * lpCurr = m_lpHead;
		while (lpCurr)
		{
			m_lpHead = lpCurr->m_lpNext;
			delete lpCurr;
			lpCurr = m_lpHead;
		}
		if (m_uiQuick_Find_Z)
			delete [] m_uiQuick_Find_Z;
		memset(m_lpcQuick_Find_Data,0,m_uiNum_List_Items * sizeof (XASTRO_ATOMIC_IONIZATION_DATA **));
		if (m_lpcQuick_Find_Data)
			delete [] m_lpcQuick_Find_Data;

		m_lpHead = NULL;
		m_lpTail = NULL;
		m_uiNum_List_Items = 0;
		m_uiQuick_Find_Z = NULL;
		m_lpcQuick_Find_Data = NULL;
	}

	
	unsigned int Find_Z_Index(unsigned int i_uiZ) const;

	XASTRO_ATOMIC_IONIZATION_DATA	Get_Ionization_Data(unsigned int i_uiZ) const;
	XASTRO_ATOMIC_IONIZATION_DATA *	Get_Ionization_Data_Ptr(unsigned int i_uiZ) const;
};
extern XASTRO_ATOMIC_IONIZATION_DATA_LIST	g_xAstro_Ionization_Energy_Data;


class XASTRO_ATOMIC_PARTITION_FUNCTION_DATA
{
	// This class is used for estimating the partition function for ionized atoms
	// method is based on Cardona, Matrinez-Arroyo & Lopez-Castillo, ApJ 711:239-245 (2010)
public:
	unsigned int 	m_uiZ;
	unsigned int	m_uiNum_Entries;
	unsigned int *	m_lpuiJ;
	double *		m_lpdE;
	unsigned int *	m_lpuiG;
	unsigned int *	m_lpuiM;
	double * 		m_lpdChi;
	double 			m_dE1;
	unsigned int	m_uiG1;

	void	Copy(const XASTRO_ATOMIC_PARTITION_FUNCTION_DATA & i_cRHO)
	{
		m_uiZ = i_cRHO.m_uiZ;
		if (m_uiNum_Entries > 0 && m_uiNum_Entries < i_cRHO.m_uiNum_Entries)
		{
			delete [] m_lpuiJ;	m_lpuiJ = NULL;
			delete [] m_lpdE;	m_lpdE = NULL;
			delete [] m_lpuiG;	m_lpuiG = NULL;
			delete [] m_lpuiM;	m_lpuiM = NULL;
			delete [] m_lpdChi;	m_lpdChi = NULL;
			m_uiNum_Entries = 0;
		}
		if (m_uiNum_Entries == 0)
		{
			m_uiNum_Entries = i_cRHO.m_uiNum_Entries;
			m_lpuiJ = new unsigned int[m_uiNum_Entries];
			m_lpdE = new double[m_uiNum_Entries];
			m_lpuiG = new unsigned int[m_uiNum_Entries];
			m_lpuiM = new unsigned int[m_uiNum_Entries];
			m_lpdChi = new double[m_uiNum_Entries];
		}
		memcpy(m_lpuiJ,i_cRHO.m_lpuiJ,sizeof(unsigned int) * m_uiNum_Entries);
		memcpy(m_lpdE,i_cRHO.m_lpdE,sizeof(double) * m_uiNum_Entries);
		memcpy(m_lpuiG,i_cRHO.m_lpuiG,sizeof(unsigned int) * m_uiNum_Entries);
		memcpy(m_lpuiM,i_cRHO.m_lpuiM,sizeof(unsigned int) * m_uiNum_Entries);
		memcpy(m_lpdChi,i_cRHO.m_lpdChi,sizeof(double) * m_uiNum_Entries);

		m_dE1 = i_cRHO.m_dE1;
		m_uiG1 = i_cRHO.m_uiG1;
	}
	void Clean(void)
	{
		m_uiZ = 0;
		m_uiNum_Entries = 0;
		if (m_lpuiJ)
			delete [] m_lpuiJ;
		m_lpuiJ = NULL;
		if (m_lpuiG)
			delete [] m_lpuiG;
		m_lpuiG = NULL;
		if (m_lpdE)
			delete [] m_lpdE;
		m_lpdE = NULL;
		m_dE1 = 0.0;
		m_uiG1 = 0;
	}

	XASTRO_ATOMIC_PARTITION_FUNCTION_DATA(void)
	{
		m_uiZ = 0;
		m_uiNum_Entries = 0;
		m_lpuiJ = m_lpuiG = NULL;
		m_lpdE = NULL;
		m_dE1 = 0.0;
		m_uiG1 = 0;
		m_lpuiM = NULL;
		m_lpdChi = NULL;
	}
	~XASTRO_ATOMIC_PARTITION_FUNCTION_DATA(void)
	{
		Clean();
	}
	XASTRO_ATOMIC_PARTITION_FUNCTION_DATA(const XASTRO_ATOMIC_PARTITION_FUNCTION_DATA & i_cRHO)
	{
		m_uiZ = 0;
		m_uiNum_Entries = 0;
		m_lpuiJ = m_lpuiG = NULL;
		m_lpdE = NULL;
		m_dE1 = 0.0;
		m_uiG1 = 0;
		m_lpuiM = NULL;
		m_lpdChi = NULL;
		Copy(i_cRHO);
	}
	XASTRO_ATOMIC_PARTITION_FUNCTION_DATA & operator =(const XASTRO_ATOMIC_PARTITION_FUNCTION_DATA & i_cRHO)
	{
		Copy(i_cRHO);
		return *this;
	}
	
	double	Get_Partition_Function(unsigned int i_uiIonization_State, const  double &i_dTemperature, const double &i_dNumber_Density) const;
		

};


class XASTRO_ATOMIC_PARTITION_FUNCTION_DATA_LI
{
public:
	XASTRO_ATOMIC_PARTITION_FUNCTION_DATA m_cData;
	XASTRO_ATOMIC_PARTITION_FUNCTION_DATA_LI * m_lpNext;
	XASTRO_ATOMIC_PARTITION_FUNCTION_DATA_LI * m_lpPrev;

	XASTRO_ATOMIC_PARTITION_FUNCTION_DATA_LI(void) : m_cData() {m_lpNext = m_lpPrev = NULL;}
	XASTRO_ATOMIC_PARTITION_FUNCTION_DATA_LI(const XASTRO_ATOMIC_PARTITION_FUNCTION_DATA &i_cRHO) : m_cData(i_cRHO) {m_lpNext = m_lpPrev = NULL;}
	XASTRO_ATOMIC_PARTITION_FUNCTION_DATA_LI & operator =(const XASTRO_ATOMIC_PARTITION_FUNCTION_DATA_LI i_cRHO)
	{
		m_cData = i_cRHO.m_cData;
		return *this;
	}

	~XASTRO_ATOMIC_PARTITION_FUNCTION_DATA_LI(void) {m_cData.Clean(); m_lpNext = NULL; m_lpPrev = NULL;}
};

class XASTRO_ATOMIC_PARTITION_FUNCTION_DATA_LIST
{
private:
	unsigned int				m_uiNum_List_Items;
	unsigned int *				m_uiQuick_Find_Z;
	XASTRO_ATOMIC_PARTITION_FUNCTION_DATA **	m_lpcQuick_Find_Data;

	
	XASTRO_ATOMIC_PARTITION_FUNCTION_DATA_LI * m_lpHead;
	XASTRO_ATOMIC_PARTITION_FUNCTION_DATA_LI * m_lpTail;

	void Initialize(void);

public:
	XASTRO_ATOMIC_PARTITION_FUNCTION_DATA_LIST(void)
	{
		m_lpHead = NULL;
		m_lpTail = NULL;
		m_uiNum_List_Items = 0;
		m_uiQuick_Find_Z = NULL;
		m_lpcQuick_Find_Data = NULL;
		Initialize();
	}
	~XASTRO_ATOMIC_PARTITION_FUNCTION_DATA_LIST(void)
	{
		XASTRO_ATOMIC_PARTITION_FUNCTION_DATA_LI * lpCurr = m_lpHead;
		while (lpCurr != NULL)
		{
			m_lpHead = lpCurr->m_lpNext;
			delete lpCurr;
			lpCurr = m_lpHead;
		}
			
		if (m_uiQuick_Find_Z)
			delete [] m_uiQuick_Find_Z;
		if (m_lpcQuick_Find_Data)
			delete [] m_lpcQuick_Find_Data;

		m_lpHead = NULL;
		m_lpTail = NULL;
		m_uiNum_List_Items = 0;
		m_uiQuick_Find_Z = NULL;
		m_lpcQuick_Find_Data = NULL;
	}
	unsigned int Find_Z_Index(unsigned int i_uiZ) const;
	XASTRO_ATOMIC_PARTITION_FUNCTION_DATA	Get_PF_Data(unsigned int i_uiZ) const;
	XASTRO_ATOMIC_PARTITION_FUNCTION_DATA *	Get_PF_Data_Ptr(unsigned int i_uiZ) const;
};
extern XASTRO_ATOMIC_PARTITION_FUNCTION_DATA_LIST	g_xAstro_Partition_Fn_Data;



class XASTRO_ATOMIC_PARTITION_FUNCTION_DATA_IRWIN
{
public:
	unsigned int uiZ;
	double	da[3][6];
	double	dlnQ_16000[3]; // ln Q for T=16000 K (test value)
	double	Get_Partition_Function(unsigned int i_uiIon, const double &i_dTemperature_K); // specifiy ion; ion = 1 = neutral, 2 = 1st ionized state, etc.  Only states I,II,III available from Irwin data
};

class XASTRO_ATOMIC_PARTITION_FUNCTION_DATA_IRWIN_LIST
{
private:
	unsigned int									m_uiNum_List_Items;
	XASTRO_ATOMIC_PARTITION_FUNCTION_DATA_IRWIN * 	m_lpData;

	void Initialize(void);

	unsigned int Find_Z_Index(unsigned int i_uiZ) const;
public:
	XASTRO_ATOMIC_PARTITION_FUNCTION_DATA_IRWIN_LIST(void)
	{
		m_uiNum_List_Items = 0;
		m_lpData = NULL;
		Initialize();
	}
	~XASTRO_ATOMIC_PARTITION_FUNCTION_DATA_IRWIN_LIST(void)
	{
		if (m_lpData)
			delete [] m_lpData;
		m_lpData = NULL;
		m_uiNum_List_Items = 0;
	}
	XASTRO_ATOMIC_PARTITION_FUNCTION_DATA_IRWIN	Get_PF_Data(unsigned int i_uiZ) const;
	XASTRO_ATOMIC_PARTITION_FUNCTION_DATA_IRWIN *	Get_PF_Data_Ptr(unsigned int i_uiZ) const;
};
extern XASTRO_ATOMIC_PARTITION_FUNCTION_DATA_IRWIN_LIST	g_xAstro_Partition_Fn_Data_Irwin;


