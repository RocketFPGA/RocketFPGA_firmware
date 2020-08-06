
#include <stdint.h>
#include "rocketfpga.h"

// Biquad

#define __PI_OVER_TWO 1.57079632679489661923132
#define __SQRT_HALF 0.70710678118654752440

static const float z_rooteps = 7.4505859692e-9;
static const float  z_rooteps_f = 1.7263349182589107e-4;
static const float TWO_OVER_PI = 0.63661977236758134308;
static const float p[] = { -0.13338350006421960681,
                             0.34248878235890589960e-2,
                            -0.17861707342254426711e-4 };
static const float q[] = { -0.46671683339755294240,
                             0.25663832289440112864e-1,
                            -0.31181531907010027307e-3,
                             0.49819433993786512270e-6 };

float fabs(float x){
	return (x >= 0) ? x : -x;
}

float tan (float x){
	float y, f, g, XN, xnum, xden, res;
	int N;

	y = fabs (x);

	/* Check for values that are out of our range. */
	if (y > 105414357.0){
      return (y);
    }

	if (x < 0.0)
		N = (int) (x * TWO_OVER_PI - 0.5);
	else
		N = (int) (x * TWO_OVER_PI + 0.5);

	XN = (double) N;

	f = x - N * __PI_OVER_TWO;

	/* Check for values that are too small. */
	if (-z_rooteps_f < f && f < z_rooteps_f){
		xnum = f;
		xden = 1.0;
	}
	/* Calculate the polynomial. */ 
	else{ 
		g = f * f;

		xnum = f * ((p[2] * g + p[1]) * g + p[0]) * g + f;
		xden = (((q[3] * g + q[2]) * g + q[1]) * g + q[0]) * g + 1.0;
	}

	if (N & 1){
		xnum = -xnum;
		res = xden / xnum;
	} else {
		res = xnum / xden;
	}

	return (res);
}

static const float two25 =  3.3554432000e+07; /* 0x4c000000 */
typedef union
{
  float value;
  uint32_t word;
} ieee_float_shape_type;

/* Get a 32 bit int from a float.  */
#define GET_FLOAT_WORD(i,d)					\
do {								\
  ieee_float_shape_type gf_u;					\
  gf_u.value = (d);						\
  (i) = gf_u.word;						\
} while (0)

/* Set a float from a 32 bit int.  */
#define SET_FLOAT_WORD(d,i)					\
do {								\
  ieee_float_shape_type sf_u;					\
  sf_u.word = (i);						\
  (d) = sf_u.value;						\
} while (0)

float copysignf(float x, float y){
	uint32_t ix,iy;
	GET_FLOAT_WORD(ix,x);
	GET_FLOAT_WORD(iy,y);
	SET_FLOAT_WORD(x,(ix&0x7fffffff)|(iy&0x80000000));
    return x;
}

#define FLT_UWORD_IS_FINITE(x) ((x)<0x7f800000L)
#define FLT_UWORD_IS_SUBNORMAL(x) ((x)<0x00800000L)
#define FLT_UWORD_IS_ZERO(x) ((x)==0)

float frexp(float x, int *eptr){
	uint32_t hx, ix;
	GET_FLOAT_WORD(hx,x);
	ix = 0x7fffffff&hx;
	*eptr = 0;
	if(!FLT_UWORD_IS_FINITE(ix)||FLT_UWORD_IS_ZERO(ix)) return x;	/* 0,inf,nan */
	if (FLT_UWORD_IS_SUBNORMAL(ix)) {		/* subnormal */
	    x *= two25;
	    GET_FLOAT_WORD(hx,x);
	    ix = hx&0x7fffffff;
	    *eptr = -25;
	}
	*eptr += (ix>>23)-126;
	hx = (hx&0x807fffff)|0x3f000000;
	SET_FLOAT_WORD(x,hx);
	return x;
}

#define OVERFLOW_INT 50000
#define FLT_SMALLEST_EXP -22
#define FLT_UWORD_MAX 0x7f7fffffL
#define FLT_LARGEST_EXP (FLT_UWORD_MAX>>23)

static const float twom25  =  2.9802322388e-08;	/* 0x33000000 */
static const float huge   = 1.0e+30;
static const float tiny   = 1.0e-30;

float scalbnf (float x, int n){
	__int32_t  k,ix;
	__uint32_t hx;

	GET_FLOAT_WORD(ix,x);
	hx = ix&0x7fffffff;
        k = hx>>23;		/* extract exponent */
	if (FLT_UWORD_IS_ZERO(hx))
	    return x;
        if (!FLT_UWORD_IS_FINITE(hx))
	    return x+x;		/* NaN or Inf */
        if (FLT_UWORD_IS_SUBNORMAL(hx)) {
	    x *= two25;
	    GET_FLOAT_WORD(ix,x);
	    k = ((ix&0x7f800000)>>23) - 25; 
            if (n< -50000) return tiny*x; 	/*underflow*/
        }
        k = k+n; 
        if (k > FLT_LARGEST_EXP) return huge*copysignf(huge,x); /* overflow  */
        if (k > 0) 				/* normal result */
	    {SET_FLOAT_WORD(x,(ix&0x807fffff)|(k<<23)); return x;}
        if (k < FLT_SMALLEST_EXP) {
            if (n > OVERFLOW_INT) 	/* in case integer overflow in n+k */
		return huge*copysignf(huge,x);	/*overflow*/
	    else return tiny*copysignf(tiny,x);	/*underflow*/
        }
        k += 25;				/* subnormal result */
	SET_FLOAT_WORD(x,(ix&0x807fffff)|(k<<23));
        return x*twom25;
}

