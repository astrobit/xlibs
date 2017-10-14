#pragma once
#include <stdio.h>
#include <math.h>
#include <vector>
#include <cassert>
#include <cstdint>
//----------------------------------------------------------------------------
//
// VECTOR class
//
// class containing an n-vector
// indices for set and get are 0 referenced
//
//----------------------------------------------------------------------------
class xvector_long
{
private:
	long double	*		m_lpdValues;
	size_t	m_uiN;
	size_t	m_uiN_Alloc;
	// copy function
	void			Copy(const xvector_long &i_vRHO);
	void			Copy(const std::vector<long double> &i_vRHO);
	void			Copy(const std::vector<double> &i_vRHO);
	void			Copy(const std::vector<float> &i_vRHO);
	void			Copy(const std::vector<unsigned int64_t> &i_vRHO);
	void			Copy(const std::vector<int64_t> &i_vRHO);
	void			Copy(const std::vector<unsigned int32_t> &i_vRHO);
	void			Copy(const std::vector<int32_t> &i_vRHO);
	void			Copy(const std::vector<unsigned int16_t> &i_vRHO);
	void			Copy(const std::vector<int16_t> &i_vRHO);
	void			Copy(const std::vector<unsigned int8_t> &i_vRHO);
	void			Copy(const std::vector<int8_t> &i_vRHO);
public:
	// default constructor
	xvector_long(void);
	// constructor specifying size only
	xvector_long(size_t i_uiN);
	// copy constructor
	xvector_long(const xvector_long &i_cRHO);
	// destructor
	~xvector_long(void);
	// contructor from c++ vector class
	xvector_long(const std::vector<int8_t> &i_vRHO);
	// contructor from c++ vector class
	xvector_long(const std::vector<unsigned int8_t> &i_vRHO);
	// contructor from c++ vector class
	xvector_long(const std::vector<int16_t> &i_vRHO);
	// contructor from c++ vector class
	xvector_long(const std::vector<unsigned int16_t> &i_vRHO);
	// contructor from c++ vector class
	xvector_long(const std::vector<int32_t> &i_vRHO);
	// contructor from c++ vector class
	xvector_long(const std::vector<unsigned int32_t> &i_vRHO);
	// contructor from c++ vector class
	xvector_long(const std::vector<int64_t> &i_vRHO);
	// contructor from c++ vector class
	xvector_long(const std::vector<unsigned int64_t> &i_vRHO);
	// contructor from c++ vector class
	xvector_long(const std::vector<float> &i_vRHO);
	// contructor from c++ vector class
	xvector_long(const std::vector<double> &i_vRHO);
	// contructor from c++ vector class
	xvector_long(const std::vector<long double> &i_vRHO);
	// get size of vector
	inline size_t Get_Size(void) const{return m_uiN;}
	inline size_t size(void) const{return m_uiN;}
	// set size of vector
	void	Set_Size(size_t i_uiN);
	// set a vector element
	void	Set(size_t i_uiRow, const long double &i_dValue);
	// get a vector element
	long double	Get(size_t i_uiRow) const;
	// swap elements
	void	Swap_Rows(size_t i_uiRow_1, size_t i_uiRow_2);
	// print the matrix to file or console
	void	Print(FILE * fileOut = NULL)  const;
	// Perform dot product
	long double	Dot(const xvector_long & i_vRHO) const;
	// Perform Cross product
	xvector_long Cross3d(const xvector_long & i_vRHO) const;
	long double Cross2d(const xvector_long & i_vRHO) const;
	//xtensor	Cross(const xvector & i_vRHO) const; // need to figure out math
	// Vector multiply: x_i = l_i * r_i
	xvector_long	Vector_Multiply(const xvector_long & i_vRHO) const;
	// Normalize the vector (make magnitude of the vector = 1)
	void Normalize(void);
	// Set all elements to zero
	void Zero(void);
	// Get the magnitude of a vector
	long double	Magnitude(void) const;

