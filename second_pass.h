#ifndef SECOND_PASS_H
#define SECOND_PASS_H
#include "statement.h"
#include "stdio.h"

int second_pass(FILE *file, node *head,char *curr_filename);
int encode_source_and_dest_registers(char *register_1, char *register_2);
int encode_2_registers(int register_1, int register_2);
int encode_instruction_word(statement curr_statement);
int encode_instruction_number(int number);
int encode_register_word(int number);
int encode_instruction_by_type(int operand_type, char *operand, node *head, int address);
int extern_label_handler(char *operand, node **head, int address, int is_external, int is_entry);
void print_decimal_12_bits(int value);

#endif


