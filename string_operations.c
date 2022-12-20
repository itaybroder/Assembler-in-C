
/*  string_operations.c*/
/*  Itay*/

#include "libs.h"

/* This function recives a line from the input file and splits it to an array by ignoring unimportant spaces and commas. It returns an array with all of the words in the line and the length of the array*/
char **split_line(char line[], int* len){
    char **newString;
    int i,j,ctr;
    newString = (char **)malloc(sizeof(char *) * 10);
    
    j=0; ctr=0;
    newString[0] = (char*)malloc(10*sizeof(char));
    for(i=0;i<=(strlen(line));i++)
    {
         /*if space or NULL found, assign NULL into newString[ctr]*/
        if((line[i]==' '||line[i]=='\0' || line[i]==',' || line[i]=='\n' || line[i]=='\t'))
        {
            if (j!=0){
                newString[ctr][j]='\0';
                ctr++;  /*or next word*/
                j=0;    /*for next word, init index to 0*/
                newString[ctr] = (char*)malloc(10*sizeof(char));
            }
        }
        else
        {
           
            newString[ctr][j]=line[i];
            j++;
        }
    }
    *len = ctr;
    return newString;
}

/* This function revices a string and start index and the length of the needed substring and returns a new string according to the specified start and length values*/
char *substring(char *string, int start, int length)
{
    char* substr;
    substr = malloc(length+1);
    strncpy(substr, string+start, length);
    substr[length] = '\0';
    return substr;
}

/* This function recives a line from the input file and checks if it is a comment line*/
int is_comment(char* word){
    if(word[0] == ';'){
        return true;
    }
    return false;
}

/* This function recives a line from the input file and checks if it has a label in the beggining of the line*/
int has_label(char* word){
    if(word[strlen(word) - 1] == ':'){
        return true;
    }
    return false;
}

/* This function recives a line from the input file and checks if it is a data saving line*/
int is_data_line(char** words, int len){
    int i;
    for(i =0; i < len; i++){
        if(strcmp(words[i], ".string")==0 || strcmp(words[i], ".data")==0){
            return true;
        }
    }
    return false;
}

/* This function recives a line from the input file and checks if it an entry line*/
int is_entry_line(char** words, int len){
    int i;
    for(i =0; i < len; i++){
        if(strcmp(words[i], ".entry")==0){
            return true;
        }
    }
    return false;
}

/* This function recives a line from the input file and checks if it an external line*/
int is_extern_line(char** words, int len){
    int i;
    for(i =0; i < len; i++){
        if(strcmp(words[i], ".extern")==0){
            return true;
        }
    }
    return false;
}

/* This function recives the code of a line and returns the bit 2s compliment representation of the number*/
char* get_2s_compliment(int data){
    int i;
    const size_t BITS = 24;
    unsigned int mask;
    char* bin_str = (char*) malloc(sizeof(char)*(24+1));
    
    for(i=0; i<BITS; i++)
    {
      mask = 1u << (BITS - 1 - i);
      bin_str[i] = (data & mask) ? '1' : '0';
    }
    bin_str[BITS] = '\0';
    return bin_str;
}

/* This function recives the 2s complimet bit representation of a number and returns the hex representation*/
char* get_hex_from_binary(char* bin){
    char* hex = (char*) malloc(sizeof(char)*(6+1));
    char* tmp;
    int i = 0;
    while(i < 6){
        tmp = substring(bin, i*4, 4);
        hex[i] = get_single_hex(tmp);
        i++;
    }
    hex[6] = '\0';
    return hex;
    
}

/* This function calculates the single hex letter of a 4 bit substring in the 2s complimets representation*/
char get_single_hex(char* bin){
    int sum = 0;
    int i;
    for(i = 0; i < 4; i++){
        sum += pow(2,3-i)*atoi(substring(bin, i, 1));
    }
    return hex_letters[sum];
}

/* This functino recives a line number and adds 0s to fill in the missing zeros in fron of the line number*/
char* get_full_index(int line){
    int i;
    char* final_snum;
    char* snum;
    snum = (char*) malloc(sizeof(char)*(7+1));
    sprintf(snum,"%d", line);
    final_snum = (char*) malloc(sizeof(char)*(7+1));
    
    for(i = 0; i < (7 - strlen(snum)); i++){
        strcat(final_snum, "0");
    }
    
    strcat(final_snum, snum);
    final_snum[7] = '\0';
    return final_snum;
}

/* This function recives a line string and checks if it is a legal lable name. meaning it is not a saved word and is made of letters and numbers*/
int is_legal_label(char* label){
    int i;
    if(strlen(label) > 31)
        return false;
    
    if((label[0] < 'a' || label[0] > 'z') && (label[0] < 'A' || label[0] > 'Z'))
        return false;
        
    for(i = 0; i < strlen(label); i++){
        if((label[i] < 'a' || label[i] > 'z') && (label[i] < 'A' || label[i] > 'Z') && (label[i] < '0' || label[i] > '9'))
            return false;
    }
    /* Check if the label is a command name*/
    for(i = 0; i < OP_CODES_LEN; i++){
        if(strcmp(label, op_names[i]) == 0){
            return false;
        }
    }
    return true;
}

/* This function recives a number from an immidiate operand and checks if it is a legal nmber*/
int is_legal_number(char* number){
    int i;
    /* There is no number*/
    if (strlen(number) == 0)
        return false;
    
    /* Is the number just a sign symbol*/
    if(strlen(number)==1 && (number[0] == '+' || number[0] == '-'))
        return false;
    
    /* Is the first char of the number legal*/
    if (number[0] != '+' && number[0] != '-' && (number[0] < '0' || number[0] > '9'))
        return false;
    
    /* is the rest of the number legal*/
    if (strlen(number) > 1) {
        for(i = 1; i < strlen(number); i++){
            if(number[i] < '0' || number[i] > '9')
                return false;
        }
    }
    return true;
}

