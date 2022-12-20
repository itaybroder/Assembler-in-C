#include "libs.h"

#define A_BIT 0x000004 /* 0b000000000000000000000100*/
#define R_BIT 0x000002 /*0b000000000000000000000010*/
#define E_BIT 0x000001 /*0b000000000000000000000001*/
#define OP_CODES_LEN 16
#define ASSEMBLY_EXTENTION ".as"
#define OBJECT_EXTENTION ".ob"
#define EXTERNAL_EXTENTION ".ext"
#define ENTITY_EXTENTION ".ent"
#define STRING_label_PREFIX ".string"
#define DATA_label_PREFIX ".data"
/* enum for true and false*/
enum boolean{false, true};

/* These are the addressing operand methods*/
enum operand_types{immidiate=0, direct=1, relative=2, reg=3};


/* This is a list of all the command names. This list is used to find the command index and then with the command index the program can find the corrosponding command code in the next arrays*/
__attribute__((unused)) static const char* op_names[] = {"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", "jsr", "red", "prn", "rts", "stop"};

/* Each command code is encoded as hex to avoide a "warning: binary constants are a GCC extension". To keep the code readable
 we leave the original binary encoding in the comments*/
static const int op_codes[] = {
    0x000000,/*mov 0b000000000000000000000000*/
    0x040000,/*cmp 0b000001000000000000000000*/
    0x080000,/*add 0b000010000000000000000000*/
    0x080000,/*sub 0b000010000000000000000000*/
    0x100000,/*lea 0b000100000000000000000000*/
    0x140000,/*clr 0b000101000000000000000000*/
    0x140000,/*not 0b000101000000000000000000*/
    0x140000,/*inc 0b000101000000000000000000*/
    0x140000,/*dec 0b000101000000000000000000*/
    0x240000,/*jmp 0b001001000000000000000000*/
    0x240000,/*bne 0b001001000000000000000000*/
    0x240000,/*jsr 0b001001000000000000000000*/
    0x300000,/*red 0b001100000000000000000000*/
    0x340000,/*prn 0b001101000000000000000000*/
    0x380000,/*rts 0b001110000000000000000000*/
    0x3C0000/*stop 0b001111000000000000000000*/

};

/* This is a list of all the funct codes ordered by the commands in the commands array*/
static const int funct[] = {
    0x000000,/*mov 0b000000000000000000000000*/
    0x000000,/*cmp 0b000000000000000000000000*/
    0x000008,/*add 0b000000000000000000001000*/
    0x000010,/*sub 0b000000000000000000010000*/
    0x000000,/*lea 0b000000000000000000000000*/
    0x000008,/*clr 0b000000000000000000001000*/
    0x000010,/*not 0b000000000000000000010000*/
    0x000018,/*inc 0b000000000000000000011000*/
    0x000020,/*dec 0b000000000000000000100000*/
    0x000008,/*jmp 0b000000000000000000001000*/
    0x000010,/*bne 0b000000000000000000010000*/
    0x000018,/*jsr 0b000000000000000000011000*/
    0x000000,/*red 0b000000000000000000000000*/
    0x000000,/*prn 0b000000000000000000000000*/
    0x000000,/*rts 0b000000000000000000000000*/
    0x000000/*stop 0b000000000000000000000000*/
};

/* this list keeps the register code per registrer per location. meaning if it's the source register or the destination register*/
static const int register_codes[2][8] = {
    {
    0x030000,/*r0 0b000000110000000000000000*/
    0x032000,/*r1 0b000000110010000000000000*/
    0x034000,/*r2 0b000000110100000000000000*/
    0x036000,/*r3 0b000000110110000000000000*/
    0x38000,/*r4 0b000000111000000000000000*/
    0x03A000,/*r5 0b000000111010000000000000*/
    0x03C000,/*r6 0b000000111100000000000000*/
    0x03E000,/*r7 0b000000111110000000000000*/
},
    {
    0x001800,/*r0 0b000000000001100000000000*/
    0x001900,/*r1 0b000000000001100100000000*/
    0x001A00,/*r2 0b000000000001101000000000*/
    0x001B00,/*r3 0b000000000001101100000000*/
    0x001C00,/*r4 0b000000000001110000000000*/
    0x001D00,/*r5 0b000000000001110100000000*/
    0x001E00,/*r6 0b000000000001111000000000*/
    0x001F00,/*r7 0b000000000001111100000000*/
}};

