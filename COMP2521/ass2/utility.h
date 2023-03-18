#ifndef UTILITY_H
#define UTILITY_H
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "graph.h"

#define MAX_NAME 4096

// open the collection file and find all pages.
char **find_collect_pages(char *file_name);

// open page file and find all outlink pages.
char **FindOutlink(char *file_name);

// count the num of pages.
int num_pages(char **pages);

void free_tokens (char **tokens);

long file_size(FILE *f);

char **tokenizeFile(FILE *f, char *sep, long size);


// debugging function
void print_token(char **tokens);
#endif