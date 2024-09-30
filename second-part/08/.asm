@256
D=A
@SP
M=D
@0
D=A
@LCL
M=D

@0
D=A
@TEMP
M=D

@0
D=A
@ARG
M=D

@0
D=A
@THIS
M=D

@0
D=A
@THAT
M=D

@init.
0;JMP

// NEW FILE Main.vm STARTED

(Main.fibonacci.)
// push command
@ARG
D=M
@0
A=D+A
D=M

@SP // push on the stack value in d register
A=M
M=D

@SP // increment stack pointer
M=M+1

// push command
@2
D=A

@SP // push on the stack value in d register
A=M
M=D

@SP // increment stack pointer
M=M+1

// arithmetic
// lt instruction

@SP // decrement stack pointer
M=M-1

A=M // set stack address

D=M // save popped value in D register

@R13 // save popped value in R13 register
M=D
@R13
D=M
@R14
M=D

@SP // decrement stack pointer
M=M-1

A=M // set stack address

D=M // save popped value in D register

@R13 // save popped value in R13 register
M=D
@R13
D=M
@R14
D=D-M
@LT..3
D;JLT
D=0
@NLT..3
0;JMP
(LT..3)
D=1
(NLT..3)

@SP // push on the stack value in d register
A=M
M=D

@SP // increment stack pointer
M=M+1

@SP // decrement stack pointer
M=M-1

A=M // set stack address

D=M // save popped value in D register

@R13 // save popped value in R13 register
M=D
@R13
D=M
@IFTRUE.
D;JNE
@IFFALSE.
0;JMP
(IFTRUE.)
// push command
@ARG
D=M
@0
A=D+A
D=M

@SP // push on the stack value in d register
A=M
M=D

@SP // increment stack pointer
M=M+1

// Function Main.fibonacci epilogue

@SP // decrement stack pointer
M=M-1

A=M // set stack address

D=M // save popped value in D register

@R13 // save popped value in R13 register
M=D
// Put return value for function which invoked that function
@R13
D=M
@R14
M=D
// Save SP value for function which invoked that function
@ARG
D=M
@R15
M=D
// Put saved memory segment values back

@SP // decrement stack pointer
M=M-1

A=M // set stack address

D=M // save popped value in D register

@R13 // save popped value in R13 register
M=D
@R13
D=M
@THAT
M=D

@SP // decrement stack pointer
M=M-1

A=M // set stack address

D=M // save popped value in D register

@R13 // save popped value in R13 register
M=D
@R13
D=M
@THIS
M=D

@SP // decrement stack pointer
M=M-1

A=M // set stack address

D=M // save popped value in D register

@R13 // save popped value in R13 register
M=D
@R13
D=M
@ARG
M=D

@SP // decrement stack pointer
M=M-1

A=M // set stack address

D=M // save popped value in D register

@R13 // save popped value in R13 register
M=D
@R13
D=M
@LCL
M=D
// Jump to return label

@SP // decrement stack pointer
M=M-1

A=M // set stack address

D=M // save popped value in D register

@R13 // save popped value in R13 register
M=D
// Set old SP value
@R15
D=M
@SP
M=D
// Push function return value on the stack
@R14
D=M

@SP // push on the stack value in d register
A=M
M=D

@SP // increment stack pointer
M=M+1
// Jump to return label
@R13
A=M
0;JMP
(IFFALSE.)
// push command
@ARG
D=M
@0
A=D+A
D=M

@SP // push on the stack value in d register
A=M
M=D

@SP // increment stack pointer
M=M+1

// push command
@2
D=A

@SP // push on the stack value in d register
A=M
M=D

@SP // increment stack pointer
M=M+1

// arithmetic
// sub instruction

@SP // decrement stack pointer
M=M-1

A=M // set stack address

D=M // save popped value in D register

@R13 // save popped value in R13 register
M=D
@R13
D=M
@R14
M=D

@SP // decrement stack pointer
M=M-1

A=M // set stack address

D=M // save popped value in D register

@R13 // save popped value in R13 register
M=D
@R13
D=M
@R14
D=D-M

@SP // push on the stack value in d register
A=M
M=D

@SP // increment stack pointer
M=M+1
// Function Main.fibonacci prologue
// Push return label value on top of the stack
@Main.fibonacci$ret.2
D=A

@SP // push on the stack value in d register
A=M
M=D

@SP // increment stack pointer
M=M+1
// Save memory addresses of current function in stack frame
@LCL
D=M

@SP // push on the stack value in d register
A=M
M=D

@SP // increment stack pointer
M=M+1
@ARG
D=M

@SP // push on the stack value in d register
A=M
M=D

@SP // increment stack pointer
M=M+1
@THIS
D=M

@SP // push on the stack value in d register
A=M
M=D

@SP // increment stack pointer
M=M+1
@THAT
D=M

@SP // push on the stack value in d register
A=M
M=D

@SP // increment stack pointer
M=M+1
// Assign new values for memory segments
@SP
D=M
// Assign new value for LCL memory segment
@LCL
M=D
// Assign new value for ARG memory segment
@ARG
M=D-1
M=M-1
M=M-1
M=M-1
M=M-1
@1
D=A
@ARG
M=M-D
// JMP to function body code
@Main.fibonacci.
0;JMP
(Main.fibonacci$ret.2)
// push command
@ARG
D=M
@0
A=D+A
D=M

