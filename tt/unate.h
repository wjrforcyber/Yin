#pragma once

#include "tt.h"

enum FUNC_TYPE { POS_UNATE = 0, NEG_UNATE, BINATE, UNKNOW };

enum FUNC_TYPE isUnate(truthTable* tt, ziArray* res);
enum FUNC_TYPE isUnateVarIndex(truthTable* tt, int i);