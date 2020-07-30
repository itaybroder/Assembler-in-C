#include "node.h"

void add(line_node *head, line_node *node){
    line_node *runner = head;
    while (runner->next != NULL)
    {
        runner = runner->next;
    }
    
    runner->next = node;
}