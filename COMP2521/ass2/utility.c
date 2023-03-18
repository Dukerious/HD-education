#include "utility.h"
static char **tokenize(char *s, char *separators);

char **find_collect_pages(char *file_name) {
    FILE *f = fopen(file_name, "rb");
	if(f == NULL) {
        fprintf(stderr, "invalid file\n");
        exit(EXIT_FAILURE);
    }
    // find the size of this file.
    long size = file_size(f);

    // store all characters into collection array.
    char collection[size + 1];
    int i = 0;
    int c;
    while ((c = fgetc(f)) != EOF) {
        collection[i] = c;
        i++;
    }
    collection[i] = '\0';
    fclose(f);
    // use tokenize function to splitting all page names. 
    return tokenize(collection, "\n ");
}

char **FindOutlink(char *file_name) {
	FILE *f = fopen(file_name, "rb");
	if(f == NULL) {
        fprintf(stderr, "invalid file!\n");
        exit(EXIT_FAILURE);
    }
    // count the num of char in this file.
    long size = file_size(f);

    char collection[size + 1];
    // Note: after the "#start Section-1" is our pages name,
    // which are 16 chars.
    fseek(f, 16, SEEK_SET);
    int i = 0;
    int c;
    // store all page names into collection array.
    while ((c = fgetc(f)) != '#') {
        collection[i] = c;
        i++;
    }
    collection[i] = '\0';
    fclose(f);
    return tokenize(collection, "\n ");
}

static char **tokenize(char str[], char *sep) {
    int capacity = MAX_NAME;
    // calloc enough space for point to all pages.
    char **pages = calloc(MAX_NAME, sizeof (*pages));
    // splitting all pages name into a 2D array.
    char *token = strtok(str, sep);
    int i;
    for (i = 0; token != NULL; i++) {
        // if we doesn't have enough space, just realloc.
        if(i == capacity) {
            capacity = 2 * i;
            pages = realloc(pages, capacity * sizeof(*pages));
        }
        // calloc the page name space.
        pages[i] = calloc(MAX_NAME, sizeof(char));
        strcpy(pages[i], token);
        token = strtok(NULL, sep);
    }
    // realloc space to avoid the memory overflow.
    pages = realloc(pages, (i + 1) * sizeof (*pages));
    pages[i] = NULL;
    return pages;
}

void free_tokens (char **tokens) {
	for (int i = 0; tokens != NULL && tokens[i] != NULL; i++)
		free (tokens[i]);
	free (tokens);
}

int num_pages(char **pages) {
    int i;
    for(i = 0; pages[i] != NULL; i++) {
    }
    return i;
}

//////////////////////////////////////////////////
// Part 2
// return the array of string for each line in a file.
char **tokenizeFile(FILE *f, char *sep, long size) {
    char part[size + 1];
    int i = 0;
    int c;
    // until we meet the end of a line.
    while ((c = fgetc(f)) != '\n') {
        if(c == EOF) break;
        part[i++] = c;
    }
    part[i] = '\0';
    return tokenize(part, sep);
}

// return the size of a file.
long file_size(FILE *f) {
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    return size;
}

// debugging function:
void print_token(char **tokens) {
    for(int i =0; tokens[i] != NULL; i++) {
        printf(".%s.\n", tokens[i]);
    }
}