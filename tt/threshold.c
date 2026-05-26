#include "threshold.h"

/** 
 \brief Given a truth table, decide whether it is a threshold function:
We are simplifying the problem to if the number of 1s in input variables exceeded or equal to threshold iT and at the same time the according value in truth table is 1, while other conditions at the same time are 0, then it is a threshold funtion and return 1, else return 0. For example, for 3-input majority function, and threshold number 2, returns 1.
 \param tt The truth table.
 \param iT The threshold number.
*/
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