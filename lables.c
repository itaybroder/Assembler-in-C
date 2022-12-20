/*  labels.c*/
/*  Itay*/

#include "libs.h"

/* This function adds a label node to the labels linked list*/
void add_label_node(label *head, label *node){
    label *runner = head;
    while (runner->next != NULL)
    {
        runner = runner->next;
    }
    
    runner->next = node;
    node->next = NULL;
}

/* This functin recives all of the parameters needed to initialize a label and creates a label node and then adds it to the labels linked list*/
void add_label_by_name_and_address(label* head, char* name, int address, int labelType){
    label* new_label = (label*)malloc(sizeof(label));
    new_label->name = (char*)malloc(sizeof(char)*strlen(name)+1);
    strcpy(new_label->name, name);
    new_label->address = address;
    new_label->labelType = labelType;
    new_label->is_entry = false;
    add_label_node(head, new_label);
    
}
/* This function returns a pointer to the label with the given name in the labels linked list. if there is no such label then null is returned*/
label* get_label_by_name(label *head, char* name){
    label *runner = head->next;
    while (runner != NULL)
    {
        if(strcmp(runner->name, name) == 0){
            return runner;
        }
        runner = runner->next;
    }
    return NULL;
}

/* This function is called at the end of the first pass and adds the ICF value to all of the labels who's type is data*/
void add_ICF_to_data_labels(label* head, int ICF){
    label *runner = head->next;
    while (runner != NULL)
    {
        if(runner->labelType == data){
            runner->address += ICF;
        }
        
        runner = runner->next;
    }
    
}
