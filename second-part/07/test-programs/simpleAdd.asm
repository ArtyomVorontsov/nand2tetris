@256
D=A
@SP
M=D
@2000
D=A
@LCL
M=D

@3000
D=A
@TEMP
M=D

// not determined
// not determined
// not determined
// not determined
// not determined
// not determined
// push command
@7
D=A

@SP // push on the stack value in d register
A=M
M=D

@SP // increment stack pointer
M=M+1

// push command
@8
D=A

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
(EOF_LOOP)
@EOF_LOOP
0;JMP
