/* This program loads an assembly file and calculates the resulting machnie code and external lables and entry lables
 It does so by itterating over all of the input files and saving the resulting files in each of the input files folders.
 each line is handled in the handel line function and then saved to a linked list that saves all of the resulting lines to be written*/


#include "libs.h"
/*-------function prototype------*/
void handle_assembly_files(char[]);
void read_lines_for_first_pass(FILE*, params *p);
void handle_line(char[], params*);
void save_data(char** words, int len, params *p);
void add_label(params *p, char *word, int label_type);
void handle_code_line(char** words, int len, params* p, line_node* current_line);
void handle_operand(char* op, int location, int* line_code, int group, params* p, line_node* current_line);
int is_register(char* op);
int is_immidiate(char* op);
int is_relative(char* op);
int validate_operand_type(int command_index, int operand_type, int location);
/*-------------------------------*/

/* In the beggining of the program This function is called to initialize all of the parameter is the params struct. There is a deeper explination of the params struct in params.h */
params *init_params(char* fileName){
    params *p = (params *)malloc(sizeof(params));
    p->IC = 100;
    p->DC = 0;
    p->line_node_head = (line_node *)malloc(sizeof(line_node));
    p->data_node_head = (data_node *)malloc(sizeof(data_node));
    p->line_node_head->next = NULL; 
    p->data_node_head->next = NULL;
    p->label_head = (label *)malloc(sizeof(label));
    p->label_head->next = NULL;
    
    p->externals = (label *)malloc(sizeof(label));
    p->externals->next = NULL;
    
    p->errors_found = false;
    p->current_line_has_label = false;
    p->input_file_line = 0;
    p->current_file_name = fileName;
    return p;
}

/*This fucntion gets a file name from the main method and open the file and send it to the read_assembly_line() fucntion which deals with the file content*/
void handle_assembly_files(char fileName[]){
    /*initialize the input stream to be the assembly file*/
    FILE *inputFile;
    params *p; /* This represents the parameters the will be passed on to almost all of the functions*/
	
    char* file_name_with_extention = (char*)malloc(sizeof(char)*MAX_STRING_LENGTH);
    strcpy(file_name_with_extention, fileName);
    strcat(file_name_with_extention, ASSEMBLY_EXTENTION);
    
    
    p = init_params(file_name_with_extention);

    inputFile = fopen(file_name_with_extention, "r");
    if(inputFile == NULL)
    {
        fprintf(stdout, "Error opening file\n");
        p->errors_found = true;
        exit(1);
    }
    read_lines_for_first_pass(inputFile, p);
    
	/*Closing the files*/
    fclose(inputFile);
    
    if(p->errors_found == true){
        return;
    }
    
    inputFile = fopen(file_name_with_extention, "r");
    if(inputFile == NULL)
    {
        fprintf(stdout, "Error opening file\n");
        p->errors_found = true;
        exit(1);
    }
    p->IC = 100;
    p->DC = 0;
    p->input_file_line = 0;
    read_lines_for_second_pass(inputFile, p);
    /*read_lines_for_f_pass(inputFile, p);*/
    
     /*Closing the files*/
    fclose(inputFile);
    if(p->errors_found == true){
        return;
    }
    create_ob_file(p, fileName);
    create_ext_file(p, fileName);
    create_ent_file(p, fileName);
}

/*This function gets an assembey file pointer and loop over the content of the file*/
void read_lines_for_first_pass(FILE *assemblyFile, params *p){
    char line[MAX_STRING_LENGTH];
    /*Reading input untill stop or EOF*/
    while (fgets(line, MAX_STRING_LENGTH, assemblyFile) != 0){
        handle_line(line, p);
    }
    p->ICF = p->IC;
    p->DCF = p->DC;
    add_ICF_to_data_labels(p->label_head, p->ICF);
}

