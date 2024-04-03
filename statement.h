#ifndef STATEMENT_H
#define STATEMENT_H

#include "commands.h"
#include "link_list.h"

/*data structure that holds information about a line of code according to the syntax of the language
label = name of current label, if it exists
command = name of current opcode
directive = name of current directive
operands = a string that contains all of the operands of the command
*/
typedef struct statement{
    char *label;
    char *command;
    char *directive;
    char *operands;
}statement;
 
statement create_statement(char **token_string, int length, int *error_flag);

char **tokenize_operands(char *statement, int operand_count);

void free_statement(statement statement_to_free);
void trim_whitespace(char *str);

int char_to_int_(char *characters);
int validate_command_operands(command curr_command, int source_operand_type, int dest_operand_type);
int determine_command_operand_type(char *operand);
int is_valid_data_digit(char *digit);
int is_valid_digit(char *digit);
int get_statement_data(statement curr_statement,node **head, int *src_address_mode, int *dest_address_mode, int *num_of_machine_words, int *is_instruction, int *ent_flag, int *ext_flag, int address);

int is_valid_operand(char *string);
int is_valid_string_operand(char *operand);
int determine_operand(char *operand);
int is_valid_data_operands(char **operand_array,int num_of_operands);
int check_directive_operands(statement curr_statement, node **head, int *num_of_machine_words, int *second_pass_flag, int *ext_flag, int address);
int check_command_operands(statement curr_statement, int *curr_source_operand, int *curr_dest_operand);

int string_to_int(char *string);
int count_commas(char *token);
int count_operands(char *token);

int is_preceding_comma(char *token);
int is_trailing_comma(char *token);
int is_double_comma(char *token);

int has_label(statement curr_statement, int current_address);



#endif


