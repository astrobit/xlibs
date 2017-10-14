#pragma once

#define xpolynomial_VER	"0.9.0b"
#define xpolynomial_VER_0_9_0_B

#include "xcomplex.h"
///////////////////////////////////////////////////////////////////////////////////////////
// class xpolynomial
//
// Purpose:
//	xpolynomial is a class container for a polynomial of positive degree (that is, 
//  a polynomial of the form a(n) x^n, where n is a positive number or zero.  This container
//  class simply stores all of the coefficients, and allows
//
//  The polynomial container includes operators for polynomial addition, subtraction,
//   and multiplication with either another polynomial or a scalar.  Division by a scalar is
//   also allowed.
//  Addition and subtraction of a scalar affect only the 0th degree term of the polynomial
//  multiplication (and division) affect all terms of the polynomial.
//
//  Additional methods are supplied to set the coefficients from an array of type double, to 
//	get the degree of the polynomial, and to get a particular coefficient.
//
//  There are also two functions to evaluate the polynomial at a particular point.  
//  The only difference between the two is whether a complex number or a simple real number is
/// input.  A complex number input produces a complex number output, whereas a real number input
//  will produce a real number output
//  The evaluate functions can also evaluate derivitives of the polynomial.  By default, the 
//  0th derivitive (e.g. the function itself) is evaluated.
//
//  there are three special functions: normalize, which reduces the coefficient of the highest
//  order term to 1 (adjusting all other coefficients appropriately),
//  a funciton which returns the maximum coefficient or the polynomial,  and a function which
//  will adjust all coefficients to attempt to stay within limits of double precision numbers

class xpolynomial
{
// Non-accessable (private) members
private:
	// array of coefficients
	xcomplex *	m_lpCoefficients;
	// degree of polynomial
	int			m_iDegree;
	// allocated length of the array - effectively equals (max degree + 1)
	int			m_iAllocated_Length;

	// reduces degree of poly if the lead coefficients are 0
	void	Collapse_Degree(void);
	// copys an array of coefficients
	void	Copy_Polynomial(const double * a_lpCoefficients, int a_iDegree);
	// copys an array of coefficients
	void	Copy_Polynomial(const xcomplex * a_lpCoefficients, int a_iDegree);

public:
	// constructors, including copy constructor
	xpolynomial(void);
	xpolynomial(const xpolynomial & cRHO);
	xpolynomial(const double * lpCoefficients, int iDegree);
	xpolynomial(const xcomplex * lpCoefficients, int iDegree);
	// destructor - deallocates internal memory
	~xpolynomial(void);

	// sets the minimum memory requirement - will be expanded if more is needed
	void Set_Allocated_Memory(int iDegree);
	// sets the polynomial, index = degree (lpCoefficients[2] = a(2)  i.e.  lpCoefficients[2] * x^2)
	void Set(const double * lpCoefficients, int iDegree);
	void Set(const xcomplex * lpCoefficients, int iDegree);

	// Sets an individual coefficient
	void SetCoefficient(const double &dCoefficient, int iDegree);
	void SetCoefficient(const xcomplex &xcCoefficient, int iDegree);

	// = operator, set one polynomial equal to another
	xpolynomial &operator = (const xpolynomial & cRHO);

	// add-assign operator:  add the right operand to the lowest order coefficient
	xpolynomial &operator += (const double & dRHO);
	// subtract-assign operator:  subtract the right operand from the lowest order coefficient
	xpolynomial &operator -= (const double & dRHO);
	// multiply-assign operator:  multiply each coefficient by the right operand
	xpolynomial &operator *= (const double & dRHO);
	// multiply-assign operator:  divide each coefficient by the right operand
	xpolynomial &operator /= (const double & dRHO);

	// add-assign operator:  add the right operand to the lowest order coefficient
	xpolynomial &operator += (const xcomplex & dRHO);
	// subtract-assign operator:  subtract the right operand from the lowest order coefficient
	xpolynomial &operator -= (const xcomplex & dRHO);
	// multiply-assign operator:  multiply each coefficient by the right operand
	xpolynomial &operator *= (const xcomplex & dRHO);
	// multiply-assign operator:  divide each coefficient by the right operand
	xpolynomial &operator /= (const xcomplex & dRHO);

	// add-assign operator:  add the right operand polynomial to this polynomial
	xpolynomial &operator += (const xpolynomial & cRHO);
	// subtract-assign operator:  subtract the right operand polynomial from this polynomial
	xpolynomial &operator -= (const xpolynomial & cRHO);
	// multiply-assign operator:  multiply the right operand polynomial from this polynomial
	xpolynomial &operator *= (const xpolynomial & cRHO);
	// note: no /= operator - would need to add a coefficient array for the denominator

	// Get the coefficient of a particular degree
	double	GetCoeff(int a_iDegree) const {double dRet = 0.0; if (a_iDegree <= m_iDegree) dRet = m_lpCoefficients[a_iDegree].dRe; return dRet;}
	xcomplex	GetCoeffC(int a_iDegree) const {xcomplex cRet = xcomplex(0.0,0.0); if (a_iDegree <= m_iDegree) cRet = m_lpCoefficients[a_iDegree]; return cRet;}
	// Get the degree of the polynomial
	int GetDegree(void) const {return m_iDegree;}

	// Evaluate the polynomial at a point X.  iDeriv is the derivitive desired: 0 = function, 
	// 1 = first derivative (df/dx), 2 = second derivitive (d^2f/dx^2), etc.
	xcomplex Eval(const xcomplex &cX, unsigned int iDeriv = 0) const;
	double Eval(const double &dX, unsigned int iDeriv = 0) const;

	// Normalize - reduce coefficient of higest order degree to 1
	xcomplex Normalize(void);
	// retrun the largest valued coefficient
	double GetMaxCoeff(void) const;
	// find the scalar value which represents the average of the exponents of the function
	// i.e. 10^60 x^2 + 10^-30 x + 1  -> the result would be 10^15.  By dividing the function by this, you get
	// 10^45 x^2 + 10^-45 + 10^-15.  This minimizes chance of floating point exponent overflow
	double Get_Reduce_Coeff(void) const;

	unsigned int Get_Trivial_Zeros(void) const;
	unsigned int Trivial_Divide(unsigned int uiDegree);
	void Trivial_Multiply(unsigned int uiDegree);
	xcomplex	Synthetic_Division(const xcomplex & xcRoot);

};
typedef xpolynomial XPOLYNOMIAL;

// TODO: Add xpolynomial_COMPLEX
// TODO: Add *,+,- operators to xpolynomial
// TODO: Add root finding function?
