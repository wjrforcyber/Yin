#include "tt.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

int main()
{
    char tts[] = "10101100";
    truthTable *tt = readTT(tts, 1);
    //shanno-decomp on var1
    truthTable tt1_ele;
    assert(initElementaryVar(&tt1_ele, 3, 1) == 0);
    truthTable * cf0 = cofactor0(tt, 1);
    truthTable * cf1 = cofactor1(tt, 1);
    assert(((~tt1_ele.ttrep & cf0->ttrep) | (tt1_ele.ttrep & cf1->ttrep)) == tt->ttrep);
    
    //shanno-decomp on var1
    truthTable tt2_ele;
    assert(initElementaryVar(&tt2_ele, 3, 2) == 0);
    truthTable * cf0_2 = cofactor0(tt, 2);
    truthTable * cf1_2 = cofactor1(tt, 2);
    assert(((~tt2_ele.ttrep & cf0_2->ttrep) | (tt2_ele.ttrep & cf1_2->ttrep)) == tt->ttrep);
    
    
    //take test from Kitty
    char ttsCo[] = "11101000";
    truthTable *ttCo = readTT(ttsCo, 1);
    //cofactor0
    truthTable* cfCo_02 = cofactor0(ttCo, 2);
    showTT(cfCo_02, BHEX);
    assert(cfCo_02->ttrep == 0x88);
    clearTT(cfCo_02);
    
    truthTable* cfCo_01 = cofactor0(ttCo, 1);
    assert(cfCo_01->ttrep == 0xa0);
    clearTT(cfCo_01);
    
    truthTable* cfCo_00 = cofactor0(ttCo, 0);
    assert( cfCo_00->ttrep == 0xc0);
    clearTT(cfCo_00);
    //cofactor1
    truthTable* cfCo_12 = cofactor1(ttCo, 2);
    assert(cfCo_12->ttrep == 0xee);
    clearTT(cfCo_12);
    
    truthTable* cfCo_11 = cofactor1(ttCo, 1);
    assert(cfCo_11->ttrep == 0xfa);
    clearTT(cfCo_11);
    
    truthTable* cfCo_10 = cofactor1(ttCo, 0);
    assert( cfCo_10->ttrep == 0xfc);
    clearTT(cfCo_10);


    clearTT(tt);
    clearTT(ttCo);
    clearTT(cf0);
    clearTT(cf1);
    clearTT(cf0_2);
    clearTT(cf1_2);
    return 0;
}