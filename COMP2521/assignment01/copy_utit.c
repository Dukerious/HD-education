// COMP2521 Assignment 1

// Written by: (z5306254)Sicong GAO
// Date: 11/10/2021

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FileType.h"
#include "utility.h"
void find_name(char *path, cahr name[NAME_MAX + 1]) {
    //printf("%s\n", path);
    int index;
    for(index = 0; path[index] == '/'; index++) {
    }
    int i = 0;
    while (path[index] != '/' && path[index] != '\0') {
        name[i] = path[index];
        i++;
        index++;
    }
    name[i] = '\0';
    return name;
}
char *find_relative_path(char *path) {
    char *name = malloc(NAME_MAX);
    int i;
    for(i = 0; path[i] == '/'; i++) {

    }
    int j;
    for(j = i; path[j] != '/'; j++) {

    }
    int index;
    for(index = 0; path[j + index] != '\0'; index++) {
        name[index] = path[j + index];
    }
    name[index] = '\0';
    return name;
}

void find_absolute_path(char absolute_path[NAME_MAX + 1], char *cur_path, char *path) {
    char brunch[NAME_MAX + 1];
    if (cur_path != NULL) {
        strcpy(brunch, cur_path);
        strcat(brunch, "/");
        strcat(brunch, path);
    } else {
        strcpy(brunch, path);
    }
        //printf("the brunch is %s\n", brunch);
        char *token = strtok(brunch, "/");
        strcpy(absolute_path, "/");
        strcat(absolute_path, token);
        //printf("the ab path 1 is %s\n", absolute_path);
        while (token != NULL) {
            token = strtok(NULL, "/");
            if (token == NULL) {
                break;
            }
            strcat(absolute_path, "/");
            strcat(absolute_path, token);
        }
        //printf("here\n");
}
bool compare(char *item, char *file_name) {
    if(strcmp(item, file_name) == 0) {
        return true;
    }
    return false;
}
bool compare_insert(char *item, char *file_name) {
    if (strcmp(item, file_name) > 0) {
        return true;
    }
    return false;
}

int find_position(char *path) {
    int index = 0;
    int count = 0;
    while (path[index] != '\0') {
        while(path[index] != '\0' && path[index] == '/') {
            index++;
        }
        count++;
        while(path[index] != '\0' && path[index] != '/') {
            index++;
        }
    }
    return count;
}



