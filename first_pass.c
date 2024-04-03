#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "parser.h"
#include "assembler.h"
#include "link_list.h"
#include "commands.h"
#include "statement.h"


int is_label(char *string){
    /*this function finds out if input string is a label declaration
    (ends with a colon) returns 1 if so, returns 0 if not a label declaration.*/
    int length = strlen(string);
    if(string[length-1]==':'){
        return 1;
    }
    return 0;
}
int is_command(char *string){
    /*this function finds out if input string is a legal command in out language*/
    int i;
    char **command_array = get_command_array();
    for(i=0;i<NUM_OF_COMMANDS;i++){
        if(strcmp(string, command_array[i]) == 0){
            return 1;
        }
    }
    return 0;
}

int is_directive(char *string){
    /*this function finds out if input string is a legal command in out language
    (checks if string starts with a dot)*/
    if(string[0] == DIRECTIVE_PREFIX){
        return 1;
    }
    return 0;
}

int is_empty_statement(char *line){
    /*this function checks if current statement is an empty statement*/
    int i=0;
    SKIP_SPACE(line,i);
    if(isspace(line[i])||line[i] == EOF){
        return 1;
    }
    return 0;    
}

int command_index(char *token){
    /*returns the index of the command in the command array*/
    int i=0;
    char **command_array = get_command_array();
    while(i<NUM_OF_COMMANDS){
        if(strcmp(token, command_array[i])==0){
            return i;
        }
        i++;
    }
    return 0;
}

int is_valid_directive(char *string){
    /*check if input string is a legal directive in our language by searching for it in the 
    directive array*/
    int i;
    char **directive_array = get_directive_array();
    if(is_directive(string)){
        for(i=0;i<NUM_OF_DIRECTIVES;i++){
            if(strcmp(string, directive_array[i])==0){
                return 1;
            }
        }
        UNKNOWN_DIRECTIVE_ERROR;
        return 0;
    }
    return 1;
}

int is_alpha_num(char *string){
    /*checks if input string contains a subset of non alpha-numberical character, 
    returns 0 if string contains such characters.*/
    char c;
    int i;
    int length = strlen(string);
    
    if(string == NULL){
        return 0;
    }
    for(i=0;i<length-1;i++){
        c = string[i];
        if(!(isalnum(c))){
            return 0;
        }
    }
    return 1;
}

int is_valid_label(char *string){
    /*checks if input string is a correct label name, (this function receives label names without the colon). */
    int length = strlen(string);
    if(length > MAX_LABEL_SIZE){
        return 0;
    }
    if(is_alpha_num(string)){
        return 1;
    }
    return 0;
}

int is_not_reserved(char *string){
    /*checks if an input string is a reserved key word in our assembly language
    returns 1 if key word is not reserved. 
    */
    int i;
    char **key_words = get_key_words();

    if(string == NULL){
        return 1;
    }
    for(i=0; i<NUM_OF_KEY_WORDS; i++){
        if(strcmp(string, key_words[i])==0){
            RESERVED_KEYWORD_ERROR;
            return 0;
        }
    }
    return 1;
}

int is_valid_label_definition(statement curr_statement, node *head){
    /*checks if string is a label declaration if so, checks if name is valid(alphanumerical), if label has already been defined
    or if the label's name is a reserved keyword
    returns 0 for non valid label names, null strings and strings that exceed max designated size of 31 characters
    */
    char *curr_label = curr_statement.label;
    if(curr_label == NULL){
        return 0;
    }

    if(is_valid_label(curr_label) && is_not_reserved(curr_label) && (find_node_by_string(head,curr_label)) == NULL){
        /*checks if the label complies with language requirements */
            return 1;
    }

    INVALID_LABEL_NAME_ERROR;
    return 0;
}

