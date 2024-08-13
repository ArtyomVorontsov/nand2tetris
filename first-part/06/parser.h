/* That is header file for parser.c, I also don't care about where that will be used
 * anyway that is garbage for my own education
 */
#include <stdbool.h>
#include <stdlib.h>

struct CInstruction {
	char* dest;
	char* comp;
	char* jump;
};

struct CInstruction* parser(char* sp);

/* That shit is needed to check if there something left to process */
bool hasMoreLines();

/* That shit is needed to move pointer across the source file */
char* advance();

/* That crap is needed to determine current instruction type */
enum InstructionType {A_INSTRUCTION, C_INSTRUCTION, L_INSTRUCTION};
enum InstructionType instructionType();

/* Needed to handle stupid A and L type of instructions, will retrieve sybols from there... */
char* symbol();

/* To decompose C instruction we need that shitty dest function, it will retrieve dest part from whole instruction */
char* dest(char*);

/* The same for below ones, cmon you are so stupid that cannot just read code? */
char* comp(char*);

char* jump(char*);


/* End of that crap */

