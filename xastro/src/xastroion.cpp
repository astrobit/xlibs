#include <stdlib.h>
#include <math.h>
#include <xmath.h>
#include <xio.h>
#include <xastroion.h>

xastro_ionization_data	g_xAstro_Ionization_Energy_Data;
//XASTRO_ATOMIC_PARTITION_FUNCTION_DATA_LIST	g_xAstro_Partition_Fn_Data;

char * XASTRO_Ion_Read_Table_Entry(double &i_dValue, char * lpszCursor)
{
	//if (lpszCursor)
	//	printf(lpszCursor);
	if (xIsWhitespace(lpszCursor))
		lpszCursor = xPassWhitespace(lpszCursor);
	if (lpszCursor && xIsANumber(lpszCursor))
	{
		i_dValue = atof(lpszCursor);
		//printf("%.2f\n",i_dValue);
	}
	else
		lpszCursor = NULL;
	if (lpszCursor)
		lpszCursor = xPassNumber(lpszCursor);
	if (lpszCursor)
		lpszCursor = xPassWhitespace(lpszCursor);
	return lpszCursor;
}

void xastro_ionization_data::Initialize(void)
{
	m_muicIon_Data.clear();
	char * lpData_Path;
	char lpszPath[256];

	if ((lpData_Path = getenv("XASTRO_DATA_PATH")) != NULL)
		sprintf(lpszPath,"%s/ionization_data.dat",lpData_Path);
#ifdef DATADIR
	else
	{
		sprintf(lpszPath,"%s/ionization_data.dat",DATADIR);
	}
#else
	else
	{
		printf("Environment variable XASTRO_DATA_PATH not defined.\nFailed to load ionization data.");
		return;
	}
#endif
	printf("Read xastro ion data from %s\n",lpszPath);

	FILE * fileIn = fopen(lpszPath,"rt");
	if (fileIn)
	{
		unsigned int uiEntry_Index = 0;
		char *lpszBuffer = new char[1024];
		double lpdIonization_Energies[30];
		char *lpszCursor;
		lpszCursor = fgets(lpszBuffer,1024,fileIn); // read header line
		while(fgets(lpszBuffer,1024,fileIn))
		{
			//printf(lpszBuffer);
			lpszCursor = lpszBuffer;
			memset(lpdIonization_Energies,0,sizeof(lpdIonization_Energies));
			if (lpszCursor[0])
			{
				if (xIsWhitespace(lpszCursor))
					lpszCursor = xPassWhitespace(lpszCursor);
				// Z
				unsigned int uiZ = atoi(lpszCursor);
				lpszCursor = xPassNumber(lpszCursor);
				lpszCursor = xPassWhitespace(lpszCursor);
				// Element name
				while (!xIsWhitespace(lpszCursor))
					lpszCursor++;
				lpszCursor = xPassWhitespace(lpszCursor);
				// Element symbol
				while (!xIsWhitespace(lpszCursor))
					lpszCursor++;
				lpszCursor = xPassWhitespace(lpszCursor);
				uiEntry_Index = 0;
				while (lpszCursor && lpszCursor[0] != 0 && lpszCursor[0] != 10) // 10 = end of line
				{
					// ionization data
					lpszCursor = XASTRO_Ion_Read_Table_Entry(lpdIonization_Energies[uiEntry_Index],lpszCursor);
					if (g_XASTRO.k_derg_eV != 0.0)
						lpdIonization_Energies[uiEntry_Index] *= g_XASTRO.k_derg_eV;
					else
						lpdIonization_Energies[uiEntry_Index] *= 1.602176565e-12;
					uiEntry_Index++;					
				}
				//printf("%i %i %f\n",uiZ,uiEntry_Index,lpdIonization_Energies[0]);
				m_muicIon_Data[uiZ] = XASTRO_ATOMIC_IONIZATION_DATA(uiZ,uiEntry_Index,lpdIonization_Energies);
			}


		}
		fclose(fileIn);
		fileIn = NULL;
	}
	else
		printf("Unable to find ionization_data.dat\n");
}


