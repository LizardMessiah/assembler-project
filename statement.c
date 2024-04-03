#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "commands.h"
#include "first_pass.h"
#include "link_list.h"
#include "parser.h"
#include "assembler.h"
#include "statement.h"
#include "second_pass.h"

int string_to_int(char *string){
    /*convert string to int*/
    int i = 0;
    int res = 0;
    int sign = 1; 

    if (string[0] == '-') {
        /*handle case of negative numbers*/
        sign = -1; 
        i = 1; 
    }

    for (; string[i] != '\0'; ++i)
        res = res * 10 + string[i] - '0';

    return res * sign; 
}

int count_commas(char *token){
    /*this function counts the number of commas in a given string*/
    int i=0;
    int comma_count = 0;
    if(token == NULL){
        return 0;
    }
    while(token[i]!= '\0'){
        if(token[i] == ','){
            comma_count++;
        }
        i++;   
    }
    return comma_count;
}

int count_operands(char *token){
    /*num of operands will always be +1 
    than number of commas*/
    if(token == NULL){
        return 0;
    }
    return count_commas(token)+1;
}

int is_preceding_comma(char *token){
    /*returns true if first value of token is a comma*/
    if(token == NULL){
        return 0;
    }
    return (token[0]==',')? 1:0;
}
int is_trailing_comma(char *token){
    /*returns true if last value of token is a comma*/
    int length;
    if(token == NULL){
        return 0;
    }
    length = strlen(token);
    return (token[length-1]==',')? 1 : 0;
}

int is_double_comma(char *token){
    /*returns true if there exists two commas in a row in input token*/
    int i = 0;
    char prev_c = 0;
    if(token == NULL){
        return 0;
    }
    while(token[i]!= '\0'){
        if(token[i]== ',' && prev_c == ','){
            return 1;
        }
        prev_c = token[i];
        i++;
    }
    return 0;
}


void free_statement(statement statement_to_free){
    /*frees input statement*/
    free(statement_to_free.label);
    free(statement_to_free.command);
    free(statement_to_free.directive);
    free(statement_to_free.operands);
}

int is_valid_operand(char *string) {
    /*check for double, preceding and trailing commas, returns 1 if operand complies with language's syntax*/
    int i=0;
    int length = 0;
    if(string == NULL){
        return 0;
    }
    length = strlen(string);
    if (string[0] == ','){
        return 0;
    }
    if (string[length - 1] == ',') {
        return 0;
    }
    for (i = 0; i < length - 1; i++) {
        if (string[i] == ',' && string[i + 1] == ',') {
            return 0;
        }
    }
    return 1;
}

statement create_statement(char **token_string, int length, int *error_flag){
    /*this function create a new statement structure that holds valuable information about the current line*/
    statement new_statement;
    int i;
    int word_index = 0;
    int no_directive_flag = 1;
    int no_command_flag = 1;
    char *string_copy;
    new_statement.label = NULL;
    new_statement.directive = NULL;
    new_statement.command = NULL;
    new_statement.operands = NULL;

    if(is_label(token_string[word_index])){
        new_statement.label = malloc_copy_string(token_string[word_index]);
        word_index++;
    }
    string_copy = malloc_copy_string(token_string[word_index]);
    if(is_directive(string_copy)) {
        new_statement.directive = string_copy;
        word_index++;
        no_directive_flag = 0;

    } else if (is_command(string_copy)) {
        new_statement.command = string_copy;
        word_index++;
        no_command_flag = 0;
    }
    if(no_command_flag && no_directive_flag){
        *error_flag = 1;
        INVALID_STATEMENT_ERROR;
        return new_statement;
    }
    if(no_command_flag){
        /**addressing the case of a directive statement**/
        if (word_index < length) {
/*            num_operands = length - word_index;
*/            /** allocate memory for operands and copy the first operand**/
            new_statement.operands = malloc_copy_string(token_string[word_index]);
            
        for (i = word_index + 1; i < length; i++){
            new_statement.operands = malloc_strcat(new_statement.operands, " ");
            new_statement.operands = malloc_strcat(new_statement.operands, token_string[i]);
            }

        }
        if(is_valid_operand(new_statement.operands)== 0){
            *error_flag = 1;
            INVALID_OPERAND_ERROR;
        }
            return new_statement;
    }

    if(no_directive_flag){
        /**addressing the case of a command statement**/
        if (word_index < length) {
        /*num_operands = length - word_index;*/
            /** allocate memory for operands and copy the first operand**/
            new_statement.operands = malloc_copy_string(token_string[word_index]);
            
            for (i = word_index + 1; i < length; i++){
                new_statement.operands = malloc_strcat(new_statement.operands, " ");
                new_statement.operands = malloc_strcat(new_statement.operands, token_string[i]);
            }
        }

        if(strcmp(new_statement.command, "stop") == 0 || strcmp(new_statement.command, "rts") == 0){
            /**if there are not operands and the command is stop or rts than skip validity test of operands 
             * and return the statement
            */
            return new_statement;
        }

        if(is_valid_operand(new_statement.operands)== 0){
            *error_flag = 1;
            INVALID_OPERAND_ERROR;
            return new_statement;
        }

        return new_statement;
    }


    return new_statement;
}


