
typedef struct truthTable_ truthTable;
typedef unsigned long tt;

// some number base
enum BASE { BIN, OCT, DEC, HEX };

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

int initElementaryVar(truthTable* a, int varNum, int ith);
truthTable* readTT(char* tts, int verbose);
int evalTTVal(truthTable* a, unsigned int val);
int evalTTMask(truthTable* a, unsigned int val);
truthTable* cofactor0(truthTable* a, int index);
truthTable* cofactor1(truthTable* a, int index);
int clearTT(truthTable* a);
void showTT(truthTable* a, enum BASE b);
