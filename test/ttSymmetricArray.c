#include "symmetric.h"
#include <assert.h>
#include <stdlib.h>

// Unpack a packed column (varNum <= 6) into a ziArray, one heap int per row.
static void columnToArray(unsigned long rep, int varNum, ziArray* a) {
    initArray(a, 1 << varNum);
    int nRows = 1 << varNum;
    for (int i = 0; i < nRows; i++) {
        int* p = (int*)malloc(sizeof(int));
        *p = (int)((rep >> i) & 1UL);
        pushArray(a, p);
    }
}

int main(void) {
    // Exhaustive cross-check over all 2- and 3-variable functions: the array
    // view must agree with isSymmetric() everywhere.
    for (int n = 2; n <= 3; n++) {
        unsigned long total = 1UL << (1 << n);
        for (unsigned long rep = 0; rep < total; rep++) {
            truthTable t;
            t.elementary = 0;
            t.varNum = n;
            t.ttrep = rep;
            t.maskSpec = mask[n];
            ziArray a;
            columnToArray(rep, n, &a);
            assert(isSymmetric(&t) == isSymmetricArray(&a));
            assert(clearEachEntryArray(&a) == 0);
        }
    }

    // Spot-check known tables (n <= 6).
    {
        char* cases[] = {"1000", "1110", "0110", "1010", "11101000"};
        int want[] = {1, 1, 1, 0, 1};
        for (int k = 0; k < 5; k++) {
            truthTable* t = readTT(cases[k], 0);
            ziArray a;
            columnToArray(t->ttrep, t->varNum, &a);
            assert(isSymmetricArray(&a) == want[k]);
            assert(clearEachEntryArray(&a) == 0);
            assert(clearTT(t) == 0);
        }
    }

    // Arbitrary length: n = 7 (128 rows) does not fit in one unsigned long.
    {
        int n = 7;
        int nRows = 1 << n;

        // symmetric: weight-3 threshold (output 1 iff Hamming weight == 3)
        ziArray thr;
        initArray(&thr, nRows);
        for (int i = 0; i < nRows; i++) {
            int* p = (int*)malloc(sizeof(int));
            *p = __builtin_popcount((unsigned)i) == 3 ? 1 : 0;
            pushArray(&thr, p);
        }
        assert(isSymmetricArray(&thr) == 1);
        assert(clearEachEntryArray(&thr) == 0);

        // symmetric: constants 0 and 1
        ziArray cz;
        ziArray co;
        initArray(&cz, nRows);
        initArray(&co, nRows);
        for (int i = 0; i < nRows; i++) {
            int* z = (int*)malloc(sizeof(int));
            *z = 0;
            pushArray(&cz, z);
            int* o = (int*)malloc(sizeof(int));
            *o = 1;
            pushArray(&co, o);
        }
        assert(isSymmetricArray(&cz) == 1);
        assert(isSymmetricArray(&co) == 1);
        assert(clearEachEntryArray(&cz) == 0);
        assert(clearEachEntryArray(&co) == 0);

        // not symmetric: depends only on bit 0
        ziArray b0;
        initArray(&b0, nRows);
        for (int i = 0; i < nRows; i++) {
            int* p = (int*)malloc(sizeof(int));
            *p = i & 1;
            pushArray(&b0, p);
        }
        assert(isSymmetricArray(&b0) == 0);
        assert(clearEachEntryArray(&b0) == 0);
    }

    // Error cases.
    assert(isSymmetricArray(NULL) == 0);
    {
        ziArray odd;
        initArray(&odd, 4);
        int vals[] = {1, 0, 1};
        for (int i = 0; i < 3; i++) {
            int* p = (int*)malloc(sizeof(int));
            *p = vals[i];
            pushArray(&odd, p);
        }
        assert(isSymmetricArray(&odd) == 0);
        assert(clearEachEntryArray(&odd) == 0);
    }

    return 0;
}
