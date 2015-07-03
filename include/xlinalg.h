#pragma once
#include <stdio.h>
#include <math.h>
#include <vector>

//----------------------------------------------------------------------------
//
// VECTOR class
//
// class containing an n-vector
// indices for set and get are 0 referenced
//
//----------------------------------------------------------------------------
class XVECTOR
{
private:
	double	*		m_lpdValues;
	unsigned int	m_uiN;
	unsigned int	m_uiN_Alloc;
	// copy function
	void			Copy(const XVECTOR &i_vRHO);
//	void			Copy(const std::vector &i_vRHO);
public:
	// default constructor
	XVECTOR(void);
	// constructor specifying size only
	XVECTOR(unsigned int i_uiN);
	// copy constructor
	XVECTOR(const XVECTOR &i_cRHO);
	// contructor from c++ vector class
//	XVECTOR(const std::vector<double> &i_vRHO);
	// get size of vector
	unsigned int Get_Size(void) const{return m_uiN;}
	// set size of vector
	void	Set_Size(unsigned int i_uiN);
	// set a vector element
	void	Set(unsigned int i_uiRow, const double &i_dValue);
	// get a vector element
	double	Get(unsigned int i_uiRow) const;
	// swap elements
	void	Swap_Rows(unsigned int i_uiRow_1, unsigned int i_uiRow_2);
	// print the matrix to file or console
	void	Print(FILE * fileOut = NULL)  const;
	// Perform dot product
	double	Dot(const XVECTOR & i_vRHO) const;
	// Perform Cross product
	XVECTOR Cross3d(const XVECTOR & i_vRHO) const;
	double Cross2d(const XVECTOR & i_vRHO) const;
	//XTENSOR	Cross(const XVECTOR & i_vRHO) const; // need to figure out math
	// Vector multiply: x_i = l_i * r_i
	XVECTOR	Vector_Multiply(const XVECTOR & i_vRHO) const;
	// Normalize the vector (make magnitude of the vector = 1)
	void Normalize(void);
	// Set all elements to zero
	void Zero(void);
	// Get the magnitude of a vector
	double	Magnitude(void) const;

	// Aritmetic operations
	XVECTOR &operator =(const XVECTOR &i_vRHO);
	XVECTOR operator +(const XVECTOR &i_vRHO) const;
	XVECTOR &operator +=(const XVECTOR &i_vRHO);
	XVECTOR operator -(const XVECTOR &i_vRHO) const;
	XVECTOR &operator -=(const XVECTOR &i_vRHO);
	// Scalar operatations
	XVECTOR operator -(void)  const;
	XVECTOR operator *(const double &i_dRHO)  const;
	XVECTOR &operator *=(const double &i_dRHO);
	XVECTOR operator /(const double &i_dRHO)  const;
	XVECTOR &operator /=(const double &i_dRHO);
	// Boolean operations
	bool	operator==(const XVECTOR &i_vRHO)  const;
	bool	operator!=(const XVECTOR &i_vRHO)  const;

	// allow SQUARE_MATRIX and TRIDIAG_MATRIX classes access
	// to private data members for efficiency
	friend class	XTENSOR;
	friend class	XSQUARE_MATRIX;
	friend class	XTRIDIAG_MATRIX;
};
XVECTOR operator * (const double &i_dLHO, const XVECTOR &i_vRHO);


class XINDEX_VECTOR
{
private:
	unsigned int *	m_lpuiValues;
	unsigned int	m_uiN;
	unsigned int	m_uiN_Alloc;
	// copy function
	void			Copy(const XINDEX_VECTOR &i_vRHO);
public:
	// default constructor
	XINDEX_VECTOR(void);
	// constructor specifying size only
	XINDEX_VECTOR(unsigned int i_uiN);
	// copy constructor
	XINDEX_VECTOR(const XINDEX_VECTOR &i_cRHO);
	// get size of vector
	unsigned int Get_Size(void) const{return m_uiN;}
	// set size of vector
	void	Set_Size(unsigned int i_uiN);
	// set an element
	void	Set(unsigned int i_uiRow, const unsigned int &i_uiValue);
	// get an element
	unsigned int	Get(unsigned int i_uiRow);
	// swap elements
	void	Swap_Rows(unsigned int i_uiRow_1, unsigned int i_uiRow_2);
	// print the matrix to file or console
	void	Print(FILE * fileOut = NULL);
	// Aritmetic operations
	XINDEX_VECTOR &operator =(const XINDEX_VECTOR &i_vRHO);
	XINDEX_VECTOR operator +(const XINDEX_VECTOR &i_vRHO) const;
	XINDEX_VECTOR &operator +=(const XINDEX_VECTOR &i_vRHO);
	XINDEX_VECTOR operator -(const XINDEX_VECTOR &i_vRHO) const;
	XINDEX_VECTOR &operator -=(const XINDEX_VECTOR &i_vRHO);
	// Scalar operatations
	XINDEX_VECTOR operator -(void)  const;
	// Boolean operations
	bool	operator==(const XINDEX_VECTOR &i_vRHO)  const;
	bool	operator!=(const XINDEX_VECTOR &i_vRHO)  const;

