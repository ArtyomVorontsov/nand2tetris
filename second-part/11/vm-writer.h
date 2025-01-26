#include <stdio.h>

enum SEGMENT
{
    CONSTANT_SEG,
    ARGUMENT_SEG,
    LOCAL_SEG,
    STATIC_SEG,
    THIS_SEG,
    THAT_SEG,
    POINTER_SEG,
    TEMP_SEG
};

enum COMMAND
{
    ADD,
    SUB,
    NEG,
    EQ,
    GT,
    LT,
    AND,
    OR,
    NOT
};

void writePush(FILE *dfp, enum SEGMENT segment, int index);
void writePop(FILE *dfp, enum SEGMENT segment, int index);
void writeArithmetic(FILE *dfp, enum COMMAND command);
void writeLabel(FILE *dfp, char *label);
void writeGoto(FILE *dfp, char *label);
void writeIf(FILE *dfp, char *label);
void writeCall(FILE *dfp, char *label);
void writeFunction(FILE *dfp, char *label);
void writeReturn(FILE *dfp);
void close(FILE *dfp);
