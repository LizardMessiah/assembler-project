#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "assembler.h"
#  
#include "parser.h"

int count_substrings(const char *str, const char *delimiters){
    /*this function counts the number of substrings divided by a specified delimiter*/
    int i=0;
    int prev_i= 0;
    int substring_count = 0; 
    int len = 0;
    len = strlen(str);
    
    while( i < len){
        while( i< len && strchr(delimiters, str[i])!= NULL){
            i++;
        }
        prev_i = i;
        while( i< len&& strchr(delimiters, str[i]) == NULL){
            i++;
        }
        if(i > prev_i){
            substring_count++;
        }
    }

    return substring_count;
}


void free_string_array(char **string_array, int size){
    /*this function frees strings that had memory dynamically allocated to them*/
    int i = 0;
    for(i=0;i<size;i++){
        free(string_array[i]);
        string_array[i] = NULL;
    }
    free(string_array);
    string_array = NULL;
}


char **divide_string(const char *str, const char *delimiters, int *count) {
    /*this function returns an array of substrings divided by a specified delimiter*/
    int num_substrings = count_substrings(str, delimiters);
    int i = 0;
    int prev_i = 0;
    int substring_index = 0;
    int substring_len = 0;
    int len = strlen(str);
    char **substrings = (char **)malloc(num_substrings * sizeof(char *));
    *count = num_substrings;
    if (substrings == NULL) {
        ALLOCATION_ERROR;
        return NULL;
    }
    while (i < len) {
        while (i < len && strchr(delimiters, str[i]) != NULL) {
            i++;
        }
        prev_i = i;
        while (i < len && strchr(delimiters, str[i]) == NULL) {
            i++;
        }
        if (i > prev_i) {
            substring_len = i - prev_i;
            substrings[substring_index] = (char *)malloc((substring_len + 1) * sizeof(char));
            if (substrings[substring_index] == NULL) {
                ALLOCATION_ERROR;
                free_string_array(substrings,substring_index);
                return NULL;
            }
            strncpy(substrings[substring_index], str + prev_i, substring_len);
            substrings[substring_index][substring_len] = '\0';
            substring_index++;
        }
    }
    
    return substrings;
}


char *malloc_copy_string(const char *str_to_copy){
    
    int len;
    char *copy;
    
    len = strlen(str_to_copy);
    copy = (char *)malloc(sizeof(char)*(len+1));
    if(copy == NULL){
        printf("ERROR!, could not copy \"%s\" into string array\n", str_to_copy);
        return NULL;
    }
    strncpy(copy, str_to_copy, len+1);
    if( (len > 0 && copy[len-1] == '\n')|| (len > 0 && copy[len-1] == ':')){
        /* removing newline character and colon from string */
        copy[len-1] = '\0';
    }

    return copy;

}


char *malloc_strcat(char *string1, char *string2){

    char *new_string = (char *) malloc(strlen(string1)+strlen(string2)+1);
    
    if(new_string == NULL){
        ALLOCATION_ERROR;
        return NULL;
    }
    strcpy(new_string, string1);
    strcat(new_string, string2);
    return new_string;

}

int is_mcro_line(char line[]){

    /*checks if the macro declaration is inline, 
    think about implementing a pointer char to do pointer arith..*/
    char key[5] = MACRO;
    
    if(strstr(line, key) == NULL){
        return 0;
    }
    return 1;
}

int is_endmcro_line(char line[]){
    /* checks if end of macro declaration is inline*/
    char key[9] = MACRO_END;
    if(strstr(line, key ) == NULL){
        return 0;
    }
    return 1;
}