	// allow SQUARE_MATRIX and TRIDIAG_MATRIX classes access
	// to private data members for efficiency
	friend class	XTENSOR;
	friend class	XSQUARE_MATRIX;
	friend class	XTRIDIAG_MATRIX;
};

//----------------------------------------------------------------------------
//
// TENSOR class
//
// Generalized tensor container
//
//----------------------------------------------------------------------------
class XTENSOR
{
private:
	double *		m_lpdValues;
	unsigned int	m_uiRank;
	XINDEX_VECTOR	m_uivDimensions;
	XINDEX_VECTOR	m_uivIndexing_Vector;
	unsigned int	m_uiNum_Entries;
	unsigned int	m_uiSize_Alloc;
	void			Copy(const XTENSOR &i_tRHO);
	void			Allocate(const XINDEX_VECTOR	&i_uivDimensions);
	unsigned int	ComputeIndex(const XINDEX_VECTOR	&i_uivElement_Index);
public:
	// default constructor
	XTENSOR(void);
	// constructor sith specified size
	XTENSOR(const XINDEX_VECTOR &i_vDimensions);
	// copy constructor
	XTENSOR(const XTENSOR &i_cRHO);
	// set size of tensor
	void	Set_Size(const XINDEX_VECTOR & i_uivDimensions);
	// set tensor element
	void	Set(const XINDEX_VECTOR & i_uivElement_Index, const double &i_dValue);
	// get tensor element
	double	Get(const XINDEX_VECTOR & i_uivElement_Index);
};
//----------------------------------------------------------------------------
//
// SQUARE_MATRIX class
//
//----------------------------------------------------------------------------
class	XSQUARE_MATRIX
{
private:
	double *		m_lpdValues;
	unsigned int	m_uiN;
	unsigned int	m_uiN_Alloc;

	bool			m_bLU_Initialized;
	double *		m_lpdL_Values;
	double *		m_lpdU_Values;
	unsigned int *	m_lpuiPivot_Table;

public:
	// default constructor
	XSQUARE_MATRIX(void);
	// constructor sith specified size
	XSQUARE_MATRIX(unsigned int i_uiN);
	// copy constructor
	XSQUARE_MATRIX(const XSQUARE_MATRIX &i_cRHO);
	// set size of matrix
	void	Set_Size(unsigned int i_uiN);
	// set matrix element
	void	Set(
				unsigned int i_uiRow, 
				unsigned int i_uiCol, 
				const double &i_dValue);
	// set column vector
	void	Set(unsigned int i_uiCol, const XVECTOR &i_vCol);
	// add a value to an element
	void	Element_Add(
			unsigned int i_uiRow, 
			unsigned int i_uiCol, 
			const double &i_dValue);
	// retrieve an element value
	double	Get(unsigned int i_uiRow, unsigned int i_uiCol) const;
	// get a column
	XVECTOR	GetColumn(unsigned int i_uiCol) const;
	// = operator: copy matrix
	XSQUARE_MATRIX & operator =(const XSQUARE_MATRIX &i_cRHO);
	// matrix multiplication
	XSQUARE_MATRIX & operator *=(const XSQUARE_MATRIX &i_cRHO);
	// matrix multiplication
	XSQUARE_MATRIX operator *(const XSQUARE_MATRIX &i_cRHO)  const;
	// vector multiplication: matrix A, vector x:  r = Ax
	XVECTOR operator * (const XVECTOR &i_cRHO)  const;
	// scalar multiplication
	XSQUARE_MATRIX & operator *=(const double &i_dRHO);
	// scalar multiplication
	XSQUARE_MATRIX operator *(const double &i_dRHO)  const;
	// matrix addition
	XSQUARE_MATRIX & operator +=(const XSQUARE_MATRIX &i_cRHO);
	XSQUARE_MATRIX operator +(const XSQUARE_MATRIX &i_cRHO) const;
	XSQUARE_MATRIX & operator -=(const XSQUARE_MATRIX &i_cRHO);
	XSQUARE_MATRIX operator -(const XSQUARE_MATRIX &i_cRHO) const;

