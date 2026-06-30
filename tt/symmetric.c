#include "symmetric.h"
#include "tt.h"

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

// Exchange the truth-table columns for variables i and j: returns rep permuted
// into the table of f with variables i and j swapped. Order-independent.
// Illustration: https://wjrforcyber.github.io/pub/permutation.pdf
static unsigned long swapVarColumns(unsigned long rep, int i, int j)
{
    if (i == j)
    {
        return rep;
    }
    if (i > j)
    {
        int tmp = i;
        i = j;
        j = tmp;
    }
    unsigned long mp = maskTT[i] & maskTTNeg[j];
    unsigned long mq = maskTTNeg[i] & maskTT[j];
    unsigned long mun = ~(mp | mq);
    int ns = (1 << j) - (1 << i);
    return (rep & mun) | ((rep & mp) << ns) | ((rep & mq) >> ns);
}

// Partial symmetry in two variables: swap the two variables via bit
// permutations and compare against the original.
int isSymmetric2Vars(truthTable* tt, int varIndex0, int varIndex1)
{
    if (varIndex0 == varIndex1)
    {
        printf("Warning: Trival case doesn't mean anything.\n");
        return 1;
    }
    unsigned long ttSwap = swapVarColumns(tt->ttrep, varIndex0, varIndex1);
    return ttSwap == tt->ttrep ? 1 : 0;
}

// Joint symmetry of two variable pairs: applying both transpositions at once
// (a0<->b0 and a1<->b1) leaves the table unchanged. The two pairs are disjoint,
// so the swaps commute and compose into the joint permutation.
int isSymmetric2Pairs(truthTable* tt, int a0, int b0, int a1, int b1)
{
    if (tt == NULL)
    {
        return 0;
    }
    int n = tt->varNum;
    int idx[4] = {a0, b0, a1, b1};
    for (int k = 0; k < 4; k++)
    {
        if (idx[k] < 0 || idx[k] >= n)
        {
            printf("isSymmetric2Pairs: index %d out of range (varNum %d).\n",
                   idx[k], n);
            return 0;
        }
    }
    for (int k = 0; k < 4; k++)
    {
        for (int l = k + 1; l < 4; l++)
        {
            if (idx[k] == idx[l])
            {
                printf("isSymmetric2Pairs: indices must be pairwise distinct.\n");
                return 0;
            }
        }
    }
    unsigned long rep = tt->ttrep;
    rep = swapVarColumns(rep, a0, b0);
    rep = swapVarColumns(rep, a1, b1);
    return rep == tt->ttrep ? 1 : 0;
}

// Arbitrary-length truth table: the output column is supplied unpacked as a
// ziArray of ints (entry i = output for input i), so varNum is not capped at 6.
// Totally symmetric iff every input with the same Hamming weight shares one
// output value.
int isSymmetricArray(ziArray* tt)
{
    if (tt == NULL)
    {
        return 0;
    }
    int size = tt->size;
    if (size <= 0)
    {
        return 0;
    }
    if ((size & (size - 1)) != 0)
    {
        printf("isSymmetricArray: length %d is not a power of two.\n", size);
        return 0;
    }
    int varNum = 0;
    int t = size;
    while ((t >>= 1) > 0)
    {
        varNum++;
    }
    int* seen = malloc((varNum + 1) * sizeof(int));
    for (int w = 0; w <= varNum; w++)
    {
        seen[w] = -1;
    }
    for (int i = 0; i < size; i++)
    {
        int w = __builtin_popcount((unsigned)i);
        int f = *(int*)fetchIndexArray(tt, i);
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