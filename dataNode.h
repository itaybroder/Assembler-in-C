typedef struct data_node_t {
    int val;
    char *cval;
    int type;

    struct data_node_t * next;
} data_node;
