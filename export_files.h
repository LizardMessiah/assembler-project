#ifndef EXPORT_FILES_H
#define EXPORT_FILES_H
#include "link_list.h"

void binary_to_base_64(int twelve_bit_number, char *base_64_word_1, char *base_64_word_2);
int make_ob_file(char *filename,int *instruction_image, int *data_image, int instruction_image_index, int data_image_index);
int make_ent_file(char *filename, node *head);
int make_ext_file(char *filename, node *head);


#endif 



