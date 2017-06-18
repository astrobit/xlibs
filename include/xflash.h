#pragma once


#define FR_MDIM 3
#define FR_VAR_STRING_SIZE 4
#define FR_LEAF_NODE 1
#define FR_MAXVARS 500 


#define FR_PART_PROP_STRING_SIZE 24
#define MAX_STRING_LENGTH 80

/*Define our variable types*/
#define XDIM 3
#define YDIM 2
#define ZDIM 1
#define NUMBLOCKS 0

/* this represents a collection of particles and all
   properties */
class XFLASH_Particle_Collection
{
public:
	unsigned int 	m_uiNumParticles;      /* number of particles in this collection */
	int * 			m_lpiProps;         /* big contiguous array of int prop values */
	double *		m_lpdProps;

	XFLASH_Particle_Collection(void);
	~XFLASH_Particle_Collection(void);
	void Void(void);
};

class XFLASH_Block
{
public:
  double *	m_lpdData; 
  unsigned int m_uiSize[3];
  /* 
  int dim1, dim2, dim3;
     Get data in cell (nxb, nyb, nzb) through data[nxb*dim1+nyb*dim2+nzb*dim3]
     Inefficient but simple. Do this later since for now we can do the 
     comparisons without knowing how the data array is arranged.
  */
};

class XFLASH_File
{
public:
	enum XFLASH_FILE_FORMAT {FMT_INVALID,FMT_HDF5,FMT_HDF5_PMESH,FMT_HDF5_CHOMBO,FMT_HDF4,FMT_NCDF};
	enum XFLASH_VAR_TYPE {VT_ERR,VT_UNK,VT_FACEX,VT_FACEY,VT_FACEZ,VT_SCRATCH};
	
	char *				m_lpszFilename;
	unsigned int		m_uiNum_Blocks;
	unsigned int		m_uiNum_Cells_per_Block;
	unsigned int		m_uiBlock_Dimensions[3]; // assuming 3-d blocks
	unsigned int		m_uiNum_Dimensions;
	int *				m_lpeBlock_Node_Type;
	double *			m_lpdBlock_Coords;
	double *			m_lpdBlock_Bounding_Box;
	double *			m_lpdBlock_Size;
	int *				m_lpiRefinement_Levels;

	unsigned int		m_uiStep;
	double				m_dTime;
	double				m_dRedshift;
	
	unsigned int		m_uiNum_Vars;
	char **				m_lpszVar_Names;
	XFLASH_VAR_TYPE *	m_lpeVar_Types;
	
	unsigned int		m_uiNum_Particles;
	unsigned int		m_uiNum_Particle_Int_Properties;
	char **				m_lpszParticle_Int_Property_Names;
	unsigned int		m_uiNum_Particle_Real_Properties;
	char **				m_lpszParticle_Real_Property_Names;

	unsigned int *		m_lpuiBlocks_Per_Refine_Level; // Chombo HDF5 only
	double *			m_lpdLevel_Size;
	unsigned int		m_uiNum_Levels;

private:
	XFLASH_FILE_FORMAT	m_eFormat;

	int					m_iFile_Handle;

	void				Void(void);

//#ifndef NO_HDF4
//	void 							Open_HDF4(void);
//	XFLASH_Block * 					GetBlock_HDF4(char *var, int block_no);
//#endif

#ifndef NO_HDF5
	int								Slurp_HDF5(int mem_type, const char *name, int dim, void *target);
	void							GetNumParticles_HDF5(void);
	void							Get_data_names_HDF5(void);
	void 							Open_HDF5(void);
	void 							Open_HDF5_Pmesh(void);
	void 							Open_HDF5_Chombo(void);
	XFLASH_Block * 					GetBlock_HDF5(unsigned int i_uiVariable_ID, unsigned int i_uiBlock_ID) const;
	XFLASH_Block * 					GetBlock_HDF5_Chombo(unsigned int i_uiVariable_ID, unsigned int i_uiBlock_ID) const;
	XFLASH_Particle_Collection * 	GetParticles_HDF5(unsigned int startingAt, unsigned int atMost, unsigned int &numberGotten) const;
#endif

//#ifndef NO_NCDF
//	void 							Open_NCDF(options_t opts);
//	XFLASH_Block * 					GetBlock_NCDF(char *var, int block_no);
//	XFLASH_Particle_Collection * 	GetParticles_NCDF(int startingAt, int atMost, int *numberGotten);
//#endif

public:
	XFLASH_File(void);
	~XFLASH_File(void);

	void 							Open(const char * i_lpszFilename);
	void 							Close(void);
	XFLASH_Block * 					GetBlock(const char * i_lpszVariable_Name, unsigned int i_uiBlock_ID) const;
	XFLASH_Block * 					GetBlock(unsigned int i_uiVariable_ID, unsigned int i_uiBlock_ID) const;
	void 							ReleaseBlock(XFLASH_Block * i_lpcBlock) const;
	XFLASH_Particle_Collection * 	GetParticles(unsigned int i_uiFirst_Particle_ID, unsigned int i_uiMax_Particles, unsigned int &o_iNumber_Retreived) const;
	void 							Release_Particles(XFLASH_Particle_Collection *) const;
	int								Get_Variable_Idx_By_Name(const char * i_lpszName) const;
};

