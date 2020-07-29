#include "main.h"
#include "dict.h"
#include "params.h"

/*-------function prototype------*/
void handle_assembly_files(char[]);
void read_assambely_lines(FILE*, dict_t**, params *p);
void handle_line(char[], dict_t **, int);
char **split_line(char line[]);
/*-------------------------------*/

/*This fucntion gets a file name from the main method and open the file and send it to the read_assembly_line() fucntion which deals with the file content*/
void handle_assembly_files(char fileName[]){
    /*initialize the input stream to be the assembly file*/
    FILE *inputFile;
	
    dict_t **dict = dictAlloc();
    params *p;
    p->IC = 100;

    inputFile = fopen(fileName, "r");
    if(inputFile == NULL)
    {
        fprintf(stdout, "Error opening file\n");
        exit(1);
    }

    read_assambely_lines(inputFile, dict, p);

    int line_number = (int)getItem(*dict, "END");
	printf("%d", line_number);

	/*Closing the files*/
    fclose(inputFile);
	
}
char *substring(char *string, int start, int length)
{
    char* substr = malloc(length+1);
    strncpy(substr, string+start, length);
    substr[length] = '\0';
    return substr;
}

/*This function adds to the dict*/
void extract_label(char line[], dict_t **dict, int line_number){
    char **words;
    words = split_line(line);

    if(words[0][strlen(words[0])-1] == ':'){
        addItem(dict, substring(words[0], 0, strlen(words[0])-1), line_number);
    
    }
}

/*This function gets an assembey file pointer and loop over the content of the file*/
void read_assambely_lines(FILE *assemblyFile, dict_t **dict, params *p){
    char line[MAX_STRING_LENGTH];
    int line_number=1;
    /*Reading input untill stop or EOF*/
    while (fgets(line, MAX_STRING_LENGTH, assemblyFile) != 0){
        handle_line(line, dict, line_number);
        line_number++;
    }

    printf("%d", p->IC);

}

/*this method will execute a singal line each time from the input assembly file*/
void handle_line(char line[], dict_t **dict, int line_number){  
    extract_label(line, dict, line_number);
}

char **split_line(char line[]){
    char **newString = (char **)malloc(sizeof(char *) * 10); 
    
    int i,j,ctr;
 
    j=0; ctr=0;
     newString[0] = (char*)malloc(10*sizeof(char));
    for(i=0;i<=(strlen(line));i++)
    {
         /**if space or NULL found, assign NULL into newString[ctr]**/
        if(line[i]==' '||line[i]=='\0')
        {
            
            newString[ctr][j]='\0';
            ctr++;  /**or next word**/
            j=0;    /**for next word, init index to 0**/
            newString[ctr] = (char*)malloc(10*sizeof(char));
        }
        else
        {
           
            newString[ctr][j]=line[i];
            j++;
        }
    }
    
    return newString;
}

char **splittt(){
    char **newString = (char **)malloc(100 *sizeof(char)); 

    return newString;

}
int main(int argc, char *argv[]){
    /*Looping through the assembly files*/
    int i;
    for(i = 1; i<argc; i++){
        handle_assembly_files(argv[i]);
    }
    return 0;
}


