// colour
@0
D=A
@white
M=D

D=A
@black
M=-1

// width in 16bit
@32	
D=A
@width
M=D

// height in lines
@256
D=A
@height
M=D

// iterators
@i
M=0
@j
M=0

(RENDER)
@SCREEN
D=A
@v
M=D
(ROW_LOOP)
(LINE_LOOP)


@24576 // get value from keyboard 
D=M

@BLACK // Jump to black if keboard is pressed
D;JNE
(WHITE)
@white
D=M
@R
0;JMP 


(BLACK)
@black
D=M

(R)
@v
A=M
M=D	// put desired color to pixel

@v
M=M+1
@j
M=M+1
@width
D=M
@j
D=D-M
@LINE_LOOP
D;JNE

@j
M=0

@i
M=M+1
@height
D=M
@i
D=D-M
@ROW_LOOP
D;JNE

@i
M=0

@j
M=0

@v
M=0

@RENDER
0;JMP

(END)
@END
0;JMP

