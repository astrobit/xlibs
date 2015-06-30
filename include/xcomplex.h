#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <float.h>
#include <stdio.h>
#include <iostream>
using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////
// class XCOMPLEX
//
// Purpose:
//	XCOMPLEX is a class container for complex numbers.
//
//  The polynomial container includes operators for complex addition, subtraction,
//   multiplication, and division with either another complex number or a real number.
//
//  Additional operators are supplied which take a complex number as a right hand operand

class XCOMPLEX
{
public:
	// components:  (dRe + dIm * i)
	double	dRe; // Real component
	double	dIm; // Imagniary component

	// Constructors, including copy constructor
	XCOMPLEX (void){dRe = 0.0; dIm = 0.0;}
	XCOMPLEX (const double &a_dRe, const double &a_dIm) {dRe = a_dRe; dIm = a_dIm;}
	XCOMPLEX (const XCOMPLEX &cRHO) {dRe = cRHO.dRe;dIm = cRHO.dIm;}

	// additive inverse
	XCOMPLEX operator -(void){XCOMPLEX cRet(dRe * -1,dIm* -1);return cRet;}

	// assignment operators with double as right operand:  affect only real component of value
	XCOMPLEX &operator +=(const double &dRHO){dRe += dRHO; return *this;}
	XCOMPLEX &operator -=(const double &dRHO){dRe -= dRHO; return *this;}
	XCOMPLEX &operator *=(const double &dRHO){dRe *= dRHO; dIm *= dRHO; return *this;}
	XCOMPLEX &operator /=(const double &dRHO){dRe /= dRHO; dIm /= dRHO; return *this;}

	// operators with double as right operand:  affect only real component of value
	XCOMPLEX operator +(const double &dRHO) const{XCOMPLEX cRet(*this); cRet += dRHO; return cRet;}
	XCOMPLEX operator -(const double &dRHO) const{XCOMPLEX cRet(*this); cRet -= dRHO; return cRet;}
	XCOMPLEX operator *(const double &dRHO) const{XCOMPLEX cRet(*this); cRet *= dRHO; return cRet;}
	XCOMPLEX operator /(const double &dRHO) const{XCOMPLEX cRet(*this); cRet /= dRHO; return cRet;}

	// assignment operators with complex as right operand
	XCOMPLEX &operator +=(const XCOMPLEX &cRHO){dRe += cRHO.dRe; dIm += cRHO.dIm; return *this;}
	XCOMPLEX &operator -=(const XCOMPLEX &cRHO){dRe -= cRHO.dRe; dIm -= cRHO.dIm; return *this;}
	XCOMPLEX &operator *=(const XCOMPLEX &cRHO){double dRe_Temp = dRe * cRHO.dRe - dIm * cRHO.dIm; double dIm_Temp = dRe * cRHO.dIm + dIm * cRHO.dRe; dRe = dRe_Temp;	dIm = dIm_Temp;	return *this;}
//	XCOMPLEX &operator /=(const XCOMPLEX &cRHO){double dMag_Rho = cRHO.dRe * cRHO.dRe + cRHO.dIm * cRHO.dIm;XCOMPLEX cRHO_Recip(cRHO.dRe / dMag_Rho,-cRHO.dIm / dMag_Rho); return (*this *= cRHO_Recip);}
	XCOMPLEX &operator /=(const XCOMPLEX &cRHO)
	{/*++ Numerical Recipes in c++, sec 5.4*/
		double	dFrac;
		double	dDenom;
		double	dNumer_Re;
		double	dNumer_Im;
		if (fabs(cRHO.dRe) >= fabs(cRHO.dIm))
		{
			dFrac = cRHO.dIm / cRHO.dRe;
			dDenom = 1.0 / (cRHO.dRe + cRHO.dIm * dFrac);
			dNumer_Re = (dRe + dIm * dFrac);
			dNumer_Im = (dIm - dRe * dFrac);
		}
		else
		{
			dFrac = cRHO.dRe / cRHO.dIm;
			dDenom = 1.0 / (cRHO.dIm + cRHO.dRe * dFrac);
			dNumer_Re = (dIm + dRe * dFrac);
			dNumer_Im = (dIm * dFrac - dRe);
		}
		dRe = dNumer_Re * dDenom;
		dIm = dNumer_Im * dDenom;
		return *this;
	}

