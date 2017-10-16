;;===============================
;;Name: Yunpeng Fan
;;===============================

.orig x3000

; CODE GOES HERE! :D
	
AND R1,R1,0;		clear R1
AND R2,R2,0;		clear R2
AND R0,R0,0;		clear R0

LD R1,A;		store value of A in R1
NOT R1,R1;		get ~ A
LD R2,B;		store value of B in R2
NOT R2,R2;		get ~ B
LD R0,ANSWER;		store value of ANSWER in R0

AND R0,R1,R2;		AND A and B and replace value of ANSWER
NOT R0,R0;		get ~ ANSWER
OUT;			return ANSWER
HALT;			stop 

A       .fill 6
B       .fill 11
ANSWER  .fill 0
.end
