#include "./vm-writer.h"
#include <stdio.h>

void writePush(FILE *dfp, enum SEGMENT segment, int index)
{
}

void writePop(FILE *dfp, enum SEGMENT segment, int index)
{
}

void writeArithmetic(FILE *dfp, enum COMMAND command)
{
}

void writeLabel(FILE *dfp, char *label)
{
}

void writeGoto(FILE *dfp, char *label)
{
}

void writeIf(FILE *dfp, char *label)
{
}

void writeCall(FILE *dfp, char *label)
{
}

void writeFunction(FILE *dfp, char *label)
{
    fprintf(dfp, "function %s\n", label);
}

void writeReturn(FILE *dfp)
{
}

void close(FILE *dfp)
{
}
