#include "simulation.h"

/*random generate a vector of simulation pattern, by default 2^n * 0.5
If n = 4, totally(exact simulation) 16 patterns, generate 8 pattern
Distribution: Evenlly distributed.
*/
void randSimEven(int varNum, ziArray * rSimVec, unsigned long sizeVec)
{
    if(sizeVec > (1 << (1 << varNum)))
    {
        printf("Error: Specified a simulation number larger than all the amount of simulation pattern.\n");
        return;
    }
    if(sizeVec == (1 << (1 << varNum)))
    {
        printf("Warning: Generating all the simulation patterns.\n");
    }
    unsigned long i  = 0;
    unsigned long step = (1 << (1 << varNum)) / sizeVec;
    while( i < sizeVec )
    {
        truthTable * pattern = initTT(varNum);
        pattern->ttrep += step * i;
        pattern->maskSpec = mask[varNum];
        pushArray(rSimVec, pattern);
        i++;
    }
    if(rSimVec->size == 0)
    {
        printf("Warning: No simulation generated in current process.\n");
    }
}

/*TODO: @Jingren Wang
Give logic operation and do stack at simulation.
*/
//void randSimStackAt(int VarNum, ziArray * rSimVec)