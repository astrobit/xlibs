#pragma once
#include "xcomplex.h"
#include "xpolynomial.h"
#include "xmath.h"
#define XROOTS_VER "0.9.2b"
#define XROOTS_VER_0_9_2_B

///////////////////////////////////////////////////////////////////////////////////////////
// XROOTS
//
// Purpose:
//	XROOTS is a set of functions which will find the root of a function or polynomial
//
//

////////////////////////////////////////////////////////////////////////////////////////////
// Newton's Method
//
// Xn+1 = Xn - f(Xn) / f'(Xn)
//
// Xn+1 is next guess
// Xn is current guess (or arbitrary starting point at n=0
// f(Xn) is function evaluated at Xn
// f'(Xn) is first derivative of function evalutated at Xn

bool			XROOT_Newton(const xpolynomial &cPoly,xcomplex & cRoot);
bool			XROOT_Newton(const xpolynomial &cPoly,double & dRoot);
bool			XROOT_Newton(QFunctionC funcQFunctionC, QFunctionC funcQFunctionDerivCconst,xcomplex & cRoot, const void * i_lpvData = NULL);
bool			XROOT_Newton(QFunctionD funcQFunctionD, QFunctionD funcQFunctionDerivD,double & dRoot, const void * i_lpvData = NULL);

bool			XROOT_Newton(const xpolynomial &cPoly,xcomplex & cRoot, const xcomplex &cStart_Pt);
bool			XROOT_Newton(const xpolynomial &cPoly,double & dRoot, const double &dStart_Pt);
bool			XROOT_Newton(QFunctionC funcQFunctionC, QFunctionC funcQFunctionDerivCconst,xcomplex & cRoot, const xcomplex &cStart_Pt, const void * i_lpvData = NULL);
bool			XROOT_Newton(QFunctionD funcQFunctionD, QFunctionD funcQFunctionDerivD,double & dRoot, const double &dStart_Pt, const void * i_lpvData = NULL);

////////////////////////////////////////////////////////////////////////////////////////////
// Secant Method
//
// Xn+1 = (Xn - Xn-1) / (f(Xn) - f(Xn-1)) * f(Xn)
//
// Xn+1 is next guess
// Xn is current guess (or arbitrary starting point at n=0)
// Xn-1 is previous guess (or arbitrary starting point at n=0, (Xn != Xn-1))
// f(Xn) is function evaluated at Xn
// f(Xn-1) is function evaluated at Xn-1
//

bool		XROOT_Secant(const xpolynomial &cPoly, xcomplex &cRoot);
bool		XROOT_Secant(const xpolynomial &cPoly, double &dRoot);
bool		XROOT_Secant(QFunctionC funcQFunctionC, xcomplex &cRoot, const void * i_lpvData = NULL);
bool		XROOT_Secant(QFunctionD funcQFunctionD, double &dRoot, const void * i_lpvData = NULL);

bool		XROOT_Secant(const xpolynomial &cPoly, xcomplex &cRoot, const xcomplex &cStart_Pt);
bool		XROOT_Secant(const xpolynomial &cPoly, double &dRoot, const double &dStart_Pt);
bool		XROOT_Secant(QFunctionC funcQFunctionC, xcomplex &cRoot, const xcomplex &cStart_Pt, const void * i_lpvData = NULL);
bool		XROOT_Secant(QFunctionD funcQFunctionD, double &dRoot, const double &dStart_Pt, const void * i_lpvData = NULL);

////////////////////////////////////////////////////////////////////////////////////////////
//  Maehly roots (technically Newton-Maehly) use a iterative Newton type approach, but will
//  find multiple roots of a function
//
//  Maehly method:
//
//
//  Xn = X0 - f(X0) / (f'(X0) - f(X0) * SIGMA(n=0->N)( 1 / (X0 - R(n)))
//
// where X0 is (arbitrary) starting point
// Xn is next guess
// f(X0) is function evaluated at X0
// f'(X0) is first derivitive of function evaluated at X0
// N is number of roots found so far
// R is list of all roots of f(x) that have been so far been found (R(n) is a root in this list
// SIGMA(n=0->N)(g(n)) is the sum of g(n) (i.e. (g(0) + g(1) + ... + g(N))
//
// Multiple versions of the function are available which can take in a polynomial using xpolynomial,
// or a non-polynomial function using a function pointer.
// Multiple versions are also avaible which look for either real or real and complex roots of the 
// function

