
/*  dataNode.c */
/*  Itay */

#include "libs.h"

void addDataNode(data_node *head, data_node *node){
    data_node *runner = head;
    while (runner->next != NULL)
    {
        runner = runner->next;
    }
    
    runner->next = node;
    node->next = NULL;
}

data_node *getData_nodeByIndex(data_node *head, int index){
    data_node *runner = head->next;
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
