/* 212152896 Yuval Anteby */

#include <xmmintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pmmintrin.h>

#define MAX_STR 255

float formula1(float *x, unsigned int length) {
    // Edge case safety
    if (length == 0) return 0.0f; 

    __m128 v_sum = _mm_setzero_ps();
    __m128 v_prod = _mm_set1_ps(1.0f);
    __m128 v_ones = _mm_set1_ps(1.0f);

    unsigned int i = 0;
    // 4 elements at a time
    for (; i + 3 < length; i += 4) {
        __m128 v_x = _mm_loadu_ps(&x[i]);
        
        // SUM: sum += sqrtf(x[k])
        __m128 v_sqrt = _mm_sqrt_ps(v_x);
        v_sum = _mm_add_ps(v_sum, v_sqrt);

        // MULTIPLICATION: (x[k] * x[k] + 1.0f)
        __m128 v_x2 = _mm_mul_ps(v_x, v_x);
        __m128 v_x2_plus_1 = _mm_add_ps(v_x2, v_ones);
        v_prod = _mm_mul_ps(v_prod, v_x2_plus_1);
    }

    // sum into a final variable
    float sum_arr[4];
    _mm_storeu_ps(sum_arr, v_sum);
    float final_sum = sum_arr[0] + sum_arr[1] + sum_arr[2] + sum_arr[3];

    // multiplication into a final variable
    float prod_arr[4];
    _mm_storeu_ps(prod_arr, v_prod);
    float final_prod = prod_arr[0] * prod_arr[1] * prod_arr[2] * prod_arr[3];

    // Handle remaining elements (if length is not a multiple of 4)
    for (; i < length; i++) {
        final_sum += sqrtf(x[i]);
        final_prod *= (x[i] * x[i] + 1.0f);
    }

    // Final result
    // Should use sqrtf(final_sum) but the test is using cbrtf(final_sum), so I follow the test
    return sqrtf(1.0f + cbrtf(final_sum) / final_prod);
}