	// operators with complex as right operand
	XCOMPLEX operator +(const XCOMPLEX &cRHO) const{XCOMPLEX cRet(*this); cRet += cRHO; return cRet;}
	XCOMPLEX operator -(const XCOMPLEX &cRHO) const{XCOMPLEX cRet(*this); cRet -= cRHO; return cRet;}
	XCOMPLEX operator *(const XCOMPLEX &cRHO) const{XCOMPLEX cRet(*this); cRet *= cRHO; return cRet;}
	XCOMPLEX operator /(const XCOMPLEX &cRHO) const{XCOMPLEX cRet(*this); cRet /= cRHO; return cRet;}

	bool operator == (const XCOMPLEX &cRHO) {return (fabs(dRe - cRHO.dRe) < DBL_EPSILON && fabs(dIm - cRHO.dIm) < DBL_EPSILON);}
	bool operator != (const XCOMPLEX &cRHO) {return (fabs(dRe - cRHO.dRe) > DBL_EPSILON || fabs(dIm - cRHO.dIm) > DBL_EPSILON);}

	// 'absolute value' of complex number:  re^2 + im^2
	double		abs(void) const{return (dRe * dRe + dIm * dIm);}
	double		argument( void ) const;
	double		modulus(void) const {return ::sqrt(abs());}
	XCOMPLEX	pow(const double &dExp) const 
	{
		double dR = ::pow( XCOMPLEX::modulus(), dExp );
		double dO = XCOMPLEX::argument();

		return XCOMPLEX( dR*cos( dExp*dO ), dR*sin( dExp*dO ) );
	}
	XCOMPLEX	sqrt(void) const {return XCOMPLEX::pow(0.5);}
#if 0
	XCOMPLEX	sqrt(void) const
	{/*++ Numerical Recipes in c++, sec 5.4*/
		XCOMPLEX cRes(0.0,0.0);
		double dW = 0.0;
		if (dRe != dIm || dRe != 0.0)
		{
			double	dFrac;
			if (fabs(dRe) >= fabs(dIm))
			{
				dFrac = dIm / dRe;
				dW = ::sqrt(fabs(dRe)) * ::sqrt(0.5 + ::sqrt(0.25 + 0.25 * dFrac * dFrac));
			}
			else
			{
				dFrac = dRe / dIm;
				dW = ::sqrt(fabs(dIm)) * ::sqrt(0.5 * dFrac + ::sqrt(0.25 + 0.25 * dFrac * dFrac));
			}
			if (dW != 0.0)
			{
				if (dRe >= 0.0)
				{
					cRes.dRe = dW;
					cRes.dIm = 0.5 * dIm / dW;
				}
				else if (dIm >= 0.0)
				{
					cRes.dRe = fabs(dIm) / dW * 0.5;
					cRes.dIm = dW;
				}
				else
				{
					cRes.dRe = fabs(dIm) / dW * 0.5;
					cRes.dIm = -dW;
				}
			}
		}
		return cRes;
	}
#endif
	XCOMPLEX	setReal(const double &dVal){dRe = dVal; return *this;}
	XCOMPLEX	setImag(const double &dVal){dIm = dVal; return *this;}
	XCOMPLEX	set(const double &a_dRe, const double &a_dIm){dRe = a_dRe; dIm = a_dIm;return *this;}


	// simple method for printing a complex number to console
	void	Print(void) const {printf("(%e,%e)",dRe,dIm);}
};
extern XCOMPLEX	xI;
// operators with complex as right operand.  Note assignment operators not defined
XCOMPLEX operator +(const double &dLHO, const XCOMPLEX &cRHO);
XCOMPLEX operator -(const double &dLHO, const XCOMPLEX &cRHO);
XCOMPLEX operator *(const double &dLHO, const XCOMPLEX &cRHO);
XCOMPLEX operator /(const double &dLHO, const XCOMPLEX &cRHO);
// square root function which returns results as a complex #.  Note that sqrt_c(-1) returns XCOMPLEX(0,1)
XCOMPLEX	sqrt_c(const double &dValue);
XCOMPLEX	exp(const XCOMPLEX &xcExp);
ostream& operator<<( ostream &out, const XCOMPLEX &c );
