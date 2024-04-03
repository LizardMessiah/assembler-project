#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "first_pass.h"
#include "second_pass.h"
#include "assembler.h"
#include "statement.h"
#include "parser.h"
#include "commands.h"
#include "export_files.h"


#define EXTERNAL_MASK 1
#define ENTRY_MASK 2


int encode_instruction_word_dest_operand(opcode curr_opcode, char *operand){
    /*encode instruction words with destination address mode only*/
    int curr_word = 0;
    int operand_type = determine_command_operand_type(operand);
    
    curr_word =  curr_opcode << 5;
    curr_word |= operand_type << 2;

    return curr_word;

}

int encode_instruction_word_no_operand(opcode curr_opcode){
    /*handles operandless opcodes and encodes them to machine code.*/
    int curr_word = 0;
    curr_word = curr_opcode << 5;
    return curr_word;
}

int encode_instruction_word(statement curr_statement){
    /*this function is responsible for encoding an instruction word that is compromised of 12 bits,
    that are divided according to the standards of our language.(as specified below)
  
       3 bits                 4 bits           3 bits            2 bits
        000                     0000            000                00
    src operand address mode - opcode - dest operand address mode  - a,r,e bits     

    */
    int i = 0;
    int curr_word = 0;
    opcode curr_opcode;
    int curr_operand_type[2];
    int num_of_operands = count_operands(curr_statement.operands);
    char **operand_array = tokenize_operands(curr_statement.operands,num_of_operands);
    
    curr_opcode = find_opcode(curr_statement.command);

    if(num_of_operands == 0){
        curr_word = encode_instruction_word_no_operand(curr_opcode);
        return curr_word;
        
    }
    
    if( num_of_operands == 1 ){
        curr_word = encode_instruction_word_dest_operand(curr_opcode, operand_array[0]);
        return curr_word;
    }

    curr_word =  curr_opcode << 5;
    for(i=0; i< num_of_operands; i++){
        curr_operand_type[i] = determine_command_operand_type(operand_array[i]);
    }

    curr_word |= curr_operand_type[0] << 9;
    curr_word |= curr_operand_type[1] << 2;
    return curr_word;
}

int encode_source_and_dest_registers(char *register_1, char *register_2){
    /*find register indices to send to encoding function, return a machine word that
    is comprised of the values of the 2 registers in their respective places in the word.*/
    int curr_word = 0;
    register_index reg_index_1 = NO_REGISTER;
    register_index reg_index_2 = NO_REGISTER;
    reg_index_1 = find_register_index(register_1);
    reg_index_2 = find_register_index(register_2);
    curr_word = encode_2_registers(reg_index_1, reg_index_2);

    return curr_word;
}

int encode_instruction_number(int number) {
    /*this function receives an integer value and shifts its bits to the left by two,
    thus creating the required representation of that for a instruction command */
    int curr_word = 0;
    curr_word = number << 2;
    return curr_word;
}

int encode_register_word(int number){
    /*this function receives a register index 
    and returns converts it into machine code*/
    int curr_word = 0 ;
    curr_word = number << 7;
    return curr_word; 
}


int encode_2_registers(int register_1, int register_2){
    /*this function receives two register indexes and encodes them into one machine word*/
    int curr_word = 0;
    curr_word = encode_register_word(register_1);
    register_2 = register_2 << 2;
    curr_word |= register_2; 
    return curr_word;
}

int encode_char(char character){
    /*returns the integer value of char*/
    int curr_word = 0;
    curr_word = (int)character;
    return curr_word;
}

void print_decimal_12_bits(int value){
    /*this function receives an integer value and prints out its representation in 12 bits using bitwise operations*/
    int bit = 0;
    int i = 0;
    for(i = 11; i >= 0; i--) {
        bit = (value >> i) & 1;
        printf("%d", bit);
    }
}

int encode_instruction_by_type(int operand_type, char *operand, node *head, int address){

    /*this function encodes instruction operands depending on their addressing mode IMMEDIATE, DIRECT or REGISTER*/

    int curr_word = 0, num = 0;
    node *temp = NULL;
    register_index reg_index = NO_REGISTER;
    if(operand_type == IMMEDIATE){
        num = string_to_int(operand);
        curr_word = encode_instruction_number(num);
        return curr_word;
    }
    if(operand_type == DIRECT){
   
        temp = find_node_by_string(head,operand);
        if(temp) {
                num = temp->address;
            curr_word = encode_instruction_number(num);
            if(temp->external == 1){
                /*if current symbol is external, a,r,e bits = 01*/ 
                curr_word = 1;
                return curr_word;
            }
        }
        /*mask current word with entry mask for .entry directives*/
        curr_word |= ENTRY_MASK;
        return curr_word;
    }
    if(operand_type == REGISTER){
        reg_index = find_register_index(operand);
        curr_word = encode_register_word(reg_index);
        return curr_word;
    }
    return ERROR_CODE;
}

