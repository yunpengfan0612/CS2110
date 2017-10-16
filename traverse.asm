;;==============================================
;; NAME:Yunpeng Fan
;;==============================================


.orig x3000

	; Start your code here
	LEA R7,ARRAY;	save initial address of array to R7
	LD R0,ARRAY; 	LOAD FIRST ELEMENT AS MAX VALUE
	LD R1,ODD_CNT;	initial odd count
	LD R2,LENGTH;	initial length
	AND R3,R3,0; 	STORE INDEX NUMBER

WHILE	LDR R4,R7,0;	STORE ARRAY AT INDEX
	ADD R5,R4,1;	SEE IF ELEMENT AT INDEX == -1
	BRz STOP;	IF YES JUMP TO STOP
IFONE	AND R5,R5,0;	CLEAR R5
	ADD R5,R5,R0;	STORE MAX VALUE TO R5
	NOT R5,R5;	NEGATE
	ADD R5,R5,1;	LET R5 = - MAX
	ADD R5,R4,R5;	ARRAY[I]-MAX
	BRp THEN1;	IF ARRAY[I]-MAX > 0, JUMP TO THEN1
	BRnz IFTWO;	IF NOT, JUMP TO NEXT IF STATEMENT
THEN1	ADD R0,R4,0;	SET MAX = ARRAY[I]
IFTWO	AND R6,R4,1;	ARRAY[I]&1
	ADD R6,R6,-1;	LET RESULT -1
	BRz THEN2;	IF RESAULT-1=0, THEN JUMP TO THEN2
	BRnp CONT;	IF NOT, JUMP TO CONT
THEN2	ADD R1,R1,1;	ODD COUNT++
CONT	ADD R3,R3,1;	INDEX++
	ADD R7,R7,1;	ADDRESS++
	BR WHILE;	JUMP BACK TO WHILE LOOP
STOP	ADD R2,R2,R3;	LENGTH=INDEX
	ST R0,MAX_VAL;	STORE TO MAX
	ST R1,ODD_CNT;	STORE TO ODD COUNT
	ST R2,LENGTH;	STORE TO LENGTH
	HALT;		STOP

MAX_VAL .fill 0			; Store the maximum value in the array at this label
ODD_CNT	.fill 0			; Store the total number of odd entries in the array at this label
LENGTH	.fill 0			; Store the length of the array at this label

ARRAY	.fill 1			; The first element of the array
	.fill 3
	.fill 7
	.fill 4
	.fill 2
	.fill 9
	.fill 3
	.fill 4			; The last element of the array
	.fill -1		; -1 will follow the last element of the array	


.end
