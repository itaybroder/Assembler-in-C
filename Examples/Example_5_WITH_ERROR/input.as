; file ps.as
.entry LIST
.extern W
MAIN:	add r3, LIST
LOOP:	prn #48
		lea W, r6
		inc r6
		mov r3, K
		sub r1, r4
		bne EN$D
		cmp K, #-6
		bne &EN$D
		dec W
.entry MAIN
		jmp &LOOP
		add L3, L3
EN$D:	stop

STR:	.string "abcd"
LIST:	.data 6, -9
		.data -100
K:		.data 31
.extern L3