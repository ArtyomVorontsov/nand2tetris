#include "./vm-writer.h"
#include <stdio.h>

void writePush(FILE *dfp, enum SEGMENT segment, int index)
{
    char *segmentStr = NULL;
    switch (segment)
    {
    case CONSTANT_SEG:
        segmentStr = "constant";
        break;

    default:
        break;
    }
    fprintf(dfp, "push %s %d\n", segmentStr, index);
}

void writePop(FILE *dfp, enum SEGMENT segment, int index)
{
    char *segmentStr = NULL;
    switch (segment)
    {
    case CONSTANT_SEG:
        segmentStr = "constant";
        break;

    case TEMP_SEG:
        segmentStr = "temp";
        break;

    default:
        break;
    }
    fprintf(dfp, "pop %s %d\n", segmentStr, index);
}

void writeArithmetic(FILE *dfp, enum COMMAND command)
{
    switch (command)
    {
    case ADD:
        fprintf(dfp, "add  \n");
        break;

    default:
        break;
    }
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
    fprintf(dfp, "call %s\n", label);
}

void writeFunction(FILE *dfp, char *label)
{
    fprintf(dfp, "function %s\n", label);
}

void writeReturn(FILE *dfp)
{
    fprintf(dfp, "return  \n");
}

void close(FILE *dfp)
{
}
