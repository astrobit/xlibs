#include <string.h>
#include <stdlib.h>
#define H5_USE_16_API  1
#include "hdf5.h"
#include "xflash.h"

typedef struct 
{
  /* DEV - hard code for now the length of 'name' - NTT */
	char name[MAX_STRING_LENGTH];
	int value;
} int_list_t;

typedef struct 
{
  /* DEV - hard code for now the length of 'name' - NTT */
	char name[MAX_STRING_LENGTH];
	double value;
} real_list_t;

/* The strings in the 'name' field of the 'integer scalars' dataset
   have a constant length of 80 characters where the meaningful part
   of the word is at the beginning and the rest is padded out with
   spaces. Here we only compare the meaningful part of the word with
   the name we are looking for, ignoring the whitespace. */
bool NameCompare(const char *a, const char *b)
{
	bool bRet = false;
	while (a && b && a[0] != 0 && b[0] != 0 && a[0] != ' ' && (a[0] == b[0]))
	{
		a++;
		b++;
	}

  /* if a or b has not terminated, it's not a match */
	if ((a[0] == 0 || a[0] == ' ') && b[0] == 0)
		bRet = true;
	return bRet;
}


///////////////////////////////////////////////////////////////////////////////////////////////
//
//	Slurp_HDF5
//
// This function reads a dataset from an HDF5 file
///////////////////////////////////////////////////////////////////////////////////////////////

int XFLASH_File::Slurp_HDF5(int mem_type, const char *name, int dim, void *target)
{
	hid_t dataset, dataset_type, dataspace, memspace;
	herr_t status;
	bool bAuto_Type = false;

	/*for special handling*/
	hsize_t dimens_3d[3],maxdimens_3d[3], mem_dimens_3d[3];
	hsize_t count_3d[3], start_3d[3], stride_3d[3];

	hsize_t dimens_2d[2], maxdimens_2d[2], mem_dimens_2d[2];
	hsize_t count_2d[2], start_2d[2], stride_2d[2];
	int rank;



	if (!mem_type)
		bAuto_Type = true;

	dataset = H5Dopen((hid_t)m_iFile_Handle, name);
	if (dataset < 0)
	{
		printf("Error reading \"%s\": dataset not found\n", name);
		return 1; 
	}

  /* Not sure how safe it is to get mem_type from the dataset, so
   I'm trying this for a while.
   Eventually eliminate the mem_type argument.
   */
	if (bAuto_Type)
	{ /* get type from dataset */
		mem_type = H5Dget_type(dataset);
		dataset_type = mem_type;
	}
	else
	{ /*just check type */
		dataset_type = H5Dget_type(dataset);
		if (H5Tget_class(mem_type) != H5Tget_class(dataset_type))
			printf("\"%s\" has an unexpected data type\n", name);
	}
  
	/*This has to change for bounding box*/
	if(strncmp("bounding box", name, 12) == 0)
	{
		/*make sure memory space is as though this is 3d*/
		dataspace = H5Dget_space(dataset);
		H5Sget_simple_extent_dims(dataspace, dimens_3d, maxdimens_3d);


		/*we have to make sure we are going to correctly map to memory*/

		start_3d[0] = 0;
		start_3d[1] = 0;
		start_3d[2] = 0;

		stride_3d[0] = 1;
		stride_3d[1] = 1;
		stride_3d[2] = 1;

		count_3d[0] = dimens_3d[0];
		count_3d[1] = (hsize_t)dim;
		count_3d[2] = dimens_3d[2];

		H5Sselect_hyperslab(dataspace, H5S_SELECT_SET, start_3d, stride_3d, count_3d, NULL);

		rank = 3;
		mem_dimens_3d[0] = dimens_3d[0];
		mem_dimens_3d[1] = (hsize_t)dim;
		mem_dimens_3d[2] = dimens_3d[2];

		memspace = H5Screate_simple(rank, mem_dimens_3d, NULL);

		H5Sselect_hyperslab(memspace, H5S_SELECT_SET, start_3d, stride_3d, count_3d, NULL);

		status = H5Dread(dataset, mem_type, memspace, dataspace, H5P_DEFAULT, target);
		if (status < 0)
		{
			printf("Error reading \"%s\"\n", name);
			return 1; 
		}  
		H5Sclose(memspace);
		H5Sclose(dataspace);

	}/*end if bounding box*/
	else if ((strncmp(name, "coordinates", 11) == 0) || (strncmp("block size", name, 10) == 0))
	{
		dataspace = H5Dget_space(dataset);
		H5Sget_simple_extent_dims(dataspace, dimens_2d, maxdimens_2d);


		/*we have to make sure we are going to correctly map to memory*/

		start_2d[0] = 0;
		start_2d[1] = 0;

		stride_2d[0] = 1;
		stride_2d[1] = 1;

		count_2d[0] = dimens_2d[0];
		count_2d[1] = (hsize_t)dim;

		H5Sselect_hyperslab(dataspace, H5S_SELECT_SET, start_2d, stride_2d, count_2d, NULL);

		rank = 2;
		mem_dimens_2d[0] = dimens_2d[0];
		mem_dimens_2d[1] = (hsize_t)dim;

		memspace = H5Screate_simple(rank, mem_dimens_2d, NULL);

		H5Sselect_hyperslab(memspace, H5S_SELECT_SET, start_2d, stride_2d, count_2d, NULL);

		status = H5Dread(dataset, mem_type, memspace, dataspace, H5P_DEFAULT, target);
		if (status < 0)
		{
			printf("Error reading \"%s\"\n", name);
			return 1; 
		}  
		H5Sclose(memspace);
		H5Sclose(dataspace);

	} /*end coordinates handling*/
	else
	{/*normal slurp*/
		status = H5Dread(dataset, dataset_type, H5S_ALL, H5S_ALL, H5P_DEFAULT, target);
		if (status < 0)
		{
			printf("Error reading \"%s\"\n", name);
			return 1; 
		}
	}
	H5Tclose(dataset_type);

	H5Dclose(dataset);
	return 0; 
}