	// Aritmetic operations
	xvector_long &operator =(const xvector_long &i_vRHO);
	xvector_long operator +(const xvector_long &i_vRHO) const;
	xvector_long &operator +=(const xvector_long &i_vRHO);
	xvector_long operator -(const xvector_long &i_vRHO) const;
	xvector_long &operator -=(const xvector_long &i_vRHO);
	xvector_long &operator =(const std::vector<long double> &i_vRHO);
	xvector_long &operator =(const std::vector<double> &i_vRHO);
	xvector_long &operator =(const std::vector<float> &i_vRHO);
	xvector_long &operator =(const std::vector<unsigned int64_t> &i_vRHO);
	xvector_long &operator =(const std::vector<int64_t> &i_vRHO);
	xvector_long &operator =(const std::vector<unsigned int32_t> &i_vRHO);
	xvector_long &operator =(const std::vector<int32_t> &i_vRHO);
	xvector_long &operator =(const std::vector<unsigned int16_t> &i_vRHO);
	xvector_long &operator =(const std::vector<int16_t> &i_vRHO);
	xvector_long &operator =(const std::vector<unsigned int8_t> &i_vRHO);
	xvector_long &operator =(const std::vector<int8_t> &i_vRHO);
	// Scalar operatations
	xvector_long operator -(void)  const;
	xvector_long operator *(const long double &i_dRHO)  const;
	xvector_long &operator *=(const long double &i_dRHO);
	xvector_long operator /(const long double &i_dRHO)  const;
	xvector_long &operator /=(const long double &i_dRHO);
	// Boolean operations
	bool	operator==(const xvector_long &i_vRHO)  const;
	bool	operator!=(const xvector_long &i_vRHO)  const;

	inline long double operator[](size_t i_iIndex) const {assert(i_iIndex >= 0 && i_iIndex <= m_uiN && m_lpdValues != nullptr); return m_lpdValues[i_iIndex];};
	inline long double &operator[](size_t i_iIndex) {assert(i_iIndex >= 0 && i_iIndex <= m_uiN && m_lpdValues != nullptr); return m_lpdValues[i_iIndex];};

	bool is_nan(void) const;
	bool is_inf(void) const;

	// allow SQUARE_MATRIX and TRIDIAG_MATRIX classes access
	// to private data members for efficiency
	friend class	xsquare_matrix_long;
};
xvector_long operator * (const long double &i_dLHO, const xvector_long &i_vRHO);


//----------------------------------------------------------------------------
//
// VECTOR class
//
// class containing an n-vector
// indices for set and get are 0 referenced
//
//----------------------------------------------------------------------------
class xvector
{
private:
	double	*		m_lpdValues;
	unsigned int	m_uiN;
	unsigned int	m_uiN_Alloc;
	// copy function
	void			Copy(const xvector &i_vRHO);
	void			Copy(const std::vector<double> &i_vRHO);
public:
	// default constructor
	xvector(void);
	// constructor specifying size only
	xvector(unsigned int i_uiN);
	// copy constructor
	xvector(const xvector &i_cRHO);
	// destructor
	~xvector(void);
	// contructor from c++ vector class
	xvector(const std::vector<double> &i_vRHO);
	// get size of vector
	inline unsigned int Get_Size(void) const{return m_uiN;}
	inline size_t size(void) const{return m_uiN;}
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
	double	Dot(const xvector & i_vRHO) const;
	// Perform Cross product
	xvector Cross3d(const xvector & i_vRHO) const;
	double Cross2d(const xvector & i_vRHO) const;
	//xtensor	Cross(const xvector & i_vRHO) const; // need to figure out math
	// Vector multiply: x_i = l_i * r_i
	xvector	Vector_Multiply(const xvector & i_vRHO) const;
	// Normalize the vector (make magnitude of the vector = 1)
	void Normalize(void);
	// Set all elements to zero
	void Zero(void);
	// Get the magnitude of a vector
	double	Magnitude(void) const;

	// Aritmetic operations
	xvector &operator =(const xvector &i_vRHO);
	xvector operator +(const xvector &i_vRHO) const;
	xvector &operator +=(const xvector &i_vRHO);
	xvector operator -(const xvector &i_vRHO) const;
	xvector &operator -=(const xvector &i_vRHO);
	xvector &operator =(const std::vector<double> &i_vRHO);
	// Scalar operatations
	xvector operator -(void)  const;
	xvector operator *(const double &i_dRHO)  const;
	xvector &operator *=(const double &i_dRHO);
	xvector operator /(const double &i_dRHO)  const;
	xvector &operator /=(const double &i_dRHO);
	// Boolean operations
	bool	operator==(const xvector &i_vRHO)  const;
	bool	operator!=(const xvector &i_vRHO)  const;