int encode_data_statement(char **operand_array,int data_image[], int num_of_machine_words,int num_of_operands,int *data_image_index,statement curr_statement){

    /*receive pointer to data image array and pointer to data image index, and encodes data and string statements */

    int i = 0;
    int curr_word = 0;
    int dc = *data_image_index;
    if(curr_statement.directive !=NULL ){
        if(strcmp(curr_statement.directive, ".data") == 0){
            /*if statement is a .data directive, iterate on operands and add each one to the data image.*/
            for(i=0;i<num_of_machine_words;i++){
                /*add value to data image*/
                curr_word = string_to_int(operand_array[i]);
                data_image[dc] = curr_word;
                dc++;
            }
            *data_image_index = dc;
            return 1;
        }
        if(strcmp(curr_statement.directive, ".string") == 0){
        /*if statement is a .string directive, translate ascii code to int and add to data image.*/
        for(i=1;i<num_of_machine_words;i++){
            /*add value to data image*/
            curr_word = encode_char(curr_statement.operands[i]);
            data_image[dc] = curr_word;
            dc++;
        }
        /*add value of null character after last char in string.*/
        data_image[dc]= 0;
        dc++;
        *data_image_index = dc;
        return 1;
    }
    if(strcmp(curr_statement.directive, ".entry")==0 || strcmp(curr_statement.directive, ".extern")==0 ){
        return 1;
        }
    }
    return 0;
}

int second_pass(FILE *file, node *head, char *curr_filename){
    
    char line[MAX_LINE_LENGTH+2];
    int instruction_image[MEMORY_SIZE];
    int data_image[MEMORY_SIZE];
    int data_image_index = 0, instruction_image_index = 0, entry_flag = 0;
    int i = 0,curr_word = 0, num_of_words = 0, error_flag = 0, instruction_flag = 0, external_flag = 0;
    int source_add_mode = 0, dest_add_mode = 0, num_of_machine_words = 0, num_of_operands = 0, line_index = 0;
    

    char **token_string;
    char **operand_array; 
    statement curr_statement;

    for (i= 0; i < MEMORY_SIZE; i++) {
        data_image[i] = 0;
        instruction_image[i] = 0;
    }


    while(fgets(line, MAX_LINE_LENGTH, file)){
       
        line_index++;
        instruction_flag = 0;
        error_flag = 0;
        source_add_mode = dest_add_mode = 0;
        source_add_mode = dest_add_mode = num_of_machine_words = instruction_flag = external_flag = 0;

        token_string = divide_string(line, " ", &num_of_words);
        if(token_string[0][0]==';'){
            /*if the line is a comment-line , skip it*/
            continue;
        }
        curr_statement = create_statement(token_string, num_of_words,&error_flag);
        if(error_flag==1){
            /*return error if statement was not created.*/
            GENERIC_ERROR_MSG;
            AT_LOCATION(line_index);
            continue;
        }

        get_statement_data(curr_statement,&head, &source_add_mode, &dest_add_mode, &num_of_machine_words, &instruction_flag,&entry_flag,&external_flag,0);

        num_of_operands = count_operands(curr_statement.operands);
        operand_array = tokenize_operands(curr_statement.operands, num_of_operands);
        
        if(operand_array==NULL){
            num_of_operands = 0;
        }
        if(instruction_flag == 1){
            /**current_statement is a instruction statement*/
            curr_word = encode_instruction_word(curr_statement);
            instruction_image[instruction_image_index] = curr_word;
            instruction_image_index++;

            if(source_add_mode == NO_OPERAND && dest_add_mode == NO_OPERAND){
                    /*if the instruction doesnt take source and destination operands(command is stop or rts) continue to next iteration*/

            }
            for(i=0;i<num_of_machine_words-1;i++){
                if(source_add_mode == REGISTER && dest_add_mode == REGISTER){
                    /*if both the source operand and dest operand are registers, they share a machine word*/
                    curr_word = encode_source_and_dest_registers(operand_array[0],operand_array[1]);
                    instruction_image[instruction_image_index] = curr_word;
                    instruction_image_index++;
                    break;
                }
                else if(source_add_mode == NO_OPERAND){
                    curr_word = encode_instruction_by_type(dest_add_mode,operand_array[0],head, data_image_index+instruction_image_index);
                    if(curr_word == ERROR_CODE){
                        ERROR_ENCODING_LINE;
                        AT_LOCATION(line_index);
                        break;
                    }
                    instruction_image[instruction_image_index] = curr_word;
                    instruction_image_index++;
                    break;
                }
                else{
                    curr_word = encode_instruction_by_type(source_add_mode,operand_array[i],head, data_image_index+instruction_image_index);
                    if(curr_word == ERROR_CODE){
                        ERROR_ENCODING_LINE;
                        AT_LOCATION(line_index);
                        continue;
                    }
                    source_add_mode = dest_add_mode;
                    instruction_image[instruction_image_index] = curr_word;
                    instruction_image_index++;
                }
                

            }
        }
        else{
            /*current_statement is a data statement, encode current statement, 
            if theres an error inside encode_data_statement return an error*/
            error_flag = encode_data_statement(operand_array, data_image, num_of_machine_words,num_of_operands, &data_image_index, curr_statement);
            if(error_flag== 0){
                GENERIC_ERROR_MSG;
                AT_LOCATION(line_index);
                continue;
            }
        }
        /*finished encoding current statement, continue to next line*/
        free_statement(curr_statement);
        for(i=0;i<num_of_words;i++) {
            free(token_string[i]);
        }
        free(token_string);
    }
    
    if(error_flag == 1){
        /*assemble .as or .am file only if there were no errors*/
        make_ob_file(curr_filename, instruction_image,data_image,instruction_image_index,data_image_index);
        make_ent_file(curr_filename,head);
        make_ext_file(curr_filename,head);
    }
    

    return 1;

}