///////////////////////////////////////////////////////////////////////////////////////////////
//
//	GetParticles
//
///////////////////////////////////////////////////////////////////////////////////////////////

XFLASH_Particle_Collection *  XFLASH_File::GetParticles_HDF5(unsigned int startingAt, unsigned int atMost, unsigned int &numberGotten) const
{
	XFLASH_Particle_Collection *	lpRet = NULL;
//	int i;
	herr_t   status;
	hsize_t  dimens_1d, maxdims_1d;
	hsize_t start_2d[2];
//	hsize_t  stride_2d[2];
	hsize_t count_2d[2];
//	hsize_t dims_out[2];
	int rank;
	hid_t dataset, dataspace, memspace;
	unsigned int  to_read;
//	hid_t particle_tid;

	numberGotten = 0;
	if (m_uiNum_Particles > 0)
	{
		to_read = atMost;
		if (to_read > (m_uiNum_Particles - startingAt))
		{
			to_read = m_uiNum_Particles - startingAt;
		}

		if (to_read > 0)
		{
			lpRet = new XFLASH_Particle_Collection;

			lpRet->m_uiNumParticles = to_read;
			lpRet->m_lpiProps = new int[to_read * m_uiNum_Particle_Int_Properties];
			lpRet->m_lpdProps = new double[to_read * m_uiNum_Particle_Real_Properties];
			
			dataset = H5Dopen((hid_t)m_iFile_Handle, "tracer particles");

			dataspace = H5Dget_space(dataset);

			rank       = 1;
			start_2d[0]   = startingAt;
			start_2d[1]   = 0;
			count_2d[0]   = (hsize_t) (to_read);
			count_2d[1]   = (hsize_t) (m_uiNum_Particle_Real_Properties);
			maxdims_1d = (hsize_t) (m_uiNum_Particles * m_uiNum_Particle_Real_Properties);
			dimens_1d  = (hsize_t) (to_read*m_uiNum_Particle_Real_Properties);

			status     = H5Sselect_hyperslab(dataspace, H5S_SELECT_SET, start_2d, NULL, count_2d, NULL);

			memspace   = H5Screate_simple(rank, &dimens_1d, &maxdims_1d);

			/* read data from the dataset */
			status = H5Dread(dataset, H5T_NATIVE_DOUBLE, memspace, dataspace, H5P_DEFAULT, lpRet->m_lpdProps);

			if (status < 0)
			{
				printf("Error reading particles from file '%s'\n", m_lpszFilename);
			}
			else
			{
				numberGotten = to_read;
			}

			H5Dclose(dataset);
			H5Sclose(dataspace);
			H5Sclose(memspace);

		} 
	}
	else
	{
		numberGotten = 0;
	}
	return lpRet;
}

