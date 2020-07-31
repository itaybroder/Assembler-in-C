typedef struct node_struct {
    int code;
    int IC;
    int label;


    struct node_struct * next;
} line_node;

extern void addLineNode(line_node *head, line_node *node);
extern line_node *getByIndex(line_node *head, int index);