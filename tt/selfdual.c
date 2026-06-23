#include "selfdual.h"

// Reverse the low 2^varNum bits of v (the truth-table column) and keep the
// reversed field aligned to bit 0. Done with a full 64-bit reversal followed by
// a right shift by (64 - 2^n) so that old bit a lands at bit (2^n - 1 - a).
static unsigned long bitReverseN(unsigned long v, int varNum) {
    v = ((v >> 1) & 0x5555555555555555UL) | ((v & 0x5555555555555555UL) << 1);
    v = ((v >> 2) & 0x3333333333333333UL) | ((v & 0x3333333333333333UL) << 2);
    v = ((v >> 4) & 0x0F0F0F0F0F0F0F0FUL) | ((v & 0x0F0F0F0F0F0F0F0FUL) << 4);
    v = ((v >> 8) & 0x00FF00FF00FF00FFUL) | ((v & 0x00FF00FF00FF00FFUL) << 8);
    v = ((v >> 16) & 0x0000FFFF0000FFFFUL) | ((v & 0x0000FFFF0000FFFFUL) << 16);
    v = (v >> 32) | (v << 32);
    int width = 1 << varNum;  // 2^n, the truth-table width
    return (v >> (64 - width)) & mask[varNum];
}

// f is self-dual  <=>  f(2^n-1-a) = ~f(a) for all a, i.e. the reversed column
// equals the complemented column (within the 2^n-bit field).
int isSelfDual(truthTable* tt) {
    if (tt == NULL || tt->varNum < 1 || tt->varNum > 6) {
        return 0;
    }
    unsigned long reversed = bitReverseN(tt->ttrep, tt->varNum);
    unsigned long dual = ~tt->ttrep & mask[tt->varNum];
    return reversed == dual;
}
