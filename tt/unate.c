
#include "unate.h"
#include "tt.h"
//check if the function is positive unate, negative unate or binate
enum FUNC_TYPE isUnate(truthTable * tt, ziArray * res)
{
    initArray(res, 10);
    int allPosUnate = 1;
    int allNegUnate = 1;
    int hasBinate = 0;
    for(int ithVar = 0; ithVar < tt->varNum; ithVar++)
    {
        int isPosUnate = 1;
        int isNegUnate = 1;
        for (int iCheck = 0; iCheck < (1 << tt->varNum); iCheck++) {
            //only choose the variable that is 0
            if (((iCheck >> ithVar) & 1) == 0) {
                int i0 = iCheck;
                //get the variable that is 1
                int i1 = iCheck | (1 << ithVar);

                int f0 = (tt->ttrep >> i0) & 1;
                int f1 = (tt->ttrep >> i1) & 1;

                if (f1 < f0) isPosUnate = 0;
                if (f1 > f0) isNegUnate = 0;
            }
        }
        if (!isPosUnate && !isNegUnate)
            hasBinate = 1;
        if (!isPosUnate)
            allPosUnate = 0;
        if (!isNegUnate)
            allNegUnate = 0;

        // A variable that does not affect the output (isPosUnate &&
        // isNegUnate both hold) is positive unate in the inclusive
        // cofactor-containment sense, since f|_{x=0} <= f|_{x=1} holds with
        // equality. It is therefore reported as POS_UNATE.
        enum FUNC_TYPE *eleEach = malloc(sizeof(enum FUNC_TYPE));
        if (isPosUnate)
            *eleEach = POS_UNATE;
        else if (isNegUnate)
            *eleEach = NEG_UNATE;
        else
            *eleEach = BINATE;
        pushArray(res, eleEach);
    }
    if (hasBinate)
        return BINATE;
    if (allPosUnate)
        return POS_UNATE;
    if (allNegUnate)
        return NEG_UNATE;
    return UNKNOW;
}

//check if the nth variable is positive/negative unate/binate
enum FUNC_TYPE isUnateVarIndex(truthTable * tt, int i )
{
    if (tt == NULL || i < 0 || i >= tt->varNum) {
        printf("isUnateVarIndex: index %d out of range (varNum %d).\n", i,
               tt == NULL ? -1 : tt->varNum);
        return UNKNOW;
    }
    int isPosUnate = 1;
    int isNegUnate = 1;
    for (int iCheck = 0; iCheck < (1 << tt->varNum); iCheck++) {
        // only choose the assignment where variable i is 0
        if (((iCheck >> i) & 1) == 0) {
            int i0 = iCheck;
            int i1 = iCheck | (1 << i);
            int f0 = (tt->ttrep >> i0) & 1;
            int f1 = (tt->ttrep >> i1) & 1;
            if (f1 < f0) isPosUnate = 0;
            if (f1 > f0) isNegUnate = 0;
        }
    }
    // A variable that does not affect the output (both flags stay 1) is
    // positive unate in the inclusive cofactor-containment sense.
    if (isPosUnate)
        return POS_UNATE;
    if (isNegUnate)
        return NEG_UNATE;
    return BINATE;
}
