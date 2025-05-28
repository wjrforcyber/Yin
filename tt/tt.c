#include <stdio.h>
#include "tt.h"

//check boundaries
int checkPredefBound( int* varNum )
{
    if(*varNum > 6)
    {
        printf("Only support 6 or less variables.\n");
        return 1;
    }
    return 0;
}

//initialize elementary variable
int initElementaryVar( truthTable* a, int varNum, int ith )
{
    if(checkPredefBound(&varNum))
    {
        return 1;
    }
    a->varNum = varNum;
    a->elementary = 1;
    a->ttrep = maskTT[ith] & mask[varNum];
    return 0;
}

//print out the truth table in various format
void showTT( truthTable* a, enum BASE b)
{
    printf("Truth table contains:\n");
    if( b == DEC)
    {
        printf("%ld\n", a->ttrep);
    }
    if( b == HEX)
    {
        printf("0x%016lX\n", a->ttrep);
    }
}