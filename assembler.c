/**
 * 
 * Elior Levian
 * 315070466
 * 
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "assembler.h"
#include "preprocessor.h"
#include "first_pass.h"
#include "link_list.h"
#include "second_pass.h"


int main(int argc, char *argv[]){

    int i = 0 ,ic = 0, dc = 0;
    int make_am_file_flag = 0;
    char *filename = NULL;
    char *am_filename = NULL;
    node *head = NULL;
    FILE *fp;

    if (argc < 2) {
        INCORRECT_COMMAND_LINE_INPUT;
        exit(EXIT_FAILURE);
    }
    for (i = 1; i < argc; i++){
        if (strlen(argv[i]) > MAX_FILE_NAME){
            MAX_FILENAME_ERROR;
            continue;
        }
        
        filename = malloc_strcat(argv[i],AS_INPUT);
        am_filename = malloc_strcat(argv[i],AM_OUTPUT);
        if(preprocess(filename, am_filename, &make_am_file_flag)){
            if(make_am_file_flag == 1){
                if(first_pass(am_filename, &head,&ic,&dc)){
                    fp = fopen(am_filename,"r");
                    second_pass(fp,head,argv[i]);
                    fclose(fp); 
                }
                else{
                    /*first pass failed*/
                    fprintf(stderr,"\nError encountered in first pass.\n");
                    free(filename);
                    free(am_filename);
                    continue;
                }      
            }
            else{
                if(first_pass(filename, &head,&ic,&dc)){
                fp = fopen(filename,"r");
                second_pass(fp,head,argv[i]);
                fclose(fp);
                }
                else{
                    /*first pass failed*/
                    fprintf(stderr,"\nError encountered in first pass.\n");
                    free(filename);
                    free(am_filename);
                    free_list(head);
                    continue;
                 
                }     
            }
            printf("\n\nfinished procsessing file no:%d \n\n", i);
        }
    /*free data for next iteration*/
    free(filename);
    free(am_filename);
    free_list(head);
    filename = NULL;
    am_filename = NULL;
    head = NULL;
    }
    return 0;
}
