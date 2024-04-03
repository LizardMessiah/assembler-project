#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include "assembler.h"
#include "link_list.h"
#include "commands.h"
#include "statement.h"

int is_label(char *string);
int is_command(char *string);
int is_directive(char *string);
int is_empty_statement(char *line);
int is_valid_directive(char *string);
int is_alpha_num(char *string);
int is_not_reserved(char *string);
int is_valid_label_definition(statement curr_statement, node *head);
int is_valid_label(char *string);
int label_handler(statement curr_statement, node **head,int address,int is_external, int is_entry);
int operand_label_handler(statement curr_statement, node **head, int address, int is_external, int is_entry);
int first_pass(char *filename, node **head,int *instruction_image_index, int *data_image_index);

#endif