int get_statement_data(statement curr_statement,node **head,  int *src_address_mode, int *dest_address_mode, int *num_of_machine_words, int *is_instruction, int *ent_flag, int *ext_flag, int address){
    /*this function returns data about statement using int pointers*/
    int machine_words = 0;
    int external_flag = 0;
    int entry_flag = 0;
    int source_add_mode = 0, dest_add_mode = 0;
    if(curr_statement.label != NULL){
        if(!is_valid_label(curr_statement.label)){
            INVALID_LABEL_NAME_ERROR;
            return 0;
        }
    }
    if(curr_statement.directive != NULL){
        if(check_directive_operands(curr_statement,head,&machine_words,&external_flag, &entry_flag, address)){
            if(external_flag == 1){
                *ext_flag = external_flag;
                return 1;
            }
            if(entry_flag == 1){
                *ent_flag = entry_flag;
                return 1;
            }
            *num_of_machine_words = machine_words;
            *is_instruction = 0;
            return 1;
        }
        INVALID_DIRECTIVE_OPERANDS_ERROR;
        return 0;
    }
    if(curr_statement.command != NULL){
        if(check_command_operands(curr_statement, &source_add_mode, &dest_add_mode)){          
            if((source_add_mode == REGISTER && dest_add_mode == REGISTER) ||(source_add_mode == NO_OPERAND && dest_add_mode != NO_OPERAND)){
                /*separate case for commands with only destination operand, also the case for 2 registers as operands, as they share a machine word.*/
                *num_of_machine_words = 2;
                *is_instruction = 1;
                *src_address_mode = source_add_mode;
                *dest_address_mode = dest_add_mode;
                return 1;
            }
            if(source_add_mode == NO_OPERAND && dest_add_mode == NO_OPERAND){
                /*addressing the case of stop & rts*/
                *num_of_machine_words = 1;
                *is_instruction = 1;
                *src_address_mode = source_add_mode;
                *dest_address_mode = dest_add_mode;
                return 1;
            }
            else{
            *src_address_mode = source_add_mode;
            *dest_address_mode = dest_add_mode;
            *num_of_machine_words = 3;
            *is_instruction = 1;
            return 1;
            }
        }
        else{
            INVALID_COMMAND_OPERANDS_ERROR;
            return 0;
        }
    }
    return 0; 
}

int is_valid_data_digit(char *digit){
    /*this function checks if input string is a valid data digit
    (meaning that it is inside the allowed range of -2048 to 2047)*/
    int i;
    int num_index = 0;
    int length = strlen(digit);
    char num[5];
    if(length > 5){
        INVALID_NUMBER_ERROR;
        return 0;
    }
    if(digit[0] == '-'|| digit[0] == '+'){
        num_index++;
    }
    for(i=num_index;i<length;i++){
        if(!isdigit(digit[i])){
            return 0;
        }
        num[i]=digit[i];
    }
    /*null terminate string*/
    num[i] = '\0';
    if(string_to_int(num)>MAX_12_BITS_NUM||string_to_int(num)<MIN_12_BITS_NUM){
        INVALID_NUMBER_ERROR;
        return 0;
    }
    return 1;
}


int is_valid_string_operand(char *operand){
    /*check for opening and closing double quotes for string*/
    int length = strlen(operand);
    if(operand[0] == '"' && operand[length-1] == '"'){
        return 1;
    }
    return 0;
}

