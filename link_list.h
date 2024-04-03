#ifndef LINKED_LIST_H
#define LINKED_LIST_H

/**
 * linked list data structure to hold information about current symbol
 * label name = current symbol's name
 * address = current symbol's address
 * external = current symbol .extern status flag
 * entry = current symbol .entry status flag
 * is_defined = this variable serves as a flag that checks if the definition of the node is complete
 */
typedef struct node {
    char *label_name;
    int address;
    int external;
    int entry;
    int is_defined;
    struct node *next;
} node;

void make_extern_label(node *extern_label);
void make_entry_label(node *entry_label);
void define_node(node *defined_node);

int update_address(node *node_to_update,int new_address);
int insert_node(node **head, const char *name, int address, int is_external, int is_entry,int is_defined);

node *create_node(const char *name, int address, int is_external,int is_entry,int is_defined);
node *find_node_by_string(node *head,char *string);


void print_list(node* head);


void free_list(node *head);


#endif 
