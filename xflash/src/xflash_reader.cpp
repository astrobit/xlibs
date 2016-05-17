#include <stdlib.h>
#include <string.h>
#include "xflash.h"

//#ifndef NO_HDF4
//#include "mfhdf.h"
//#endif

#ifndef NO_HDF5
#include "hdf5.h"
#endif

//#ifndef NO_NCDF
//#include "pnetcdf.h"
//#endif

XFLASH_Block *  XFLASH_File::GetBlock(const char * i_lpszVariable_Name, unsigned int i_uiBlock_ID) const
{
	XFLASH_Block * lpRet = NULL;
	unsigned int uiVar_ID = 0;

	while (strcmp(m_lpszVar_Names[uiVar_ID],i_lpszVariable_Name) != 0 && uiVar_ID < m_uiNum_Vars)
		uiVar_ID++;

	if (uiVar_ID < m_uiNum_Vars)
		lpRet = GetBlock(uiVar_ID,i_uiBlock_ID);

	return lpRet;
}


XFLASH_Block *  XFLASH_File::GetBlock(unsigned int i_uiVariable_ID, unsigned int i_uiBlock_ID) const
{
	XFLASH_Block * lpRet = NULL;

	switch (m_eFormat)
	{

//	#ifndef NO_HDF4
//	case FMT_HDF4:
//		lpRet = GetBlock_HDF4(i_uiVar, i_uiBlock_ID);
//		break;
//	#endif

	#ifndef NO_HDF5
	case FMT_HDF5_PMESH:
	case FMT_HDF5:
		lpRet = GetBlock_HDF5(i_uiVariable_ID, i_uiBlock_ID);
		break;
	#endif

//	#ifndef NO_NCDF
//	case FMT_NCDF:
//		lpRet = GetBlock_NCDF(i_uiVar, i_uiBlock_ID);
//		break;
//	#endif

	case FMT_HDF5_CHOMBO:
		lpRet = GetBlock_HDF5_Chombo(i_uiVariable_ID, i_uiBlock_ID);
		break;

	default:
		break;
	}
	return lpRet;
}

XFLASH_Particle_Collection * XFLASH_File::GetParticles(unsigned int i_uiFirst_Particle_ID, unsigned int i_uiMax_Particles, unsigned int &o_iNumber_Retreived) const
{
	XFLASH_Particle_Collection * lpParticles = NULL;
	switch (m_eFormat)
	{

//	#ifndef NO_HDF4
//	case FMT_HDF4:
//		lpParticles = GetParticles_HDF4(i_uiFirst_Particle_ID, i_uiMax_Particles, o_iNumber_Retreived);
//		break;
//	#endif

	#ifndef NO_HDF5
	case FMT_HDF5_PMESH:
		lpParticles = GetParticles_HDF5(i_uiFirst_Particle_ID, i_uiMax_Particles, o_iNumber_Retreived);
		break;
	#endif

//	#ifndef NO_NCDF
//	case FR_NCDF:
//		lpParticles = GetParticles_NCDF(i_uiFirst_Particle_ID, i_uiMax_Particles, o_iNumber_Retreived);
//		break;
//	#endif

	default:
		break;
	}
	return lpParticles;
}

void  XFLASH_File::ReleaseBlock(XFLASH_Block * i_lpBlock) const
{
	if (i_lpBlock)
	{
		if (i_lpBlock->m_lpdData)
			delete [] i_lpBlock->m_lpdData;
		delete [] i_lpBlock;
	}
}

void XFLASH_Particle_Collection::Void(void)
{
	m_uiNumParticles = 0;
	m_lpiProps = NULL;
	m_lpdProps = NULL;

}

XFLASH_Particle_Collection::XFLASH_Particle_Collection(void)
{
	Void();
}
XFLASH_Particle_Collection::~XFLASH_Particle_Collection(void)
{
	if (m_lpiProps)
		delete [] m_lpiProps;
	if (m_lpdProps)
		delete [] m_lpdProps;

	Void();
}

void XFLASH_File::Release_Particles(XFLASH_Particle_Collection * i_lpiParticles) const
{
	if (i_lpiParticles)
		delete [] i_lpiParticles;
}

void XFLASH_File::Open(const char *i_lpszFilename)
{
	FILE * fileIn;
	int lpcSignature[8];

	m_lpszFilename = new char[strlen(i_lpszFilename) + 1];
	strcpy(m_lpszFilename,i_lpszFilename);

  /* From H5Fpkg.h */
  /* const char *H5F_SIGNATURE = "\211HDF\r\n\032\n"; */
	const unsigned char chH5F_SIGNATURE[8] = {137, 72, 68, 70, 13, 10, 26, 10};
	const int iH5F_SIGNATURE_LEN = sizeof(chH5F_SIGNATURE);

	const char *chNCDF_SIGNATURE = "CDF";
	const int iNCDF_SIGNATURE_LEN = 3;

	fileIn = fopen(i_lpszFilename, "r");
	if (fileIn)
	{
		fread(lpcSignature,1,iH5F_SIGNATURE_LEN,fileIn);
	    bool bIsHDF5 = (memcmp(lpcSignature,chH5F_SIGNATURE,iH5F_SIGNATURE_LEN) == 0);
	    bool bIsNCDF = (memcmp(lpcSignature,chNCDF_SIGNATURE,iNCDF_SIGNATURE_LEN) == 0);
		
		if (bIsHDF5)
			m_eFormat = FMT_HDF5;
		else if (bIsNCDF)
			m_eFormat = FMT_NCDF;
		else
			m_eFormat = FMT_HDF4;
		fclose(fileIn);
		fileIn = NULL;

		switch (m_eFormat)
		{
	//	#ifndef NO_HDF4
	//	case FMT_HDF4:
	//		Open_HDF4();
	//		break;
	//	#endif

		#ifndef NO_HDF5
		case FMT_HDF5:
			Open_HDF5();
			break;
		#endif

	//	#ifndef NO_NCDF
	//	case FMT_NCDF:
	//		Open_NCDF();
	//		break;
	//	#endif

		default:
			break;
		}
	}
}