	// set the matrix to the idenity matrix
	void	Identity(void);
	// set all elements to zero
	void	Zero(void);
	// scale an entire matrix row by a value
	void	Scale_Row(unsigned int i_uiRow, const double &i_dScalar);
	// add two rows of the matrix together, leaving the result in the 'left 
	// hand operand' row.  A scalar factor is applied to the 'right hand
	// operand' row before the addition.  If the Assume RR flag is true,
	// only columns > right hand operand are modified. (the matrix is
	// assumed to be 0 in the RHO row to the left of this column.
	void	Add_Rows(
				unsigned int i_uiRow_LHO, 
				unsigned int i_uiRow_RHO, 
				const double &i_dRHO_Scalar, 
				bool i_bAssume_RR = false) ;
	// Swap two rows
	void	Swap_Rows(unsigned int i_uiRow_1, unsigned int i_uiRow_2);

	// Invert a matrix using Gauss-Jordan elimination
	void	Inverse_GJ(void);
	// Using a specified set, perform back substitution
	XVECTOR	Back_Substituion(const double * lpdValue_set, const XVECTOR &i_vB);
	// using the U matrix, perform forward substituion
	XVECTOR	Forward_Substituion(const XVECTOR &i_vB);

	// Perform Gaussian elimination with back substituion, given vector b,
	// find vector X such that Ax = b
	XVECTOR	Solve_GEb(const XVECTOR &i_vB);
	// Generate the L and U matrices for this matrix
	// warning: Pivoting not implemented at this time
	void Perform_LU_Decomposition(void);
	// Solves Ax = b, using LU method
	XVECTOR	Solve_LU(const XVECTOR &i_vB);
	// Inverts a matrix using LU decomposition method
	void Invert_LUD(void);
	enum WHICH {MAT_A,MAT_L,MAT_U};
	// print the matrix to file or console
	void	Print(const char * lpszFormat = "%f", char chSeparator = '\t', 
					FILE * i_fileOut = NULL, WHICH i_eWhich = MAT_A);
	// Generate rotation matrix, given rotation about z axis (theta) and x axis (phi)
	void	Create_Rotation_Matrix(const double & i_dPhi_Rad,const double &i_dTheta_Rad, const double & i_dPsi_Rad);

	// Transpose the matrix
	void	Transpose(void);
};
XSQUARE_MATRIX operator * (const double &i_dLHO, const XSQUARE_MATRIX &i_vRHO);

//----------------------------------------------------------------------------
//
// TRIDIAG_MATRIX class
// for a matrix of the form
// [b_1  c_1  0    0  ...  0 ]
// [a_2  b_2  c_2  0  ...  0 ]
// [0    a_3  b_3 c_3 ...  0 ]
// [...  ...  ... ... ... ...]
// [0 ... 0      a_(n-1) b_(n-1)]
//
//----------------------------------------------------------------------------
class	XTRIDIAG_MATRIX
{
private:
	double *		m_lpdValues;
	unsigned int	m_uiN;
	unsigned int	m_uiN_Alloc;

	double	*		m_lpdGH;
public:

	// Set size n of matrix
	void	Set_Size(unsigned int i_uiN);
	// default constructor
	XTRIDIAG_MATRIX(void);
	// constructor to set size
	XTRIDIAG_MATRIX(unsigned int i_uiN);
	// copy constructor
	XTRIDIAG_MATRIX(const XTRIDIAG_MATRIX &i_cRHO);
	// set a particlar row/column
	void	Set(
		unsigned int i_uiRow, 
		unsigned int i_uiCol, 
		const double &i_dValue);

	// set a row
	void	Set(
		unsigned int i_uiRow, 
		const double &i_dValue_Alpha, 
		const double &i_dValue_Beta, 
		const double &i_dValue_Gamma);
	// add a value to a particular row/column element
	void	Element_Add(
		unsigned int i_uiRow, 
		unsigned int i_uiCol, 
		const double &i_dValue);
	// get a row/column element
	double	Get(unsigned int i_uiRow, unsigned int i_uiCol);
	// get g,h for a row
	void	Get_GH(unsigned int i_uiRow, double &o_dGi, double &o_dHi);
	// = operator (copy another tridiagonal matrix)
	XTRIDIAG_MATRIX & operator =(const XTRIDIAG_MATRIX &i_cRHO);
	// multiply a vector x into the matrix, such that r = Ax
	XVECTOR operator * (const XVECTOR &i_cRHO);

	// scale a row by a given value
	void	Scale_Row(unsigned int i_uiRow, const double &i_dScalar);
	// For the expression Ax = b, compute x given b.
	XVECTOR	Compute_Inverse(const XVECTOR &i_vB);
	// print the matrix to file or console
	void	Print(FILE * fileOut = NULL);
};

