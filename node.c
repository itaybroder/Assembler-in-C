#include "libs.h"

/* This function recives a line node head and a node that needs to be added to the lineked list and adds the node at the end of the linked list*/
void addLineNode(line_node *head, line_node *node){
    line_node *runner = head;
    while (runner->next != NULL)
    {
        runner = runner->next;
    }
    
    runner->next = node;
    node->next = NULL; 
}

/* This function recives a line node head and an IC value and finds the line node with this IC value*/
line_node *getByIC(line_node *head, int IC){
    line_node *runner = head->next;
    while (runner != NULL)
    {
        if(runner->IC == IC){
            return runner;
        }
        runner = runner->next;
    }
    return NULL;
}
