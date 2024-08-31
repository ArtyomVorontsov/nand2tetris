/* That is header file for parser.c, I also don't care about where that will be used
 * anyway that is garbage for my own education
 */
#include "../assembler.h"

struct CInstruction** parser(char *sp);

/* That shit is needed to check if there something left to process */
bool hasMoreLines(char *sp);

/* That shit is needed to move pointer across the source file */
char *advance(char *sp);

/* That crap is needed to determine current instruction type */
enum InstructionType {A_INSTRUCTION, C_INSTRUCTION, L_INSTRUCTION};
char *instructionType(char *sp);

/* Needed to handle stupid A and L type of instructions, will retrieve sybols from there... */
char *symbol();

/* To decompose C instruction we need that shitty dest function, it will retrieve dest part from whole instruction */
char *dest(char*);

/* The same for two below ones, cmon you are so stupid that cannot just read code? */
char *comp(char*);
char *jump(char*);
char *symbolC(char *sp);
char *address(char *sp);
char *label(char *sp);
char *jumpComp(char* sp);

bool isJump(char *sp);
bool isAssignement(char *sp);
bool isSymbol(char *sp);
bool isAddress(char *sp);
bool isLabel(char *sp);
/* End of that crap */