///////////////////////////////////////////////////////////////////////////////////////////////
//
//	GetNumParticles
//
//  This function sets the m_uiNum_Particles member of XFLASH_File, using the file data in
//  the already open file (m_iFile_Handle)
//
///////////////////////////////////////////////////////////////////////////////////////////////

void XFLASH_File::GetNumParticles_HDF5(void)
{

	hid_t dataset, dataspace;
	hsize_t maximum_dims[10];
	hsize_t dataspace_dims[10];
	herr_t (*old_func)(void*);
	void *old_client_data;
//	hid_t partTypeId;

//	int numElements;

	/* temporarily turn off error handling, to probe for the dataset's existence */
	H5Eget_auto(&old_func, &old_client_data);
	H5Eset_auto(NULL, NULL);

	/* get the dataset ID for the tracer partlcies record, and the dataspace in
	this record */
	dataset = H5Dopen((hid_t) m_iFile_Handle, "tracer particles");

	/* restore the error handling */
	H5Eset_auto(old_func, old_client_data);

	m_uiNum_Particles = 0;
	if (dataset >= 0)
	{
		dataspace = H5Dget_space(dataset);

		/* get the dimensions of the dataspace */    
		H5Sget_simple_extent_dims(dataspace, dataspace_dims, maximum_dims);

		m_uiNum_Particles = dataspace_dims[0];

		H5Dclose(dataset);
		H5Sclose(dataspace);

	}
}




///////////////////////////////////////////////////////////////////////////////////////////////
//
//	Open_HDF5
//
//  This function opens the file, determines its format, and reads the data
//
///////////////////////////////////////////////////////////////////////////////////////////////

void XFLASH_File::Open_HDF5(void)
{
//	hid_t handle;
	hid_t gid;
	int status;

	//Turn off automatic error printing
	status = H5Eset_auto(NULL, NULL);

 
  	m_iFile_Handle = H5Fopen(m_lpszFilename, H5F_ACC_RDONLY, H5P_DEFAULT);
//  	*((hid_t *)out->handle) = handle;

  	if (m_iFile_Handle >= 0)
	{
	  	//Determine if this is a Chombo or Paramesh file
		if( (gid= H5Gopen((hid_t)m_iFile_Handle, "Chombo_global")) < 0)
		{
			m_eFormat = FMT_HDF5_PMESH;
			Open_HDF5_Pmesh();
		}
		else
		{
			m_eFormat = FMT_HDF5_CHOMBO;
			Open_HDF5_Chombo();
		}

		H5Gclose(gid);
	}
}

#if 0
// These functions are used for debugging purposes
void PrintIntData(const int_list_t & cInt_List_Data)
{
	unsigned int uiI = 0;
	while (uiI < MAX_STRING_LENGTH && cInt_List_Data.name[uiI] != ' ' && cInt_List_Data.name[uiI] != 0)
	{
		printf("%c",cInt_List_Data.name[uiI]);
		uiI++;
	}
	printf("\t%i\n",cInt_List_Data.value);
}

void PrintRealData(const real_list_t & cList_Data)
{
	unsigned int uiI = 0;
	while (uiI < MAX_STRING_LENGTH && cList_Data.name[uiI] != ' ' && cList_Data.name[uiI] != 0)
	{
		printf("%c",cList_Data.name[uiI]);
		uiI++;
	}
	printf("\t%.2e\n",cList_Data.value);
}
#endif

///////////////////////////////////////////////////////////////////////////////////////////////
//
//	Open_HDF5_Pmesh
//
//  This function reads the basic file data from a P-mesh HDF5 file
//
///////////////////////////////////////////////////////////////////////////////////////////////