/* This function recives a label that needs to be saved with the label type and checks if it is already in the labels linked list and if so return an error (unless it is a external label) */
void add_label(params *p, char *word, int label_type) {
    char* label_name; /* The lable name after removing the ':'*/
    label* res; /* The lable pointer if found*/
    int val; /* The value of the address of the lable*/
    int sub_from_end = 1; /* The number of chars to go back from the end when splitting the label name*/
    
    if(label_type == external)
        sub_from_end = 0;
    
    label_name = substring(word, 0, (int)strlen(word)-sub_from_end);
    
    if (is_legal_label(label_name) == false) {
        p->errors_found = true;
        printf("ERROR in file %s in line %d: invalid label name\n", p->current_file_name, p->input_file_line);
        return;
    }
    
    res = get_label_by_name(p->label_head, label_name);
    /* check if the label already exists and if so raise an exception (except for when the label is extern) */
    if(res != NULL && label_type != external){
        p->errors_found = true;
        printf("Error in file %s in line %d, label already exists\n", p->current_file_name, p->input_file_line);
        return;
    }
    
    val = p->IC;
    if(label_type == data)
        val = p->DC;
    if (label_type == external) {
        val = 0;
    }
    
    
    add_label_by_name_and_address(p->label_head, label_name, val, label_type);
}


/*This function recives a line to be parsed and manages all of the functions needed to be called in order to finish parsing the line.*/
void handle_line(char line[], params *p){
    int len; /* the number of the words in the line*/
    line_node *current_line; /* the cuurent line node pointer*/
    char** words = split_line(line, &len); /* all of the words in the line*/
    p->input_file_line++;

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
        save_data(words, len, p);
        return;
    }
    
    if(is_extern_line(words, len)){
        if(p->current_line_has_label){
            printf("WARNING in line %d: extern shouldn't have a label before it\n", p->input_file_line);
            add_label(p, words[2], external);
        }
        else
            add_label(p, words[1], external);
        return;
    }
    
    if(is_entry_line(words, len)){
        if(p->current_line_has_label){
            printf("WARNING in line %d: entry shouldn't have a label before it\n", p->input_file_line);
        }
        return;
    }
    
    /*This is a code line*/
    if (p->current_line_has_label) {
        add_label(p, words[0], code);
    }
    
    /* Here it can be assumed this is a code line because every other possibility has been checked*/
    
    /* creating the line node that will save all of the relevant information about the line*/
    current_line = (line_node *)malloc(sizeof(line_node));
    current_line->IC = p->IC;
    current_line->num_of_operands = 0;
    current_line->num_of_immidiates = 0;
    p->IC++;
    current_line->first_operand_name = NULL;
    current_line->second_operand_name = NULL;
    addLineNode(p->line_node_head, current_line);
    
    handle_code_line(words, len, p, current_line);
    
    
    
}

/* This function recives the words of the line in an array and if this function is called it means the line is a code line that needs to be parsed. because if it is a data line or a comment or an entry line then the above function (handle line) would finish and then wouldnt call this function.*/
void handle_code_line(char** words, int len, params* p, line_node* current_line){
    int i;
    int op_location = 0; /* the index of the operand in the words array that represents the current line from the input file*/
    int op_code_index; /* the index of the command in the commands array from params.h*/
    int line_code; /* the final bit representation of the line that will be written*/
    if(p->current_line_has_label == true){
        op_location = 1;
    }
    op_code_index = -1;
    for(i = 0; i < OP_CODES_LEN; i++){
        if(strcmp(words[op_location], op_names[i]) == 0){
            op_code_index = i;
            break;
        }
    }
    if(op_code_index == -1){
        p->errors_found = true;
        printf("ERROR in file %s in line %d: Invalid command name\n", p->current_file_name, p->input_file_line);
        return;
    }
    current_line->command_index = op_code_index;
    line_code = 0x000000; /* 0b000000000000000000000000 */
    line_code += op_codes[op_code_index];
    line_code += funct[op_code_index];
    line_code += A_BIT;
    if(groups[op_code_index] == 1){
        if ((p->current_line_has_label && len != 4) || (p->current_line_has_label==false && len != 3)) {
            printf("ERROR in file %s in line %d: invalid amout of openrands, should be 2\n", p->current_file_name, p->input_file_line);
            p->errors_found = true;
            return;
        }
        handle_operand(words[op_location+1], 0, &line_code, groups[op_code_index], p, current_line);
        handle_operand(words[op_location+2], 1, &line_code, groups[op_code_index], p, current_line);
    }
    else if (groups[op_code_index] == 2){
        if ((p->current_line_has_label && len != 3) || (p->current_line_has_label==false && len != 2)) {
            printf("ERROR in file %s in line %d: invalid amout of openrands, should be 1\n", p->current_file_name, p->input_file_line);
            p->errors_found = true;
            return;
        }
        handle_operand(words[op_location+1], 1, &line_code, groups[op_code_index], p, current_line);
    }
    else if(groups[op_code_index] == 3){
        if ((p->current_line_has_label && len != 2) || (p->current_line_has_label==false && len != 1)) {
            printf("ERROR in file %s in line %d: invalid amout of openrands, should be 0\n", p->current_file_name, p->input_file_line);
            p->errors_found = true;
            return;
        }
    }

    current_line->code = line_code;
    
    return;
}