@SP // push on the stack value in d register
A=M
M=D

@SP // increment stack pointer
M=M+1

// push command
@1
D=A

@SP // push on the stack value in d register
A=M
M=D

@SP // increment stack pointer
M=M+1

// arithmetic
// sub instruction

@SP // decrement stack pointer
M=M-1

A=M // set stack address

D=M // save popped value in D register

@R13 // save popped value in R13 register
M=D
@R13
D=M
@R14
M=D

@SP // decrement stack pointer
M=M-1

A=M // set stack address

D=M // save popped value in D register

@R13 // save popped value in R13 register
M=D
@R13
D=M
@R14
D=D-M

@SP // push on the stack value in d register
A=M
M=D

@SP // increment stack pointer
M=M+1
// Function Main.fibonacci prologue
// Push return label value on top of the stack
@Main.fibonacci$ret.3
D=A

@SP // push on the stack value in d register
A=M
M=D

@SP // increment stack pointer
M=M+1
// Save memory addresses of current function in stack frame
@LCL
D=M

@SP // push on the stack value in d register
A=M
M=D

@SP // increment stack pointer
M=M+1
@ARG
D=M

@SP // push on the stack value in d register
A=M
M=D

@SP // increment stack pointer
M=M+1
@THIS
D=M

@SP // push on the stack value in d register
A=M
M=D

@SP // increment stack pointer
M=M+1
@THAT
D=M

@SP // push on the stack value in d register
A=M
M=D

@SP // increment stack pointer
M=M+1
// Assign new values for memory segments
@SP
D=M
// Assign new value for LCL memory segment
@LCL
M=D
// Assign new value for ARG memory segment
@ARG
M=D-1
M=M-1
M=M-1
M=M-1
M=M-1
@1
D=A
@ARG
M=M-D
// JMP to function body code
@Main.fibonacci.
0;JMP
(Main.fibonacci$ret.3)
// arithmetic
// add instruction

@SP // decrement stack pointer
M=M-1

A=M // set stack address

D=M // save popped value in D register

@R13 // save popped value in R13 register
M=D
@R13
D=M
@R14
M=D

@SP // decrement stack pointer
M=M-1

A=M // set stack address

D=M // save popped value in D register

@R13 // save popped value in R13 register
M=D
@R13
D=M
@R14
D=D+M

@SP // push on the stack value in d register
A=M
M=D

@SP // increment stack pointer
M=M+1
// Function Main.fibonacci epilogue

@SP // decrement stack pointer
M=M-1

A=M // set stack address

D=M // save popped value in D register

@R13 // save popped value in R13 register
M=D
// Put return value for function which invoked that function
@R13
D=M
@R14
M=D
// Save SP value for function which invoked that function
@ARG
D=M
@R15
M=D
// Put saved memory segment values back

@SP // decrement stack pointer
M=M-1

A=M // set stack address

D=M // save popped value in D register

@R13 // save popped value in R13 register
M=D
@R13
D=M
@THAT
M=D

@SP // decrement stack pointer
M=M-1

A=M // set stack address

D=M // save popped value in D register

@R13 // save popped value in R13 register
M=D
@R13
D=M
@THIS
M=D

@SP // decrement stack pointer
M=M-1

A=M // set stack address

D=M // save popped value in D register

@R13 // save popped value in R13 register
M=D
@R13
D=M
@ARG
M=D

@SP // decrement stack pointer
M=M-1

A=M // set stack address

D=M // save popped value in D register

@R13 // save popped value in R13 register
M=D
@R13
D=M
@LCL
M=D
// Jump to return label

@SP // decrement stack pointer
M=M-1

A=M // set stack address

D=M // save popped value in D register

@R13 // save popped value in R13 register
M=D
// Set old SP value
@R15
D=M
@SP
M=D
// Push function return value on the stack
@R14
D=M

@SP // push on the stack value in d register
A=M
M=D

@SP // increment stack pointer
M=M+1
// Jump to return label
@R13
A=M
0;JMP
(EOF_LOOP)
@EOF_LOOP
0;JMP
// NEW FILE Sys.vm STARTED

(Sys.init.)
// push command
@4
D=A

@SP // push on the stack value in d register
A=M
M=D

@SP // increment stack pointer
M=M+1

// Function Sys.init prologue
// Push return label value on top of the stack
@Main.fibonacci$ret.1
D=A

@SP // push on the stack value in d register
A=M
M=D

@SP // increment stack pointer
M=M+1
// Save memory addresses of current function in stack frame
@LCL
D=M

@SP // push on the stack value in d register
A=M
M=D

@SP // increment stack pointer
M=M+1
@ARG
D=M

@SP // push on the stack value in d register
A=M
M=D

@SP // increment stack pointer
M=M+1
@THIS
D=M

@SP // push on the stack value in d register
A=M
M=D

@SP // increment stack pointer
M=M+1
@THAT
D=M

@SP // push on the stack value in d register
A=M
M=D

@SP // increment stack pointer
M=M+1
// Assign new values for memory segments
@SP
D=M
// Assign new value for LCL memory segment
@LCL
M=D
// Assign new value for ARG memory segment
@ARG
M=D-1
M=M-1
M=M-1
M=M-1
M=M-1
@1
D=A
@ARG
M=M-D
// JMP to function body code
@Main.fibonacci.
0;JMP
(Main.fibonacci$ret.1)
(WHILE.)
@WHILE.
0;JMP