void XFLASH_File::Open_HDF5_Pmesh(void)
{
	unsigned int i;
	int status;
	char	* lpszTempNames;
	unsigned int uiTempNameSize;

	hid_t dataspace, memspace, dataset;//, name_dataset;
//	hsize_t maximum_dims[10];
//	hsize_t dataspace_dims[10];
	hsize_t dimens_1d, maxdimens_1d;
	hid_t string_type;
	hid_t int_list_type;
	int_list_t *int_list;

	hid_t real_list_type;
	real_list_t *real_list;

//	hid_t num_particles;
//	hid_t partTypeId;
//	int numElements;
//	char *name;
//	hid_t type, native_type;
//	int numRealPropsFound, numIntPropsFound;

//	herr_t (*old_func)(void*);
//	void *old_client_data;

//	int num_vars;


	/* integer scalars */

	/* grab the data with the name 'integer scalars' from the file */
	dataset = H5Dopen((hid_t)m_iFile_Handle, "integer scalars"); 

	dataspace = H5Dget_space(dataset);

	/* read the extent of 'dataspace' into 'dimens_1d' */
	H5Sget_simple_extent_dims(dataspace, &dimens_1d, &maxdimens_1d);

	/* malloc a pointer to a list of int_list_t's */
	int_list = new int_list_t[dimens_1d]; 

	/* create an empty vessel sized to hold one int_list_t's worth of data */
	int_list_type = H5Tcreate(H5T_COMPOUND, sizeof(int_list_t));

	string_type = H5Tcopy(H5T_C_S1);
	H5Tset_size(string_type, MAX_STRING_LENGTH);

	/* subdivide the empty vessel into its component sections (name and value) */
	H5Tinsert(int_list_type, "name", HOFFSET(int_list_t, name), string_type);

	H5Tinsert(int_list_type, "value", HOFFSET(int_list_t, value), H5T_NATIVE_INT);

	/* create a new simple dataspace of 1 dimension and size of 'dimens_1d' */
	memspace = H5Screate_simple(1, &dimens_1d, NULL);

	status = H5Dread(dataset, int_list_type, memspace, dataspace, H5P_DEFAULT, int_list);
	if (status >= 0)
	{
		/* compare this value's 'name' field to the word we're looking for
		using our 'specialcmp' function (defined above) */  
		for (i = 0; i < dimens_1d; i++)
		{
			if (NameCompare(int_list[i].name, "globalnumblocks")) m_uiNum_Blocks = int_list[i].value;
			else if (NameCompare(int_list[i].name, "nxb")) m_uiBlock_Dimensions[0] = int_list[i].value;
			else if (NameCompare(int_list[i].name, "nyb")) m_uiBlock_Dimensions[1] = int_list[i].value;
			else if (NameCompare(int_list[i].name, "nzb")) m_uiBlock_Dimensions[2] = int_list[i].value;
			else if (NameCompare(int_list[i].name, "nstep")) m_uiStep = int_list[i].value;
			
		}

		H5Tclose(int_list_type);

		H5Sclose(dataspace);
		H5Dclose(dataset);

		dataset = H5Dopen((hid_t)m_iFile_Handle, "real scalars"); 

		dataspace = H5Dget_space(dataset);

		/* read the extent of 'dataspace' into 'dimens_1d' */
		H5Sget_simple_extent_dims(dataspace, &dimens_1d, &maxdimens_1d);

		/* malloc a pointer to a list of int_list_t's */
		real_list = new real_list_t[dimens_1d]; 

		/* create an empty vessel sized to hold one int_list_t's worth of data */
		real_list_type = H5Tcreate(H5T_COMPOUND, sizeof(real_list_t));

		string_type = H5Tcopy(H5T_C_S1);
		H5Tset_size(string_type, MAX_STRING_LENGTH);

		/* subdivide the empty vessel into its component sections (name and value) */
		H5Tinsert(real_list_type, "name", HOFFSET(real_list_t, name), string_type);

		H5Tinsert(real_list_type, "value", HOFFSET(real_list_t, value), H5T_NATIVE_DOUBLE);

		/* create a new simple dataspace of 1 dimension and size of 'dimens_1d' */
		memspace = H5Screate_simple(1, &dimens_1d, NULL);

		status = H5Dread(dataset, real_list_type, memspace, dataspace, H5P_DEFAULT, real_list);
		if (status >= 0)
		{
			/* compare this value's 'name' field to the word we're looking for
			using our 'specialcmp' function (defined above) */  
			for (i = 0; i < dimens_1d; i++)
			{
				if (NameCompare(real_list[i].name, "time")) m_dTime = real_list[i].value;
				else if (NameCompare(real_list[i].name, "redshift")) m_dRedshift = real_list[i].value;
			
			}
		}

		H5Tclose(real_list_type);

		H5Sclose(dataspace);
		H5Dclose(dataset);
		/* done with integer scalars */

		/* use ncells_vec to get dim */
		if (m_uiBlock_Dimensions[2] > 1)
			m_uiNum_Dimensions = 3;
		else if (m_uiBlock_Dimensions[1] > 1)
			m_uiNum_Dimensions = 2;
		else
			m_uiNum_Dimensions = 1;

		m_uiNum_Cells_per_Block = m_uiBlock_Dimensions[0] * m_uiBlock_Dimensions[1] * m_uiBlock_Dimensions[2];

		m_lpeBlock_Node_Type = new int[m_uiNum_Blocks];
		m_lpiRefinement_Levels = new int[m_uiNum_Blocks];
		m_lpdBlock_Coords = new double[m_uiNum_Blocks * m_uiNum_Dimensions];
		m_lpdBlock_Bounding_Box = new double[m_uiNum_Blocks * m_uiNum_Dimensions * 2];
		m_lpdBlock_Size = new double[m_uiNum_Blocks * m_uiNum_Dimensions];

		if(Slurp_HDF5(H5T_NATIVE_INT, "node type",m_uiNum_Dimensions, m_lpeBlock_Node_Type))
			return;

		if(Slurp_HDF5(H5T_NATIVE_INT, "refine level",m_uiNum_Dimensions, m_lpiRefinement_Levels))
			return;

		if(Slurp_HDF5(H5T_NATIVE_DOUBLE, "coordinates", m_uiNum_Dimensions, m_lpdBlock_Coords))
			return;

		if(Slurp_HDF5(H5T_NATIVE_DOUBLE, "block size", m_uiNum_Dimensions, m_lpdBlock_Size))
			return;

		if(Slurp_HDF5(H5T_NATIVE_DOUBLE, "bounding box", m_uiNum_Dimensions, m_lpdBlock_Bounding_Box))
			return;


		/* number of variables and variable names */

//		if(Slurp_HDF5((hid_t) 0, "unknown names", m_uiNum_Dimensions, lpszTempNames))
//			return;

		Get_data_names_HDF5();
		GetNumParticles_HDF5();

		m_uiNum_Particle_Real_Properties = 0;
		m_uiNum_Particle_Int_Properties = 0;
		if (m_uiNum_Particles > 0)
		{
			/* new file format without compound datatype, and no integer properties */

			/* number of real properties and names */
			uiTempNameSize = FR_PART_PROP_STRING_SIZE;
			lpszTempNames = new char[uiTempNameSize];
			memset(lpszTempNames,0,uiTempNameSize);

			if(Slurp_HDF5((hid_t) 0, "particle names", m_uiNum_Dimensions, lpszTempNames))
				return;

			while (lpszTempNames[m_uiNum_Particle_Real_Properties*FR_PART_PROP_STRING_SIZE] != 0 && m_uiNum_Particle_Real_Properties < FR_MAXVARS)
			{
				m_uiNum_Particle_Real_Properties++;
			}

			m_lpszParticle_Real_Property_Names = new char *[m_uiNum_Particle_Real_Properties];
			for (i = 0; i < m_uiNum_Particle_Real_Properties; i++)
			{
				int iLen = strlen(&lpszTempNames[i*FR_PART_PROP_STRING_SIZE]) + 1;
				if (iLen > FR_PART_PROP_STRING_SIZE)
					iLen = FR_PART_PROP_STRING_SIZE;
				m_lpszParticle_Real_Property_Names[i] = new char [iLen];
				strncpy(m_lpszParticle_Real_Property_Names[i],&lpszTempNames[i*FR_PART_PROP_STRING_SIZE],iLen);
				m_lpszParticle_Real_Property_Names[i][iLen - 1] = 0;
			}

			delete [] lpszTempNames;
		}
	}
	delete [] int_list;
}

