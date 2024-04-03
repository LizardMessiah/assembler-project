#ifndef PARSER_H
#define PARSER_H

#define SKIP_SPACE(string, index) \
        for (;string[(index)] && (string[(index)] == '\t' || string[(index)] == ' '); (++(index)))
        

char *malloc_strcat(char *string1, char *string2);
char *malloc_copy_string(const char *str);
char **divide_string(const char *str, const char *delimiters, int *count);

void free_string_array(char **string_array, int size);
int count_substrings(const char *str, const char *delimiters);
int is_mcro_line(char line[]);
int is_endmcro_line(char line[]);


#endif