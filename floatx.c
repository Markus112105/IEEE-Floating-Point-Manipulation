#include "floatx.h"
#include <assert.h>
#include <limits.h> // for CHAR_BIT - number of bits per byte
#include <math.h> // for isinf and isnan
#include "bitFields.h"

floatx doubleToFloatx(double val,int totBits,int expBits) {

/*-----------------------------------------------------------------------------------------------
	1. Extracting the sign bit from the double value, and inserting it into the floatx value at
		the correct position.
	2. Extract the biased exponent from the double value.
	3. Extract the fraction bits from the double value.
	4. Handle special cases, such as infinity, or not-a-number.		
	5. Return result.
----------------------------------------------------------------------------------------------------*/

	// Make assertions then implement algorithim
	assert(totBits >= 3 && totBits <= 64);
	assert(expBits >= 1 && expBits <= totBits);

	int fracBits = totBits - expBits - 1;

	union{
		double dbl;
		unsigned long lng;
	} unionVal;
	unionVal.dbl = val;

	unsigned long result = 0;

	unsigned long sign_bit = getBit(63, unionVal.lng);
	unsigned long double_exp = getBitFld(52, 11, unionVal.lng);
	unsigned long double_mant = getBitFld(0, 52, unionVal.lng);

	int double_bias = 1023;
	int floatx_bias = (1 << (expBits - 1)) - 1;

    if (isinf(val)) { // infinity
        setBitFld(totBits - expBits - 1, expBits, (1UL << expBits) - 1, &result);
        setBit(totBits - 1, sign_bit, &result);
        return result;
    }
    if (isnan(val)) { // NaN
        setBitFld(totBits - expBits - 1, expBits, (1UL << expBits) - 1, &result);
        setBit(totBits - 1, sign_bit, &result);
        setBit(0, 1, &result);
        return result;
    }
    if (val == 0.0) { // zero
        setBit(totBits - 1, sign_bit, &result);
        return result;
    }

	long unbiased_exp;
	unsigned long adjusted_mant;
	if (double_exp == 0){ // subnormal
		unbiased_exp = 1 - double_bias;
		adjusted_mant = double_mant;
	} else{ // normal
		unbiased_exp = double_exp - double_bias;
		adjusted_mant = double_mant | (1UL << 52); // set implicit leading 1
	}

	long floatx_exp = unbiased_exp + floatx_bias;
	if (floatx_exp >= ((1 << expBits) - 1)){ // overflow
		setBitFld(totBits - expBits - 1, expBits, (1UL << expBits) - 1, &result);
        setBit(totBits - 1, sign_bit, &result);
        return result;
	}

	if (floatx_exp <= 0) { // subnormal
        int shift_right = 1 - floatx_exp;
        if (shift_right >= 64) {
            adjusted_mant = 0;
        } else {
            adjusted_mant = adjusted_mant >> shift_right;
        }
        floatx_exp = 0;
    }

	int shift_amt = 52 - fracBits;
	unsigned long result_frac;
	if (shift_amt >= 0){ // truncate
		result_frac = adjusted_mant >> shift_amt;
	} else{ // extend
		result_frac = adjusted_mant << (-shift_amt);
	}
	result_frac &= ((1UL << fracBits) - 1);

	setBit(totBits - 1, sign_bit, &result);
    setBitFld(totBits - expBits - 1, expBits, floatx_exp, &result);
    setBitFld(0, fracBits, result_frac, &result);

	return result;
}
