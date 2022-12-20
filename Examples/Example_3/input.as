; new test file
.extern TM
MAIN: inc r4
 sub r1, r4
 add r5, NUMBERS
 mov r5, B
 dec TM

END: stop

STR: .string "abcd"
NUMBERS: .data 8, -122
B: .data 120