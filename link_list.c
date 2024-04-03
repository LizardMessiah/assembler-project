#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "assembler.h"
#include "link_list.h"


node *create_node(const char *name, int address, int is_external, int is_entry, int define){
    /*this function creates a new node initialized with the input parameters*/
    node *new_node = (node*)malloc(sizeof(node));
    if (new_node == NULL) {
        ALLOCATION_ERROR;
        return NULL;
    }
    new_node->label_name = malloc_copy_string(name);
    new_node->address = address;
    new_node->external = is_external;
    new_node->entry = is_entry;
    new_node->is_defined = define;
    new_node->next = NULL;

    return new_node;  

}

void define_node(node *defined_node){
    /*this function changed the definition status of a certain label to 1(meaning it is defined)*/
    defined_node->is_defined = 1;
}

void make_entry_label(node *entry_label){
    /*this function changed the entry status of a certain label to 1(meaning it is an .entry symbol)*/
    entry_label->entry = 1;
}

void make_extern_label(node *extern_label){
    /*this function changed the extern status of a certain label to 1(meaning it is .extern)*/    
    extern_label->external = 1;
}

int update_address(node *node_to_update, int new_address){
    /*this function changes the current node's address to the input integer value*/    
    node_to_update->address = new_address; 
    return 1;
}
    
int insert_node(node **head, const char *name, int address, int is_external, int is_entry, int is_defined) {
    /*this function creates a new node initialized with the input parameters and inserts it into the list*/
    node *current = NULL; 
    node *new_node = create_node(name, address, is_external, is_entry, is_defined);
    if (new_node == NULL) {
        return 0; 
    }
    if (*head == NULL) {
        *head = new_node;
        return 1;
    }
    current = *head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new_node;
    return 1;
}


void print_list(node *head) {
/** this function prints a linked list given its head as a parameter  */
    node *current = head;

    while (current != NULL) {
        printf("label_name: %s, at address: %d\n", current->label_name, current->address);
        current = current->next;
    }
}


void free_list(node *head) {
    
    node *temp = head;
    while (head != NULL) {
        temp = head;
        head = head->next;
        if (temp->label_name != NULL) {
            free(temp->label_name);
        }
        free(temp);
    }
}

node *find_node_by_string(node *head,char *string) {
    /*this function receives a pointer to the head of a linked list
    and a string, if the label name is the same as the name of the string
    it returns the node, if no such node is found it returns null*/
    node *current = head;
    
    while (current != NULL) {
        if (strcmp(current->label_name, string) == 0) {
            return current; 
        }
        current = current->next;
    } 
    
    return NULL; 
}
