#pragma once
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define XASTROLINE_LINEDATAVERSION 	0
#define XASTROLINE_LINEDATASUBVERSION 	1

class XASTROLINE_STATEDATA
{
public:
	
	char	lpszState[32];
	double	dEnergy_eV;
	double	dAngular_Momentum_J;
	unsigned int uiStatistical_Weight;

	void Clean(void)
	{
		memset(lpszState,0,sizeof(lpszState));
		dEnergy_eV = 0.0;
		dAngular_Momentum_J = 0;
		uiStatistical_Weight = 0;
	}
};
	
class XASTROLINE_LINEDATA
{
public:
	double	dWavelength_A;
	double	dTransisiton_Probability_sm1;
	double	dOscillator_Strength;
	XASTROLINE_STATEDATA	cLower_State;
	XASTROLINE_STATEDATA	cUpper_State;

	//SobolevTauCoeff: Calculate the constant portion of the coefficient under the Sobolev approximation
	double	SobolevTauCoeff(void) const
	{
		double	dTauCoeff = dWavelength_A * dWavelength_A * dWavelength_A * dTransisiton_Probability_sm1 * 1.0e-24; // 10^-24 converts to cm from A
		if (cUpper_State.uiStatistical_Weight > 0 && cLower_State.uiStatistical_Weight > 0)
			dTauCoeff *= cUpper_State.uiStatistical_Weight / (double)cLower_State.uiStatistical_Weight;
		return dTauCoeff;
	}

	void Clean(void)
	{
		dWavelength_A = 0.0;
		dTransisiton_Probability_sm1 = 0.0;
		dOscillator_Strength = 0.0;
		cLower_State.Clean();
		cUpper_State.Clean();
	}
	XASTROLINE_LINEDATA(void)
	{
		Clean();
	}
};


class XASTROLINE_IONDATA
{
private:
	void Copy(const XASTROLINE_IONDATA & i_cIon_Data)
	{
		Clean();
		uiIon_State = i_cIon_Data.uiIon_State;
		uiNum_Lines = i_cIon_Data.uiNum_Lines;
		if (uiNum_Lines != 0)
		{
			lpcLine_Data = new XASTROLINE_LINEDATA[uiNum_Lines];
			memcpy(lpcLine_Data,i_cIon_Data.lpcLine_Data,sizeof(XASTROLINE_LINEDATA) * uiNum_Lines);
		}
	}

public:
	unsigned int uiIon_State; // 1 = neutral, 2 = singly ionized, etc.
	unsigned int uiNum_Lines;
	XASTROLINE_LINEDATA *	lpcLine_Data;

	XASTROLINE_IONDATA(void)
	{
		lpcLine_Data = NULL;
		Clean();
	}
	XASTROLINE_IONDATA(const XASTROLINE_IONDATA &i_cRHO)
	{
		lpcLine_Data = NULL;
		Copy(i_cRHO);
	}
	~XASTROLINE_IONDATA(void)
	{
		Clean();
		lpcLine_Data = NULL;
	}
	XASTROLINE_IONDATA & operator = (const XASTROLINE_IONDATA &i_cRHO)
	{
		Copy(i_cRHO);
		return *this;
	}

	void Clean(void)
	{
		uiIon_State = 0;
		uiNum_Lines = 0;
		if (lpcLine_Data)
			delete [] lpcLine_Data;
		lpcLine_Data = NULL;
	}
};

class XASTROLINE_ELEMENTDATA
{
private:
	void WriteBinaryFileHeader(FILE * i_fileOut);
	void WriteBinaryDataFileIonData(const XASTROLINE_IONDATA & i_cIonData, FILE * i_fileOut);
	void WriteCSVDataFileIonData(const XASTROLINE_IONDATA & i_cIonData, FILE * i_fileOut);
	bool ReadBinaryFileHeader(FILE * i_fileIn);
	void ReadBinaryDataFileIonData(XASTROLINE_IONDATA & i_cIonData, FILE * i_fileIn);

	void Copy(const XASTROLINE_ELEMENTDATA &i_cRHO)
	{
		unsigned int uiI;
		Clean();
		lpszElement_ID[0] = i_cRHO.lpszElement_ID[0];
		lpszElement_ID[1] = i_cRHO.lpszElement_ID[1];
		uiA = i_cRHO.uiA;
		uiZ = i_cRHO.uiZ;
		lpcIonData = new XASTROLINE_IONDATA[uiZ];
		for (uiI = 0; uiI < uiZ; uiI++)
			lpcIonData[uiI] = i_cRHO.lpcIonData[uiI];
	}
		
	
public:
	char 			lpszElement_ID[3];
	unsigned int 	uiA;
	unsigned int 	uiZ;
	XASTROLINE_IONDATA * 		lpcIonData;

	void Clean(void)
	{
		lpszElement_ID[0] = 0;
		lpszElement_ID[1] = 0;
		lpszElement_ID[2] = 0;
		uiA = 0;
		uiZ = 0;
		if (lpcIonData)
			delete [] lpcIonData;
		lpcIonData = NULL;
	}

	XASTROLINE_ELEMENTDATA(void)
	{
		lpcIonData = NULL;
		Clean();
	}

	XASTROLINE_ELEMENTDATA(const XASTROLINE_ELEMENTDATA &i_cRHO)
	{
		lpcIonData = NULL;
		Copy(i_cRHO);
	}

	~XASTROLINE_ELEMENTDATA(void)
	{
		printf("Closing Element data list\n");
		Clean();
		lpcIonData = NULL;
	}

	XASTROLINE_ELEMENTDATA & operator =(const XASTROLINE_ELEMENTDATA &i_cRHO)
	{
		Copy(i_cRHO);
		return *this;
	}

	void WriteDataFile(void);
	void ReadDataFile(const char * i_lpszFile_Path, const char * i_lpszElement_ID = NULL);
	void WriteCSVFile(void);
};

