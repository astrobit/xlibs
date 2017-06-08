#pragma once
#include <stdio.h>
#include <math.h>
#include <vector>
#include <cassert>
#include <cstdint>
#include <xextprec.h>
//----------------------------------------------------------------------------
//
// VECTOR class
//
// class containing an n-vector
// indices for set and get are 0 referenced
//
//----------------------------------------------------------------------------
class xvector_ep
{
private:
	expdouble	*		m_lpdValues;
	size_t	m_uiN;
	size_t	m_uiN_Alloc;
	// copy function
	void			Copy(const xvector_ep &i_vRHO);
	void			Copy(const std::vector<expdouble> &i_vRHO);
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
	xvector_ep(void);
	// constructor specifying size only
	xvector_ep(size_t i_uiN);
	// copy constructor
	xvector_ep(const xvector_ep &i_cRHO);
	// destructor
	~xvector_ep(void);
	// contructor from c++ vector class
	xvector_ep(const std::vector<int8_t> &i_vRHO);
	// contructor from c++ vector class
	xvector_ep(const std::vector<unsigned int8_t> &i_vRHO);
	// contructor from c++ vector class
	xvector_ep(const std::vector<int16_t> &i_vRHO);
	// contructor from c++ vector class
	xvector_ep(const std::vector<unsigned int16_t> &i_vRHO);
	// contructor from c++ vector class
	xvector_ep(const std::vector<int32_t> &i_vRHO);
	// contructor from c++ vector class
	xvector_ep(const std::vector<unsigned int32_t> &i_vRHO);
	// contructor from c++ vector class
	xvector_ep(const std::vector<int64_t> &i_vRHO);
	// contructor from c++ vector class
	xvector_ep(const std::vector<unsigned int64_t> &i_vRHO);
	// contructor from c++ vector class
	xvector_ep(const std::vector<float> &i_vRHO);
	// contructor from c++ vector class
	xvector_ep(const std::vector<double> &i_vRHO);
	// contructor from c++ vector class
	xvector_ep(const std::vector<expdouble> &i_vRHO);
	// get size of vector
	inline size_t Get_Size(void) const{return m_uiN;}
	inline size_t size(void) const{return m_uiN;}
	// set size of vector
	void	Set_Size(size_t i_uiN);
	// set a vector element
	void	Set(size_t i_uiRow, const expdouble &i_dValue);
	// get a vector element
	expdouble	Get(size_t i_uiRow) const;
	// swap elements
	void	Swap_Rows(size_t i_uiRow_1, size_t i_uiRow_2);
	// print the matrix to file or console
	void	Print(FILE * fileOut = NULL)  const;
	// Perform dot product
	expdouble	Dot(const xvector_ep & i_vRHO) const;
	// Perform Cross product
	xvector_ep Cross3d(const xvector_ep & i_vRHO) const;
	expdouble Cross2d(const xvector_ep & i_vRHO) const;
	//xtensor	Cross(const xvector & i_vRHO) const; // need to figure out math
	// Vector multiply: x_i = l_i * r_i
	xvector_ep	Vector_Multiply(const xvector_ep & i_vRHO) const;
	// Normalize the vector (make magnitude of the vector = 1)
	void Normalize(void);
	// Set all elements to zero
	void Zero(void);
	// Get the magnitude of a vector
	expdouble	Magnitude(void) const;

	// Aritmetic operations
	xvector_ep &operator =(const xvector_ep &i_vRHO);
	xvector_ep operator +(const xvector_ep &i_vRHO) const;
	xvector_ep &operator +=(const xvector_ep &i_vRHO);
	xvector_ep operator -(const xvector_ep &i_vRHO) const;
	xvector_ep &operator -=(const xvector_ep &i_vRHO);
	xvector_ep &operator =(const std::vector<expdouble> &i_vRHO);
	xvector_ep &operator =(const std::vector<double> &i_vRHO);
	xvector_ep &operator =(const std::vector<float> &i_vRHO);
	xvector_ep &operator =(const std::vector<unsigned int64_t> &i_vRHO);
	xvector_ep &operator =(const std::vector<int64_t> &i_vRHO);
	xvector_ep &operator =(const std::vector<unsigned int32_t> &i_vRHO);
	xvector_ep &operator =(const std::vector<int32_t> &i_vRHO);
	xvector_ep &operator =(const std::vector<unsigned int16_t> &i_vRHO);
	xvector_ep &operator =(const std::vector<int16_t> &i_vRHO);
	xvector_ep &operator =(const std::vector<unsigned int8_t> &i_vRHO);
	xvector_ep &operator =(const std::vector<int8_t> &i_vRHO);
	// Scalar operatations
	xvector_ep operator -(void)  const;
	xvector_ep operator *(const expdouble &i_dRHO)  const;
	xvector_ep &operator *=(const expdouble &i_dRHO);
	xvector_ep operator /(const expdouble &i_dRHO)  const;
	xvector_ep &operator /=(const expdouble &i_dRHO);
	// Boolean operations
	bool	operator==(const xvector_ep &i_vRHO)  const;
	bool	operator!=(const xvector_ep &i_vRHO)  const;

	inline expdouble operator[](size_t i_iIndex) const {assert(i_iIndex >= 0 && i_iIndex <= m_uiN && m_lpdValues != nullptr); return m_lpdValues[i_iIndex];};
	inline expdouble &operator[](size_t i_iIndex) {assert(i_iIndex >= 0 && i_iIndex <= m_uiN && m_lpdValues != nullptr); return m_lpdValues[i_iIndex];};