	inline double operator[](int i_iIndex) const {assert(i_iIndex >= 0 && i_iIndex <= m_uiN && m_lpdValues != nullptr); return m_lpdValues[i_iIndex];};
	inline double &operator[](int i_iIndex) {assert(i_iIndex >= 0 && i_iIndex <= m_uiN && m_lpdValues != nullptr); return m_lpdValues[i_iIndex];};
	inline double operator[](unsigned int i_iIndex) const {assert(i_iIndex >= 0 && i_iIndex <= m_uiN && m_lpdValues != nullptr); return m_lpdValues[i_iIndex];};
	inline double &operator[](unsigned int i_iIndex) {assert(i_iIndex >= 0 && i_iIndex <= m_uiN && m_lpdValues != nullptr); return m_lpdValues[i_iIndex];};
	inline double operator[](size_t i_iIndex) const {assert(i_iIndex >= 0 && i_iIndex <= m_uiN && m_lpdValues != nullptr); return m_lpdValues[i_iIndex];};
	inline double &operator[](size_t i_iIndex) {assert(i_iIndex >= 0 && i_iIndex <= m_uiN && m_lpdValues != nullptr); return m_lpdValues[i_iIndex];};

	bool is_nan(void) const;
	bool is_inf(void) const;

	// allow SQUARE_MATRIX and TRIDIAG_MATRIX classes access
	// to private data members for efficiency
	friend class	xtensor;
	friend class	xsquare_matrix;
	friend class	xtridiag_matrix;
};
xvector operator * (const double &i_dLHO, const xvector &i_vRHO);
typedef xvector	XVECTOR;


class xindex_vector
{
private:
	unsigned int *	m_lpuiValues;
	unsigned int	m_uiN;
	unsigned int	m_uiN_Alloc;
	// copy function
	void			Copy(const xindex_vector &i_vRHO);
public:
	// default constructor
	xindex_vector(void);
	// constructor specifying size only
	xindex_vector(unsigned int i_uiN);
	// copy constructor
	xindex_vector(const xindex_vector &i_cRHO);
	// destructor
	~xindex_vector(void);
	// get size of vector
	inline unsigned int Get_Size(void) const{return m_uiN;}
	inline size_t size(void) const {return m_uiN;}
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
	xindex_vector &operator =(const xindex_vector &i_vRHO);
	xindex_vector operator +(const xindex_vector &i_vRHO) const;
	xindex_vector &operator +=(const xindex_vector &i_vRHO);
	xindex_vector operator -(const xindex_vector &i_vRHO) const;
	xindex_vector &operator -=(const xindex_vector &i_vRHO);
	// Scalar operatations
	xindex_vector operator -(void)  const;
	// Boolean operations
	bool	operator==(const xindex_vector &i_vRHO)  const;
	bool	operator!=(const xindex_vector &i_vRHO)  const;
	// Set all elements to zero
	void Zero(void);

	// allow SQUARE_MATRIX and TRIDIAG_MATRIX classes access
	// to private data members for efficiency
	friend class	xtensor;
	friend class	xsquare_matrix;
	friend class	xtridiag_matrix;
};
typedef xindex_vector	XINDEX_VECTOR;

