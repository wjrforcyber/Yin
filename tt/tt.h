
typedef struct truthTable_ truthTable;
typedef unsigned long tt;

static tt maskTT[] = 
{
    0xAAAAAAAAAAAAAAAA,
    0xCCCCCCCCCCCCCCCC,
    0xF0F0F0F0F0F0F0F0,
    0xFF00FF00FF00FF00,
    0xFFFF0000FFFF0000,
    0xFFFFFFFF00000000
};

static tt mask[] = 
{
    0x0000000000000001,
    0x0000000000000003,
    0x000000000000000F,
    0x00000000000000FF,
    0x000000000000FFFF,
    0x00000000FFFFFFFF,
    0xFFFFFFFFFFFFFFFF
};

struct truthTable_
{
    int varNum;
    int elementary;
    unsigned long ttrep;
};

int initElementaryVar( truthTable* a, int varNum, int ith );
