/* This struct represents one line of code in the final code linked list. It saves the IC of the line and the hex code that is eventually saved to the file. It also saves the operands of the command if its the first line in the command. this is done to make the second pass easier.*/
typedef struct node_struct {
    int code;
    int IC;
    int label; /* flag that signals that this line has a flag*/

    int num_of_operands; /* nubmer of operands in the cammand */
    char* first_operand_name;
    int first_operand_type;
    char* second_operand_name;
    int second_operand_type;
    int num_of_immidiates; /* how many immidiate operands does this command have*/
    int command_index; /* the index of the command in the command name array*/
    
    struct node_struct * next;
} line_node;

extern void addLineNode(line_node *head, line_node *node);
extern line_node *getByIC(line_node *head, int IC);
