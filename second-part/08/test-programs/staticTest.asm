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
@111
D=A

@SP // push on the stack value in d register
A=M
M=D

@SP // increment stack pointer
M=M+1

// push command
@333
D=A

@SP // push on the stack value in d register
A=M
M=D

@SP // increment stack pointer
M=M+1

// push command
@888
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
@R13
D=M
@staticTest.vm.8
M=D
// pop command

@SP // decrement stack pointer
M=M-1

A=M // set stack address

D=M // save popped value in D register

@R13 // save popped value in R13 register
M=D
@R13
D=M
@staticTest.vm.3
M=D
// pop command

@SP // decrement stack pointer
M=M-1

A=M // set stack address

D=M // save popped value in D register

@R13 // save popped value in R13 register
M=D
@R13
D=M
@staticTest.vm.1
M=D
// push command
@staticTest.vm.3
D=M

@SP // push on the stack value in d register
A=M
M=D

@SP // increment stack pointer
M=M+1

// push command
@staticTest.vm.1
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
@staticTest.vm.8
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