///////////////////////////////////////////////////////////////////////////////////////////////
//
//	GetBlock_HDF5
//
//  This function retrieves a block of data from the file
//
///////////////////////////////////////////////////////////////////////////////////////////////

XFLASH_Block * XFLASH_File::GetBlock_HDF5(unsigned int i_uiVariable_ID, unsigned int i_uiBlock_ID) const
{
	XFLASH_Block * lpRet = NULL;
	int rank;
	hsize_t dimens_4d[4];
	hsize_t start_4d[5];
	hsize_t stride_4d[5], count_4d[5];
	hid_t ierr;
	herr_t status;
	hid_t dataspace, memspace, dataset;

//	unsigned int i;//, varType;
	int xCells, yCells, zCells, numCells;

	if (i_uiVariable_ID < m_uiNum_Vars && i_uiBlock_ID < m_uiNum_Blocks)
	{
		xCells = m_uiBlock_Dimensions[0];
		yCells = m_uiBlock_Dimensions[1];
		zCells = m_uiBlock_Dimensions[2];
	
		switch(m_lpeVar_Types[i_uiVariable_ID])
		{
		case VT_UNK:
			/*do nothing*/  
			break;
		case VT_FACEX:
			xCells += 1;
			break;
		case VT_FACEY:
			yCells += 1;
			break;
		case VT_FACEZ:
			zCells += 1;
			break;
		case VT_SCRATCH:
			xCells += 1;
			yCells += (m_uiNum_Dimensions >=2)?1:0;
			zCells += (m_uiNum_Dimensions >=3)?1:0;
			break;
		default:
			printf("in default case: HALP!\n");
			/*defaults to unk behavior*/
			break;
		
		}
		numCells = xCells*yCells*zCells;

		lpRet = new XFLASH_Block;
		lpRet->m_lpdData = new double[numCells];

		lpRet->m_uiSize[0] = xCells;
		lpRet->m_uiSize[1] = yCells;
		lpRet->m_uiSize[2] = zCells;

	  /* Now do the hdf5 dance */
	  
	  /* define the dataspace */
		rank = 4;
		dimens_4d[0] = m_uiNum_Blocks;
		dimens_4d[1] = zCells; /* reverse order */
		dimens_4d[2] = yCells;
		dimens_4d[3] = xCells;

		start_4d[0]  = (hsize_t) i_uiBlock_ID;
		start_4d[1]  = 0;
		start_4d[2]  = 0;
		start_4d[3]  = 0;

		stride_4d[0] = 1;
		stride_4d[1] = 1;
		stride_4d[2] = 1;
		stride_4d[3] = 1;

		count_4d[0]  = 1; /*Just take one block */
		count_4d[1]  = zCells;
		count_4d[2]  = yCells;
		count_4d[3]  = xCells;

		dataspace = H5Screate_simple(rank, dimens_4d, NULL);
		ierr = H5Sselect_hyperslab(dataspace, H5S_SELECT_SET, start_4d, stride_4d, count_4d, NULL);

	  /* define the memspace - just a small modification of the above */
		dimens_4d[0] = 1; /* Again, just one block */
		start_4d[0] = 0;
		memspace = H5Screate_simple(rank, dimens_4d, NULL);
		ierr = H5Sselect_hyperslab(memspace, H5S_SELECT_SET, start_4d, stride_4d, count_4d, NULL);

	  /* Now read */
		dataset = H5Dopen((hid_t)m_iFile_Handle, m_lpszVar_Names[i_uiVariable_ID]);
		status  = H5Dread(dataset, H5T_NATIVE_DOUBLE, memspace, dataspace, H5P_DEFAULT, (void *) lpRet->m_lpdData);
		H5Sclose(memspace);
		H5Sclose(dataspace);
		H5Dclose(dataset);
	}
  
	return lpRet;
}