int determine_operand(char *operand){
    /*find the address mode type of input operand.*/
    if(isdigit(operand[0])){
        return IMMEDIATE;
    }
    if(is_label(operand)){
        return DIRECT;
    }
    if(is_alpha_num(operand)){
        return DIRECT;
    }
    if(is_register_operand(operand)){
        return REGISTER;
    }
    /*if not one of the above, return error code -1 */
    return -1;
}

void trim_whitespace(char *str){
    /*function that trims whitespaces from strings*/
    char *end = NULL;
    char *start = str;
    while (*start == ' ' || *start == '\t') {
        start++;
    }
    if (start != str) {
        while (*start != '\0') {
            *(str++) = *(start++);
        }
        *str = '\0';
    }
    end = str + strlen(str) - 1;
    while (end >= str && (*end == ' ' || *end == '\t')) {
        end--;
    }
    *(end + 1) = '\0';
}

char **tokenize_operands(char *curr_operand, int operand_count) {
    int current_operand = 0;
    char *curr_operand_copy = NULL;
    char *token = NULL;
    char **operands = NULL;

    if (curr_operand == NULL) {
        return NULL;
    }

    curr_operand_copy = malloc_copy_string(curr_operand);
    if (curr_operand_copy == NULL) {
        return NULL;
    }

    token = strtok(curr_operand_copy, ",");
    operands = malloc(operand_count * sizeof(char *));

    while (token != NULL && current_operand < operand_count) {
        operands[current_operand] = malloc(strlen(token) + 1);
        if (operands[current_operand] != NULL) {
            strncpy(operands[current_operand], token, strlen(token) + 1);
            trim_whitespace(operands[current_operand]);
            current_operand++;
        }
        token = strtok(NULL, ",");
    }

    free(curr_operand_copy); 
    return operands;
}

int is_valid_digit(char *digit){
    /*this function checks if input string is a valid instruction digit
    (meaning that it is inside the allowed range of 511 to -512)*/
    int i;
    int sign = 1;
    int num_index = 0;
    int result = 0;
    int length = strlen(digit);
    char num[4];
    for(i=0;i<4;i++){
        num[i]=0;
    }
    
    if(length > 4){
        INVALID_NUMBER_ERROR;
        return 0;
    }
    if(digit[0] == '-'|| digit[0] == '+'){
        if(digit[0] == '-'){
            sign = -1;
        }
        num_index++;
    }
    for(i=num_index;i<length;i++){
        if(!isdigit(digit[i])){
            return 0;
        }
        num[i] = digit[i];
    }
    /*null terminate the string*/
    num[i] = '\0';
    result = string_to_int(num) * sign;
    if(result > MAX_10_BITS_NUM||result < MIN_10_BITS_NUM){
        INVALID_NUMBER_ERROR;
        return 0;
    }
    return 1;
}

int determine_command_operand_type(char *operand){
    /**
     * returns address mode for input operand
    */
    if(operand == NULL){
        return 0;
    }
    if(isdigit(operand[0])||operand[0] == '-' || operand[0] == '+'){
        if(is_valid_digit(operand)){
            return IMMEDIATE;
        }
    }
    if(is_valid_label(operand)){
        return DIRECT;
    }
    if(is_valid_register(operand)){
        return REGISTER;        
    }
    return 0;
}

int validate_command_operands(command curr_command, int source_operand_type, int dest_operand_type){
    int i;
    int source_op_flag = 0;
    int dest_op_flag = 0;
    for(i=0;i<3;i++){
        if(curr_command.src_add[i] == source_operand_type){
            source_op_flag = 1;
        }
        if(curr_command.dest_add[i] == dest_operand_type){
            dest_op_flag = 1;
        }
    }
    if(source_op_flag && dest_op_flag){
        return 1;
    }
    return 0;
}


int has_label(statement curr_statement, int current_address){
    
    if(curr_statement.label != NULL){
        if(curr_statement.directive!= NULL){
            /*add to symbol table at dc*/
            return 1;

        }
        if(curr_statement.command!= NULL){
            /*add to symbol table at ic*/
            return 1;
        }
    }

    return 0;
}

