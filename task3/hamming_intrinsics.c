/* 212152896 Yuval Anteby */
#include <emmintrin.h>   // SSE2
#include <smmintrin.h>   // SSE4.1
#include <string.h>

#define MAX_STR 256

int hamming_dist(char str1[MAX_STR], char str2[MAX_STR]) {
    // Calculate the lengths of both strings
    int len1 = strlen(str1);
    int len2 = strlen(str2);
    
    // Find the minimum length to compare overlapping characters
    int min_len = len1 < len2 ? len1 : len2;
    
    // For strings of unequal length, the length difference immediately
    // contributes to the final Hamming distance.
    int dist = len1 > len2 ? len1 - len2 : len2 - len1;

    int i = 0;
    
    // Process the strings in chunks of 16 bytes (128 bits) at a time
    for (; i <= min_len - 16; i += 16) {
        // Load 16 bytes from each string into 128 bit vector registers.
        // Use unaligned loads (_mm_loadu_si128) because string array
        // alignments are not strictly guaranteed here
        __m128i v1 = _mm_loadu_si128((__m128i*)&str1[i]);
        __m128i v2 = _mm_loadu_si128((__m128i*)&str2[i]);
        
        // Compare each of the 16 bytes for equality.
        // For each matching byte the result is 0xFF, For non matches 0x00.
        __m128i cmp = _mm_cmpeq_epi8(v1, v2);
        
        // Create a 16 bit integer mask from the MSB of each byte in comparison vector
        // 1 means chars matched, 0 means they didn't
        int mask = _mm_movemask_epi8(cmp);
        
        // We want to count the number of DIFFERENCES (where mask is 0).
        // So we bitwise NOT the mask (~mask) to turn mismatches into 1s,
        // mask off the upper bits with & 0xFFFF to ensure we only look at 16 bits,
        // and use __builtin_popcount to efficiently count the number of 1s
        dist += __builtin_popcount(~mask & 0xFFFF);
    }

    // Handle any remaining chars that were left over
    // (e.g. if min_len is not a multiple of 16)
    for (; i < min_len; ++i) {
        if (str1[i] != str2[i]) {
            dist++;
        }
    }

    return dist;
}

