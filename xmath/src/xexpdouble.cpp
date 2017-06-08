#include <xextprec.h>

expdouble EDBL_MIN(0x8000000000000000,0,false);
expdouble EDBL_MAX(0xffffffffffffffff,0x7ffffffffffffffe,false);
expdouble EDBL_EPSILON(0x8000000000000000,4611686018427387902 - 63,false);
expdouble EDBL_LOG2_10(expdouble((long double)10.0).log2());
expdouble EDBL_LOG2_e(expdouble(std::exp((long double)1.0)).log2());

