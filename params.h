#include "dataNode.h"
#include "node.h"
enum opcode{
    MOV=0b000000000000000000000000,
    CMP=0b000001000000000000000000,
    ADD=0b000010000000000000000000,
    SUB=0b000010000000000000000000,
    LEA=0b000000000000000000000000
}

typedef struct params_struct {
    int IC;
    int DC;

    line_node line_node_head;
    data_node data_node_head;


    int data[100];
    
} params;