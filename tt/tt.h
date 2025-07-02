
#include "array.h"
typedef struct truthTable_ truthTable;
typedef struct cube_ cube;
typedef unsigned long tt;

// some number base
enum BASE { BBIN, BOCT, BDEC, BHEX };

static tt maskTT[] = {0xAAAAAAAAAAAAAAAA, 0xCCCCCCCCCCCCCCCC,
                      0xF0F0F0F0F0F0F0F0, 0xFF00FF00FF00FF00,
                      0xFFFF0000FFFF0000, 0xFFFFFFFF00000000};

static tt maskTTNeg[] = {0x5555555555555555, 0x3333333333333333,
                         0x0F0F0F0F0F0F0F0F, 0x00FF00FF00FF00FF,
                         0x0000FFFF0000FFFF, 0x00000000FFFFFFFF};

static tt mask[] = {0x0000000000000001, 0x0000000000000003, 0x000000000000000F,
                    0x00000000000000FF, 0x000000000000FFFF, 0x00000000FFFFFFFF,
                    0xFFFFFFFFFFFFFFFF};

struct truthTable_ {
    int varNum;
    int elementary;
    unsigned long ttrep;
    unsigned long maskSpec;
};

struct cube_ {
    int varNum;
    unsigned long polarity;  // each bit shows the polarity of the indexed
                             // variable (low priority)
    unsigned long
        mask;  // indicates which literal shows up in this cube (high priority)
};

truthTable* initTT(int varNum);
cube* initCube(truthTable* tt);
int initElementaryVar(truthTable* a, int varNum, int ith);
truthTable* readTT(char* tts, int verbose);
int evalTTVal(truthTable* a, unsigned int val);
int evalTTMask(truthTable* a, unsigned int val);
int isConst(truthTable* tt);
int hasVar(truthTable* tt, int index);
truthTable* cofactor0(truthTable* a, int index);
truthTable* cofactor1(truthTable* a, int index);
truthTable* notCopy(truthTable* tt);
void notInplace(truthTable* tt);
truthTable* andCopy(truthTable* tt0, truthTable* tt1);
truthTable* orCopy(truthTable* tt0, truthTable* tt1);
truthTable* xorCopy(truthTable* tt0, truthTable* tt1);
truthTable* zero(truthTable* tt);
truthTable* one(truthTable* tt);
int getWeight(truthTable* tt);
int getDistance(truthTable* tt0, truthTable* tt1);
void addLit(cube* cube, int varCur, int polarity);
truthTable* isop(truthTable* tt, truthTable* dc, int varIndex, ziArray* cubes);
int clearTT(truthTable* a);
void showTT(truthTable* a, enum BASE b);
