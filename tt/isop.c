#include <assert.h>
#include "tt.h"

/*
Give Minato-Morreale algorithm on generating ISOP(Irredundant Sum of Product)
Referenced
[1] Kitty.
[2] Fast Generation of Prime Irredundant Covers from Binary Decision Diagrams.
[3] Finding all simple disjunctive decompositions using irredundant sum-of-products forms.
*/

truthTable* isop(truthTable* tt, int varIndex, ziArray * cubes)
{
    truthTable * res = NULL;
    //all the conditions should be 0 or dc, which also means can't be 1
    if(isConst(tt) == 0){
        return zero(tt);
    }
    //all the conditions should be 1 or dc, which also means can't be 0
    if(isConst(tt) == 1){
        cube * empCube = initCube(tt);
        pushArray(cubes, empCube);
        return one(tt);
    }
    //get one variable v
    int varCur = varIndex - 1;
    assert(varCur >= 0);

    //check if current tt depends on elementary varIndex
    while(varCur >=0 )
    {
        if(hasVar(tt, varCur))
        {
            break;
        }
        varCur--;
    }

    //cofactor0 f_0 on v
    truthTable * cf0 = cofactor0(tt, varCur);
    //cofactor1 f_1 on v
    truthTable * cf1 = cofactor1(tt, varCur);

    //compute f_0' based on rule, check the original paper
    int cubes0_start = cubes->size;
    truthTable * not_cf1 = notCopy(cf1);
    //printf("f0', not_cf1\n");
    //showTT(not_cf1, BHEX);
    truthTable * andcp0 = andCopy(cf0, not_cf1);
    clearTT(not_cf1);
    truthTable * rec_0 = isop(andcp0, varCur, cubes );
    clearTT(andcp0);
    int cubes0_end = cubes->size;

    //compute f_1' based on rule
    int cubes1_start = cubes0_end;
    truthTable * not_cf0 = notCopy(cf0);
    //printf("f1' not_cf0\n");
    //showTT(cf1, BHEX);
    truthTable * andcp1 = andCopy(cf1, not_cf0);
    clearTT(not_cf0);
    truthTable * rec_1 = isop(andcp1, varCur, cubes);
    clearTT(andcp1);
    int cubes1_end = cubes->size;
    
    //give isop_0(g_0) from f_0'
    //give isop_1(g_1) from f_1'
    //compute f0'' based on rule
    truthTable * not_rec_0 = notCopy(rec_0);
    //printf("f0'', not_rec_0\n");
    //showTT(cf0, BHEX);
    truthTable * f_0_pp = andCopy(cf0, not_rec_0);
    clearTT(not_rec_0);

    //compute f1'' based on rule
    truthTable * not_rec_1 = notCopy(rec_1);
    //printf("f1'', not _rec_1\n");
    //showTT(not_rec_1, BHEX);
    truthTable * f_1_pp = andCopy(cf1, not_rec_1);
    clearTT(not_rec_1);

    //compute f* based on rule, check the original paper, trick here, NEED to use OR operator
    truthTable * or_rec_star = orCopy(f_0_pp, f_1_pp);
    clearTT(f_0_pp);
    clearTT(f_1_pp);

    //give isop* from f*
    truthTable * rec_star = isop(or_rec_star, varCur, cubes);
    clearTT(or_rec_star);

    //isop = ~v*isop_0 + v* isop_1 + isop*
    truthTable * v_not = initTT(tt->varNum);
    initElementaryVar(v_not, tt->varNum,varCur );
    notInplace(v_not);

    truthTable * v = initTT(tt->varNum);
    initElementaryVar(v, tt->varNum,varCur );

    res = initTT(tt->varNum);
    res->ttrep = (rec_0->ttrep & v_not->ttrep) | (rec_1->ttrep & v->ttrep) | rec_star->ttrep;
    res->maskSpec = tt->maskSpec;

    for(int i = cubes0_start; i < cubes0_end; i++){
        cube *tmp = (cube *)fetchIndexArray(cubes, i);
        addLit(tmp, varCur, 0);
    }
    for(int i = cubes1_start; i < cubes1_end; i++){
        cube *tmp = (cube *)fetchIndexArray(cubes, i);
        addLit(tmp, varCur, 1);
    }
    clearTT(cf0);
    clearTT(cf1);
    clearTT(rec_0);
    clearTT(rec_1);
    clearTT(v_not);
    clearTT(v);
    clearTT(rec_star);
    return res;
}