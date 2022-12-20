enum DATANODETYPE{intVal, charVal};

typedef struct data_node_t {
    int val;
    char cval;
    int type;

    struct data_node_t * next;
} data_node;

extern void addDataNode(data_node *head, data_node *node);
extern data_node *getData_nodeByIndex(data_node *head, int index);
