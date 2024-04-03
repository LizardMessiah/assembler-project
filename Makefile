CC = gcc
CFLAGS = -Wall -ansi -pedantic

SRC_FILES = assembler.c assembler.h commands.c commands.h export_files.c export_files.h \
			first_pass.c first_pass.h link_list.c link_list.h parser.c parser.h \
			preprocessor.c preprocessor.h second_pass.c second_pass.h statement.c statement.h \
			tables.c tables.h

OBJ_FILES = $(SRC_FILES:.c=.o)

all: $(OBJ_FILES)
	$(CC) $(CFLAGS) $(OBJ_FILES) -o assembler

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_FILES) assembler

