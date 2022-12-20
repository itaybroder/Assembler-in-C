
/*  second_pass.c*/
/*  Itay*/


#include "libs.h"

void fill_in_operand(line_node* current_line, line_node* operand_node, params* p, char* label_name, int operand_type);
void second_pass_handle_line(char line[], params *p);


/* This function recives the input assembly file and starts the second pass*/
void read_lines_for_second_pass(FILE *assemblyFile, params *p){
        char line[MAX_STRING_LENGTH];
        /*Reading input untill stop or EOF*/
        while (fgets(line, MAX_STRING_LENGTH, assemblyFile) != 0){
            second_pass_handle_line(line, p);
        }
}

/* This function handels all of the operations needed to called in the second pass. once this function is finishd then the program moves on to the next line that needs to be parsed. It checks what the line type is and if there are any operands that need to be filled with the label address from the first pass*/
void second_pass_handle_line(char line[], params *p){
    int len; /* the number of words in the line*/
    char** words; /* the words in the line*/
    line_node *current_line; /* the current line node pointer*/
    int location; /* the location of a word in the words array*/
    label* res; /* the lable node pointer of a lable found*/
    p->input_file_line++;
    words = split_line(line, &len);
    p->current_line_has_label = false;
    /* This is an empty line*/
    if(len == 0){
        return;
    }

    if(is_comment(words[0])){
        return;
    }


    if(has_label(words[0])){
        p->current_line_has_label = true;
    }

    if(is_data_line(words, len)){
        return;
    }

    /* If this is an entry line then the label is */
    if(is_entry_line(words, len)){
        if(len <=1 || len >= 4){
            printf("ERROR in file %s in line %d: invalid number of operands in entry line\n", p->current_file_name, p->input_file_line);
            p->errors_found = true;
            return;
        }
            
        location = 1;
        if(p->current_line_has_label)
            location = 2;
        res = get_label_by_name(p->label_head, words[location]);
        if(res == NULL){
            printf("ERROR in file %s in line %d: entry without label\n", p->current_file_name, p->input_file_line);
            p->errors_found = true;
            return;
        } else{
            res->is_entry = true;
        }
        return;
    }
    if(is_extern_line(words, len)){
        return;
    }


    /* this is a valid line*/
    current_line = getByIC(p->line_node_head, p->IC);
    if (current_line->num_of_operands >= 1) {
        fill_in_operand(current_line, current_line->next, p, current_line->first_operand_name, current_line->first_operand_type);
    }
    if (current_line->num_of_operands == 2) {
        fill_in_operand(current_line, current_line->next->next, p, current_line->second_operand_name, current_line->second_operand_type);
    }
    
    p->IC += (current_line->num_of_operands + current_line->num_of_immidiates + 1);
    

    
}

void fill_in_operand(line_node* current_line, line_node* operand_node, params* p, char* label_name, int operand_type){
    int line_code; /* the final bit representation */
    label* res; /* the lable node pointer of the found lable*/
    res = get_label_by_name(p->label_head, label_name);
    if(res == NULL){
        printf("ERROR in file %s in line %d: label not foud\n", p->current_file_name, p->input_file_line);
        p->errors_found = true;
        return;
    }
    if(res->labelType == external){
        add_label_by_name_and_address(p->externals, label_name, operand_node->IC, 0);
    }
    line_code = 0x000000; /* 0b000000000000000000000000 */
    if (res->labelType == external){
        line_code += E_BIT;
    }
    else if (operand_type == 1){
        line_code += R_BIT;
        line_code += (res->address<<3);
    }
    else if (operand_type == 2){
        line_code += A_BIT;
        line_code += ((res->address - current_line->IC) << 3);
    }
    
    operand_node->code = line_code;
}