int is_valid_data_operands(char **operand_array,int num_of_operands){
    /*this function checks the validity of current .data directive operand*/
    int i;
    for(i = 0; i < num_of_operands; i++){
        if(!is_valid_data_digit(operand_array[i])){
            return 0;
        }
    }
    return 1;
}

int check_directive_operands(statement curr_statement, node **head, int *num_of_machine_words, int *entry_flag, int *ext_flag, int address){
    
    /*receives a directive statement checks if its valid and returns the number of machine words it will output */
    int length = 0;
    char **operand_array = NULL;
    char *operands = curr_statement.operands;
    int num_of_operands = count_operands(curr_statement.operands);
    operand_array = tokenize_operands(curr_statement.operands, num_of_operands);

    if(strcmp(curr_statement.directive,".data")==0){
        if(is_valid_data_operands(operand_array, num_of_operands)){
            *num_of_machine_words = num_of_operands;
            return 1;
        }
    }
    else if(strcmp(curr_statement.directive,".string")==0){
        if(is_valid_string_operand(operands)){
            length = strlen(operands);
            *num_of_machine_words = length-1;
            return 1;
        }
    }
    else if(strcmp(curr_statement.directive,".entry")==0){
        if(curr_statement.label != NULL){
            SYMBOL_DEFINED_AT_ENTRY_DIRECTIVE_WARNING;
        }
        if(num_of_operands > 1){
            /*if entry directive have more than 1 operand then return error*/
            INVALID_DIRECTIVE_OPERANDS_ERROR;
            return 0;
        }
        if(strlen(curr_statement.operands)>MAX_LABEL_SIZE){
            INVALID_DIRECTIVE_OPERANDS_ERROR;
            return 0;
        }

        return 1;
    }
    else if(strcmp(curr_statement.directive,".extern")==0){
        if(curr_statement.label != NULL){
            EXTERN_ERROR;
            return 0;
        }
        if(strlen(curr_statement.operands)>MAX_LABEL_SIZE){
            INVALID_DIRECTIVE_OPERANDS_ERROR;
            return 0;
        }
        return 1;
    }

        return 0;
}



int check_command_operands(statement curr_statement, int *curr_source_operand, int *curr_dest_operand){
    
    char **operand_array = NULL;
    int src_operand_exists_flag = 1;
    int dest_operand_exists_flag =1;
    int first_operand_type = 0;
    int second_operand_type = 0;
    int num_of_operands = count_operands(curr_statement.operands);
    command *curr_command = find_command(curr_statement.command);

    if(*curr_command->src_add == NO_OPERAND){
        src_operand_exists_flag = 0;
    }

    if(*curr_command->dest_add == NO_OPERAND){
        dest_operand_exists_flag = 0;
    }
    if(num_of_operands > 2){
        INVALID_NUMBER_OF_OPERANDS;
        return 0;
    }

    if(src_operand_exists_flag == 0 && dest_operand_exists_flag == 0){
        /*if both of command's operands are empty than the command doesn't takes operand(in case of rts/stop commands)*/
        if(validate_command_operands(*curr_command,NO_OPERAND,NO_OPERAND)){
            /*check to see if command is stop or rts*/
            *curr_dest_operand = NO_OPERAND;
            *curr_source_operand = NO_OPERAND;
            return 1;
        }
        return 0;
    }
    operand_array = tokenize_operands(curr_statement.operands, num_of_operands);
    if(src_operand_exists_flag == 0){
        /*command takes only destination operands*/
        first_operand_type = determine_command_operand_type(operand_array[0]);
        if(validate_command_operands(*curr_command,NO_OPERAND,first_operand_type)){
            /*check if address mode complies with current command*/
            *curr_dest_operand = first_operand_type;
            *curr_source_operand = NO_OPERAND;
            return 1;
        }
        return 0;
    }
    /*check if source and destination address modes complies with current command*/
    first_operand_type = determine_command_operand_type(operand_array[0]);
    second_operand_type = determine_command_operand_type(operand_array[1]);
    if(validate_command_operands(*curr_command,first_operand_type,second_operand_type)){
        /*valid statement, save values to access outside of functions*/
        *curr_source_operand = first_operand_type;
        *curr_dest_operand = second_operand_type;
        return 1;
    }

    return 0;
}
