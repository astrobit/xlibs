#pragma once
#define XMATH_VER "0.0.1c"
#define XMATH_VER_0_0_1_C
#include <cmath>
#include <cstdlib>
#include <vector>
#include <map>
#include <utility>

class XMATH_CONSTANTS
{
public:
	double	dPi;
	double	dE;
	double	dLn2;
	XMATH_CONSTANTS(void);
};
extern XMATH_CONSTANTS	g_cConstants;

class XVECTOR;
class XCOMPLEX;

typedef XCOMPLEX (*QFunctionC)(const XCOMPLEX &i_cX, const void * i_lpvData);
typedef double (*QFunctionD)(const double &i_dX, const void * i_lpvData);
typedef double (*QFunctionV)(const XVECTOR &i_vX, void * i_lpvData);
void XMATH_Set_Convergence_Threshold(const double &dValue);
double XMATH_Get_Convergence_Threshold(void);
bool XMATH_Delta_Threshold_Test(const double &i_dX, const double  &i_dDelta, unsigned int uiMaximum_Log_Diff = 47);


double XM_Simpsons_Integration(QFunctionD lpfFn, const double &dStart, const double &dEnd, unsigned int uiNumSteps, const void * i_lpvData = NULL);
double XM_Simpsons_Integration_Fast(QFunctionD lpfFn, const double &dStart, const double &dEnd, const void * i_lpvData = NULL);

XCOMPLEX XM_Simpsons_Integration(QFunctionC lpfFn, const XCOMPLEX &xcStart, const XCOMPLEX &xcEnd, unsigned int uiNumSteps, const void * i_lpdData);
XCOMPLEX XM_Simpsons_Integration_Fast(QFunctionC lpfFn, const XCOMPLEX &xcStart, const XCOMPLEX &xcEnd, const void * i_lpvData = NULL);
XCOMPLEX XM_Runge_Kutte(QFunctionC lpfFn, QFunctionC lpfFnD, const XCOMPLEX &xcX, const XCOMPLEX &xcStep, const void * i_lpvData = NULL);

// XM_Runge_Kutte
// Determines an integral step direction in the complex plane.  Return value is the argument of the solution.
double	XM_Runge_Kutte(const XCOMPLEX &xcX, QFunctionC lpfFn, const double &dStep, const void * i_lpvData = NULL);
// XM_Runge_Kutte_Init
// Determines an integral step direction in the complex plane.  The initial step has been computed externally, and is passed as a parameter.
// Return value is the argument of the solution.
double	XM_Runge_Kutte_Init(const XCOMPLEX &xcX, QFunctionC lpfFn, const double &dPhi1, const double &dStep, const void * i_lpvData = NULL);

inline double	XM_Sign(const double &dVal){if (dVal < 0.0) return -1.0; else if (dVal > 0.0) return 1.0; else return 0.0;}
inline double	XM_Angular_Normalize(double &dVal){if (dVal >= M_PI) dVal = fmod(dVal,M_PI); else if (dVal < -M_PI) dVal = fmod(dVal,M_PI); return dVal;}

// Factorial functions
// integer form (uses int
void XFactLarge(unsigned int i_uiX, unsigned long long &o_uiResult, unsigned int &o_uiResult_Exponent); // perform large factorials; more precise than Ramanjan but slow
unsigned long long XFactUI(unsigned int i_uiX); //integer factorial of x, returns int64
double	XFactRamunjan(unsigned int i_uiX); // Ramanjan approximation of factorial
double	XFact(unsigned int i_uiX); // factorial; exact for x <= 25

// Interpolation routines
double XInterpolate_Linear(const double &i_dX, const double * i_lpdX, const double * i_lpdY, unsigned int i_uiNum_Data_Points);  // perform a linear interpolation given a data set
double XInterpolate_CSpline(const double & i_dX, const double i_lpdX[3], const double i_lpdY[3]); //Perform a cublic spline interpolation given three data points.  Note that this function is inefficient if interpolating multiple points in the same data set; in this case use the XSPLINE_DATA class below.


class XSPLINE_DATA
{
private:
	double	*	m_lpdX;
	double	*	m_lpdY;
	double	*	m_lpdFirst_Derivatives;
	double	*	m_lpdSecond_Derivatites;
	unsigned int	m_uiNum_Points;
	unsigned int	m_uiNum_Points_Allocated;

	void		Clear_pointers(void);
	void		Allocate(unsigned int i_uiNum_Data_Points);
	void		Initialize_Derivatives(void);
	void		Copy(const XSPLINE_DATA &i_cRHO);
public:
	XSPLINE_DATA(const XSPLINE_DATA &i_cRHO);
	XSPLINE_DATA(void){ Clear_pointers(); };
	void Initialize(const std::map<double, double> &i_mddData);
	void Initialize(const std::vector<double> i_vdX, std::vector<double> &i_vdY);
	void Initialize(const std::vector< std::pair<double, double> > &i_vdData);
	void Initialize(const double * i_lpdX, const double * i_lpdY, unsigned int i_uiNum_Data_Points);
	XSPLINE_DATA(const double * i_lpdX, const double * i_lpdY, unsigned int i_uiNum_Data_Points){Clear_pointers(); Initialize(i_lpdX,i_lpdY,i_uiNum_Data_Points);}
	XSPLINE_DATA(const std::vector<double> i_vdX, std::vector<double> &i_vdY){Clear_pointers(); Initialize(i_vdX,i_vdY);}
	XSPLINE_DATA(const std::vector< std::pair<double, double> > &i_vdData){Clear_pointers(); Initialize(i_vdData);}
	XSPLINE_DATA(const std::map<double, double> &i_mddData){Clear_pointers(); Initialize(i_mddData);}
	double Interpolate(const double &i_dX) const;
	XSPLINE_DATA & operator =(const XSPLINE_DATA & i_cRHO);
};

inline int sign(const double & i_dVal) {if (i_dVal < 0.0) return -1; else return 1;}
