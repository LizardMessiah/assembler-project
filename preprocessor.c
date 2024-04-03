#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "assembler.h"
#include "parser.h"
#include "tables.h"
#include "preprocessor.h"
#include "statement.h"

int preprocess(char *filename, char *filename_am, int *make_am_file){

    /*this function receives a filename and parses the text inside of the file
    if there are any macro assembler it creates a .am file of the same name, if there are any errors
    the file will not 'compile' and the user will be notified.
    */

    int i = 0, words = 0, is_macro = 0;
    int line_index = 0, line_length = 0;
    int no_error_flag = 1;

    char line[MAX_LINE_LENGTH+3];
    char **line_divided_string = NULL;
    char *curr_macro_name = NULL;

    macro_list *macro_table = create_macro_table();
    macro_list *current_entry = NULL;

    FILE *file = fopen(filename, "r");
    FILE *am_file = fopen(filename_am, "w+");
    
    if(file == NULL){
        FILE_NOT_FOUND_ERROR;
        return 0;
    }
    if(am_file == NULL){
        FILE_NOT_FOUND_ERROR;
        return 0;
    }

    while(fgets(line,MAX_LINE_LENGTH+3, file)){
        
        line_index++;
        i = 0, words = 0;
        line_length = strlen(line);
        if(line_length > MAX_LINE_LENGTH + 2){
            LINE_LENGTH_ERROR;
            AT_LOCATION(line_index);
            no_error_flag = 0;
            continue;
        }

        SKIP_SPACE(line,i);
        if(isspace(line[i])||line[i] == EOF || line[i] == '\n'){
            /*skip loop iteration if entire line is empty*/
            continue;
        }
        /*tokenize line*/
        line_divided_string = divide_string(line+i, SPACE_CHARACTER, &words);
        /*create copy of possible macro name, remove \n and adds the null terminator to the end of the new name*/
        curr_macro_name = malloc_copy_string(line_divided_string[0]);
        if((find_by_name(macro_table,curr_macro_name)!= NULL)){
            /*first word in line is a macro name that is detected in the macro table*/
            if((current_entry->macro_definition)!= NULL){
                /*copy line contents to .am file*/
                fputs(current_entry->macro_definition, am_file);
            }
        }   
        else if((strcmp(line_divided_string[0], MACRO)) == 0 ){ 
            /*current word is a macro declaration
            start macro definition, is_macro flag on */    
            is_macro = 1;
            *make_am_file = 1; 
            current_entry = create_macro(macro_table, line_divided_string[1]);
        }
        else if(strstr(line, MACRO_END)){
            /*line contains end of macro definition - turn macro flag off.*/
            is_macro = 0;
        }
        else if(is_macro == 1){
            /*macro definition has not finished, append other lines to the definition*/
            update_definition(current_entry, line);
        }
        else{
            /*print line to file*/
            fputs(line, am_file);
        }
        for(i=0;i<words;i++) {
            free(line_divided_string[i]);
        }
        free(line_divided_string);
        free(curr_macro_name);
        curr_macro_name = NULL;
    }
    
    delete_table(macro_table);
    
    macro_table = NULL;
    line_divided_string = NULL;
    
    
    fclose(file);
    fclose(am_file);

    if(*make_am_file == 0 || no_error_flag != 1){
        /*if there were no macro declarations in the file or preproccesing 
        the file reusulted in errors - remove the .am file */
        remove(filename_am);
    }

    /*return 1 if no errors encountered during this stage*/

    return no_error_flag;
}

