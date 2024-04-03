#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "assembler.h"
#include "tables.h"
#include "first_pass.h"
#include "statement.h"

macro_list *create_macro_table(){
    
    macro_list *macro_table = (macro_list *)malloc(sizeof(macro_list));
    if(macro_table == NULL){
        ALLOCATION_ERROR;
        return NULL;
    }
    
    macro_table -> macro_name = NULL;
    macro_table -> macro_definition = NULL;
    macro_table -> next = NULL;
    
    return macro_table;
}

macro_list *create_macro(macro_list *head,char *macro_name){
    /*creates a new macro node and adds it to the macro table. 
    returns pointer to new entry */
    macro_list *new_node = (macro_list *)malloc(sizeof(macro_list));
    char *name_copy = (char *)malloc(strlen(macro_name) + 1);
    name_copy = malloc_copy_string(macro_name);
    if(new_node == NULL){
        ALLOCATION_ERROR;
        return NULL;
    }
    
    if(is_not_reserved(name_copy)==0){
        /*macro name is a reserved keyword in the language*/
        RESERVED_KEYWORD_ERROR;
        return NULL;
    }
    new_node -> macro_name = (char *)malloc(strlen(name_copy)+1);
    if((new_node->macro_name) == NULL){
        ALLOCATION_ERROR;
        free(new_node);
        return NULL;
    }
    new_node-> macro_name = name_copy;
    new_node -> macro_definition = NULL;
    new_node -> next = NULL;
    
    add_to_table(head, new_node);
    
    return new_node;

}

int update_name(macro_list *macro_node,char *name){
    /*this function updates the name of an input node*/
    macro_node -> macro_name = (char *)malloc(strlen(name)+1);
    macro_node -> macro_name = malloc_copy_string(name);
    if( (macro_node->macro_name) == NULL ){
        ALLOCATION_ERROR;
        return 0;
    }
    strcpy(macro_node->macro_name,name);
    return 1;
}


int update_definition(macro_list *macro_node, char *definition){
    /*this function appends to definition of an input node*/
    int current_length;
    int new_length;
    
    if((macro_node->macro_definition) == NULL){
        macro_node->macro_definition = (char *)malloc(strlen(definition)+1);
        if((macro_node->macro_definition) == NULL ){
            ALLOCATION_ERROR;
            return 0;
        }
        trim_whitespace(definition);
        strcpy(macro_node->macro_definition, definition);
        return 1;
    }
    
    current_length = strlen(macro_node->macro_definition)+1;
    new_length = current_length + strlen(definition)+1;

    macro_node->macro_definition = (char *)realloc(macro_node->macro_definition, new_length);
    if((macro_node->macro_definition) == NULL){
        ALLOCATION_ERROR;
        return 0;
    }
    trim_whitespace(definition);
    strcat(macro_node->macro_definition, definition);

    return 1;
}




int define_macro(macro_list *entry, char *definition){
    if(entry == NULL){
        return 0;
    }   
    /*receive a macro entry that has only a name and allocate memory and implement its definition*/

    if((entry->macro_definition) == NULL){

        /*if macro has not been defined yet allocate memory and copy the definition*/

        entry->macro_definition = (char *)malloc(sizeof(strlen(definition))+1);
        if((entry->macro_definition) == NULL){
            ALLOCATION_ERROR;
            return 0;
        }
        strcpy(entry->macro_definition, definition);
    }
    entry->macro_definition = (char *)realloc(entry->macro_definition,sizeof(strlen(entry->macro_definition)+1+strlen(definition)+1));
    strcpy(entry->macro_definition,(malloc_strcat(entry->macro_definition,definition)));
    
    /*allocation successful*/
    return 1;
}


int add_to_table(macro_list *head ,macro_list *new_entry){
    /*this function adds to an input node to the linked list*/

    macro_list *temp;

    if(head == NULL){
        /*if macro table is empty, append new entry to head*/
        head = new_entry;
        return 1;
    }

    temp = head -> next;
    head -> next = new_entry;
    new_entry -> next = temp;
    
    return 1;
}

macro_list *create_macro_node(char *name){
    /* creates a node with name of the macro only, definition will be appended later*/
    macro_list *new_node = (macro_list *)malloc(sizeof(macro_list));
    char *name_copy = malloc_copy_string(name);
    if(new_node == NULL){
        ALLOCATION_ERROR;
        return NULL;
    }

    new_node -> macro_name = name_copy;
    new_node -> macro_definition = NULL;
    new_node -> next = NULL;
    
    return new_node;
}

macro_list *find_by_name(macro_list *head,char *name){
    /*this function looks for a node with a name field that is similar to the input name variable, 
    if it exists it returns it, otherwise returns null*/    
    macro_list *temp = head;
    while(temp!= NULL){
        if(temp->macro_name!= NULL){
            if(strcmp(temp->macro_name,name)==0){
                return temp;
            }
        }
        temp = temp->next;
    }
    /*default return value for non existent value*/
    return NULL;
}

void delete_node(macro_list *macro_node){
    /*deletes an entry from the list, given a pointer to that entry*/

    free(macro_node -> macro_name);
    free(macro_node->macro_definition);
    free(macro_node);
}

void delete_table(macro_list *head){
    /*frees the memory allocated to the entire macro table*/
    macro_list *temp;

    while(head != NULL){
        temp = head;
        head = head -> next;
        delete_node(temp);
    }

}

int print_table(macro_list *head) {
    /*function that prints linked list*/
    macro_list *current; 
    if(head-> next == NULL){
        printf("empty list\n");
        return 1;    
    }
    current = head->next;
    
    printf("Macro Table:\n");
    while (current != NULL) {
        printf("Macro Name: %s, Macro Definition: %s\n", current->macro_name, current->macro_definition);
        current = current->next;
    }
    printf("-------------------\n");
    return 1;
}

