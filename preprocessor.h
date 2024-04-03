#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H
#include "tables.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "assembler.h"
#include "parser.h"



int preprocess(char *filename, char *filename_am, int *make_am_file);


#endif