///////////////////////////////////////////////////////////////////////////////////////////////
//
//	Get_data_names_HDF5
//
//  This function retrieves the names of UNK, FACE, and SCRATCH variables in the file
//
///////////////////////////////////////////////////////////////////////////////////////////////

void XFLASH_File::Get_data_names_HDF5(void)
{
	hsize_t num_objs;//, num_dims, num_obj, obj_type, name_size;
	hid_t dataset, dataspace;

	hsize_t dimens_4d[4], maxdimens_4d[4];

	char obj_name[MAX_STRING_LENGTH];

	unsigned int i, iVar;


	H5Gget_num_objs((hid_t)m_iFile_Handle, &num_objs);
	m_uiNum_Vars = 0;
	for( i = 0; i < num_objs; ++i)
	{

    /*have to iterate over objects, look at dimensions, and store the ones we want*/
	    if(H5Gget_objtype_by_idx((hid_t)m_iFile_Handle, i) == H5G_DATASET)
		{
			H5Gget_objname_by_idx((hid_t)m_iFile_Handle, (hsize_t)i,obj_name, 80);
		
			dataset = H5Dopen((hid_t)m_iFile_Handle, obj_name);
			dataspace = H5Dget_space(dataset);
			if(dataspace >= 0 && H5Sget_simple_extent_ndims(dataspace) == 4)
			    m_uiNum_Vars++;
		}
	}
	m_lpszVar_Names = new char * [m_uiNum_Vars];
	m_lpeVar_Types = new XFLASH_VAR_TYPE[m_uiNum_Vars];

	iVar = 0;
	for( i = 0; i < num_objs; ++i)
	{

    /*have to iterate over objects, look at dimensions, and store the ones we want*/
	    if(H5Gget_objtype_by_idx((hid_t)m_iFile_Handle, i) == H5G_DATASET)
		{
			H5Gget_objname_by_idx((hid_t)m_iFile_Handle, (hsize_t)i,obj_name, 80);
		
			dataset = H5Dopen((hid_t)m_iFile_Handle, obj_name);
			dataspace = H5Dget_space(dataset);
			if(dataspace >= 0 && H5Sget_simple_extent_ndims(dataspace) == 4)
			{
				int iLen = strlen(obj_name) + 1;
				if (iLen > MAX_STRING_LENGTH)
					iLen = MAX_STRING_LENGTH;
				m_lpszVar_Names[iVar] = new char[iLen];
				
				strncpy(m_lpszVar_Names[iVar], obj_name, iLen);
				m_lpszVar_Names[iVar][iLen - 1] = 0;

				/*determine the type of variable we are looking at (face vs unk?)*/
				/*A bit ugly for now, but should work. */

				H5Sget_simple_extent_dims(dataspace, dimens_4d, maxdimens_4d);

				bool bXplus1 = dimens_4d[XDIM] == (m_uiBlock_Dimensions[0] + 1);
				bool bYplus1 = dimens_4d[YDIM] == (m_uiBlock_Dimensions[0] + 1);
				bool bZplus1 = dimens_4d[ZDIM] == (m_uiBlock_Dimensions[0] + 1);
				bool bFace = (bXplus1 ^ bYplus1 ^ bZplus1);
				bool bX = dimens_4d[XDIM] == m_uiBlock_Dimensions[0];
				bool bY = dimens_4d[YDIM] == m_uiBlock_Dimensions[1];
				bool bZ = dimens_4d[ZDIM] == m_uiBlock_Dimensions[2];

				if (bX && bY && bZ)
					m_lpeVar_Types[iVar] = VT_UNK;
				else if (bXplus1 && bYplus1 && bZplus1)
					m_lpeVar_Types[iVar] = VT_SCRATCH;
				else if (bFace && bXplus1)
					m_lpeVar_Types[iVar] = VT_FACEX;
				else if (bFace && bYplus1)
					m_lpeVar_Types[iVar] = VT_FACEY;
				else if (bFace && bZplus1)
					m_lpeVar_Types[iVar] = VT_FACEZ;
				else
				{
					m_lpeVar_Types[m_uiNum_Vars] = VT_ERR;
					printf("Unknown variable type.\n");
				}
			    iVar++;
			}
			H5Sclose(dataspace);
			H5Dclose(dataset);
		}
	}
}

#undef H5_USE_16_API