XASTRO_ATOMIC_IONIZATION_DATA	xastro_ionization_data::Get_Ionization_Data(unsigned int i_uiZ) const
{
	XASTRO_ATOMIC_IONIZATION_DATA cData;
	if (m_muicIon_Data.count(i_uiZ) == 1)
		cData = m_muicIon_Data.at(i_uiZ);
	return cData;
}

/*
double	xastro_atomic_partition_function_ion_data::Get_Partition_Function(unsigned int i_uiIonization_State, const  double &i_dTemperature, const double &i_dNumber_Density) const
{
	unsigned int uiIndex = (unsigned int)(-1);
	double	dU = 1.0;
	if (m_cData.count(i_uiIonization_State) == 1)
	{
	{
		double	d1_kt = 1.0 / (g_XASTRO.k_dKb * i_dTemperature);
		double	dZeff = (i_uiIonization_State + 1);
		double dq = sqrt(dZeff / (g_cConstants.dPi * 2.0 * g_XASTRO.k_da0)) * pow(i_dNumber_Density,-1.0 / 6.0) * 0.5;
		double dn_star = dq * (1.0 + sqrt(1.0 + 2.0 / dq));
		int iN_star = (int)(dn_star + 0.5);
		double	dE_nstar = m_cData[i_uiIonization_State].dChi - dZeff * dZeff / (iN_star * iN_star) * g_XASTRO.k_dRy;

		double	dA = m_cData[i_uiIonization_State].uiG1 * exp(-m_cData[i_uiIonization_State].dE1 * d1_kt);
		double	dB = m_cData[i_uiIonization_State].uiG * exp(-m_cData[i_uiIonization_State].dE * d1_kt);
		double	dC;
		if (iN_star > 7) // n' was chosen to be 7, so if n* < 7, drop these terms
			dC = m_cData[i_uiIonization_State].uiM / 3.0 * (iN_star * iN_star * iN_star - 343) * exp(-dE_nstar * d1_kt);
		else
			dC = 0.0;

		dU = dA + dB + dC;
	}
	return dU;
}

void XASTRO_ATOMIC_PARTITION_FUNCTION_DATA_LIST::Initialize(void)
{
	m_uiNum_List_Items = 0;
	char * lpData_Path;
	char lpszPath[256];
	if ((lpData_Path = getenv("XASTRO_DATA_PATH")) != NULL)
		sprintf(lpszPath,"%s/pf_data.dat",lpData_Path);
	else
	{
		printf("Environment variable XASTRO_DATA_PATH not defined.\nFailed to load partition function data.");
		return;
	}
//	printf("Read xastro PF data from %s\n",lpszPath);
	FILE * fileIn = fopen(lpszPath,"rt");
	if (fileIn)
	{
		//printf("\t File is open\n");
		XASTRO_ATOMIC_PARTITION_FUNCTION_DATA_LI * lpCurr;
		unsigned int uiEntry_Index = 0;
		char *lpszBuffer = new char[1024];
		//printf("\t Buffer allocated\n");
		unsigned int uiZ = 0, uiZnew;
		unsigned int lpuiJ[30];
		double lpdEnergy[30];
		unsigned int lpuiG[30];
		unsigned int lpuiM[30];
		char *lpszCursor;
		lpszCursor = fgets(lpszBuffer,1024,fileIn); // read header line
		while(fgets(lpszBuffer,1024,fileIn))
		{
			lpszCursor = lpszBuffer;
			if (xIsWhitespace(lpszCursor))
				lpszCursor = xPassWhitespace(lpszCursor);
			// Z
			uiZnew = atoi(lpszCursor);
			lpszCursor = xPassNumber(lpszCursor);
			lpszCursor = xPassWhitespace(lpszCursor);
			if (uiZnew != uiZ && uiZ != 0)
			{ 		// add entry to list
//				printf("\t Adding entry for %i\n",uiZ);
				xastro_atomic_partition_function_ion_data::pf_poly_data cPoly;

				XASTRO_ATOMIC_IONIZATION_DATA cIon_Data = g_xAstro_Ionization_Energy_Data.Get_Ionization_Data(uiZ);


				lpCurr->m_cData.m_dE1 = 0.0;//cIon_Data.m_lpIonization_energies_erg[0];
				lpCurr->m_cData.m_uiG1 = 2; // @@TODO

				m_uiNum_List_Items++;
				// insert into list; make sure list remains sorted by Z
				if (!m_lpHead)
				{
					m_lpHead = m_lpTail = lpCurr;
				}
				else
				{
					if (uiZ > m_lpTail->m_cData.m_uiZ) // end of list
					{
						m_lpTail->m_lpNext = lpCurr;
						lpCurr->m_lpPrev = m_lpTail;
						m_lpTail = lpCurr;
					}
					else if (uiZ < m_lpHead->m_cData.m_uiZ) // beginning of list
					{
						m_lpHead->m_lpPrev = lpCurr;
						lpCurr->m_lpNext = m_lpHead;
						m_lpHead = lpCurr;
					}
					else // somewhere in the middle
					{
						XASTRO_ATOMIC_PARTITION_FUNCTION_DATA_LI * lpPrev = m_lpTail;
						while (lpPrev && lpPrev->m_cData.m_uiZ > uiZ)
							lpPrev = lpPrev->m_lpPrev;
						if (lpPrev) // we'd better get here!
						{
							XASTRO_ATOMIC_PARTITION_FUNCTION_DATA_LI * lpNext = lpPrev->m_lpNext;
							lpPrev->m_lpNext = lpCurr;
							lpCurr->m_lpPrev = lpPrev;
							lpCurr->m_lpNext = lpNext;
							if (lpNext)
								lpNext->m_lpPrev = lpCurr;
						}
					}
				}

				uiEntry_Index = 0;
			}
			uiZ = uiZnew;

			lpuiJ[uiEntry_Index] = atoi(lpszCursor);
			lpszCursor = xPassNumber(lpszCursor);
			lpszCursor = xPassWhitespace(lpszCursor);
			if (lpszCursor[0] != '*') // no data
			{
				lpdEnergy[uiEntry_Index] = atof(lpszCursor);
				lpszCursor = xPassNumber(lpszCursor);
				lpszCursor = xPassWhitespace(lpszCursor);

				lpuiG[uiEntry_Index] = atoi(lpszCursor);
				lpszCursor = xPassNumber(lpszCursor);
				lpszCursor = xPassWhitespace(lpszCursor);
			
				lpuiM[uiEntry_Index] = atoi(lpszCursor);
				lpszCursor = xPassNumber(lpszCursor);
				lpszCursor = xPassWhitespace(lpszCursor);
				uiEntry_Index++;
			}
		}
		if (uiZ != 0)
		{
//			printf("\t Adding entry for %i\n",uiZ);
			lpCurr = new XASTRO_ATOMIC_PARTITION_FUNCTION_DATA_LI;
			lpCurr->m_cData.m_uiZ = uiZ;
			lpCurr->m_cData.m_uiNum_Entries = uiEntry_Index;
			lpCurr->m_cData.m_lpuiJ = new unsigned int[uiEntry_Index];
			lpCurr->m_cData.m_lpdE = new double[uiEntry_Index];
			lpCurr->m_cData.m_lpuiG = new unsigned int [uiEntry_Index];
			lpCurr->m_cData.m_lpuiM = new unsigned int [uiEntry_Index];
			lpCurr->m_cData.m_lpdChi = new double [uiEntry_Index];

			XASTRO_ATOMIC_IONIZATION_DATA cIon_Data = g_xAstro_Ionization_Energy_Data.Get_Ionization_Data(uiZ);

			memcpy(lpCurr->m_cData.m_lpuiJ,lpuiJ,sizeof(unsigned int) * uiEntry_Index);
			memcpy(lpCurr->m_cData.m_lpdE,lpdEnergy,sizeof(double) * uiEntry_Index);
			memcpy(lpCurr->m_cData.m_lpuiG,lpuiG,sizeof(unsigned int) * uiEntry_Index);
			memcpy(lpCurr->m_cData.m_lpuiM,lpuiM,sizeof(unsigned int) * uiEntry_Index);
			for (unsigned int uiI = 0; uiI < uiEntry_Index; uiI++)
			{
				lpCurr->m_cData.m_lpdChi[uiI] = cIon_Data.Get_Ion_State_Potential(lpuiJ[uiI]);
			}

			lpCurr->m_cData.m_dE1 = cIon_Data.Get_Ion_State_Potential(0);
			lpCurr->m_cData.m_uiG1 = 2; // @@TODO
			m_uiNum_List_Items++;
			// insert into list; make sure list remains sorted by Z
			if (!m_lpHead)
			{
				m_lpHead = m_lpTail = lpCurr;
			}
			else
			{
				if (uiZ > m_lpTail->m_cData.m_uiZ) // end of list
				{
					m_lpTail->m_lpNext = lpCurr;
					lpCurr->m_lpPrev = m_lpTail;
					m_lpTail = lpCurr;
				}
				else if (uiZ < m_lpHead->m_cData.m_uiZ) // beginning of list
				{
					m_lpHead->m_lpPrev = lpCurr;
					lpCurr->m_lpNext = m_lpHead;
					m_lpHead = lpCurr;
				}
				else // somewhere in the middle
				{
					XASTRO_ATOMIC_PARTITION_FUNCTION_DATA_LI * lpPrev = m_lpTail;
					while (lpPrev && lpPrev->m_cData.m_uiZ > uiZ)
						lpPrev = lpPrev->m_lpPrev;
					if (lpPrev) // we'd better get here!
					{
						XASTRO_ATOMIC_PARTITION_FUNCTION_DATA_LI * lpNext = lpPrev->m_lpNext;
						lpPrev->m_lpNext = lpCurr;
						lpCurr->m_lpPrev = lpPrev;
						lpCurr->m_lpNext = lpNext;
						if (lpNext)
							lpNext->m_lpPrev = lpCurr;
					}
				}
			}
		}

		fclose(fileIn);
		fileIn = NULL;
		// generate quick find lists
		//printf("\t Generating quick Z search\n");
		m_uiQuick_Find_Z = new unsigned int[m_uiNum_List_Items];
		m_lpcQuick_Find_Data = new XASTRO_ATOMIC_PARTITION_FUNCTION_DATA *[m_uiNum_List_Items];
		uiEntry_Index = 0;
		lpCurr = m_lpHead;
		while (lpCurr)
		{
			m_uiQuick_Find_Z[uiEntry_Index] = lpCurr->m_cData.m_uiZ;
			m_lpcQuick_Find_Data[uiEntry_Index] = &(lpCurr->m_cData);
			uiEntry_Index++;
			lpCurr = lpCurr->m_lpNext;
		}
	}
	else
		printf("Unable to find pf_data.dat\n");
}
unsigned int XASTRO_ATOMIC_PARTITION_FUNCTION_DATA_LIST::Find_Z_Index(unsigned int i_uiZ) const
{
	unsigned int uiIndex = (unsigned int)(-1);
	if (m_uiNum_List_Items > 0)
	{
		unsigned int uiHigh_Index = m_uiNum_List_Items - 1;
		unsigned int uiLow_Index = 0;

		while(uiHigh_Index != uiLow_Index && uiIndex == (unsigned int)(-1))
		{
			unsigned int uiTest_Idx = (uiHigh_Index + uiLow_Index) >> 1;		
			if (m_uiQuick_Find_Z[uiTest_Idx] == i_uiZ)
				uiIndex = uiTest_Idx;
			else if (m_uiQuick_Find_Z[uiTest_Idx] > i_uiZ)
				uiHigh_Index = uiTest_Idx;
			else if (uiTest_Idx == uiLow_Index)
				uiLow_Index = uiTest_Idx + 1;
			else
				uiLow_Index = uiTest_Idx;
		}
		if (uiIndex == (unsigned int)(-1) && m_uiQuick_Find_Z[uiHigh_Index] == i_uiZ)
			uiIndex = uiHigh_Index;
	}
	return uiIndex;
}
XASTRO_ATOMIC_PARTITION_FUNCTION_DATA	XASTRO_ATOMIC_PARTITION_FUNCTION_DATA_LIST::Get_PF_Data(unsigned int i_uiZ) const
{
	XASTRO_ATOMIC_PARTITION_FUNCTION_DATA cData;
	unsigned int uiZ_Index = Find_Z_Index(i_uiZ);
	if (uiZ_Index != (unsigned int)(-1))
		cData = m_lpcQuick_Find_Data[uiZ_Index][0];
	return cData;
}
XASTRO_ATOMIC_PARTITION_FUNCTION_DATA *	XASTRO_ATOMIC_PARTITION_FUNCTION_DATA_LIST::Get_PF_Data_Ptr(unsigned int i_uiZ) const
{
	XASTRO_ATOMIC_PARTITION_FUNCTION_DATA * cData = NULL;
	unsigned int uiZ_Index = Find_Z_Index(i_uiZ);
	if (uiZ_Index != (unsigned int)(-1))
		cData = m_lpcQuick_Find_Data[uiZ_Index];
	return cData;
}




//---------------------------------------------------------------------------------------------
//
//	Irwin partition function
//
//
//--------------------------------------------------------------------------------------------

double	XASTRO_ATOMIC_PARTITION_FUNCTION_DATA_IRWIN::Get_Partition_Function(unsigned int i_uiIon, const double &i_dTemperature_K) const
{
	double dRet = 1.0;
	if (i_uiIon <= 3)
	{
		if (i_uiIon != 0)
			i_uiIon--;
		double dLnTemp = log(i_dTemperature_K);
		dRet = ((((da[i_uiIon][5] * dLnTemp + da[i_uiIon][4]) * dLnTemp + da[i_uiIon][3]) * dLnTemp + da[i_uiIon][2]) * dLnTemp + da[i_uiIon][1]) * dLnTemp + da[i_uiIon][0];
		dRet = exp(dRet);
	}
	return dRet;
}

unsigned int XASTRO_ATOMIC_PARTITION_FUNCTION_DATA_IRWIN_LIST::Find_Z_Index(unsigned int i_uiZ) const
{
	unsigned int uiIndex = (unsigned int)(-1);
	if (m_uiNum_List_Items > 0)
	{
		unsigned int uiHigh_Index = m_uiNum_List_Items - 1;
		unsigned int uiLow_Index = 0;

		while(uiHigh_Index != uiLow_Index && uiIndex == (unsigned int)(-1))
		{
			unsigned int uiTest_Idx = (uiHigh_Index + uiLow_Index) >> 1;		
			if (m_lpData[uiTest_Idx].uiZ == i_uiZ)
				uiIndex = uiTest_Idx;
			else if (m_lpData[uiTest_Idx].uiZ > i_uiZ)
				uiHigh_Index = uiTest_Idx;
			else if (uiTest_Idx == uiLow_Index)
				uiLow_Index = uiTest_Idx + 1;
			else
				uiLow_Index = uiTest_Idx;
		}
		if (uiIndex == (unsigned int)(-1) && m_lpData[uiHigh_Index].uiZ == i_uiZ)
			uiIndex = uiHigh_Index;
	}
	return uiIndex;
}


XASTRO_ATOMIC_PARTITION_FUNCTION_DATA_IRWIN	XASTRO_ATOMIC_PARTITION_FUNCTION_DATA_IRWIN_LIST::Get_PF_Data(unsigned int i_uiZ) const
{
	XASTRO_ATOMIC_PARTITION_FUNCTION_DATA_IRWIN cRet;
	unsigned int uiIdx = Find_Z_Index(i_uiZ);
	if (uiIdx != (unsigned int)(-1))
	{
		cRet = m_lpData[uiIdx];
	}
	return cRet;
}
XASTRO_ATOMIC_PARTITION_FUNCTION_DATA_IRWIN *	XASTRO_ATOMIC_PARTITION_FUNCTION_DATA_IRWIN_LIST::Get_PF_Data_Ptr(unsigned int i_uiZ) const
{
	XASTRO_ATOMIC_PARTITION_FUNCTION_DATA_IRWIN * cRet = NULL;
	unsigned int uiIdx = Find_Z_Index(i_uiZ);
	if (uiIdx != (unsigned int)(-1))
	{
		cRet = &m_lpData[uiIdx];
	}
	return cRet;
}

void XASTRO_ATOMIC_PARTITION_FUNCTION_DATA_IRWIN_LIST::Initialize(void)
{
	char * lpData_Path;
	char lpszPath[256];
	if ((lpData_Path = getenv("XASTRO_DATA_PATH")) != NULL)
		sprintf(lpszPath,"%s/Irwin_pf_data.csv",lpData_Path);
	else
	{
		printf("Environment variable XASTRO_DATA_PATH not defined.\nFailed to load partition function data.");
		return;
	}
	printf("Read xastro Irwin PF data from %s\n",lpszPath);
	XDATASET	cFileData;
	unsigned int uiI;
	cFileData.ReadDataFile(lpszPath,false,false,',',4);
	m_uiNum_List_Items = cFileData.GetNumElements();
	//printf("%i items in file\n",m_uiNum_List_Items);
	if (m_uiNum_List_Items != 0)
	{
		m_lpData = new XASTRO_ATOMIC_PARTITION_FUNCTION_DATA_IRWIN[m_uiNum_List_Items];
		for (uiI = 0; uiI < m_uiNum_List_Items; uiI++)
		{
			m_lpData[uiI].uiZ = (unsigned int)cFileData.GetElement(0,uiI);
			m_lpData[uiI].da[0][0] = cFileData.GetElement(1,uiI);
			m_lpData[uiI].da[0][1] = cFileData.GetElement(2,uiI);
			m_lpData[uiI].da[0][2] = cFileData.GetElement(3,uiI);
			m_lpData[uiI].da[0][3] = cFileData.GetElement(4,uiI);
			m_lpData[uiI].da[0][4] = cFileData.GetElement(5,uiI);
			m_lpData[uiI].da[0][5] = cFileData.GetElement(6,uiI);
			m_lpData[uiI].dlnQ_16000[0] = cFileData.GetElement(7,uiI);
//			printf("%.2e %.2e %.2e %.2e %.2e %.2e %.2e\n",m_lpData[uiI].da[0][0],m_lpData[uiI].da[0][1], m_lpData[uiI].da[0][2], m_lpData[uiI].da[0][3], m_lpData[uiI].da[0][4], m_lpData[uiI].da[0][5],m_lpData[uiI].dlnQ_16000[0]);
			m_lpData[uiI].da[1][0] = cFileData.GetElement(8,uiI);
			m_lpData[uiI].da[1][1] = cFileData.GetElement(9,uiI);
			m_lpData[uiI].da[1][2] = cFileData.GetElement(10,uiI);
			m_lpData[uiI].da[1][3] = cFileData.GetElement(11,uiI);
			m_lpData[uiI].da[1][4] = cFileData.GetElement(12,uiI);
			m_lpData[uiI].da[1][5] = cFileData.GetElement(13,uiI);
			m_lpData[uiI].dlnQ_16000[1] = cFileData.GetElement(14,uiI);
//			printf("%.2e %.2e %.2e %.2e %.2e %.2e %.2e\n",m_lpData[uiI].da[1][0],m_lpData[uiI].da[1][1], m_lpData[uiI].da[1][2], m_lpData[uiI].da[1][3], m_lpData[uiI].da[1][4], m_lpData[uiI].da[1][5],m_lpData[uiI].dlnQ_16000[1]);
			m_lpData[uiI].da[2][0] = cFileData.GetElement(15,uiI);
			m_lpData[uiI].da[2][1] = cFileData.GetElement(16,uiI);
			m_lpData[uiI].da[2][2] = cFileData.GetElement(17,uiI);
			m_lpData[uiI].da[2][3] = cFileData.GetElement(18,uiI);
			m_lpData[uiI].da[2][4] = cFileData.GetElement(19,uiI);
			m_lpData[uiI].da[2][5] = cFileData.GetElement(20,uiI);
			m_lpData[uiI].dlnQ_16000[2] = cFileData.GetElement(21,uiI);
//			printf("%.2e %.2e %.2e %.2e %.2e %.2e %.2e\n",m_lpData[uiI].da[2][0],m_lpData[uiI].da[2][1], m_lpData[uiI].da[2][2], m_lpData[uiI].da[2][3], m_lpData[uiI].da[2][4], m_lpData[uiI].da[2][5],m_lpData[uiI].dlnQ_16000[2]);

			double dLog1 = log(m_lpData[uiI].Get_Partition_Function(1,16000.0));
			double dLog2 = log(m_lpData[uiI].Get_Partition_Function(2,16000.0));
			double dLog3 = log(m_lpData[uiI].Get_Partition_Function(3,16000.0));
			double dErr1 = fabs(dLog1 - m_lpData[uiI].dlnQ_16000[0]);
			if (m_lpData[uiI].dlnQ_16000[0] > 0.0)
				dErr1 /= m_lpData[uiI].dlnQ_16000[0];
			double dErr2 = fabs(dLog2 - m_lpData[uiI].dlnQ_16000[1]);
			if (m_lpData[uiI].dlnQ_16000[1] > 0.0)
				dErr2 /= m_lpData[uiI].dlnQ_16000[1];
			double dErr3 = fabs(dLog3 - m_lpData[uiI].dlnQ_16000[2]);
			if (m_lpData[uiI].dlnQ_16000[2] > 0.0)
				dErr3 /= m_lpData[uiI].dlnQ_16000[2];
			if (dErr1 > 1.1e-8)
			{
				printf("Xastro: excessive error (1) loading element %i Irwin PF data [%.1e %.1e %.1e]\n",m_lpData[uiI].uiZ,dLog1,m_lpData[uiI].dlnQ_16000[0],dErr1);
			}
			if (dErr2 > 1.1e-8)
			{
				printf("Xastro: excessive error (2) loading element %i Irwin PF data [%.1e %.1e %.1e]\n",m_lpData[uiI].uiZ,dLog2,m_lpData[uiI].dlnQ_16000[1],dErr2);
			}
			if (dErr3 > 1.1e-8)
			{
				printf("Xastro: excessive error (3) loading element %i Irwin PF data [%.1e %.1e %.1e]\n",m_lpData[uiI].uiZ,dLog3,m_lpData[uiI].dlnQ_16000[2],dErr3);
			}
		}
	}
	else
		printf("Xastro: failed to open file %s\n",lpszPath);
}
XASTRO_ATOMIC_PARTITION_FUNCTION_DATA_IRWIN_LIST	g_xAstro_Partition_Fn_Data_Irwin;
*/
