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
    a->maskSpec = mask[6] & mask[varNum];
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
    a->ttrep = 0UL;
    a->maskSpec = 0UL;
    return a;
}

cube* initCube(truthTable * tt)
{
    cube * c = (cube *)malloc(sizeof(cube));
    c->mask = (unsigned long)0UL & tt->maskSpec;
    c->polarity = (unsigned long)0UL & tt->maskSpec;
    c->varNum = tt->varNum;
    return c;
}

//copy a truth table
truthTable* copyTT(truthTable* a)
{
    truthTable* copy = initTT(a->varNum);
    copy->elementary = a->elementary;
    copy->maskSpec = a->maskSpec;
    copy->ttrep = a->ttrep;
    copy->varNum = a->varNum;
    return copy;
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
            default:
              printf("Unknown symbol %c.\n", *ch);
              return NULL;
        }
        ch++;
        len--;
    }
    if(verbose == 1)
    {
        showTT(a, BHEX);
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

int hasVar(truthTable* tt, int index)
{
    return ((tt->ttrep >> ((unsigned long)1 << index)) & maskTTNeg[index]) != (tt->ttrep & maskTTNeg[index]);
}

//cofactor0
truthTable* cofactor0(truthTable*a, int index)
{
    truthTable* copy = copyTT(a);
    copy->ttrep = ((copy->ttrep & maskTTNeg[index]) | ( (copy->ttrep & maskTTNeg[index]) << (1 << index) )) & a->maskSpec;
    return copy;
}

//cofactor1
truthTable* cofactor1(truthTable*a, int index)
{
    truthTable* copy = copyTT(a);
    copy->ttrep = ((copy->ttrep & maskTT[index]) | ((copy->ttrep & maskTT[index]) >> (1 << index) )) & a->maskSpec;
    return copy;
}

//isConst, if defined bits are all 0 return 0, all 1 return 1. ignore dc here
int isConst(truthTable * tt)
{
    if(tt->ttrep  == (tt->maskSpec & 0x0000000000000000))
    {
        return 0;
    }
    else if(tt->ttrep == (tt->maskSpec & mask[6])){
        return 1;
    }
    return -1;
}

//return a zero based on the given tt
truthTable* zero(truthTable * tt)
{
    truthTable * z = (truthTable *)malloc(sizeof(truthTable));
    z->elementary = 0;
    z->ttrep = (unsigned long)0UL & tt->maskSpec;
    z->maskSpec = tt->maskSpec;
    z->varNum = tt->varNum;
    return z;
}

//return a zero based on the given tt
truthTable* one(truthTable * tt)
{
    truthTable * z = (truthTable *)malloc(sizeof(truthTable));
    z->elementary = 0;
    z->ttrep = mask[6] & tt->maskSpec;
    z->maskSpec = tt->maskSpec;
    z->varNum = tt->varNum;
    return z;
}

truthTable * notCopy(truthTable *tt)
{
    truthTable * notTT = copyTT(tt);
    notTT->ttrep = (~tt->ttrep & tt->maskSpec);
    return notTT;
}

void notInplace(truthTable *tt)
{
    tt->ttrep = (~tt->ttrep & tt->maskSpec);
}

truthTable * andCopy(truthTable * tt0, truthTable * tt1)
{
    if(tt0->varNum != tt1->varNum)
    {
        printf("Illegal AND operation due to different variable number.\n");
    }
    if(tt0->maskSpec != tt1->maskSpec)
    {
        printf("AND: Mask bits are different(%lu, %lu).\n",tt0->maskSpec, tt1->maskSpec);
    }
    truthTable * andTTRes = initTT(tt0->varNum);
    andTTRes->varNum = tt0->varNum;
    andTTRes->ttrep = tt0->ttrep & tt1->ttrep;
    andTTRes->maskSpec = tt0->maskSpec;
    return andTTRes;
}

truthTable * orCopy(truthTable * tt0, truthTable * tt1)
{
    if(tt0->varNum != tt1->varNum)
    {
        printf("Illegal OR operation due to different variable number.\n");
    }
    if(tt0->maskSpec != tt1->maskSpec)
    {
        printf("OR: Mask bits are different.\n");
    }
    truthTable * orTTRes = initTT(tt0->varNum);
    orTTRes->varNum = tt0->varNum;
    orTTRes->ttrep = tt0->ttrep | tt1->ttrep;
    orTTRes->maskSpec = tt0->maskSpec;
    return orTTRes;
}

void addLit(cube * cube, int varCur, int polarity)
{
    //set the varCur position to 1 in mask
    cube->mask |= (1 << varCur);
    //set the corresponding polarity
    polarity == 1 ? (cube->polarity |= (1 << varCur)) : (cube->polarity &= ~(1 << varCur));
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
    if( b == BDEC)
    {
        printf("%ld\n", a->ttrep);
    }
    if( b == BHEX)
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