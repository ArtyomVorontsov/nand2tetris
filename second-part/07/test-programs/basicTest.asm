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

@4000
D=A
@ARG
M=D

@5000
D=A
@THIS
M=D

@6000
D=A
@THAT
M=D

// not determined
// not determined
// not determined
// not determined
// not determined
// not determined
// push command
@10
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
// push command
@21
D=A

@SP // push on the stack value in d register
A=M
M=D

@SP // increment stack pointer
M=M+1

// push command
@22
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

@ARG // calculate ARG with offset and safe in R14
D=M
@2
D=D+A
@R14
M=D

@R13 // store value in R13 in address which is stored in R14
D=M
@R14
A=M
M=D
// pop command

@SP // decrement stack pointer
M=M-1

A=M // set stack address

D=M // save popped value in D register

@R13 // save popped value in R13 register
M=D

@ARG // calculate ARG with offset and safe in R14
D=M
@1
D=D+A
@R14
M=D

@R13 // store value in R13 in address which is stored in R14
D=M
@R14
A=M
M=D
// push command
@36
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

@THIS // calculate THIS with offset and safe in R14
D=M
@6
D=D+A
@R14
M=D

@R13 // store value in R13 in address which is stored in R14
D=M
@R14
A=M
M=D
// push command
@42
D=A

@SP // push on the stack value in d register
A=M
M=D

@SP // increment stack pointer
M=M+1

// push command
@45
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

@THAT // calculate THAT with offset and safe in R14
D=M
@5
D=D+A
@R14
M=D

@R13 // store value in R13 in address which is stored in R14
D=M
@R14
A=M
M=D
// pop command

@SP // decrement stack pointer
M=M-1

A=M // set stack address

D=M // save popped value in D register

@R13 // save popped value in R13 register
M=D

@THAT // calculate THAT with offset and safe in R14
D=M
@2
D=D+A
@R14
M=D

@R13 // store value in R13 in address which is stored in R14
D=M
@R14
A=M
M=D
// push command
@510
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

@TEMP // calculate TEMP with offset and safe in R14
D=M
@6
D=D+A
@R14
M=D

@R13 // store value in R13 in address which is stored in R14
D=M
@R14
A=M
M=D
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

// push command
@THAT
D=M
@5
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
// push command
@ARG
D=M
@1
A=D+A
D=M

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
// push command
@THIS
D=M
@6
A=D+A
D=M

@SP // push on the stack value in d register
A=M
M=D

@SP // increment stack pointer
M=M+1

// push command
@THIS
D=M
@6
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
// push command
@TEMP
D=M
@6
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
(EOF_LOOP)
@EOF_LOOP
0;JMP