	bool is_nan(void) const;
	bool is_inf(void) const;

	// allow SQUARE_MATRIX and TRIDIAG_MATRIX classes access
	// to private data members for efficiency
	friend class	xsquare_matrix_ep;
};
xvector_ep operator * (const expdouble &i_dLHO, const xvector_ep &i_vRHO);

//----------------------------------------------------------------------------
//
// SQUARE_MATRIX class
//
//----------------------------------------------------------------------------
class	xsquare_matrix_ep
{
private:
	expdouble *		m_lpdValues;
	size_t	m_uiN;
	size_t	m_uiN_Alloc;

	bool			m_bLU_Initialized;
	expdouble *		m_lpdL_Values;
	expdouble *		m_lpdU_Values;
	size_t *	m_lpuiPivot_Table;

public:
	// default constructor
	xsquare_matrix_ep(void);
	// constructor sith specified size
	xsquare_matrix_ep(size_t i_uiN);
	// copy constructor
	xsquare_matrix_ep(const xsquare_matrix_ep &i_cRHO);
	// destructor
	~xsquare_matrix_ep(void);
	// set size of matrix
	void	Set_Size(size_t i_uiN);
	// get size of matrix
	inline size_t	Get_Size(void) const{return m_uiN;}
	inline size_t	size(void) const{return m_uiN;}
	// set matrix element
	void	Set(
				size_t i_uiRow, 
				size_t i_uiCol, 
				const expdouble &i_dValue);
	// set column vector
	void	Set(size_t i_uiCol, const xvector_ep &i_vCol);
	// add a value to an element
	void	Element_Add(
			size_t i_uiRow, 
			size_t i_uiCol, 
			const expdouble &i_dValue);
	// retrieve an element value
	expdouble	Get(size_t i_uiRow, size_t i_uiCol) const;
	// get a column
	xvector_ep	GetColumn(size_t i_uiCol) const;
	// = operator: copy matrix
	xsquare_matrix_ep & operator =(const xsquare_matrix_ep &i_cRHO);
	// matrix multiplication
	xsquare_matrix_ep & operator *=(const xsquare_matrix_ep &i_cRHO);
	// matrix multiplication
	xsquare_matrix_ep operator *(const xsquare_matrix_ep &i_cRHO)  const;
	// vector multiplication: matrix A, vector x:  r = Ax
	xvector_ep operator * (const xvector_ep &i_cRHO)  const;
	// scalar multiplication
	xsquare_matrix_ep & operator *=(const expdouble &i_dRHO);
	// scalar multiplication
	xsquare_matrix_ep operator *(const expdouble &i_dRHO)  const;
	// matrix addition
	xsquare_matrix_ep & operator +=(const xsquare_matrix_ep &i_cRHO);
	xsquare_matrix_ep operator +(const xsquare_matrix_ep &i_cRHO) const;
	xsquare_matrix_ep & operator -=(const xsquare_matrix_ep &i_cRHO);
	xsquare_matrix_ep operator -(const xsquare_matrix_ep &i_cRHO) const;

	// set the matrix to the idenity matrix
	void	Identity(void);
	// set all elements to zero
	void	Zero(void);
	// scale an entire matrix row by a value
	void	Scale_Row(size_t i_uiRow, const expdouble &i_dScalar);
	// add two rows of the matrix together, leaving the result in the 'left 
	// hand operand' row.  A scalar factor is applied to the 'right hand
	// operand' row before the addition.  If the Assume RR flag is true,
	// only columns > right hand operand are modified. (the matrix is
	// assumed to be 0 in the RHO row to the left of this column.
	void	Add_Rows(
				size_t i_uiRow_LHO, 
				size_t i_uiRow_RHO, 
				const expdouble &i_dRHO_Scalar, 
				bool i_bAssume_RR = false) ;
	// Swap two rows
	void	Swap_Rows(size_t i_uiRow_1, size_t i_uiRow_2);

	// Invert a matrix using Gauss-Jordan elimination
	void	Inverse_GJ(void);
	// Using a specified set, perform back substitution
	xvector_ep	Back_Substituion(const expdouble * lpdValue_set, const xvector_ep &i_vB);
	// using the U matrix, perform forward substituion
	xvector_ep	Forward_Substituion(const xvector_ep &i_vB);

	// Perform Gaussian elimination with back substituion, given vector b,
	// find vector X such that Ax = b
	xvector_ep	Solve_GEb(const xvector_ep &i_vB);
	// Generate the L and U matrices for this matrix
	// warning: Pivoting not implemented at this time
	void Perform_LU_Decomposition(void);
	// Solves Ax = b, using LU method
	xvector_ep	Solve_LU(const xvector_ep &i_vB);
	// Inverts a matrix using LU decomposition method
	void Invert_LUD(void);
	enum WHICH {MAT_A,MAT_L,MAT_U};
	// Generate rotation matrix, given rotation about z axis (theta) and x axis (phi)
	void	Create_Rotation_Matrix(const expdouble & i_dPhi_Rad,const expdouble &i_dTheta_Rad, const expdouble & i_dPsi_Rad);

	// Transpose the matrix
	void	Transpose(void);

	// test for nan or inf
	bool is_nan(void) const;
	bool is_inf(void) const;

	xvector_ep	Get_Eigenvector(const expdouble & i_dLambda = 1.0) const;
};
xsquare_matrix_ep operator * (const expdouble &i_dLHO, const xsquare_matrix_ep &i_vRHO);