//----------------------------------------------------------------------------
//
// TENSOR class
//
// Generalized tensor container
//
//----------------------------------------------------------------------------
class xtensor
{
private:
	double *		m_lpdValues;
	unsigned int	m_uiRank;
	xindex_vector	m_uivDimensions;
	xindex_vector	m_uivIndexing_Vector;
	unsigned int	m_uiNum_Entries;
	unsigned int	m_uiSize_Alloc;
	void			Copy(const xtensor &i_tRHO);
	void			Allocate(const xindex_vector	&i_uivDimensions);
	unsigned int	ComputeIndex(const xindex_vector	&i_uivElement_Index) const;
public:
	// default constructor
	xtensor(void);
	// constructor sith specified size
	xtensor(const xindex_vector &i_vDimensions);
	// copy constructor
	xtensor(const xtensor &i_cRHO);
	// destructor
	~xtensor(void);
	// get size of tensor
	inline size_t Get_Rank(void) const {return m_uiRank;}
	inline xindex_vector Get_Dimensions(void) const {return m_uivDimensions;}
	// set size of tensor
	void	Set_Size(const xindex_vector & i_uivDimensions);
	// set tensor element
	void	Set(const xindex_vector & i_uivElement_Index, const double &i_dValue);
	// get tensor element
	double	Get(const xindex_vector & i_uivElement_Index) const;
};
typedef xtensor	XTENSOR;
//----------------------------------------------------------------------------
//
// SQUARE_MATRIX class
//
//----------------------------------------------------------------------------
class	xsquare_matrix
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
	xsquare_matrix(void);
	// constructor sith specified size
	xsquare_matrix(unsigned int i_uiN);
	// copy constructor
	xsquare_matrix(const xsquare_matrix &i_cRHO);
	// destructor
	~xsquare_matrix(void);
	// set size of matrix
	void	Set_Size(unsigned int i_uiN);
	// get size of matrix
	inline size_t	Get_Size(void) const{return m_uiN;}
	inline size_t	size(void) const{return m_uiN;}
	// set matrix element
	void	Set(
				unsigned int i_uiRow, 
				unsigned int i_uiCol, 
				const double &i_dValue);
	// set column vector
	void	Set(unsigned int i_uiCol, const xvector &i_vCol);
	// add a value to an element
	void	Element_Add(
			unsigned int i_uiRow, 
			unsigned int i_uiCol, 
			const double &i_dValue);
	// retrieve an element value
	double	Get(unsigned int i_uiRow, unsigned int i_uiCol) const;
	// get a column
	xvector	GetColumn(unsigned int i_uiCol) const;
	// = operator: copy matrix
	xsquare_matrix & operator =(const xsquare_matrix &i_cRHO);
	// matrix multiplication
	xsquare_matrix & operator *=(const xsquare_matrix &i_cRHO);
	// matrix multiplication
	xsquare_matrix operator *(const xsquare_matrix &i_cRHO)  const;
	// vector multiplication: matrix A, vector x:  r = Ax
	xvector operator * (const xvector &i_cRHO)  const;
	// scalar multiplication
	xsquare_matrix & operator *=(const double &i_dRHO);
	// scalar multiplication
	xsquare_matrix operator *(const double &i_dRHO)  const;
	// matrix addition
	xsquare_matrix & operator +=(const xsquare_matrix &i_cRHO);
	xsquare_matrix operator +(const xsquare_matrix &i_cRHO) const;
	xsquare_matrix & operator -=(const xsquare_matrix &i_cRHO);
	xsquare_matrix operator -(const xsquare_matrix &i_cRHO) const;

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
	xvector	Back_Substituion(const double * lpdValue_set, const xvector &i_vB);
	// using the U matrix, perform forward substituion
	xvector	Forward_Substituion(const xvector &i_vB);

	// Perform Gaussian elimination with back substituion, given vector b,
	// find vector X such that Ax = b
	xvector	Solve_GEb(const xvector &i_vB);
	// Generate the L and U matrices for this matrix
	// warning: Pivoting not implemented at this time
	void Perform_LU_Decomposition(void);
	// Solves Ax = b, using LU method
	xvector	Solve_LU(const xvector &i_vB);
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

	// test for nan or inf
	bool is_nan(void) const;
	bool is_inf(void) const;

	xvector	Get_Eigenvector(const double & i_dLambda = 1.0) const;
};
xsquare_matrix operator * (const double &i_dLHO, const xsquare_matrix &i_vRHO);
typedef xsquare_matrix XSQUARE_MATRIX;

//----------------------------------------------------------------------------
//
// SQUARE_MATRIX class
//
//----------------------------------------------------------------------------
class	xsquare_matrix_long
{
private:
	long double *		m_lpdValues;
	size_t	m_uiN;
	size_t	m_uiN_Alloc;

	bool			m_bLU_Initialized;
	long double *		m_lpdL_Values;
	long double *		m_lpdU_Values;
	size_t *	m_lpuiPivot_Table;

public:
	// default constructor
	xsquare_matrix_long(void);
	// constructor sith specified size
	xsquare_matrix_long(size_t i_uiN);
	// copy constructor
	xsquare_matrix_long(const xsquare_matrix_long &i_cRHO);
	// destructor
	~xsquare_matrix_long(void);
	// set size of matrix
	void	Set_Size(size_t i_uiN);
	// get size of matrix
	inline size_t	Get_Size(void) const{return m_uiN;}
	inline size_t	size(void) const{return m_uiN;}
	// set matrix element
	void	Set(
				size_t i_uiRow, 
				size_t i_uiCol, 
				const long double &i_dValue);
	// set column vector
	void	Set(size_t i_uiCol, const xvector_long &i_vCol);
	// add a value to an element
	void	Element_Add(
			size_t i_uiRow, 
			size_t i_uiCol, 
			const long double &i_dValue);
	// retrieve an element value
	long double	Get(size_t i_uiRow, size_t i_uiCol) const;
	// get a column
	xvector_long	GetColumn(size_t i_uiCol) const;
	// = operator: copy matrix
	xsquare_matrix_long & operator =(const xsquare_matrix_long &i_cRHO);
	// matrix multiplication
	xsquare_matrix_long & operator *=(const xsquare_matrix_long &i_cRHO);
	// matrix multiplication
	xsquare_matrix_long operator *(const xsquare_matrix_long &i_cRHO)  const;
	// vector multiplication: matrix A, vector x:  r = Ax
	xvector_long operator * (const xvector_long &i_cRHO)  const;
	// scalar multiplication
	xsquare_matrix_long & operator *=(const long double &i_dRHO);
	// scalar multiplication
	xsquare_matrix_long operator *(const long double &i_dRHO)  const;
	// matrix addition
	xsquare_matrix_long & operator +=(const xsquare_matrix_long &i_cRHO);
	xsquare_matrix_long operator +(const xsquare_matrix_long &i_cRHO) const;
	xsquare_matrix_long & operator -=(const xsquare_matrix_long &i_cRHO);
	xsquare_matrix_long operator -(const xsquare_matrix_long &i_cRHO) const;

