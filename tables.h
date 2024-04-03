#ifndef TABLES_H
#define TABLES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assembler.h"
#include "parser.h"

/*linked list structure to hold the contents of a macro 'table'
macro_name = name of the current macro
macro_definition = scope of lines that are declared inside the macro
next = pointer to next node in the list
*/
typedef struct macro_list{    
    char *macro_name;
    char *macro_definition;
    struct macro_list *next;
}macro_list;

int add_to_table(macro_list *head ,macro_list *new_entry);
int define_macro(macro_list *entry, char *definition);
int update_name(macro_list *node,char *name);
int update_definition(macro_list *node,char *definition);


macro_list *create_macro(macro_list *head, char *macro_name);
macro_list *create_macro_node(char *name);
macro_list *find_by_name(macro_list *head,char *name);
macro_list *create_macro_table();


void delete_node(macro_list *node);
void delete_table(macro_list *head);
int print_table(macro_list *head);

#endif 
