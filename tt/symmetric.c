#include "symmetric.h"

// Totally symmetric iff the function depends only on the Hamming weight of its
// inputs: bucket rows by weight and check the value is constant per bucket.
int isSymmetric(truthTable* tt)
{
    int nRows = 1 << tt->varNum;
    int maxWeight = tt->varNum + 1;
    int *seen = malloc(maxWeight * sizeof(int));
    for (int w = 0; w < maxWeight; w++)
    {
        seen[w] = -1;
    }
    for (int i = 0; i < nRows; i++)
    {
        int w = __builtin_popcount(i);
        int f = (tt->ttrep >> i) & 1;
        if (seen[w] == -1)
        {
            seen[w] = f;
        }
        else if (seen[w] != f)
        {
            free(seen);
            return 0;
        }
    }
    free(seen);
    return 1;
}

// Partial symmetry in two variables: build the table with the two variables
// swapped via bit permutations and compare against the original.
// Illustration: https://wjrforcyber.github.io/pub/permutation.pdf
int isSymmetric2Vars(truthTable* tt, int varIndex0, int varIndex1)
{
    if (varIndex0 == varIndex1)
    {
        printf("Warning: Trival case doesn't mean anything.\n");
        return 1;
    }
    if (varIndex0 > varIndex1)
    {
        int tmp = varIndex0;
        varIndex0 = varIndex1;
        varIndex1 = tmp;
    }
    unsigned long mp = maskTT[varIndex0] & maskTTNeg[varIndex1];
    unsigned long mq = maskTTNeg[varIndex0] & maskTT[varIndex1];
    unsigned long mun = ~(mp | mq);
    int ns = (1 << varIndex1) - (1 << varIndex0);
    unsigned long ttSwap =
        (tt->ttrep & mun) | ((tt->ttrep & mp) << ns) | ((tt->ttrep & mq) >> ns);
    return ttSwap == tt->ttrep ? 1 : 0;
}