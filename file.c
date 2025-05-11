/* parser.c */
#define _POSIX_C_SOURCE 200809L
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<ctype.h>
#define EXTERN   ".extern"
#define ENTRY    ".entry"
#define MCRO     "mcro "
#define MCROEND  "mcroend"
#define MAX_LINE 82
/* error flag */
int error = 0;

/*–– Global macro list and count ––*/
static char ***mcro_list = NULL;  /* mcro_list[i] is a char** (array of lines), with [0] == label */
static size_t mcro_count = 0;     /* number of macros defined */
static size_t label_count = 0;
static size_t unknown_count = 0;
static size_t entry_count = 0;
static size_t extern_count = 0;
static char ***entry_list =  NULL;
static char ***extern_list = NULL;
static char ***label_list = NULL;
static char ***unknown_list = NULL; 
static FILE* first_pass_file = NULL;

/*–– Helper to trim left/right in place ––*/
static void trim(char *s) {
    char *p = s;
    while (*p == ' ') p++;
    memmove(s, p, strlen(p)+1);

    size_t len = strlen(s);
    while (len && (s[len-1] == ' ' || s[len-1] == '\n'))
        s[--len] = '\0';
}

void parser(FILE* file) {
    unsigned int IC = 100;
    unsigned char in_mcro = 0;
    char line[MAX_LINE];
    char current_mcro[32];
    size_t mcro_idx;

    while (fgets(line, MAX_LINE, file)) {
        trim(line);
        if (in_mcro) {
            if (mcroend(line)) {
                in_mcro = 0;
            } else {
                add_line_to_mcro(line, current_mcro);
            }
        }
        else if (is_mcro_def(line)) {
            in_mcro = 1;
            add_mcro(current_mcro, file);
        }
        else if (*line == ';' || *line == '\0') {
            /* skip comment or blank */
        }
        else if (is_extern(line)) {
            handle_extern(line);
        }
        else if (is_entry(line)) {
            handle_entry(line);
        }
        else if (is_label(line)) {
            handle_label(line, &IC);
        }
        else if (is_instruction(line)) {
            handle_instruction(line, &IC);
        }
        else if ((mcro_idx = is_macro_call(line)) != (size_t)-1) {
            handle_mcro(mcro_idx, &IC);
        }
        else {
            printf("[-] Error in line %u: %s\n", IC - 100, line);
            error = 1;
	    continue;
        }
	printf("%d, %s", IC, line);

    }

/* at the end of the preproceeing if we had a line like mov r3, L and we dont know what L is we would save it in the ***uknown and then   
we will look in the extern and labels if we dont see it there we will throw error and the program won't go to phash 2*/
/*code will be here*/


    /* clean up macro storage*/
    for (size_t i = 0; i < mcro_count; i++) {
        char **body = mcro_list[i];
        for (size_t j = 0; body[j] != NULL; j++) {
            free(body[j]);
        }
        free(body);
    }
    free(mcro_list);
}

