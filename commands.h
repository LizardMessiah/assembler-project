#ifndef COMMANDS_H
#define COMMANDS_H
#include "assembler.h"

typedef struct command {
    char *command;
    int src_add[3];
    int dest_add[3];
} command;

typedef struct number_operands{
    int numbers[10];
    int numbers_count;
}number_operands;

typedef union directive_operands{
    number_operands number;
    char *string;
    char label[MAX_LABEL_SIZE+1];
}directive_operands;

typedef struct directive {
    char *directive_name;
    directive_operands operands;
}directive;

typedef enum opcode {
    MOV, CMP, ADD, SUB, NOT, CLR, LEA, INC, DEC,
    JMP, BNE, RED, PRN, JSR, RTS, STOP
}opcode;

typedef enum register_index{
    r0, r1, r2, r3, r4, r5, r6, r7, NO_REGISTER = -1
}register_index;


char **get_key_words();
char **get_register_array();
char **get_directive_array();
char **get_command_array();
char **get_2_operand_commands();
char **get_1_operand_commands();
char **get_0_operand_commands();
int is_register_operand(char *token);
int is_valid_register(char *token);

command *find_command(const char *name);
command *get_opcode_array();
opcode find_opcode(char *name);
register_index find_register_index(char *name);

void free_command(command *curr_command);



#endif 
