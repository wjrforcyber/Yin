
#include "unate.h"
//check if the function is positive unate, negative unate or binate
enum FUNC_TYPE isUnate(truthTable * tt, ziArray * res)
{
    int i;
    initArray(res, 10);
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
        if(isPosUnate == 1 && isNegUnate == 0)
        {
            enum FUNC_TYPE *eleEach = malloc(sizeof(enum FUNC_TYPE));
            *eleEach = POS_UNATE;
            pushArray(res, eleEach);
            //return POS_UNATE;
        }
        else if(isPosUnate == 0 && isNegUnate == 1)
        {
            enum FUNC_TYPE *eleEach = malloc(sizeof(enum FUNC_TYPE));
            *eleEach = NEG_UNATE;
            pushArray(res, eleEach);
        }
        else if(isPosUnate == 0 && isNegUnate == 0)
        {
            enum FUNC_TYPE *eleEach = malloc(sizeof(enum FUNC_TYPE));
            *eleEach = BINATE;
            pushArray(res, eleEach);
        }
        else if(isPosUnate == 1 && isNegUnate == 1)
        {
            enum FUNC_TYPE *eleEach = malloc(sizeof(enum FUNC_TYPE));
            *eleEach = IRRELEVANT;
            pushArray(res, eleEach);
            //return IRRELEVANT;
        }
        else {
            printf("Something wrong during detection.\n");
            return UNKNOW;
        }
    }
    int countPosSum = 0;
    int countNegSum = 0;
    int countBinate = 0;
    int countIrr = 0;
    ArrayForEachItem(res, i)
    {
        enum FUNC_TYPE fetch = *(enum FUNC_TYPE*)fetchIndexArray(res, i);
        switch (fetch) {
            case POS_UNATE:
                countPosSum++;
                break;
            case NEG_UNATE:
                countNegSum++;
                break;
            case BINATE:
                countBinate++;
                break;
            case IRRELEVANT:
                countIrr++;
                break;
            default:
                printf("UNKNOW shouldn't shown up here.\n");
                return UNKNOW;
        }
    }
    if (countBinate > 0)
    {
        return BINATE;
    }
    if (countPosSum + countIrr == tt->varNum && countNegSum == 0)
    {
        return POS_UNATE;
    }
    if (countNegSum + countIrr == tt->varNum && countPosSum == 0)
    {
        return NEG_UNATE;
    }
    if (countIrr == tt->varNum)
    {
        return IRRELEVANT;
    }
    return UNKNOW;
}