/*–– Predicates ––*/
int mcroend(const char* line) {
    return strncmp(line, MCROEND, strlen(MCROEND)) == 0;
}
int is_extern(const char* line) {
    return strncmp(line, EXTERN, strlen(EXTERN)) == 0;
}
int is_entry(const char* line) {
    return strncmp(line, ENTRY, strlen(ENTRY)) == 0;
}
int is_label(const char* line) {
    char* label = strchr(line,':');
    if (label == NULL)
     return 0;
    return (label - line < 32 && label - line > 0);
}
int is_instruction(const char* line) {
	int i;
	int length = 4;
	char[5] instruction;
	for(i = 0;i<length;i++) 
		if
	for(i =0;i< NUM_OF_INSTRUCTIONS;i++)
		if(strcmp(list_of_instructions[i],instructions);
				return i;
        return 0;
}
int is_mcro_def(const char* line) {
    return strncmp(line, MCRO, strlen(MCRO)) == 0;
}
size_t is_macro_call(const char* line) {
    for (size_t i = 0; i < mcro_count; i++) {
        if (strcmp(line, mcro_list[i][0]) == 0) {
            return i;
        }
    }
    return (size_t)-1;
}

/*–– Macro‐management ––*/
void add_mcro(const char* line, FILE* file) {
    char* copy_line  = strdup(line);
    char label_line[81];
    int n = 0;
    strcpy(label_line,copy_line+strlen(MCRO));
    free(copy_line);
    copy_line = NULL;
	trim(label_line);
	if(!isalpha(label_line[n])){
	    error = 1;
	    printf("[-] Erorr: the first letter of a label %s should be alphabatic",line);
	 
        return;
	}
	for(n = 1;n < 32;n++)
	{
        /*not number maybe a ' ' or diffrent char and will send an error in the next line*/
	    if(!isalnum(label_line[n]))
		    break;
	}
        char label[32];
        strncpy(label,label_line,n);
	for(;n < strlen(label_line);n++)
	{
        /* we starting a comment so we dont care if the code have some data at the end of the line if it's in a comment*/
            if(label_line[n] == ';')
            {
                break;
            } 
            /*data that's not in a comment */
            else if(label_line[n] != ' ')
            {
                printf("[-] Error: should have nothing in the same line in the entry line only the label and comments");
                consume_mcro(file);
	       
                return;
      		}
    }
    /* check duplicate */
    size_t i;
    for (i = 0; i < mcro_count; i++) {
        if (strcmp(mcro_list[i][0], label) == 0) {
            printf("[-] Error: mcro label name `%s` is already taken\n", label);
            consume_mcro(file);
            error = 1;
           
            return;
        }
    }
    for(i = 0;i < label_count;i++)
    {
	    if (strcmp(label_list[i][0], label) == 0) {
            printf("[-] Error: mcro label name `%s` is already taken\n", label);
            consume_mcro(file);
            error = 1;
           
            return;
    
     }
    }
    /* new macro header + terminator slot */
    char **new_m = malloc(2 * sizeof *new_m);
    if (!new_m) { perror("malloc"); exit(EXIT_FAILURE); }
    new_m[0] = strdup(label);
    new_m[1] = NULL;

    /* append to mcro_list */
    mcro_count++;
    mcro_list = realloc(mcro_list, mcro_count * sizeof *mcro_list);
    if (!mcro_list) { perror("realloc"); exit(EXIT_FAILURE); }
    mcro_list[mcro_count - 1] = new_m;
  
}

void add_line_to_mcro(const char* line, const char* current_mcro) {
    
    /* find the right macro */
    size_t idx = (size_t)-1;
    for (size_t i = 0; i < mcro_count; i++) {
        if (strcmp(mcro_list[i][0], current_mcro) == 0) {
            idx = i;
            break;
        }
    }
    if (idx == (size_t)-1) return; /* shouldn’t happen */

    /* count existing lines */
    char **body = mcro_list[idx];
    size_t len = 0;
    while (body[len] != NULL) len++;

    /* extend by one more line + NULL */
    body = realloc(body, (len + 2) * sizeof *body);
    if (!body) { perror("realloc"); exit(EXIT_FAILURE); }

    body[len]     = strdup(line);
    body[len + 1] = NULL;
    mcro_list[idx] = body;
}

/*–– Stub handlers (fill these in next) ––*/
void handle_extern(const char *line)
{
 	char* copy_line = strdup(line);
	char label_line[81];
	int n = 0;

	strncpy(label_line,copy_line+strlen(EXTERN),strlen(label_line));
    free(copy_line);
    copy_line = NULL;
	trim(label_line);
	if(!isalpha(label_line[n])){
	    error = 1;
	    printf("[-] Erorr: the first letter of a label %s should be alphabatic",line);
	  
        return;
	}
	for(n = 1;n < 32;n++)
	{
	    if(!isalnum(label_line[n]))
		    break;
	}
        char label[32];
        strncpy(label, label_line,n);
	for(;n < strlen(label_line);n++)
	{
        /* we starting a comment so we dont care if the code have some data at the end of the line if it's in a comment*/
            if(label_line[n] == ';')
            {
                break;
            } 
            /*data that's not in a comment */
            else if(label_line[n] != ' ')
            {
                printf("[-] Error: should have nothing in the same line in the entry line only the label and comments");
	        
                return;
      		}
    }
    /*check if we already did .extren for that label*/
    size_t i;
    for (i = 0; i <extern_count ; i++) {
        if (strcmp(extern_list[i][0], label_line) == 0) {
            printf("[-] Error: did extern twice for label %s", label_line);
            error = 1;
           
            return;
        }
    }
    /* new macro header + terminator slot */
    char **new_m = malloc(2 * sizeof *new_m);
    if (!new_m) { perror("malloc"); exit(EXIT_FAILURE); }
    new_m[0] = strdup(label);
    new_m[1] = NULL;

    /* append to mcro_list */
    extern_count++;
    extern_list = realloc(extern_list, extern_count * sizeof *extern_list);
    if (!extern_list) { perror("realloc"); exit(EXIT_FAILURE); }
    extern_list[extern_count - 1] = new_m;



 /*…*/
}
void handle_entry(const char *line)
{
	char* copy_line = strdup(line);
	char label_line[81];
	int n;
    /*could use strcpy since the line is char[81] and by adding 6 to it's pointer we have a string with 74 chars
    but wanted to have good code practice */
	strncpy(label_line,copy_line+strlen(ENTRY),strlen(label_line));
    free(copy_line);
    copy_line = NULL;
	trim(label_line);
	if(!isalpha(label_line[0])){
	    error = 1;
	    printf("[-] Erorr: the first letter of a label %s should be alphabatic",line);
	  
        return;
	}
	for(;n < 32;n++)
	{
            /*should fail on :, anything else is an error*/
	    if(!isalnum(label_line[n]))
		    break;
	}
	for(;n <strlen(label_line) ;n++)
	{
		/*a commnet*/
		if (label_line[n] == ';')
			break;
	        if(label_line[n] != ' ')
            {
               		printf("[-] Error: should have nothing in the same line in the entry line only the label and comments");
			        
                    return;
            }
      	}

	strncpy(label_line,label_line,n);
	/*finish this code*/
        char **new_m = malloc(2 * sizeof *new_m);
        if (!new_m) { perror("malloc"); exit(EXIT_FAILURE); }
        new_m[0] = strdup(label_line);
        new_m[1] = NULL;

    /* append to mcro_list */
    entry_count++;
    entry_list = realloc(entry_list, entry_count * sizeof *entry_list);
    if (!entry_list) { perror("realloc"); exit(EXIT_FAILURE); }
    entry_list[entry_count - 1] = new_m;





	/*…*/
}
void handle_label(const char *line, unsigned int *IC)     
{
	int n = 0;
	char* copy_line = strdup(line);
        char label[MAX_LINE];
	char* instruction = NULL;\
        strncpy(label,copy_line,MAX_LINE);
        free(copy_line);
        copy_line = NULL;
        trim(label);
        if(!isalpha(label[0])){
	    error = 1;
	    printf("[-] Erorr: the first letter of a label %s should be alphabatic",line);
	  return;
    }

    for(;n < 32;n++)
    {
	    if(!isalnum(label[n]))
		    break;
    }
    if(copy_line[n] != ':'){
	    printf("[-] line %u Error: the ':' should be next to the last letter of the label\n%s",*IC - 100 , line);
       return;
    }
    char **new_m = malloc(2 * sizeof *new_m);
    if (!new_m) { perror("malloc"); exit(EXIT_FAILURE); }
    new_m[0] = strndup(label,n);
    new_m[1] = NULL;

    /* append to mcro_list */
    label_count++;
    label_list = realloc(label_list, label_count * sizeof *label_list);
    if (!extern_list) { perror("realloc"); exit(EXIT_FAILURE); }
    label_list[label_count - 1] = new_m;

    instruction = strdup(label+n+1);
    trim(instruction);
    handle_instruction(instruction,IC);
}
void handle_instruction(const char *line, unsigned int *IC) 
{
	/*finish this*/
	int count  = count_register(line);
	/*should never happen*/
	if(count == -1)
	{
		printf("Error: unknown instruction in line %d",*IC);
		error = 1;
	}
	if(count == 3)
	{
		handle_data(line,&IC)
	}
	else
	{
		
	}
}
void handle_mcro(size_t index, unsigned int *IC){ /*…*/ }

/*–– the function is used when we get an error for an already used or any error relate for a mcro ––*/
void consume_mcro(FILE *file) {
    char buf[81];
    while (fgets(buf, sizeof buf, file)) {
        trim(buf);
        if (mcroend(buf)) break;
    }
}
void add_extern_location(int *IC,const char* extern_label);
void add_entry_loction();
void handle_data(const char* line, unsigned int *IC )
{
	char data[MAX_LINE];
	int i , count = 0;
	char* string  = strstr(line,".string"); 
	if(string != NULL)
	{
		
		strncpy(data,string+strlen(".string"),MAX_LINE);
		trim(data);
		if(!(line[0] == '"'))
		{
			printf("Error in line %d, .string data type must start with \"",*IC);
			error=1
			return;
		}
		
		for(;i< strlen(line);i++)
		{
			if(line[i] == '"')
			{
				count++;
				(*IC) += count;
				return;
			}
			else
			{
				count++;
			}
		}
		if(line[strlen(line)-1] != '"')
		{
			printf("Error in line %d: should have \" at the end of a .string data type",*IC);
			error = 1;
			return;
		}
	}

	
	
	else
	{
		char* data_string = strstr(line,".data");
		strncpy(data,data_string+strlen(".data"),MAX_LINE);
		trim(data);
		count = parse_and_count_integers(data);
		if(count == -1)
		{
			printf("In line %d",*IC);
			error =1;
			return;
		}
		(*IC) += count;
	}

}
int parse_and_count_integers(const char *input) {
    int count = 0;
    const char *ptr = input;
    char temp[20];

    while (*ptr) {
        /* Find end of current number (comma or end of string)*/
        const char *start = ptr;
        const char *end = strchr(ptr, ',');
        if (!end) end = ptr + strlen(ptr);

        /*Copy token to temp buffer*/
        int len = end - start;
        if (len == 0 || len >= sizeof(temp)) {
            printf("Error: Empty or too long number.\n");
            return -1;
        }
        strncpy(temp, start, len);
        temp[len] = '\0';

        /* Validate token*/
        char *p = temp;
        if (*p == '+' || *p == '-') p++;
        if (!*p) {
            printf("Error: Sign with no digits in '%s'\n", temp);
            return -1;
        }
        while (*p) {
            if (!isdigit(*p)) {
                printf("Error: Invalid character in '%s'\n", temp);
                return -1;
            }
            p++;
        }

        count++;

        /* Move to next token if any*/
        ptr = *end ? end + 1 : end;
    }

    return count;
}

int count_registers(const char* line)
{
	int i, count = 0;
	if(strstr(line,".data") != NULL || strstr(line,".string"))
		return 3;
	for(;i<strlen(instruction_list);i++)
	{
		if(strstr(line,instruciton_list[i][0]) != NULL)
			return instruction_list[i][1];
	}

	return -1;
}
