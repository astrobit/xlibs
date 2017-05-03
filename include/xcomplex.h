#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <float.h>
#include <stdio.h>
#include <iostream>
using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////
// class xcomplex
//
// Purpose:
//	xcomplex is a class container for complex numbers.
//
//  The polynomial container includes operators for complex addition, subtraction,
//   multiplication, and division with either another complex number or a real number.
//
//  Additional operators are supplied which take a complex number as a right hand operand

class xcomplex
{
public:
	// components:  (dRe + dIm * i)
	double	dRe; // Real component
	double	dIm; // Imagniary component

	// Constructors, including copy constructor
	xcomplex (void){dRe = 0.0; dIm = 0.0;}
	xcomplex (const double &a_dRe, const double &a_dIm) {dRe = a_dRe; dIm = a_dIm;}
	xcomplex (const xcomplex &cRHO) {dRe = cRHO.dRe;dIm = cRHO.dIm;}

	// additive inverse
	xcomplex operator -(void){xcomplex cRet(dRe * -1,dIm* -1);return cRet;}

	// assignment operators with double as right operand:  affect only real component of value
	xcomplex &operator +=(const double &dRHO){dRe += dRHO; return *this;}
	xcomplex &operator -=(const double &dRHO){dRe -= dRHO; return *this;}
	xcomplex &operator *=(const double &dRHO){dRe *= dRHO; dIm *= dRHO; return *this;}
	xcomplex &operator /=(const double &dRHO){dRe /= dRHO; dIm /= dRHO; return *this;}

	// operators with double as right operand:  affect only real component of value
	xcomplex operator +(const double &dRHO) const{xcomplex cRet(*this); cRet += dRHO; return cRet;}
	xcomplex operator -(const double &dRHO) const{xcomplex cRet(*this); cRet -= dRHO; return cRet;}
	xcomplex operator *(const double &dRHO) const{xcomplex cRet(*this); cRet *= dRHO; return cRet;}
	xcomplex operator /(const double &dRHO) const{xcomplex cRet(*this); cRet /= dRHO; return cRet;}

	// assignment operators with complex as right operand
	xcomplex &operator +=(const xcomplex &cRHO){dRe += cRHO.dRe; dIm += cRHO.dIm; return *this;}
	xcomplex &operator -=(const xcomplex &cRHO){dRe -= cRHO.dRe; dIm -= cRHO.dIm; return *this;}
	xcomplex &operator *=(const xcomplex &cRHO){double dRe_Temp = dRe * cRHO.dRe - dIm * cRHO.dIm; double dIm_Temp = dRe * cRHO.dIm + dIm * cRHO.dRe; dRe = dRe_Temp;	dIm = dIm_Temp;	return *this;}
//	xcomplex &operator /=(const xcomplex &cRHO){double dMag_Rho = cRHO.dRe * cRHO.dRe + cRHO.dIm * cRHO.dIm;xcomplex cRHO_Recip(cRHO.dRe / dMag_Rho,-cRHO.dIm / dMag_Rho); return (*this *= cRHO_Recip);}
	xcomplex &operator /=(const xcomplex &cRHO)
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
	xcomplex operator +(const xcomplex &cRHO) const{xcomplex cRet(*this); cRet += cRHO; return cRet;}
	xcomplex operator -(const xcomplex &cRHO) const{xcomplex cRet(*this); cRet -= cRHO; return cRet;}
	xcomplex operator *(const xcomplex &cRHO) const{xcomplex cRet(*this); cRet *= cRHO; return cRet;}
	xcomplex operator /(const xcomplex &cRHO) const{xcomplex cRet(*this); cRet /= cRHO; return cRet;}

	bool operator == (const xcomplex &cRHO) {return (fabs(dRe - cRHO.dRe) < DBL_EPSILON && fabs(dIm - cRHO.dIm) < DBL_EPSILON);}
	bool operator != (const xcomplex &cRHO) {return (fabs(dRe - cRHO.dRe) > DBL_EPSILON || fabs(dIm - cRHO.dIm) > DBL_EPSILON);}

	// 'absolute value' of complex number:  re^2 + im^2
	double		abs(void) const{return (dRe * dRe + dIm * dIm);}
	double		argument( void ) const;
	double		modulus(void) const {return ::sqrt(abs());}
	xcomplex	pow(const double &dExp) const 
	{
		double dR = ::pow( xcomplex::modulus(), dExp );
		double dO = xcomplex::argument();

		return xcomplex( dR*cos( dExp*dO ), dR*sin( dExp*dO ) );
	}
	xcomplex	sqrt(void) const {return xcomplex::pow(0.5);}
#if 0
	xcomplex	sqrt(void) const
	{/*++ Numerical Recipes in c++, sec 5.4*/
		xcomplex cRes(0.0,0.0);
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
	xcomplex	setReal(const double &dVal){dRe = dVal; return *this;}
	xcomplex	setImag(const double &dVal){dIm = dVal; return *this;}
	xcomplex	set(const double &a_dRe, const double &a_dIm){dRe = a_dRe; dIm = a_dIm;return *this;}


	// simple method for printing a complex number to console
	void	Print(void) const {printf("(%e,%e)",dRe,dIm);}
};
typedef xcomplex XCOMPLEX;
extern xcomplex	xI;
// operators with complex as right operand.  Note assignment operators not defined
xcomplex operator +(const double &dLHO, const xcomplex &cRHO);
xcomplex operator -(const double &dLHO, const xcomplex &cRHO);
xcomplex operator *(const double &dLHO, const xcomplex &cRHO);
xcomplex operator /(const double &dLHO, const xcomplex &cRHO);
// square root function which returns results as a complex #.  Note that sqrt_c(-1) returns xcomplex(0,1)
xcomplex	sqrt_c(const double &dValue);
xcomplex	exp(const xcomplex &xcExp);
ostream& operator<<( ostream &out, const xcomplex &c );
