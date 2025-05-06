/* parser.h */
#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>

/* error flag */
extern int error;

/* main entry */
void parser(FILE *file);

/* predicates */
int mcroend(const char *line);
int is_extern(const char *line);
int is_entry(const char *line);
int is_label(const char *line);
int is_instruction(const char *line);
int is_mcro_def(const char *line, char out_name[32]);
size_t is_macro_call(const char *line);

/* macro‐management */
void consume_mcro(FILE *file); /* you must provide this */
void add_mcro(const char *label, FILE *file);
void add_line_to_mcro(const char *line, const char *current_mcro);

/* directive/inst handlers */
void handle_extern(const char *line);
void handle_entry(const char *line);
void handle_label(const char *line, unsigned int *IC);
void handle_instruction(const char *line, unsigned int *IC);
void handle_mcro(size_t index, unsigned int *IC);

#endif /* PARSER_H */