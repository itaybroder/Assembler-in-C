
/*  string_operations.h*/
/*  Itay*/




extern char **split_line(char line[], int* len);
extern char *substring(char *string, int start, int length);
extern int is_comment(char* word);
extern int has_label(char* word);
extern int is_data_line(char** words, int len);
extern int is_entry_line(char** words, int len);
extern int is_extern_line(char** words, int len);
extern char* get_2s_compliment(int data);
extern char* get_hex_from_binary(char* bin);
extern char get_single_hex(char* bin);
extern char* get_full_index(int line);
extern int is_legal_label(char* label);
extern int is_legal_number(char* number);
#ifndef string_operations_h
#define string_operations_h

#include <stdio.h>

#endif /* string_operations_h */
