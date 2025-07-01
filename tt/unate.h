#include "tt.h"

enum FUNC_TYPE { POS_UNATE = 0, NEG_UNATE, BINATE, IRRELEVANT, UNKNOW };

enum FUNC_TYPE isUnate(truthTable* tt, ziArray* unate_record);