#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#define MAX_MACRO_NAME 30
#define MAX_LABEL_SIZE 31
#define MAX_FILE_NAME 30
#define MAX_LINE_LENGTH 80
#define MEMORY_SIZE 1024
#define MAX_NUM_OF_INSTRUCTIONS_AND_DATA 924
#define MACHINE_WORD_SIZE 12
#define NUM_OF_KEY_WORDS 26
#define NUM_OF_COMMANDS 16
#define NUM_OF_DIRECTIVES 4
#define NUM_OF_REGISTERS 7
#define ERROR_CODE 5000 

#define MAX_OPERANDS 10
#define NO_OPERAND 0
#define MAX_12_BITS_NUM 2047
#define MIN_12_BITS_NUM -2048
#define MAX_10_BITS_NUM 511
#define MIN_10_BITS_NUM -512


#define IMMEDIATE 1
#define DIRECT 3
#define REGISTER 5


#define AS_INPUT ".as"
#define  AM_OUTPUT ".am"
#define OB_OUTPUT ".ob"
#define ENT_OUTPUT ".ent"
#define EXT_OUTPUT ".ext"
#define TXT_INPUT ".txt"

#define SPACE_CHARACTER " "
#define COMMENT_CHARACTER ';'
#define MACRO "mcro"
#define MACRO_END "endmcro"
#define DIRECTIVE_PREFIX '.'


#define AT_LOCATION(line_index) printf("Error detected at line:%d\n", line_index)
#define GENERIC_ERROR_MSG fprintf(stderr,"Error: unexpected token encountered. Please review code for syntax errors and try again.\n")
#define INCORRECT_COMMAND_LINE_INPUT printf("Error: please use the following format: program_name file_name_1 file_name_2 file_name_n\n")
#define FILE_NOT_FOUND_ERROR printf("Error: file not found in directory.\n")
#define COULD_NOT_OPEN_ERROR printf("Error: could not open file.\n")
#define MAX_FILENAME_ERROR printf("Error: input name exceeds maximum size.")

#define PREPROCESSOR_ERROR(filename) printf("Error: could not preprocess file:%s.\n", filename)
#define ALLOCATION_ERROR printf("Error: could not allocate memory.\n")
#define LINE_LENGTH_ERROR fprintf(stderr, "Error: current line length exceeds maximum amount.\n")

#define MEMORY_OVERFLOW_ERROR printf("The amount of data or instructions provided exceeds the available memory capacity. Please reduce the input size and try again\n")
#define SYMBOL_DEFINED_AT_ENTRY_DIRECTIVE_WARNING printf("Warning: labels declared at start of .entry directive are redundant.\n ")
#define EXTERN_ERROR printf("Error: declaration of label at start of .extern directive is forbidden.\n")
#define UNKNOWN_DIRECTIVE_ERROR printf("Error: line calls to an unknown directive.\n ")
#define SYMBOL_REDECLARATION_ERROR(string) printf("Error: %s has already been defined.\n",string)
#define RESERVED_KEYWORD_ERROR printf("Error: the selected idenftifier is a reserved keyword.\n")
#define INVALID_LABEL_NAME_ERROR printf("Error: the identifier selected for the label is invalid, please use alphanumerical characters only.\n")
#define LARGE_LABEL_NAME_ERROR printf("Error: the name chosen for the label exceeds the maximum 31 characters limit.\n")
#define SYNTAX_ERROR(line_index) printf("Error: syntax error detected.\n Location:%d \n")
#define INVALID_OPERAND_ERROR printf("Error: invalid operand\n")
#define INVALID_STATEMENT_ERROR printf("Error: invalid statement declaration\n")
#define ADD_TO_SYMBOL_TABLE_ERROR printf("Error: could not add current label to symbol table.\n")

#define INVALID_NUMBER_ERROR printf("Error: invalid number used as operand\n")
#define INVALID_NUMBER_OF_OPERANDS printf("Error: invalid number of operands for command statement.\n")
#define INVALID_NUMBER_OF_OPERANDS_DATA printf("Error: invalid operands for data statement.\n")
#define INVALID_COMMAND_OPERANDS_ERROR printf("Error:input command operands are of wrong type.\n")
#define INVALID_DIRECTIVE_OPERANDS_ERROR printf("Error:input directive operands are of wrong type.\n")

#define ERROR_ENCODING_LINE fprintf(stderr,"Error: could not encode line to machine code.\n")


/*driver function*/
int main(int argc, char *argv[]); 



#endif


