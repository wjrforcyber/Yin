#include <stdio.h>
#include "tt.h"
#include <stdlib.h>
#include <string.h>

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
    a->maskSpec = mask[6];
    return 0;
}

//initialize a general truth table
truthTable* initTT( int varNum )
{
    truthTable* a = (truthTable*)malloc(sizeof(truthTable));
    if(checkPredefBound( &varNum ) == 1)
    {
        return NULL;
    }
    a->varNum = varNum;
    a->elementary = 0;
    a->ttrep = 0;
    a->maskSpec = 0;
    return a;
}

//log2 of unsigned int
int getVarNum( int len )
{
    int res = 0;
    while((len >> 1) > 0)
    {
        res++;
        len >>= 1;
    }
    return res;
}

//read truth table
/*
Support 0,1,-
- means don't care
*/
truthTable* readTT( char * tts, int verbose)
{
    char* ch = tts;
    int len = strlen(tts) - 1;
    int varNum = getVarNum(len);
    truthTable *a = initTT( varNum );
    unsigned long *res = &(a->ttrep);
    unsigned long *maskBit = &(a->maskSpec);
    while(*ch != '\0')
    {
        switch(*ch)
        {
            case '1':
              *res |= (1 << len);
              *maskBit |= (1 << len);
              break;
            case '0':
              *maskBit |= (1 << len);
              break;
            case '-':
              break;
        }
        ch++;
        len--;
    }
    if(verbose == 1)
    {
        showTT(a, HEX);
    }
    return a;
}

//evaluate the value of the truth table
int evalTTVal(truthTable*a, unsigned int val)
{
    if(a->ttrep == val)
    {
        return 0;
    }
    else {
        return 1;
    }
}

//evaluate the value of the mask without don't care
int evalTTMask(truthTable*a, unsigned int val)
{
    if(a->maskSpec == val)
    {
        return 0;
    }
    else {
        return 1;
    }
}

//clear truth table
int clearTT(truthTable* a)
{
    free(a);
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

//get a specific bit
int getBit( truthTable* a, int index)
{
    //check if index valid
    if(index < 0 || index > (2 << a->varNum))
    {
        printf("Invalid index to fetch from Truth table.\n");
    }
    tt maskBit = 1 << index;
    return (maskBit & a->ttrep) > 0 ? 1 : 0;
}