/* This function recives an operand and its location in the line (meaning if it's a source operand or a detination operand), it also recives the code (meaning the final word in bits that needs to be written) and also the group of the command wich the operand is related to.  */
void handle_operand(char* op, int location, int* line_code, int group, params* p, line_node* current_line){
    int immidiate_value; /* the value of the immidiate operand*/
    line_node* immidiate_operand_line; /* the pointer to the line node of the operand*/
    int sum;
    if(is_register(op)){
        int register_number = atoi(&op[1]);
        if (register_number > 7 || register_number < 0) {
            p->errors_found = true;
            printf("ERROR in file %s in line %d: Invalid register number, register number should be btween 0 and 7\n", p->current_file_name, p->input_file_line);
            return;
        }
        if(validate_operand_type(current_line->command_index, reg, location) == 0){
            printf("ERROR in file %s in line %d: invalid oprand type\n", p->current_file_name, p->input_file_line);
            p->errors_found = true;
            return;
        }
        *line_code += register_codes[location][register_number];
        
        return;
    }
    
    if(is_immidiate(op)){
        char* number_string = substring(op, 1, (int)strlen(op));
        if (is_legal_number(number_string) == false) {
            printf("ERROR in file %s, in line %d: immidiate value, number must contain only digits and possibly a sign symbol in the beggining\n", p->current_file_name, p->input_file_line);
            p->errors_found = true;
            return;
        }
        if(validate_operand_type(current_line->command_index, immidiate, location) == 0){
            printf("ERROR in file %s in line %d: invalid oprand type\n", p->current_file_name, p->input_file_line);
            p->errors_found = true;
            return;
        }
        immidiate_value = atoi(number_string);
        immidiate_operand_line = (line_node *)malloc(sizeof(line_node));
        immidiate_operand_line->IC = p->IC;
        immidiate_operand_line->first_operand_name = NULL;
        immidiate_operand_line->second_operand_name = NULL;
        p->IC++;
        immidiate_operand_line->code = 0x000000; /* 0b000000000000000000000000*/
        immidiate_operand_line->code += A_BIT;
        immidiate_value <<= 3;
        sum = immidiate_operand_line->code + immidiate_value; /* create the final bit line by adding the bit representation of each component*/
        immidiate_operand_line->code = sum;
        addLineNode(p->line_node_head, immidiate_operand_line);
        current_line->num_of_immidiates++;
        return;
    }
    
    immidiate_operand_line = (line_node *)malloc(sizeof(line_node));
    immidiate_operand_line->IC = p->IC;
    p->IC++;
    immidiate_operand_line->code = -1;
    addLineNode(p->line_node_head, immidiate_operand_line);
    
    
    /* Copying the operand name to the current line node for the second pass*/
    current_line->num_of_operands+= 1;
    
    /* This is relative addressing*/
    if(is_relative(op)){
        if(validate_operand_type(current_line->command_index, relative, location) == 0){
            printf("ERROR in file %s in line %d: invalid oprand type\n", p->current_file_name, p->input_file_line);
            p->errors_found = true;
            return;
        }
        *line_code += relative_operand_codes[location]; /* setting the addressing method*/
        if (current_line->num_of_operands == 1) {
            current_line->first_operand_type = 2;
        } else{
            current_line->second_operand_type = 2;
        }
    }
    /* This is a label (Direct)*/
    else{
        if(validate_operand_type(current_line->command_index, direct, location) == 0){
            printf("ERROR in file %s in line %d: invalid oprand type\n", p->current_file_name, p->input_file_line);
            p->errors_found = true;
            return;
        }
        *line_code += direct_operand_codes[location]; /* setting the addressing method*/
        if (current_line->num_of_operands == 1) {
            current_line->first_operand_type = direct;
        } else{
            current_line->second_operand_type = direct;
        }
    }
    
    /* add the operand name to the current line node*/
    if (current_line->num_of_operands == 1) {
        if(current_line->first_operand_type == direct){
            current_line->first_operand_name = (char*) malloc(sizeof(char)*strlen(op));
            strcpy(current_line->first_operand_name, op);
        }
        else if(current_line->first_operand_type == relative){
            current_line->first_operand_name = (char*) malloc(sizeof(char)*(strlen(op) - 1));
            strcpy(current_line->first_operand_name, substring(op, 1, (int)strlen(op)));
        }
    }
    
    else{
        if(current_line->second_operand_type == direct){
            current_line->second_operand_name = (char*) malloc(sizeof(char)*strlen(op));
            strcpy(current_line->second_operand_name, op);
        }
        else if(current_line->second_operand_type == relative){
            current_line->second_operand_name = (char*) malloc(sizeof(char)*(strlen(op) - 1));
            strcpy(current_line->second_operand_name, substring(op, 1, (int)strlen(op)));
        }
    }
    return;
    
}
/* This function revices an operand type, an operands location (meaning if its source then the location is 0 and if its a destination operand then it's location is 1) and the command's index. It then gets the legal operand types from params.h array which saved the legal operand types for each command and the location of the operand in the command. if there is a match between the operand type and the legal operand types then the result of the bitwise and wouldnt be 0 and if there isn't a mathc the result would be one.*/
int validate_operand_type(int command_index, int operand_type, int location){
    return legal_operand_types[location][command_index] & (operand_to_binary_flag[operand_type]);
}

