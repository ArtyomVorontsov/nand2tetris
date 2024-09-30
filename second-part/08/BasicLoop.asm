(sum.BasicLoop)
// push command
@0
D=A

@SP // push on the stack value in d register
A=M
M=D

@SP // increment stack pointer
M=M+1

// pop command

@SP // decrement stack pointer
M=M-1

A=M // set stack address

D=M // save popped value in D register

@R13 // save popped value in R13 register
M=D

@LCL // calculate LCL with offset and safe in R14
D=M
@0
D=D+A
@R14
M=D

@R13 // store value in R13 in address which is stored in R14
D=M
@R14
A=M
M=D
(LOOP_START.BasicLoop)
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
@LCL
D=M
@0
A=D+A
D=M

@SP // push on the stack value in d register
A=M
M=D

@SP // increment stack pointer
M=M+1

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
// pop command

@SP // decrement stack pointer
M=M-1

A=M // set stack address

D=M // save popped value in D register

@R13 // save popped value in R13 register
M=D

@LCL // calculate LCL with offset and safe in R14
D=M
@0
D=D+A
@R14
M=D

@R13 // store value in R13 in address which is stored in R14
D=M
@R14
A=M
M=D
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
// pop command

@SP // decrement stack pointer
M=M-1

A=M // set stack address

D=M // save popped value in D register

@R13 // save popped value in R13 register
M=D

@ARG // calculate ARG with offset and safe in R14
D=M
@0
D=D+A
@R14
M=D

@R13 // store value in R13 in address which is stored in R14
D=M
@R14
A=M
M=D
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


@SP // decrement stack pointer
M=M-1

A=M // set stack address

D=M // save popped value in D register

@R13 // save popped value in R13 register
M=D
@R13
D=M
@LOOP_START.BasicLoop
D;JNE
// push command
@LCL
D=M
@0
A=D+A
D=M

@SP // push on the stack value in d register
A=M
M=D

@SP // increment stack pointer
M=M+1

// Function sum epilogue

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
// not determined
(init.BasicLoop)
// push command
@10
D=A

@SP // push on the stack value in d register
A=M
M=D

@SP // increment stack pointer
M=M+1

// Function init prologue
// Push return label value on top of the stack
@sum$ret.1
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
@sum.BasicLoop
0;JMP
(sum$ret.1)
// not determined
// not determined
// not determined
(EOF_LOOP)
@EOF_LOOP
0;JMP