/* This list keeps the legal addressing types for each command by source or destination.
It is orederd by the 4 types of addressing: immidiate, direct, relative, reg
 so if the commands legal addressing types is 0b1011 or in hex 0xB then that means the legal addressing types are
 immidiate,  direct and reg but not relative because there is a zero in the 3rd bit*/
static const int legal_operand_types[2][16] = {
    {
    0xB,/*mov 0b1011*/
    0xB,/*cmp 0b1011*/
    0xB,/*add 0b1011*/
    0xB,/*sub 0b1011*/
    0x2,/*lea 0b0010*/
    0x0,/*clr 0b0000*/
    0x0,/*not 0b0000*/
    0x0,/*inc 0b0000*/
    0x0,/*dec 0b0000*/
    0x0,/*jmp 0b0000*/
    0x0,/*bne 0b0000*/
    0x0,/*jsr 0b0000*/
    0x0,/*red 0b0000*/
    0x0,/*prn 0b0000*/
    0x0,/*rts 0b0000*/
    0x0 /*stop 0b0000*/

},
    {
    0xA,/*mov 0b1010*/
    0xB,/*cmp 0b1011*/
    0xA,/*add 0b1010*/
    0xA,/*sub 0b1010*/
    0xA,/*lea 0b1010*/
    0xA,/*clr 0b1010*/
    0xA,/*not 0b1010*/
    0xA,/*inc 0b1010*/
    0xA,/*dec 0b1010*/
    0x6,/*jmp 0b0110*/
    0x6,/*bne 0b0110*/
    0x6,/*jsr 0b0110*/
    0xA,/*red 0b1010*/
    0xB,/*prn 0b1011*/
    0x0,/*rts 0b0000*/
    0x0/*stop 0b0000*/
}};

/* This array represents each operand type as a binary flag*/
static const int operand_to_binary_flag[4] = {
    0x1, /* immidiate 0b0001*/
    0x2, /* direct 0b0010*/
    0x4, /* relative 0b0100*/
    0x8  /* register 0b1000*/
};

/* Operand adressing method 1 (Direct Adressing)*/
static const int direct_operand_codes[2] = {
    0x010000, /* 0b000000010000000000000000*/
    0x000800 /* 0b000000000000100000000000*/
};

/* Operand adressing method 2 (Relative adressing)*/
static const int relative_operand_codes[2] = {
    0x020000, /* 0b000000100000000000000000*/
    0x001000 /* 0b000000000001000000000000*/
};

/* this list saves the group of each command by index*/
static const int groups[] = {1,/*mov*/
        1,/*cmp*/
        1,/*add*/
        1,/*sub*/
        1,/*lea*/
        2,/*clr*/
        2,/*not*/
        2,/*inc*/
        2,/*dec*/
        2,/*jmp*/
        2,/*bne*/
        2,/*jsr*/
        2,/*red*/
        2,/*prn*/
        3,/*rts*/
        3 /*stop*/
};

/* this list saved the corrosponding hex letter for each number between 0 and 16*/
static const char hex_letters[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};


/* This struct saves the important information that is needed in almost every function. This information is sved in a struct to
 prevent passing a lot of parameter to each function*/
typedef struct params_struct {
    int IC; /* The current Ic value*/
    int DC; /* The current DC value*/
    int ICF; /* The final IC value*/
    int DCF; /* The final DC value*/
    int current_line_has_label; /* Does the current line have a label in the beggining*/
    int input_file_line; /* The current line in the input line that is being parsed*/

    line_node *line_node_head; /* This linked list saves all the code words in order*/
    data_node *data_node_head; /* This linked list saves all the data words in order*/
    label *label_head; /* This linked list saves all of the labels found in order*/
    label* externals; /* This linked list saves all of the external labels found in order*/
    char* current_file_name;
    
    int errors_found; /* This variable is true if an error has been found*/
    
} params;
