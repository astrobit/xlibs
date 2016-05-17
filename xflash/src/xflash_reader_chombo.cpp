#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define H5_USE_16_API  1
#include "hdf5.h"
#include "xflash.h"

/* structs used to read bounding box dimensions from hdf5 file*/
#define PADSIZE 4

///////////////////////////////////////////////////////////////////////////////////////////////
//
//	GetBlock_HDF5
//
//  This function retrieves a block of data from the file
//
///////////////////////////////////////////////////////////////////////////////////////////////

XFLASH_Block * XFLASH_File::GetBlock_HDF5_Chombo(unsigned int i_uiVariable_ID, unsigned int i_uiBlock_ID) const
{
	XFLASH_Block * lpRet = NULL;
	char refine_level_str[MAX_STRING_LENGTH];
	hid_t handle = (hid_t)m_iFile_Handle;
	int refine_level, ncells;
	int lo_i, lo_j, lo_k, hi_i, hi_j, hi_k;
	int dim, nxb, nyb, nzb;
	/*int rank;*/
//	int d, b;
	unsigned int b;
	double *buf;
	/*double size;*/
	
	hid_t dspace, dset, mspace, gid ;
//	herr_t status;
	hsize_t dimens, offset, count;

	/*size = file->size[block_no];*/
	/*rank =1;*/
	
	/*first figure out what kind of block we're looking at*/

	if (i_uiVariable_ID < m_uiNum_Vars && i_uiBlock_ID < m_uiNum_Blocks)
	{
		dim = m_uiNum_Dimensions;
		refine_level = m_lpiRefinement_Levels[i_uiBlock_ID];

		int lev_block0 = 0;
		for(refine_level=0; refine_level < m_lpiRefinement_Levels[i_uiBlock_ID]; refine_level++)
			lev_block0 += m_lpuiBlocks_Per_Refine_Level[refine_level];
		b = i_uiBlock_ID - lev_block0;
	
		sprintf(refine_level_str, "level_%d", refine_level);
		gid = H5Gopen(handle, refine_level_str);
	
		/* get offsets */
		dset = H5Dopen(gid, "data:offsets=0");
		dspace = H5Dget_space(dset);
		int *offsets = new int [m_lpuiBlocks_Per_Refine_Level[refine_level] + 1];
		H5Dread(dset, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, offsets);
		offset = offsets[b];
		ncells = (offsets[b+1]-offsets[b]) / m_uiNum_Vars;
		offset = offsets[b] + ncells * i_uiVariable_ID;
		delete [] offsets;
		H5Dclose(dset);
	
		/* Number of cells in this block. Need to get coordinates of bounding box of block
		 * from the flat array into which we parsed it earlier*/
		nxb = 1; nyb = 1; nzb = 1;

		lo_i = i_uiBlock_ID * dim * 2;
		hi_i = lo_i + dim;

		lo_j = i_uiBlock_ID * dim * 2 + 1;
		hi_j = lo_j + dim;

		lo_k = i_uiBlock_ID * dim * 2 + 2;
		hi_k = lo_k + dim;

		nxb = m_uiBlock_Dimensions[0];
		if(dim > 1)
			nyb = m_uiBlock_Dimensions[1];
		if(dim > 2)
			nzb = m_uiBlock_Dimensions[2];
	
		/* Initialize block data struct */
		lpRet = new XFLASH_Block;
		buf = new double [ncells];
		lpRet->m_lpdData = new double [nxb*nyb*nzb];
		lpRet->m_uiSize[0] = nxb;
		lpRet->m_uiSize[1] = nyb;
		lpRet->m_uiSize[2] = nzb;
		int gx=0, gy=0, gz=0;
		while((nxb+2*gx)*(nyb+2*gy)*(nzb+2*gz) < ncells)
		{
			gx += dim > 0 ? 1 : 0;
			gy += dim > 1 ? 1 : 0;
			gz += dim > 2 ? 1 : 0;
		}
		nxb += 2*gx; nyb += 2*gy; nzb += 2*gz;
		if(nxb*nyb*nzb != ncells)
			printf("ERROR: irregular ghost cells.\n");
	
		/* Get Data */
		dset = H5Dopen(gid, "data:datatype=0");
		dspace = H5Dget_space(dset);

		dimens = ncells ; /*select all values for 1 variable for 1 block */
		count= ncells;
		mspace= H5Screate_simple(1, &dimens, NULL);
		H5Sselect_hyperslab(dspace, H5S_SELECT_SET, &offset, /*&stride*/0x0, &count, NULL);

		H5Dread(dset, H5T_NATIVE_DOUBLE, mspace, dspace, H5P_DEFAULT, (void*)buf);
		unsigned int i, j, k;
		for(i=0; i < lpRet->m_uiSize[2]; i++)
			for(j=0; j < lpRet->m_uiSize[1]; j++)
				for(k=0; k < lpRet->m_uiSize[0]; k++)
					lpRet->m_lpdData[(i*lpRet->m_uiSize[1] + j)*lpRet->m_uiSize[0] + k] = buf[((i+gz)*nyb + j+gy)*nxb + k+gx];
		delete [] buf;
		H5Sclose(mspace);
		H5Sclose(dspace);
		H5Sclose(dset);
		H5Gclose(gid);
	}
	return lpRet;
}

