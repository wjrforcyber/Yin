#include "threshold.h"

// Threshold function of weight iT: output 1 exactly on assignments whose
// Hamming weight is >= iT.
int isThreshold(truthTable* tt, int iT)
{
    int nRows = 1 << tt->varNum;
    for (int i = 0; i < nRows; i++)
    {
        int f = (tt->ttrep >> i) & 1;
        int expected = __builtin_popcount(i) >= iT ? 1 : 0;
        if (f != expected)
        {
            return 0;
        }
    }
    return 1;
}