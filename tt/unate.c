
#include "unate.h"
//check if the function is positive unate, negative unate or binate
enum FUNC_TYPE isUnate(truthTable * tt, ziArray * unate_record)
{
    int i;
    ziArray *res = unate_record;
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
        else if(isNegUnate == 1 && isNegUnate == 1)
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
    //check if all pos_unate
    ArrayForEachItem(res, i) for (i = 0; i < res->size; i++)
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
    //check if all neg_unate
    if(countPosSum == tt->varNum)
    {
        return POS_UNATE;
    }
    //check if binate
    if(countNegSum == tt->varNum)
    {
        return NEG_UNATE;
    }
    //check if binate
    if(countBinate == tt->varNum)
    {
        return BINATE;
    }
    //check if const
    if(countIrr == tt->varNum)
    {
        return IRRELEVANT;
    }
    return UNKNOW;
}