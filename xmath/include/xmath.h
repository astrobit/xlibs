#pragma once
#define XMATH_VER "0.0.1c"
#define XMATH_VER_0_0_1_C
#include <cmath>
#include <cstdlib>
#include <vector>
#include <map>
#include <utility>

class xmath_constants
{
public:
	double	dPi;
	double	dE;
	double	dLn2;
	xmath_constants(void);
};
extern xmath_constants	g_cConstants;

class xvector;
class xcomplex;

typedef xcomplex (*QFunctionC)(const xcomplex &i_cX, const void * i_lpvData);
typedef double (*QFunctionD)(const double &i_dX, const void * i_lpvData);
typedef double (*QFunctionV)(const xvector &i_vX, void * i_lpvData);
void XMATH_Set_Convergence_Threshold(const double &dValue);
double XMATH_Get_Convergence_Threshold(void);
bool XMATH_Delta_Threshold_Test(const double &i_dX, const double  &i_dDelta, unsigned int uiMaximum_Log_Diff = 47);


double XM_Simpsons_Integration(QFunctionD lpfFn, const double &dStart, const double &dEnd, unsigned int uiNumSteps, const void * i_lpvData = NULL);
double XM_Simpsons_Integration_Fast(QFunctionD lpfFn, const double &dStart, const double &dEnd, const void * i_lpvData = NULL);

xcomplex XM_Simpsons_Integration(QFunctionC lpfFn, const xcomplex &xcStart, const xcomplex &xcEnd, unsigned int uiNumSteps, const void * i_lpdData);
xcomplex XM_Simpsons_Integration_Fast(QFunctionC lpfFn, const xcomplex &xcStart, const xcomplex &xcEnd, const void * i_lpvData = NULL);
xcomplex XM_Runge_Kutte(QFunctionC lpfFn, QFunctionC lpfFnD, const xcomplex &xcX, const xcomplex &xcStep, const void * i_lpvData = NULL);

// XM_Runge_Kutte
// Determines an integral step direction in the complex plane.  Return value is the argument of the solution.
double	XM_Runge_Kutte(const xcomplex &xcX, QFunctionC lpfFn, const double &dStep, const void * i_lpvData = NULL);
// XM_Runge_Kutte_Init
// Determines an integral step direction in the complex plane.  The initial step has been computed externally, and is passed as a parameter.
// Return value is the argument of the solution.
double	XM_Runge_Kutte_Init(const xcomplex &xcX, QFunctionC lpfFn, const double &dPhi1, const double &dStep, const void * i_lpvData = NULL);

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
double XInterpolate_CSpline(const double & i_dX, const double i_lpdX[3], const double i_lpdY[3]); //Perform a cublic spline interpolation given three data points.  Note that this function is inefficient if interpolating multiple points in the same data set; in this case use the xspline_data class below.


class xspline_data
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
	void		Copy(const xspline_data &i_cRHO);
public:
	xspline_data(const xspline_data &i_cRHO);
	xspline_data(void){ Clear_pointers(); };
	void Initialize(const std::map<double, double> &i_mddData);
	void Initialize(const std::vector<double> i_vdX, std::vector<double> &i_vdY);
	void Initialize(const std::vector< std::pair<double, double> > &i_vdData);
	void Initialize(const double * i_lpdX, const double * i_lpdY, unsigned int i_uiNum_Data_Points);
	xspline_data(const double * i_lpdX, const double * i_lpdY, unsigned int i_uiNum_Data_Points){Clear_pointers(); Initialize(i_lpdX,i_lpdY,i_uiNum_Data_Points);}
	xspline_data(const std::vector<double> i_vdX, std::vector<double> &i_vdY){Clear_pointers(); Initialize(i_vdX,i_vdY);}
	xspline_data(const std::vector< std::pair<double, double> > &i_vdData){Clear_pointers(); Initialize(i_vdData);}
	xspline_data(const std::map<double, double> &i_mddData){Clear_pointers(); Initialize(i_mddData);}
	double Interpolate(const double &i_dX) const;
	xspline_data & operator =(const xspline_data & i_cRHO);
};
typedef xspline_data XSPLINE_DATA;

inline int sign(const double & i_dVal) {if (i_dVal < 0.0) return -1; else return 1;}
