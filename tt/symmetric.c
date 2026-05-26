#include "symmetric.h"


/**
  \brief Given a truth table, decide whether this is a totally symmetric function, which means swapping any variables in the original function wouldn't change the truth table. (Hamming weight)
*/
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