
/*  labels.h*/
/*  Itay*/

/* This enum saves the types of labels*/
enum labelTypeEnum{code, data, external};

/* This struct represents a label node in the labels linked list. It saves the name of the label and the address where it is found. it also saves the type of the label and if it's an entry label*/
typedef struct label
{
    /*Label of the symbol*/
    char * name;
    /*Address of the symbol*/
    int address;
    
    int labelType;
    
    int is_entry;
        
  /*pointer to next*/
  struct label * next;
} label;

extern void add_label_node(label *head, label *node);
extern label* get_label_by_name(label *head, char* name);
extern void add_label_by_name_and_address(label* head, char* name, int address, int labelType);
extern void add_ICF_to_data_labels(label* head, int ICF);

#ifndef labels_h
#define labels_h


#endif /* labels_h */
