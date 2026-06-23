#include <assert.h>
#include <stdio.h>
#include "selfdual.h"

// Naive self-dual check: f(a) must equal ~f(~a) for every assignment a, so the
// two complementary outputs must differ on every pair.
static int naiveSelfDual(unsigned long rep, int n) {
    int N = 1 << n;  // 2^n assignments
    for (int a = 0; a < N; a++) {
        int b = (N - 1) ^ a;  // bitwise complement of a within n bits
        int fa = (int)((rep >> a) & 1UL);
        int fb = (int)((rep >> b) & 1UL);
        if (fa == fb)
            return 0;
    }
    return 1;
}

static void checkKnown(char *s, int expected) {
    truthTable *t = readTT(s, 0);
    assert(t != NULL);
    assert(isSelfDual(t) == expected);
    clearTT(t);
}

/* Build a truth table directly from its packed column and check self-duality.
   Covers fields too wide to write conveniently as a readTT string. */
static void checkRep(int n, unsigned long rep, int expected) {
    truthTable t;
    t.elementary = 0;
    t.varNum = n;
    t.ttrep = rep;
    t.maskSpec = mask[n];
    assert(isSelfDual(&t) == expected);
}

int main(void) {
    /* known self-dual functions */
    checkKnown("10", 1);         /* x0 (1 var) */
    checkKnown("01", 1);         /* ~x0 (1 var) */
    checkKnown("1010", 1);       /* x0 (2 vars) */
    checkKnown("0101", 1);       /* ~x0 (2 vars) */
    checkKnown("1100", 1);       /* x1 (2 vars) */
    checkKnown("0011", 1);       /* ~x1 (2 vars) */
    checkKnown("11101000", 1);   /* majority(a,b,c) = ab+ac+bc */

    /* known non-self-dual functions */
    checkKnown("00", 0);         /* const 0 (1 var) */
    checkKnown("11", 0);         /* const 1 (1 var) */
    checkKnown("1000", 0);       /* AND */
    checkKnown("1110", 0);       /* OR */
    checkKnown("0110", 0);       /* XOR */
    checkKnown("0111", 0);       /* NAND */

    /* exhaustive cross-check against the naive definition (n = 1..4) and verify
       the count of self-dual functions equals 2^(2^(n-1)) */
    truthTable t;
    t.elementary = 0;
    for (int n = 1; n <= 4; n++) {
        unsigned long total = 1UL << (1 << n);  /* 2^(2^n) functions */
        int seen = 0;
        for (unsigned long rep = 0; rep < total; rep++) {
            t.varNum = n;
            t.ttrep = rep;
            t.maskSpec = mask[n];
            int got = isSelfDual(&t);
            int want = naiveSelfDual(rep, n);
            assert(got == want);
            if (got)
                seen++;
        }
        unsigned long expectedCount = 1UL << (1 << (n - 1));
        assert((unsigned long)seen == expectedCount);
    }

    /* Regression: projection functions x_var are self-dual at every width,
       including n=5/n=6 where the reversed field spans bits 16-47 (the region
       that line 11's old unmasked shift corrupted). maskTT[var] is the truth
       table of x_var; clip it to the 2^n-bit field. Cross-check the naive
       oracle as well. */
    for (int n = 5; n <= 6; n++)
        for (int var = 0; var < n; var++) {
            unsigned long rep = maskTT[var] & mask[n];
            checkRep(n, rep, 1);
            assert(naiveSelfDual(rep, n) == 1);
        }

    /* NULL guard */
    assert(isSelfDual(NULL) == 0);
    return 0;
}