void XFLASH_File::Open_HDF5_Chombo(void)
{
	hid_t handle = (hid_t)m_iFile_Handle;

	unsigned int nRefineLevels, numVariables, dim;
	unsigned int n_rBlocks ;
	hsize_t nB;

	hid_t gid, dspace, dset, attrib, varNameType;
	herr_t status;

	char component_name[MAX_STRING_LENGTH];
	char level_name[MAX_STRING_LENGTH];

	char tmpVarName[FR_VAR_STRING_SIZE + 1];
	

	hid_t tmpType; 

//	float dx;
	
//	int *buf;
	int idx_bbox; 
	
	unsigned int i, j, k;


	/*prepare file struct*/
//	nodetype=NULL;

	/*We are not going to use these. But sfocu code checks for them so make them 
	 * trivially zero*/
	for( i=0; i < FR_MDIM; i++)
		m_uiBlock_Dimensions[i] =0;


	m_eFormat = FMT_HDF5_CHOMBO;
	
	
	/*get total number of refinement level*/
	attrib= H5Aopen_name( handle, "num_levels");
	status= H5Aread(attrib, H5T_NATIVE_INT, (void *)&nRefineLevels);
	H5Aclose(attrib); 
	m_uiNum_Levels = nRefineLevels;
	


	/*Dimensions*/
	gid= H5Gopen(handle, "Chombo_global");
	attrib= H5Aopen_name( gid, "SpaceDim");
	status= H5Aread(attrib, H5T_NATIVE_INT, (void *)&dim);
	H5Aclose(attrib); 
	m_uiNum_Dimensions = dim;
	H5Gclose(gid);
	



	/*number of components*/
	attrib= H5Aopen_name( handle, "num_components");
	status= H5Aread(attrib, H5T_NATIVE_INT, (void *)&numVariables);
	H5Aclose(attrib); 
	m_uiNum_Vars = numVariables;

	/*Get component names*/
	varNameType = H5Tcopy(H5T_C_S1);
	H5Tset_size(varNameType, 4);
	
	tmpVarName[FR_VAR_STRING_SIZE ] ='\0';


	
	m_lpszVar_Names = new char *[numVariables];
	m_lpeVar_Types = new XFLASH_VAR_TYPE [numVariables];
	for(i=0; i < numVariables; i++)
	{
		sprintf(component_name,"component_%d", i); 
	
		attrib= H5Aopen_name( handle, component_name);
		status= H5Aread(attrib, varNameType, (void *)tmpVarName);
		H5Aclose(attrib); 

		int iLen = strlen(tmpVarName) + 1;
		if (iLen > FR_VAR_STRING_SIZE)
			iLen = FR_VAR_STRING_SIZE;
		m_lpszVar_Names[i] = new char[iLen];
		strncpy(m_lpszVar_Names[i], (char *)tmpVarName, FR_VAR_STRING_SIZE);
		m_lpszVar_Names[i][iLen - 1] = 0;
		m_lpeVar_Types[i] = VT_UNK;
	}

	H5Tclose(varNameType);
	

	/* Get Total Number of Blocks, refine level for each Block */
	
	m_uiNum_Blocks = 0;
	m_lpuiBlocks_Per_Refine_Level = new unsigned int [nRefineLevels];

	

	idx_bbox=0;

	int * buf=NULL;
	unsigned int uiBufSize = 0;

	for(i =0; i < nRefineLevels; i++)
	{
		sprintf(level_name, "level_%d", i);
		gid= H5Gopen(handle, level_name);

		dset= H5Dopen(gid, "boxes");
		dspace = H5Dget_space(dset);

		status = H5Sget_simple_extent_dims(dspace, &nB, NULL);
		n_rBlocks = (int)nB;
		
//		int block0 = m_uiNum_Blocks;
		m_uiNum_Blocks += n_rBlocks; 
		m_lpuiBlocks_Per_Refine_Level[i] = n_rBlocks;


		/*get bounding box for all the blocks on this refine level */
		
		/*
		attrib= H5Aopen_name( gid, "dx");
		status= H5Aread(attrib, H5T_NATIVE_FLOAT, (void *)&dx);
		H5Aclose(attrib); 
		*/
		unsigned int uiSize = n_rBlocks * (2 * dim + PADSIZE / 4);

		if (uiSize > uiBufSize)
		{
			if (buf)
				delete [] buf;
			buf = new int [uiSize];
			uiBufSize = uiSize;
		}

		tmpType= H5Dget_type(dset);
		status= H5Dread(dset, tmpType, H5S_ALL, H5S_ALL, H5P_DEFAULT, buf);
		H5Tclose(tmpType);

		m_lpdBlock_Bounding_Box = new double [m_uiNum_Blocks * 2 * dim];
		
		/*unroll struct into the out->bbox double array*/
		for(j = 0; j < n_rBlocks; j++)
		{
			for(k=0; k < (dim *2); k++)
			{
				m_lpdBlock_Bounding_Box[idx_bbox++] = (*buf);
				buf++;
			}
			buf += (PADSIZE/sizeof(int)); 
		}
			
		buf = NULL;



		/***clean up************/
		
		H5Sclose(dspace);
		H5Dclose(dset);
		H5Gclose(gid);

	}
	if (buf)
		delete [] buf;

	/*All Chombo blocks are LEAF blocks*/
	m_lpeBlock_Node_Type = new int[m_uiNum_Blocks];
	for(j=0; j < m_uiNum_Blocks; j++)
		m_lpeBlock_Node_Type[j]= FR_LEAF_NODE; 
	
	/* Fill size and coords array. This data set is redundant so we will fill it with zeros so that it trivially
	 * passes some checks in the sameblock() function from the old sfocu code for
	 * compatibility
	 */
	m_lpdBlock_Coords = new double [m_uiNum_Blocks * m_uiNum_Dimensions];
	m_lpdBlock_Size = new double [m_uiNum_Blocks * m_uiNum_Dimensions];
	for(j=0; j < (m_uiNum_Blocks * m_uiNum_Dimensions ); j++)
	{
		m_lpdBlock_Coords[j] = 0.;
		m_lpdBlock_Size[j] = 0.;
	}

	/* Fill lref array */
	int idx_lref =0;
	m_lpiRefinement_Levels = new int [m_uiNum_Blocks];
	for(j = 0; j < m_uiNum_Levels; j++)
	{
		for(k=0; k < m_lpuiBlocks_Per_Refine_Level[j]; k++)
			m_lpiRefinement_Levels[idx_lref++] = j;
	}
}

#undef H5_USE_16_API