void			XROOT_Maehly(const xpolynomial &cPoly, xcomplex * lpcResults, unsigned int iMax_Results, unsigned int &iRoots_Found, bool bIsAngular = false);
void			XROOT_Maehly(const xpolynomial &cPoly, double * lpcResults, unsigned int iMax_Results, unsigned int &iRoots_Found, bool bIsAngular = false);
void			XROOT_Maehly(QFunctionC funcQFunctionC, QFunctionC funcQFunctionDerivC, xcomplex * lpcResults, unsigned int iMax_Results, unsigned int &iRoots_Found, const void * i_lpvData = NULL, bool bIsAngular = false, xcomplex * lpPrior_Zeros = NULL, unsigned int uiNum_Prior_Zeros = 0 );
void			XROOT_Maehly(QFunctionD funcQFunctionD, QFunctionD funcQFunctionDerivD, double * lpdResults, unsigned int iMax_Results, unsigned int &iRoots_Found, const void * i_lpvData = NULL, bool bIsAngular = false);

void			XROOT_Maehly_Secant(const xpolynomial &cPoly, xcomplex * lpcResults, unsigned int iMax_Results, unsigned int &iRoots_Found);
void			XROOT_Maehly_Secant(const xpolynomial &cPoly, double * lpcResults, unsigned int iMax_Results, unsigned int &iRoots_Found);
void			XROOT_Maehly_Secant(QFunctionC funcQFunctionC, QFunctionC funcQFunctionDerivC, xcomplex * lpcResults, unsigned int iMax_Results, unsigned int &iRoots_Found, const void * i_lpvData = NULL);
void			XROOT_Maehly_Secant(QFunctionD funcQFunctionD, QFunctionD funcQFunctionDerivD, double * lpdResults, unsigned int iMax_Results, unsigned int &iRoots_Found, const void * i_lpvData = NULL);

////////////////////////////////////////////////////////////////////////////////////////////
//  Muller roots a second order method which will find complex and real roots of a 
//  polynomial
bool			XROOT_Muller(const xpolynomial &cPoly, xcomplex &xcResult, bool bIsAngular = false);
bool			XROOT_Muller(const xpolynomial &cPoly, double &dResult, bool bIsAngular = false);
bool			XROOT_Muller(QFunctionC funcQFunctionC, xcomplex &xcResult, const void * i_lpvData = NULL, bool bIsAngular = false );
bool			XROOT_Muller(QFunctionD funcQFunctionD, double &dResult, const void * i_lpvData = NULL, bool bIsAngular = false);

void			XROOT_Muller_Multi(const xpolynomial &cPoly, xcomplex * lpcResults, unsigned int iMax_Results, unsigned int &iRoots_Found, const void * i_lpvData = NULL, bool bIsAngular = false);
void			XROOT_Muller_Multi(const xpolynomial &cPoly, double * lpdResults, unsigned int iMax_Results, unsigned int &iRoots_Found, const void * i_lpvData = NULL, bool bIsAngular = false);

////////////////////////////////////////////////////////////////////////////////////////////
//  Laguerre's Method: works only on polynomials.  
//  
bool			XROOT_Laguerre(const xpolynomial &cPoly, xcomplex &xcResult, bool bIsAngular = false);
bool			XROOT_Laguerre(const xpolynomial &cPoly, double &dResult, bool bIsAngular = false);
void			XROOT_Laguerre_Multi(const xpolynomial &cPoly, xcomplex * lpcResults, unsigned int iMax_Results, unsigned int &iRoots_Found, bool bIsAngular = false);
void			XROOT_Laguerre_Multi(const xpolynomial &cPoly, double * lpdResults, unsigned int iMax_Results, unsigned int &iRoots_Found, bool bIsAngular = false);

// Get/Set the threshold for deciding that a root has been found
void			XROOT_Set_Root_Found_Threshold(const double &dThreshold); // default is 0.00000001
double			XROOT_Get_Root_Found_Threshold(void);
// Get/Set the maximum # of iterations that can be done to find a root
void			XROOT_Set_Max_Iter(unsigned int uiMax); // default is 20
unsigned int	XROOT_Get_Max_Iter(void);
// Get the most iterations it took to successfully find a root on the last attempt
unsigned int	XROOT_Get_Max_Used_Iter(void);

xcomplex		XROOT_NR_Newton(const xpolynomial &cPoly, const xcomplex &cX1, const xcomplex &cX2, const xcomplex &cXacc);
bool			XROOT_NR_Newton_2(const xpolynomial &cPoly, xcomplex &cRoot);

double			XROOT_Get_Durand_Kerner_Threshold(void);
void			XROOT_Durand_Kerner(const xpolynomial &xpolyPoly, xcomplex * lpZeros, unsigned int uiMax_Zeros, unsigned int & uiNum_Zeros);

bool			XROOT_Threshold_Test(const double &i_dX, const double &i_dDelta, unsigned int uiCriterion = DBL_MANT_DIG - 6);
bool			XROOT_Threshold_Test(const xcomplex &cX, const xcomplex &cDelta, unsigned int uiCriterion = DBL_MANT_DIG - 6);