	// set the matrix to the idenity matrix
	void	Identity(void);
	// set all elements to zero
	void	Zero(void);
	// scale an entire matrix row by a value
	void	Scale_Row(size_t i_uiRow, const long double &i_dScalar);
	// add two rows of the matrix together, leaving the result in the 'left 
	// hand operand' row.  A scalar factor is applied to the 'right hand
	// operand' row before the addition.  If the Assume RR flag is true,
	// only columns > right hand operand are modified. (the matrix is
	// assumed to be 0 in the RHO row to the left of this column.
	void	Add_Rows(
				size_t i_uiRow_LHO, 
				size_t i_uiRow_RHO, 
				const long double &i_dRHO_Scalar, 
				bool i_bAssume_RR = false) ;
	// Swap two rows
	void	Swap_Rows(size_t i_uiRow_1, size_t i_uiRow_2);

	// Invert a matrix using Gauss-Jordan elimination
	void	Inverse_GJ(void);
	// Using a specified set, perform back substitution
	xvector_long	Back_Substituion(const long double * lpdValue_set, const xvector_long &i_vB);
	// using the U matrix, perform forward substituion
	xvector_long	Forward_Substituion(const xvector_long &i_vB);

	// Perform Gaussian elimination with back substituion, given vector b,
	// find vector X such that Ax = b
	xvector_long	Solve_GEb(const xvector_long &i_vB);
	// Generate the L and U matrices for this matrix
	// warning: Pivoting not implemented at this time
	void Perform_LU_Decomposition(void);
	// Solves Ax = b, using LU method
	xvector_long	Solve_LU(const xvector_long &i_vB);
	// Inverts a matrix using LU decomposition method
	void Invert_LUD(void);
	enum WHICH {MAT_A,MAT_L,MAT_U};
	// print the matrix to file or console
	void	Print(const char * lpszFormat = "%Lf", char chSeparator = '\t', 
					FILE * i_fileOut = NULL, WHICH i_eWhich = MAT_A);
	// Generate rotation matrix, given rotation about z axis (theta) and x axis (phi)
	void	Create_Rotation_Matrix(const long double & i_dPhi_Rad,const long double &i_dTheta_Rad, const long double & i_dPsi_Rad);

	// Transpose the matrix
	void	Transpose(void);

	// test for nan or inf
	bool is_nan(void) const;
	bool is_inf(void) const;

	xvector_long	Get_Eigenvector(const long double & i_dLambda = 1.0) const;
};
xsquare_matrix_long operator * (const long double &i_dLHO, const xsquare_matrix_long &i_vRHO);

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
class	xtridiag_matrix
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
	xtridiag_matrix(void);
	// constructor to set size
	xtridiag_matrix(unsigned int i_uiN);
	// copy constructor
	xtridiag_matrix(const xtridiag_matrix &i_cRHO);
	// destructor
	~xtridiag_matrix(void);
	// Get size
	inline unsigned int Get_Size(void) const {return m_uiN;}
	inline size_t size(void) const {return m_uiN;}
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
	xtridiag_matrix & operator =(const xtridiag_matrix &i_cRHO);
	// multiply a vector x into the matrix, such that r = Ax
	xvector operator * (const xvector &i_cRHO);

	// scale a row by a given value
	void	Scale_Row(unsigned int i_uiRow, const double &i_dScalar);
	// For the expression Ax = b, compute x given b.
	xvector	Compute_Inverse(const xvector &i_vB);
	// print the matrix to file or console
	void	Print(FILE * fileOut = NULL);
};
typedef xtridiag_matrix	XTRIDIAG_MATRIX;

