#pragma once
#include <stdarg.h>
#include <xastro.h>
#include <cstring> // get the declaration of NULL
#include <map>

class XASTRO_ATOMIC_IONIZATION_DATA
{
public:
	std::map<unsigned int, double> m_midIonization_energies_erg;
	unsigned int m_uiZ;
private:
public:
	void Clean(void)
	{
		m_uiZ = 0;
		m_midIonization_energies_erg.clear();
	}
	XASTRO_ATOMIC_IONIZATION_DATA(void)
	{
		m_uiZ = 0;
	}
	~XASTRO_ATOMIC_IONIZATION_DATA(void)
	{
		Clean();
	}
	XASTRO_ATOMIC_IONIZATION_DATA(unsigned int i_uiZ, unsigned int i_uiIonization_Energies_Count, const double * i_lpdEnergies)
	{
		m_uiZ = i_uiZ;
//		printf("Ion %i");
		for (unsigned int uiI = 0; uiI < i_uiIonization_Energies_Count; uiI++)
		{
//			printf(" %.1e ",i_lpdEnergies[uiI]);
			m_midIonization_energies_erg[uiI] = i_lpdEnergies[uiI];
		}
//		printf("\n");
	}
	double	Get_Ion_State_Potential(unsigned int i_uiState) const
	{
		double	dRet = 1.0e-6; // ~10^6 eV
		if (i_uiState < m_midIonization_energies_erg.size())
			dRet = m_midIonization_energies_erg.at(i_uiState);
		return dRet;
	}
};


class xastro_ionization_data
{
private:
	std::map<unsigned int, XASTRO_ATOMIC_IONIZATION_DATA> m_muicIon_Data;
	
	void Initialize(void);

public:
	xastro_ionization_data(void)
	{
		Initialize();
	}

	
	XASTRO_ATOMIC_IONIZATION_DATA	Get_Ionization_Data(unsigned int i_uiZ) const;
};
extern xastro_ionization_data	g_xAstro_Ionization_Energy_Data;

/*
class xastro_atomic_partition_function_ion_data
{
	// This class is used for estimating the partition function for ionized atoms
	// method is based on Cardona, Matrinez-Arroyo & Lopez-Castillo, ApJ 711:239-245 (2010)
public:
	class pf_poly_data
	{
	public:
		unsigned int uiJ;
		double m_dE;
		unsigned int uiG;
		unsigned int uiM;
		double dChi;
		double dE1;
		double dG1;
	};
	unsigned int 	m_uiZ;
	std::map<unsigned int,pf_poly_data> m_cData;

	void Clean(void)
	{
		m_uiZ = 0;
		m_cData.clear();
	}

	xastro_atomic_partition_function_ion_data(void)
	{
		m_uiZ = 0;
	}
	
	// ionization state is 0 indexed: 0 = neutral, 1 = singly ionized, ...
	double	Get_Partition_Function(unsigned int i_uiIonization_State, const  double &i_dTemperature, const double &i_dNumber_Density) const;
		

};



class xastro_atomic_partition_function_data
{
private:
	std::map<unsigned int, xastro_atomic_partition_function_ion_data> m_cData;

	void Initialize(void);

public:
	xastro_atomic_partition_function_data(void){Initialize();}

	xastro_atomic_partition_function_ion_data	Get_PF_Data(unsigned int i_uiZ) const;
};
extern xastro_atomic_partition_function_data	g_xAstro_Partition_Fn_Data;



class xastro_atomic_partition_function_ion_data_irwin
{
public:
	unsigned int uiZ;
	double	da[3][6];
	double	dlnQ_16000[3]; // ln Q for T=16000 K (test value)
	double	Get_Partition_Function(unsigned int i_uiIon, const double &i_dTemperature_K) const; // specifiy ion; ion = 1 = neutral, 2 = 1st ionized state, etc.  Only states I,II,III available from Irwin data
};

class xastro_atomic_partition_function_data_irwin
{
private:
	std::map<unsigned int, xastro_atomic_partition_function_data_irwin> m_cData;

	void Initialize(void);

public:
	xastro_atomic_partition_function_data_irwin(void)
	{
		Initialize();
	}
	~xastro_atomic_partition_function_data_irwin(void)
	{
		if (m_lpData)
			delete [] m_lpData;
		m_lpData = NULL;
		m_uiNum_List_Items = 0;
	}
	xastro_atomic_partition_function_ion_data_irwin	Get_PF_Data(unsigned int i_uiZ) const;
};
extern xastro_atomic_partition_function_data_irwin	g_xAstro_Partition_Fn_Data_Irwin;

*/
