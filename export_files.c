#include <stdio.h>
#include "parser.h"
#include "assembler.h"
#include <stdlib.h>
#include "statement.h"


#define FIRST_6_BITS_MASK 63

int make_ext_file(char *filename, node *head){
    /*this function receives a file's name and the head to symbol table, and prints out all of the .extern labels*/
    int create_flag = 0;
    FILE *ext_file;
    node *temp = head;
    char *new_filename = malloc_strcat(filename, EXT_OUTPUT);

    ext_file = fopen(new_filename, "w");
    if(ext_file == NULL){
        COULD_NOT_OPEN_ERROR;
        free(new_filename);    
        return 0;
    }
    while(temp!= NULL){
        if((temp->external == 1) && (temp->address != 0)){
            /*if current node is an .extern label, print it to file*/
            fprintf(ext_file,"%s\t%d\n", temp->label_name, temp->address);
            create_flag = 1;
        }
        temp = temp -> next;
    }
    fclose(ext_file);
    if(create_flag == 0){
        /*removes file if there were no .extern labels*/
        remove(new_filename);
    }
    free(new_filename);
    return 1;
}

int make_ent_file(char *filename, node *head){
    /*this function receives a file's name and the head to symbol table, and prints out all of the .entry labels*/

    int create_flag = 0;
    FILE *ent_file;
    node *temp = head;
    char *new_filename = malloc_strcat(filename, ENT_OUTPUT);

    ent_file = fopen(new_filename, "w");
    if(ent_file == NULL){
        COULD_NOT_OPEN_ERROR;
        free(new_filename);
        return 0;
    }
    while(temp!= NULL){
        if((temp->entry) == 1){
            /*if current node is an .entry label, print it to file*/
            create_flag=1;
            fprintf(ent_file,"%s\t%d\n", temp->label_name, temp->address);
        }
        temp = temp -> next;
    }
    fclose(ent_file);
    if(create_flag == 0){
        /*if there were no .entry nodes in the list, delete file */
        remove(new_filename);
        
    }
    free(new_filename);
    return 1;
}

void binary_to_base_64(int twelve_bit_number, char *base_64_word_1, char *base_64_word_2) {
    /*convert to base 64 from 12 bit binary using a base 64 lookup table and bitwise operators*/
    int base64_index_1, base64_index_2;
    char base_64_lookup_table[64] = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
        'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
        'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'
    };
    /*masking the first 6 and 6 last bits with input number and receiving their index in the lookup table*/
    base64_index_1 = twelve_bit_number & FIRST_6_BITS_MASK;
    base64_index_2 = (twelve_bit_number >> 6) & FIRST_6_BITS_MASK;
    
    /*assigning the value in base64 to input char pointers*/
    *base_64_word_1 = base_64_lookup_table[base64_index_1];
    *base_64_word_2 = base_64_lookup_table[base64_index_2];

}

int make_ob_file(char *filename,int *instruction_image, int *data_image, int instruction_image_index, int data_image_index){
    /*this function receives a filename, the data image and instruction image of a certain assembly file, it encodes
    every line of machine code to its equlevant in base 64*/
    int i = 0;
    char first_word = 0,second_word = 0;
    char *new_filename = malloc_strcat(filename, OB_OUTPUT); 
    FILE *ob_file = fopen(new_filename, "w");
    if(ob_file == NULL){
        COULD_NOT_OPEN_ERROR;
        free(new_filename);
        return 0;
    }

    fprintf(ob_file, "%d %d\n", instruction_image_index, data_image_index);

    for(i=0;i<instruction_image_index;i++){
        binary_to_base_64(instruction_image[i],&first_word,&second_word);
        fprintf(ob_file, "%c%c\n", second_word,first_word);
    }
    for(i=0;i<data_image_index;i++){
        binary_to_base_64(data_image[i],&first_word,&second_word);
        fprintf(ob_file, "%c%c\n", second_word,first_word);
    }

    fclose(ob_file);
    free(new_filename);

    return 1;
}


