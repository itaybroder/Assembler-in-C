#include "node.h"

void addLineNode(line_node *head, line_node *node){
    line_node *runner = head;
    while (runner->next != NULL)
    {
        runner = runner->next;
    }
    
    runner->next = node;
    node->next = NULL; 
}

line_node *getByIndex(line_node *head, int index){
    line_node *runner = head->next;
    int i = 0;
    while (runner != NULL)
    {
        if(i == index){
            return runner;
        }
        runner = runner->next;
        i++;
    }
    
    return NULL;
}