int label_handler(statement curr_statement, node **head,int address,int is_external, int is_entry){

    /* 
    This function is responsible for handling symbols and labels, it receives information about a current statement
    and adds labels and operands that are labels to symbol table. if a symbol is already 
    */

    int i = 0;
    int num_of_operands = 0;
    char **operands_token = NULL;
    node *curr_symbol = NULL;

    num_of_operands = count_operands(curr_statement.operands);
    operands_token = tokenize_operands(curr_statement.operands,num_of_operands);
    if(curr_statement.label!=NULL){
        /*if current label has already been encountered as an operand .entry directive
        update its address*/    
        curr_symbol = find_node_by_string(*head,curr_statement.label);
            if(curr_symbol != NULL){
                if(curr_symbol->entry==1){
                    /*current symbol is an entry statement*/
                    if(curr_symbol->address==0){
                        update_address(curr_symbol,address);
                        
                        for(i=0;i<num_of_operands;i++){
                            /*if operands are labels, update them as well.*/
                            curr_symbol = find_node_by_string(*head,operands_token[i]);
                            if(curr_symbol != NULL){
                                if(curr_symbol->external==1){
                                    insert_node(head,operands_token[i],address+1,1,0,1);
                                }
                            }
                        }
                        free_string_array(operands_token,num_of_operands);
                        return 1;
                        }
                    }
                }
        /*if current statement's label wasn't found in the symbol table
        (disregarding the case of labels without .extern or .entry flags),
        add it to the symbol table.*/
        insert_node(head,curr_statement.label,address,is_external,is_entry,1);
        
        free_string_array(operands_token,num_of_operands);
        return 1;
    }
    if(curr_statement.command != NULL){
    /*if operands are undeclared labels, add them to symbol table*/       
        if(*head != NULL){
                for(i = 0; i < num_of_operands; i++){
                    curr_symbol = find_node_by_string(*head,operands_token[i]);
                    if(curr_symbol!= NULL){
                        if(curr_symbol->external==1){
                            /*adds each use of .extern directive to symbol table for future references*/
                            insert_node(head,operands_token[i],address+1,1,0,0);
                            return 1;
                        }
                        if((curr_symbol->entry == 1) &&( curr_symbol->address == 0)){
                            /*updates address of entry operand*/
                            update_address(curr_symbol,address);
                            return 1;
                        }
                    }
                }       
        }
        for(i=0; i < num_of_operands; i++){
            /*add operands to symbol table*/
            if(is_valid_label(operands_token[i])){
                insert_node(head, operands_token[i],address+1,0,0,0);
            }
            return 1;
        }
    }
    if(curr_statement.directive != NULL){
        /*handles cases of labels of directive statements*/
        if(strcmp(curr_statement.directive,".entry") == 0){
            /*if label was declared .entry  */
            if(*head != NULL){
                curr_symbol = find_node_by_string(*head,curr_statement.operands);
                if(curr_symbol != NULL){
                    if(curr_symbol->is_defined == 1){ 
                        /*if current node is already in the table, change its status to extern label*/
                        make_entry_label(curr_symbol);
                        return 1;
                    }
                }
            }

            insert_node(head,curr_statement.operands,0,0,1,1);
            return 1;
        }
        if(strcmp(curr_statement.directive,".extern")==0){
            /*label was declared an extern label*/
            if(*head != NULL){
                curr_symbol = find_node_by_string(*head, curr_statement.operands);
                if(curr_symbol != NULL){
                    /*if current node is already in the table, change its status to extern label*/
                    make_extern_label(curr_symbol);
                    return 1;
                }
            }
            /*if new .extern statement, add to symbol table*/
            insert_node(head,curr_statement.operands,0,1,0,0);
            free_string_array(operands_token,num_of_operands);
            return 1;
        }
    }
    free_string_array(operands_token,num_of_operands);
    return 0;
}

int first_pass(char *filename, node **head_pointer, int *instruction_image_index, int *data_image_index){
    
    int line_index = 0, is_entry = 0;
    int ic = 100,dc = 0, num_of_words_in_line = 0;
    int first_operand_add_mode = 0, second_operand_add_mode = 0, error_flag = 0; 
    int machine_words = 0, instruction_flag = 0, second_pass_flag = 0, external_flag = 0;
    int no_error_flag = 1;
    char **token_line = NULL;
    char line[MAX_LINE_LENGTH];
    node *head = *head_pointer;
    statement my_statement;

    FILE *file = fopen(filename, "r");
    if(file == NULL){
        FILE_NOT_FOUND_ERROR;
        return 0;
    }

    while (fgets(line, MAX_LINE_LENGTH, file)){
        if(ic+dc > MAX_NUM_OF_INSTRUCTIONS_AND_DATA){
            /*handles case of instruction and data statements exceeding machine memory size.*/
            MEMORY_OVERFLOW_ERROR;
            AT_LOCATION(line_index);
            no_error_flag = 0;
            continue;
        }
        line_index++;
        machine_words = 0;
        error_flag = 0;
        token_line = divide_string(line, " ",&num_of_words_in_line);
        if(*token_line[0] == ';'){
            /*skip comment lines*/
            free_string_array(token_line,num_of_words_in_line);
            continue;
        }
        my_statement = create_statement(token_line, num_of_words_in_line,&error_flag); 
        if(error_flag == 1){
            INVALID_STATEMENT_ERROR;
            AT_LOCATION(line_index);
            no_error_flag = 0;
        }
        if(get_statement_data(my_statement,&head,&first_operand_add_mode, &second_operand_add_mode,&machine_words, &instruction_flag, &second_pass_flag, &external_flag,ic+dc)){
            if(instruction_flag == 1){
                label_handler(my_statement, &head, ic,external_flag,is_entry);
                ic+=machine_words;
            }
            else{
                label_handler(my_statement, &head,dc+ic,external_flag,is_entry);
                dc+=machine_words;
            }
        }
        else{
            /*statement is not valid, print error message and return 0.*/
            INVALID_STATEMENT_ERROR;
            AT_LOCATION(line_index);
            free_string_array(token_line,num_of_words_in_line);
            no_error_flag=0;
            continue;
        }
        free_statement(my_statement);
        free_string_array(token_line,num_of_words_in_line);     
    }

    *data_image_index = dc;
    *instruction_image_index = ic;
    *head_pointer = head;

    /*returns 1 if no errors were encountered, otherwise returns 0*/
    return no_error_flag;
}
