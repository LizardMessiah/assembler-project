#include <stdio.h>
#include <string.h>
#include "assembler.h"
#define IMMEDIATE 1
#define DIRECT 3
#define REGISTER 5
#define NO_OPERAND 0


/*struct to hold information about current opcode
command name = current command's name
src_add = current command viable address modes
dest_add = current command viable address modes
*/

typedef struct command {
    char *command_name;
    int src_add[3];
    int dest_add[3];
}command;

/*opcode enum used for indexing*/
typedef enum opcode{
    MOV, CMP, ADD, SUB, NOT, CLR, LEA, INC, DEC,
    JMP, BNE, RED, PRN, JSR, RTS, STOP, NO_COMMAND =-1
}opcode;

/*register_index enum used for indexing*/
typedef enum register_index{
    r0, r1, r2, r3, r4, r5, r6, r7, NO_REGISTER = -1
}register_index;


command *get_opcode_array() {
    /*this function returns an array that holds all of the languages
    opcodes and information about their viable addressing types.
    */
    static command comm_arr[] = {
        {"mov", {IMMEDIATE, DIRECT, REGISTER}, {DIRECT, REGISTER}},
        {"cmp", {IMMEDIATE, DIRECT, REGISTER}, {IMMEDIATE, DIRECT, REGISTER}},
        {"add", {IMMEDIATE, DIRECT, REGISTER}, {DIRECT, REGISTER}},
        {"sub", {IMMEDIATE, DIRECT, REGISTER}, {DIRECT, REGISTER}},
        {"not", {NO_OPERAND}, {DIRECT, REGISTER}},
        {"clr", {NO_OPERAND}, {DIRECT, REGISTER}},
        {"lea", {DIRECT}, {DIRECT, REGISTER}},
        {"inc", {NO_OPERAND}, {DIRECT, REGISTER}},
        {"dec", {NO_OPERAND}, {DIRECT, REGISTER}},
        {"jmp", {NO_OPERAND}, {DIRECT, REGISTER}},
        {"bne", {NO_OPERAND}, {DIRECT, REGISTER}},
        {"red", {NO_OPERAND}, {DIRECT, REGISTER}},
        {"prn", {NO_OPERAND}, {IMMEDIATE, DIRECT, REGISTER}},
        {"jsr", {NO_OPERAND}, {DIRECT, REGISTER}},
        {"rts", {NO_OPERAND}, {NO_OPERAND}},
        {"stop", {NO_OPERAND}, {NO_OPERAND}}
    };
    return comm_arr;
}

command *find_command(char *name){
    /*this function receives a string if string is a reserved opcode name
    it returns the opcode information otherwise, returns null*/
    int i=0;
    command *command_array = get_opcode_array();
    for (i = 0; i < NUM_OF_COMMANDS; i++){
        if (strcmp(name, command_array[i].command_name) == 0) {
            return &command_array[i];
        }
    }
    return NULL; 
}

opcode find_opcode(char *name){
    /*this function finds out the certain command */
    int i=0;
    command *command_array = get_opcode_array();
    for (i = 0; i < NUM_OF_COMMANDS; i++) {
        if (strcmp(name, command_array[i].command_name) == 0) {
            return (opcode)i;
        }
    }
    return NO_COMMAND; 
}

char **get_key_words(){
    /*this function returns a lookup table of the reserved keywords
    on our imaginary machine*/
    static char *key_words[] = {
        ".data", ".string", ".extern", ".entry",
        "mov", "cmp", "add", "sub",
        "not", "clr", "lea", "inc",
        "dec", "jmp", "bne", "red",
        "prn", "jsr", "rts", "stop",
        "r7", "r6", "r5", "r4", "r3", "r2", "r1", "r0"
    };

    return key_words;
}


char **get_directive_array(){
    /*this function returns a lookup table of available directives
    on our imaginary machine*/
    static char *directive_array[] = {
    ".data", ".string", 
    ".extern", ".entry"
    };

    return directive_array;
}
char **get_register_array(){
    /*this function returns a lookup table of available registers
    on our imaginary machine*/

    static char *register_array[] = {
        "@r0", "@r1", "@r2", "@r3",
        "@r4", "@r5", "@r6", "@r7"
    };

    return register_array;
}

int is_register_operand(char *token){
    /*returns 1 if input string is a reserved register name*/
    if(token == NULL){
        return 0;
    }
    if(token[0] == '@'){
        return 1;
    }
    return 0;
}

int is_valid_register(char *token){
    /*returns 1 if input string is a valid register name*/
    int i;
    char **register_array = get_register_array();
    if(token == NULL){
        return 0;
    }
    if(!is_register_operand(token)){
        return 0;
    }
    for(i=0;i<NUM_OF_REGISTERS;i++){
        if(strcmp(token,register_array[i])==0){
            return 1;
        }
    }
    return 0;
}

register_index find_register_index(char *name){
    /*returns the index of the register in the register array*/
    int i=0;
    char **register_array = get_register_array();
    for(i=0;i<NUM_OF_REGISTERS;i++){
        if(strcmp(name, register_array[i])==0){
            return (register_index)i;
        }
    }
    return NO_REGISTER;
}


char **get_command_array(){
    /*this function returns a string array of the opcodes in our language*/
    static char *command_array[] = {
    "mov", "cmp", "add", "sub", "not",
     "clr", "lea", "inc", "dec","jmp",
      "bne", "red", "prn", "jsr", "rts",
       "stop"
     };
     return command_array;

}


