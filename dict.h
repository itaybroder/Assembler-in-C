#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct dict_t_struct {
    char *key;
    void *value;
    struct dict_t_struct *next;
} dict_t;

extern void dictDealloc(dict_t **);
extern void *getItem(dict_t *, char *);
extern void delItem(dict_t **, char *);
extern void addItem(dict_t **, char *, void *);