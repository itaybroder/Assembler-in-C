
/*  file_creation.c*/
/*  Itay*/
#include "libs.h"

/* This function is called at the end of the program and creates the object file result if there are any lines to be written*/
void create_ob_file(params* p, char* fileName){
    char* file_name_with_extention; /* the file name when adding the correct extetion*/
    FILE * fp;
    line_node* line_runner; /* A runner pointer to itterate over the linked list*/
    int line; /* the line nubmer to be printed*/
    char* line_number; /* The line number as a string after adding zeros in front of the number*/
    char* code;/* the hex representation of the code to be written to the file*/
    data_node* data_runner; /* a pointer runner for the data linked list to itterate over the */
    if (p->line_node_head->next == NULL) {
        return;
    }
    file_name_with_extention = (char*)malloc(sizeof(char)*MAX_STRING_LENGTH);
    strcpy(file_name_with_extention, fileName);
    strcat(file_name_with_extention, OBJECT_EXTENTION);
    /* open the file for writing*/
    fp = fopen(file_name_with_extention, "w");
    

    fprintf(fp, "%d %d\n", p->ICF-100, p->DCF);
    line_runner = p->line_node_head->next;
    line = line_runner->IC;
    while (line_runner != NULL){
        line = line_runner->IC;
        line_number = get_full_index(line);
        code = get_hex_from_binary(get_2s_compliment(line_runner->code));
        fprintf(fp, "%s %s\n", line_number, code);
        line_runner = line_runner->next;
    }
    line++;
    data_runner = p->data_node_head->next;
    while (data_runner != NULL){
        line_number = get_full_index(line);
        if (data_runner->type == charVal)
            code = get_hex_from_binary(get_2s_compliment(data_runner->cval));
        else
            code = get_hex_from_binary(get_2s_compliment(data_runner->val));
        fprintf(fp, "%s %s\n", line_number, code);
        data_runner = data_runner->next;
        line++;
    }
    /* close the file*/
    fclose (fp);
    return;
}

/* This function is called at the end of the program and creates the external file result if there are any lines to be written*/
void create_ext_file(params* p, char* fileName){
    char* file_name_with_extention;
    FILE * fp;
    label* runner;
    char* line_number;
    if (p->externals->next == NULL) {
        return;
    }
    file_name_with_extention = (char*)malloc(sizeof(char)*MAX_STRING_LENGTH);
    strcpy(file_name_with_extention, fileName);
    strcat(file_name_with_extention, EXTERNAL_EXTENTION);
    
    /* open the file for writing*/
    fp = fopen (file_name_with_extention,"w");
    runner = p->externals->next;
    while(runner != NULL){
        line_number = get_full_index(runner->address);
        fprintf(fp, "%s %s\n", runner->name, line_number);
        runner = runner->next;
    }
    /* close the file*/
    fclose (fp);
    return;
}

/* This function is called at the end of the program and creates the entry file result if there are any lines to be written*/
void create_ent_file(params* p, char* fileName){
    char* file_name_with_extention;
    FILE * fp;
    label* runner;
    char* line_number;
    if (has_ent_label(p) == false) {
        return;
    }
    file_name_with_extention = (char*)malloc(sizeof(char)*MAX_STRING_LENGTH);
    strcpy(file_name_with_extention, fileName);
    strcat(file_name_with_extention, ENTITY_EXTENTION);
    
    /* open the file for writing*/
    fp = fopen (file_name_with_extention,"w");
    
    runner = p->label_head->next;
    while(runner != NULL){
        if (runner->is_entry) {
            line_number = get_full_index(runner->address);
            fprintf(fp, "%s %s\n", runner->name, line_number);
        }
        
        runner = runner->next;
    }
    fclose (fp);
    return;
}

/* This function checks if there is an entry lable in the lables linked list*/
int has_ent_label(params* p){
    label* runner;
    runner = p->label_head->next;
    while(runner != NULL){
        if (runner->is_entry) {
            return true;
        }
        runner = runner->next;
    }
    return false;
}