/* This function revices a label and checks if the addressing type of the label is relative*/
int is_relative(char* op){
    if (strlen(op) > 1 && op[0] == '&') {
        return true;
    }
    return false;
}

/* This function revices an operand and checks if the addressing type of the opperand is immidiate*/
int is_immidiate(char* op){
    if(strlen(op) > 0 && op[0]== '#'){
        return true;
    }
    return false;
}

/* This function revices an operand and checks if the operand is a register*/
int is_register(char* op){
    if(strlen(op)==2 && op[0] == 'r'){
        return true;
    }
    return false;
}



/* This funciton recives a line broken up to it's words saves in the words array and the length of the array. This funcion is called when it is determied the line type is a data line and all of the information in it needs to be saved to the data linked list*/
void save_data(char** words, int len, params *p){
    int datatype_index = 0;
    int i;
    data_node* d;
    if(p->current_line_has_label){
        add_label(p, words[0], data);
        datatype_index = 1;
    }
    if(strcmp(words[datatype_index], DATA_label_PREFIX)==0){
        for(i =datatype_index+1; i < len; i++){
            data_node* d = (data_node*) malloc(sizeof(data_node));
            d->type = intVal;
            d->val = atoi(words[i]);
            addDataNode(p->data_node_head, d);
            p->DC++;
        }
    }
    else if(strcmp(words[datatype_index], STRING_label_PREFIX)==0){
        for(i = 1; i < strlen(words[datatype_index+1])-1; i++){
            data_node* d = (data_node*) malloc(sizeof(data_node));
            d->type = charVal;
            d->cval = words[datatype_index+1][i];
            addDataNode(p->data_node_head, d);
            p->DC++;
        }
        d = (data_node*) malloc(sizeof(data_node));
        d->type = charVal;
        d->cval = '\0';
        addDataNode(p->data_node_head, d);
        p->DC++;
        
    }
    
    
}


int main(int argc, char *argv[]){
    /*Looping through the assembly files*/
    int i;
    for(i = 1; i<argc; i++){
        handle_assembly_files(argv[i]);
    }
    return 0;
}


