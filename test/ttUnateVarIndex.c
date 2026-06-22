#include <assert.h>
#include <stdio.h>
#include "unate.h"

// isUnateVarIndex(tt, i) returns the unateness of the i-th variable (x_i,
// 0-indexed) directly from the truth table. At most 6 variables are supported.
// Expected per-variable labels match isUnate(): a variable that does not affect
// the output is POS_UNATE (inclusive cofactor-containment definition).
int main(void) {
    // 2-input AND "1000": x0 POS_UNATE, x1 POS_UNATE
    {
        char s[] = "1000";
        truthTable *t = readTT(s, 0);
        assert(isUnateVarIndex(t, 0) == POS_UNATE);
        assert(isUnateVarIndex(t, 1) == POS_UNATE);
        clearTT(t);
    }

    // 2-input XOR "0110": x0 BINATE, x1 BINATE
    {
        char s[] = "0110";
        truthTable *t = readTT(s, 0);
        assert(isUnateVarIndex(t, 0) == BINATE);
        assert(isUnateVarIndex(t, 1) == BINATE);
        clearTT(t);
    }

    // 2-input NAND "0111": x0 NEG_UNATE, x1 NEG_UNATE
    {
        char s[] = "0111";
        truthTable *t = readTT(s, 0);
        assert(isUnateVarIndex(t, 0) == NEG_UNATE);
        assert(isUnateVarIndex(t, 1) == NEG_UNATE);
        clearTT(t);
    }

    // f = x0 "1010": x0 POS_UNATE, x1 does not affect output -> POS_UNATE
    {
        char s[] = "1010";
        truthTable *t = readTT(s, 0);
        assert(isUnateVarIndex(t, 0) == POS_UNATE);
        assert(isUnateVarIndex(t, 1) == POS_UNATE);
        clearTT(t);
    }

    // f = ~x0 "0101": x0 NEG_UNATE, x1 does not affect output -> POS_UNATE
    {
        char s[] = "0101";
        truthTable *t = readTT(s, 0);
        assert(isUnateVarIndex(t, 0) == NEG_UNATE);
        assert(isUnateVarIndex(t, 1) == POS_UNATE);
        clearTT(t);
    }

    // 3-var f = x0 & x1, x2 irrelevant "10001000": all POS_UNATE
    {
        char s[] = "10001000";
        truthTable *t = readTT(s, 0);
        assert(isUnateVarIndex(t, 0) == POS_UNATE);
        assert(isUnateVarIndex(t, 1) == POS_UNATE);
        assert(isUnateVarIndex(t, 2) == POS_UNATE);
        clearTT(t);
    }

    // 3-var f = x0 ^ x1, x2 irrelevant "01100110": x0 BINATE, x1 BINATE,
    // x2 does not affect output -> POS_UNATE
    {
        char s[] = "01100110";
        truthTable *t = readTT(s, 0);
        assert(isUnateVarIndex(t, 0) == BINATE);
        assert(isUnateVarIndex(t, 1) == BINATE);
        assert(isUnateVarIndex(t, 2) == POS_UNATE);
        clearTT(t);
    }

    // 1-var f = x0 "10": POS_UNATE
    {
        char s[] = "10";
        truthTable *t = readTT(s, 0);
        assert(isUnateVarIndex(t, 0) == POS_UNATE);
        clearTT(t);
    }

    // cross-check: isUnateVarIndex must agree with isUnate's per-variable array
    {
        char s[] = "01100110";
        truthTable *t = readTT(s, 0);
        ziArray r;
        isUnate(t, &r);
        for (int i = 0; i < t->varNum; i++) {
            assert(isUnateVarIndex(t, i) ==
                   *(enum FUNC_TYPE *)fetchIndexArray(&r, i));
        }
        clearTT(t);
        clearEachEntryArray(&r);
    }

    // out-of-range index -> UNKNOW
    {
        char s[] = "0110";
        truthTable *t = readTT(s, 0);
        assert(isUnateVarIndex(t, -1) == UNKNOW);
        assert(isUnateVarIndex(t, 2) == UNKNOW);  /* varNum == 2 */
        clearTT(t);
    }

    return 0;
}