float ldexp(float value, int exp){
	if(value == (float)0.0) return value;
	value = scalbnf(value, exp);
	return value;
}


float sqrt (float x){
	float f, y;
	int exp, i, odd;

	/* Initial checks are performed here. */
	if (x == 0.0 || x < 0)
		return (0.0);

	/* Find the exponent and mantissa for the form x = f * 2^exp. */
	f = frexp (x, &exp);

	odd = exp & 1;

	/* Get the initial approximation. */
	y = 0.41731 + 0.59016 * f;

	f /= 2.0;
	/* Calculate the remaining iterations. */
	for (i = 0; i < 3; ++i)
		y = y / 2.0 + f / y;

	/* Calculate the final value. */
	if (odd){
		y *= __SQRT_HALF;
		exp++;
		}
	exp >>= 1;
	y = ldexp (y, exp);

	return (y);
}

typedef int16_t fixed_point_t;
#define FIXED_POINT_FRACTIONAL_BITS 14

inline fixed_point_t float_to_fixed(float input)
{
    return (fixed_point_t)(input * (1 << FIXED_POINT_FRACTIONAL_BITS));
}

void set_biquad(biquad_type type, float f, float peak_gain_lin, float Q) {
	float a0, a1, a2, b1, b2;
    float norm;
    float V = peak_gain_lin;
    float K = tan(M_PI * (f/(float)SAMPLING_FREQ));
    switch (type) {
        case LOWPASS:
            norm = 1 / (1 + K / Q + K * K);
            a0 = K * K * norm;
            a1 = 2 * a0;
            a2 = a0;
            b1 = 2 * (K * K - 1) * norm;
            b2 = (1 - K / Q + K * K) * norm;
            break;
            
        case HIGHPASS:
            norm = 1 / (1 + K / Q + K * K);
            a0 = 1 * norm;
            a1 = -2 * a0;
            a2 = a0;
            b1 = 2 * (K * K - 1) * norm;
            b2 = (1 - K / Q + K * K) * norm;
            break;
            
        case BANDPASS:
            norm = 1 / (1 + K / Q + K * K);
            a0 = K / Q * norm;
            a1 = 0;
            a2 = -a0;
            b1 = 2 * (K * K - 1) * norm;
            b2 = (1 - K / Q + K * K) * norm;
            break;
            
        case NOTCH:
            norm = 1 / (1 + K / Q + K * K);
            a0 = (1 + K * K) * norm;
            a1 = 2 * (K * K - 1) * norm;
            a2 = a0;
            b1 = a1;
            b2 = (1 - K / Q + K * K) * norm;
            break;
            
        case PEAK:
            if (peak_gain_lin >= 0) {    // boost
                norm = 1 / (1 + 1/Q * K + K * K);
                a0 = (1 + V/Q * K + K * K) * norm;
                a1 = 2 * (K * K - 1) * norm;
                a2 = (1 - V/Q * K + K * K) * norm;
                b1 = a1;
                b2 = (1 - 1/Q * K + K * K) * norm;
            }
            else {    // cut
                norm = 1 / (1 + V/Q * K + K * K);
                a0 = (1 + 1/Q * K + K * K) * norm;
                a1 = 2 * (K * K - 1) * norm;
                a2 = (1 - 1/Q * K + K * K) * norm;
                b1 = a1;
                b2 = (1 - V/Q * K + K * K) * norm;
            }
            break;
        case LOWSHELF:
            if (peak_gain_lin >= 0) {    // boost
                norm = 1 / (1 + sqrt(2) * K + K * K);
                a0 = (1 + sqrt(2*V) * K + V * K * K) * norm;
                a1 = 2 * (V * K * K - 1) * norm;
                a2 = (1 - sqrt(2*V) * K + V * K * K) * norm;
                b1 = 2 * (K * K - 1) * norm;
                b2 = (1 - sqrt(2) * K + K * K) * norm;
            }
            else {    // cut
                norm = 1 / (1 + sqrt(2*V) * K + V * K * K);
                a0 = (1 + sqrt(2) * K + K * K) * norm;
                a1 = 2 * (K * K - 1) * norm;
                a2 = (1 - sqrt(2) * K + K * K) * norm;
                b1 = 2 * (V * K * K - 1) * norm;
                b2 = (1 - sqrt(2*V) * K + V * K * K) * norm;
            }
            break;
        case HIGHSHELF:
            if (peak_gain_lin >= 0) {    // boost
                norm = 1 / (1 + sqrt(2) * K + K * K);
                a0 = (V + sqrt(2*V) * K + K * K) * norm;
                a1 = 2 * (K * K - V) * norm;
                a2 = (V - sqrt(2*V) * K + K * K) * norm;
                b1 = 2 * (K * K - 1) * norm;
                b2 = (1 - sqrt(2) * K + K * K) * norm;
            }
            else {    // cut
                norm = 1 / (V + sqrt(2*V) * K + K * K);
                a0 = (1 + sqrt(2) * K + K * K) * norm;
                a1 = 2 * (K * K - 1) * norm;
                a2 = (1 - sqrt(2) * K + K * K) * norm;
                b1 = 2 * (K * K - V) * norm;
                b2 = (V - sqrt(2*V) * K + K * K) * norm;
            }
            break;
    }
	set_biquad_a0(float_to_fixed(a0));
	set_biquad_a1(float_to_fixed(a1));
	set_biquad_a2(float_to_fixed(a2));
	set_biquad_b1(float_to_fixed(b1));
	set_biquad_b2(float_to_fixed(b2));
    return;
}