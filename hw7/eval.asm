; Name:Yunpeng Fan


; Main
; Do not edit this function!

.orig x3000

	LD R6, STACK	; Initialize the stack

	LEA R0, STRING	; R0 = &str[0]
	ADD R1, R0, 0

SL_LOOP	LDR R2, R1, 0	; \ R1 = strlen(str)
	BRz SL_END	; |
	ADD R1, R1, 1	; |
	BR SL_LOOP	; |
SL_END	NOT R2, R0	; |
	ADD R2, R2, 1	; |
	ADD R1, R1, R2	; /

	ADD R6, R6, -2	; \ R0 = eval(str, len)
	STR R0, R6, 0	; |
	STR R1, R6, 1	; |
	LD R2, EVALPTR	; |
	JSRR R2		; |
	LDR R0, R6, 0	; |
	ADD R6, R6, 3	; /

	ST R0, ANS
	HALT

STACK	.fill xf000
ANS	.fill -1
EVALPTR	.fill EVAL
STRING	.stringz "1*2+3*4+5"
	.blkw 200
	

	; Write your function here
	

	;FP-2	i
	;FP-1	left
	;FP	right
	;FP+1	OFP
	;FP+2	return address
	;FP+3	return value
	;FP+4	str(address)
	;FP+5	len(string length)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;R0	i
	;R1	str
	;R2	len
	;R3	TEMP
	;R4	TEMP
	;R5	FP
	;R6	SP/RETURN VALUE
	;R7	RETURN ADDRESS

PLUSS	.stringz "+"
MULTS	.stringz "*"
ZEROS	.stringz "0"


EVAL	ADD R6,R6,-4;	allocate space for Old FP, return address, return value
	STR R5,R6,1;	push old FP
	ADD R5,R6,0;	FP = SP
	STR R7,R5,2;	push return address

	ADD R6,R6,-5;	allocate space for registers and locals
	STR R0,R5,-5;	push i into stack
	STR R1,R5,-4;	push R1 into stack
	STR R2,R5,-3;	push R2 into stack	

	AND R0,R0,0;	clear R0, R0 = i = 0;
	LDR R2,R5,4;	load str, not neccessary to repeat since won't change

WHILE1	LDR R1,R5,5;	load len 
	NOT R3,R1;	\ R3 = -len
	ADD R3,R3,1;	/
	ADD R3,R0,R3;	R3 = i - len, see if i < len
	BRzp SKIPWL1;	if !(i < len) skip while loop 1
	 
IF1	ADD R3,R2,R0;	R3 = str + i
	LD R4,PLUSS;	R4 = "+"
	NOT R4,R4;	\ R4 = -'+'
	ADD R4,R4,1;	/
	LDR R1,R3,0;	R3 = *(str + i)	
	ADD R3,R1,R4;	R3 = *(str + i) - '+'
	BRnp SKIPIF1; 	if *(str + i) != '+', skip IF1
	
GETLS	ADD R6,R6,-2;	allocate space for argument str and i
	STR R2,R6,0;	store str into stack
	STR R0,R6,1;	store i into stack
	JSR EVAL;	recursion 
	LDR R3,R6,0;	load return value of left into R3
	STR R3,R5,-1;	push return value of left into stack
	ADD R6,R6,3;	set SP below last argument

GETRS	ADD R6,R6,-2;	allocate space for "str+i+1" and "len-i-1"
	ADD R3,R2,R0;	R3 = str + i
	ADD R3,R3,1;	R3 = str + i + 1
	STR R3,R6,0;	store str + i + 1 into stack
	NOT R3,R0;	R3 = - i - 1 (-i = NOT R0 + 1 => -i - 1 = NOT R0 + 1 - 1)
	LDR R1,R5,5;	R1 = len
	ADD R3,R3,R1;	R3 = len - i - 1
	STR R3,R6,1;	store len - i - 1 into stack	
	JSR EVAL;	recursion
	LDR R3,R6,0;	load return value of right into R3
	STR R3,R5,0;	push return value of right into stack
	ADD R6,R6,3;	set SP below last argument

SUMLR	LDR R3,R5,-1;	load left into R3
	LDR R4,R5,0;	load right into R4
	ADD R3,R3,R4;	R3 = left + right
	STR R3,R5,3;	push left + right into stack for return value
	BR EXIT;	jump out

SKIPIF1	ADD R0,R0,1;	i++
	BR WHILE1;	jump back to while loop 1

SKIPWL1	AND R0,R0,0;	clear R0, R0 = i = 0;
	
WHILE2	LDR R1,R5,5;	load len 
	NOT R3,R1;	\ R3 = -len
	ADD R3,R3,1;	/
	ADD R3,R0,R3;	R3 = i - len, see if i < len
	BRzp SKIPWL2;	if !(i < len) skip while loop 1
	 
IF2	ADD R3,R2,R0;	R3 = str + i 
	LD R4,MULTS;	R4 = "*"
	NOT R4,R4;	\ R4 = -'*'
	ADD R4,R4,1;	/
	LDR R1,R3,0;	R3 = *(str + i)
	ADD R3,R1,R4;	R3 = *(str + i) - '*'
	BRnp SKIPIF2; 	if *(str + i) != '*', skip IF1
	
GETLM	ADD R6,R6,-2;	allocate space for argument str and i 
	STR R2,R6,0;	store i into stack
	STR R0,R6,1;	store str into stack
	JSR EVAL;	recursion 
	LDR R3,R6,0;	load return value of left into R3
	STR R3,R5,-1;	push return value of left into stack
	ADD R6,R6,3;	set SP below last argument

GETRM	ADD R6,R6,-2;	allocate space for "str+i+1" and "len-i-1"
	ADD R3,R2,R0;	R3 = str + i
	ADD R3,R3,1;	R3 = str + i + 1
	STR R3,R6,0;	store str + i + 1 into stack
	NOT R3,R0;	R3 = - i - 1 (-i = NOT R0 + 1 => -i - 1 = NOT R0 + 1 - 1)
	LDR R1,R5,5;	R1 = len
	ADD R3,R3,R1;	R3 = len - i - 1
	STR R3,R6,1;	store len - i - 1 into stack	
	JSR EVAL;	recursion
	LDR R3,R6,0;	load return value of right into R3
	STR R3,R5,0;	push return value of right into stack
	ADD R6,R6,3;	set SP below last argument

	LDR R3,R5,-1;	load left into R3
	LDR R4,R5,0;	load right into R4
	AND R1,R1,0;	clear R1
MULT	ADD R1,R1,R3;	keep adding left
	ADD R4,R4,-1;	function as left * right	
 	BRnz MEXIT;	if multiplication is done jump out
	BRp MULT;	else, keep adding

MEXIT	STR R1,R5,3;	push left * right into stack for return value
	BR EXIT;	jump out

SKIPIF2	ADD R0,R0,1;	i++
	BR WHILE2;	jump back to while loop 1

SKIPWL2	LDR R3,R2,0;	load *str into R3
	LD R4,ZEROS;	R4 = '0'
	NOT R4,R4;	\ R4 = - '0'
	ADD R4,R4,1;	/
	ADD R3,R3,R4;	R3 = *str - '0'
	STR R3,R5,3;	store return value into stack
	
EXIT	LDR R0,R5,-5;	\ restore everything
	LDR R1,R5,-4;	|
	LDR R2,R5,-3;	|
	ADD R6,R5,0;	|
	LDR R7,R5,2;	|
	LDR R5,R5,1;	|
	ADD R6,R6,3;	/ move SP to an initial position
	RET;		return to next instruction store in R7	



.end
