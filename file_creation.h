
/*  file_creation.h*/
/*  Itay*/

#ifndef file_creation_h
#define file_creation_h

#include <stdio.h>
extern void create_ob_file(params* p, char* fileName);
extern void create_ext_file(params* p, char* fileName);
extern void create_ent_file(params* p, char* fileName);
extern int has_ent_label(params* p);
#endif /* file_creation_h */