void XFLASH_File::Close(void)
{
	unsigned int uiI;
	if (m_lpeBlock_Node_Type)
		delete [] m_lpeBlock_Node_Type;
	if (m_lpdBlock_Coords)
		delete [] m_lpdBlock_Coords;
	if (m_lpdBlock_Bounding_Box)
		delete [] m_lpdBlock_Bounding_Box;
	if (m_lpdBlock_Size)
		delete [] m_lpdBlock_Size;
	if (m_lpiRefinement_Levels)
		delete [] m_lpiRefinement_Levels;
	
	if (m_lpszVar_Names)
	{
		for (uiI = 0; uiI < m_uiNum_Vars; uiI++)
			delete [] m_lpszVar_Names[uiI];
		delete [] m_lpszVar_Names;
	}
	if (m_lpeVar_Types)
		delete [] m_lpeVar_Types;
	
	if (m_lpszParticle_Int_Property_Names)
	{
		for (uiI = 0; uiI < m_uiNum_Particle_Int_Properties; uiI++)
			delete [] m_lpszParticle_Int_Property_Names[uiI];
		delete [] m_lpszParticle_Int_Property_Names;
	}
	if (m_lpszParticle_Real_Property_Names)
	{
		for (uiI = 0; uiI < m_uiNum_Particle_Real_Properties; uiI++)
			delete [] m_lpszParticle_Real_Property_Names[uiI];
		delete [] m_lpszParticle_Real_Property_Names;
	}

	if (m_lpuiBlocks_Per_Refine_Level)
		delete [] m_lpuiBlocks_Per_Refine_Level; // Chombo HDF5 only
	if (m_lpdLevel_Size)
		delete [] m_lpdLevel_Size;

	switch (m_eFormat)
	{

//	#ifndef NO_HDF4
//	case FR_HDF4:
//		SDend(*((int32 *)m_lpvFile_Handle)); 
//		break;
//	#endif

	#ifndef NO_HDF5
	case FMT_HDF5:
	case FMT_HDF5_PMESH:
	case FMT_HDF5_CHOMBO:
		H5Fclose((hid_t)m_iFile_Handle);
		break;
	#endif

//	#ifndef NO_NCDF
//	case FR_NCDF:
//		ncmpi_close(*((int *)m_lpvFile_Handle));
//		break;
//	#endif

	default:
		break;
	}

	Void();
}

void XFLASH_File::Void(void)
{
	m_eFormat = FMT_INVALID;
	m_lpszFilename = NULL;
	m_uiNum_Blocks = 0;
	m_uiNum_Cells_per_Block = 0;
	m_uiBlock_Dimensions[0] = 0; // assuming 3-d blocks
	m_uiBlock_Dimensions[1] = 0; // assuming 3-d blocks
	m_uiBlock_Dimensions[2] = 0; // assuming 3-d blocks
	m_uiNum_Dimensions = 0;
	m_lpeBlock_Node_Type = NULL;
	m_lpdBlock_Coords = NULL;
	m_lpdBlock_Bounding_Box = NULL;
	m_lpdBlock_Size = NULL;
	m_lpiRefinement_Levels = NULL;
	
	m_uiNum_Vars = 0;
	m_lpszVar_Names = NULL;
	m_lpeVar_Types = NULL;
	
	m_uiNum_Particles = 0;
	m_uiNum_Particle_Int_Properties = 0;
	m_lpszParticle_Int_Property_Names = NULL;
	m_uiNum_Particle_Real_Properties = 0;
	m_lpszParticle_Real_Property_Names = NULL;

	m_lpuiBlocks_Per_Refine_Level = NULL; // Chombo HDF5 only
	m_lpdLevel_Size  = NULL;
	m_uiNum_Levels = 0;

	m_iFile_Handle = -1;

}

XFLASH_File::XFLASH_File(void)
{
	Void();
}

XFLASH_File::~XFLASH_File(void)
{
	Close();
}


int XFLASH_File::Get_Variable_Idx_By_Name(const char * i_lpszName) const
{
	int iRet = -1;
	for (int iJ = 0; iJ < m_uiNum_Vars && iRet == -1; iJ++)
	{
		if (strcmp(i_lpszName,m_lpszVar_Names[iJ]) == 0)
			iRet = iJ;
	}
	return iRet;
}

