#define _USE_MATH_DEFINES
#include <math.h>
#include "xcomplex.h"

XCOMPLEX	xI(0.0,1.0);
////////////////////////////////////////////////////////////////////////////////////////
//
//	XCOMPLEX.cpp
//
//	Source code for XCOMPLEX, as defined in xcomplex.h
//
//	Dependencies:
//		xcomplex.h
//		math.h
//
////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
//
//	operators    (double, XPOLYNOMIAL)
//
////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
//	operator + (double,XPOLYNOMIAL)
////////////////////////////////////////////////////////////////////////////////////////
XCOMPLEX operator +(const double &dLHO, const XCOMPLEX &cRHO)
{
	return (cRHO + dLHO);
}
////////////////////////////////////////////////////////////////////////////////////////
//	operator - (double,XPOLYNOMIAL)
////////////////////////////////////////////////////////////////////////////////////////
XCOMPLEX operator -(const double &dLHO, const XCOMPLEX &cRHO)
{
	return (cRHO * -1 + dLHO);
}
////////////////////////////////////////////////////////////////////////////////////////
//	operator * (double,XPOLYNOMIAL)
////////////////////////////////////////////////////////////////////////////////////////
XCOMPLEX operator *(const double &dLHO, const XCOMPLEX &cRHO)
{
	return (cRHO * dLHO);
}
////////////////////////////////////////////////////////////////////////////////////////
//	operator / (double,XPOLYNOMIAL)
////////////////////////////////////////////////////////////////////////////////////////
XCOMPLEX operator /(const double &dLHO, const XCOMPLEX &cRHO)
{
	XCOMPLEX cLHO(dLHO,0.0);
	return (cLHO / cRHO);
}

////////////////////////////////////////////////////////////////////////////////////////
//
//	sqrt_c
//
////////////////////////////////////////////////////////////////////////////////////////

XCOMPLEX	sqrt_c(const double &dValue)
{
	XCOMPLEX cRet(0,0);
	if (dValue < 0.0)
		cRet.dIm = sqrt(-dValue);
	else
		cRet.dRe = sqrt(dValue);
	return cRet;
}
double		XCOMPLEX::argument( void ) const 
{
	double	dRet;
/*	if( dRe == 0.0)
	{
		if (dIm == 0.0 )
			dRet = 0.0;
		else if (dIm < 0)
			dRet = -M_PI_2;
		else
			dRet = M_PI_2;
	}
	else if (dIm == 0.0)
	{
		if (dRe < 0.0)
			dRet = -M_PI_4;
		else
			dRet = M_PI_4;
	}
	else*/
		dRet = atan2(dIm,dRe);

	return dRet;
}


ostream& operator<<( ostream &out, const XCOMPLEX &xC ) {

	//out << c.real << ( c.imag >= 0 ? "+" : "" ) << c.imag << "I";
	out << xC.dRe << "," << xC.dIm;

	return out;

}

XCOMPLEX	exp(const XCOMPLEX &xcExp)
{
	XCOMPLEX	xcRet(cos(xcExp.dIm),sin(xcExp.dIm));
	xcRet *= exp(xcExp.dRe);
	return xcRet;
}
