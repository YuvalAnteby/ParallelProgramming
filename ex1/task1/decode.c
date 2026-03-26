/* 212152896 Yuval Anteby */

/**
 * calculates (x-y)
 * @param x %rdi
 * @param y %rsi
 * @param z %rdx
 * @return %rax
 */
long decode_c_version(long x, long y, long z) {
    // subq %rdx, %rsi
    long temp = y - z;
    // imulq %rsi, %rdi
    x = x * temp;
    // salq $63, %rax
    x = x << 63;
    // sarq $63, %rax
    x = x >> 63;
    // xorq %rdi, %rax
    x = x ^ temp;
    return x;

}