#include <assert.h>
#include "tt.h"

/*
Give Minato-Morreale algorithm on generating ISOP(Irredundant Sum of Product)
Referenced
[1] Kitty. Write in Truth table but think in BDD branch.
[2] Fast Generation of Prime Irredundant Covers from Binary Decision Diagrams.
[3] Finding all simple disjunctive decompositions using irredundant sum-of-products forms.
*/

truthTable* isop(truthTable* tt, truthTable * dc, int varIndex, ziArray * cubes)
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
    //clearTT(orInit);
    //get one variable v
    int varCur = varIndex - 1;
    assert(varCur >= 0);

    //check if current tt depends on elementary varIndex
    while(varCur >=0 )
    {
        if(hasVar(tt, varCur) || hasVar(dc, varCur))
        {
            break;
        }
        varCur--;
    }
    
    if (varCur < 0) {
        return zero(tt);
    }

    //cofactor0 f_0 on v
    truthTable * cf0 = cofactor0(tt, varCur);
    //cofactor1 f_1 on v
    truthTable * cf1 = cofactor1(tt, varCur);
    //cofactor0 on dc
    truthTable * dc0 = cofactor0(dc, varCur);
    //cofactor1 on dc
    truthTable * dc1 = cofactor1(dc, varCur);

    //compute f_0' based on rule, check the original paper
    int cubes0_start = cubes->size;
    truthTable * not_dc1 = notCopy(dc1);
    //printf("f0', not_cf1\n");
    //showTT(not_cf1, BHEX);
    truthTable * andcp0 = andCopy(cf0, not_dc1);
    clearTT(not_dc1);
    //take advantage of don't care (definitely on cf0 but not in dc1)
    truthTable * rec_0 = isop(andcp0, dc0, varCur, cubes );
    clearTT(andcp0);
    int cubes0_end = cubes->size;

    //compute f_1' based on rule
    int cubes1_start = cubes0_end;
    truthTable * not_dc0 = notCopy(dc0);
    //printf("f1' not_cf0\n");
    //showTT(cf1, BHEX);
    truthTable * andcp1 = andCopy(cf1, not_dc0);
    clearTT(not_dc0);
    //take advantage of don't care (definitely on cf1 but not in dc0)
    truthTable * rec_1 = isop(andcp1, dc1, varCur, cubes);
    clearTT(andcp1);
    int cubes1_end = cubes->size;

    /*
    Modified by referencing Kitty
    cf0 & ~rec_0 (Get intersection of definate on 0 branch and those don't care not on it)
    cf1 & ~res_1 (Get intersection of definate on 1 branch and those don't care not on it)
    */
    truthTable * rec0Not = notCopy(rec_0);
    truthTable * and0Rest = andCopy(cf0, rec0Not);
    clearTT(rec0Not);

    truthTable * rec1Not = notCopy(rec_1);
    truthTable * and1Rest = andCopy(cf1, rec1Not);
    clearTT(rec1Not);
    
    truthTable * orRest = orCopy(and0Rest, and1Rest);
    truthTable * dcRest = andCopy(dc0, dc1);
    truthTable * rest = isop(orRest, dcRest, varCur, cubes);

    clearTT(orRest);
    clearTT(dcRest);

    //isop = ~v*isop_0 + v* isop_1 + isop*
    truthTable * v_not = initTT(tt->varNum);
    initElementaryVar(v_not, tt->varNum,varCur );
    notInplace(v_not);

    truthTable * v = initTT(tt->varNum);
    initElementaryVar(v, tt->varNum,varCur );

    res = initTT(tt->varNum);
    res->ttrep = (rec_0->ttrep & v_not->ttrep) | (rec_1->ttrep & v->ttrep) | rest->ttrep;
    res->maskSpec = tt->maskSpec;

    for(int i = cubes0_start; i < cubes0_end; i++){
        addLit((cube *)fetchIndexArray(cubes, i), varCur, 0);
    }
    for(int i = cubes1_start; i < cubes1_end; i++){
        addLit((cube *)fetchIndexArray(cubes, i), varCur, 1);
    }
    clearTT(cf0);
    clearTT(cf1);
    clearTT(rec_0);
    clearTT(rec_1);
    clearTT(rest);
    clearTT(v_not);
    clearTT(v);
    return res;
}