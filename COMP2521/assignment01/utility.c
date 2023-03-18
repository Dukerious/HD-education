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
// check file whether is '..' or '.'
bool check_implicit(char *path, char **f_type) {
    char *str = strrchr(path, '/');
    // if path is a signal file name.
    if (str == NULL) {
        if(compare(path, "..")) {
            printf("%s: cannot create %s '..': File exists\n", f_type[0], f_type[1]);
            return true;
        }
        if(compare(path, ".")) {
            printf("%s: cannot create %s '.': File exists\n", f_type[0], f_type[1]);
            return true;
        }
        // if is a path.
    } else {
        if(compare(str, "/..")) {
            printf("%s: cannot create %s '..': File exists\n", f_type[0], f_type[1]);
            return true;
        }
        if(compare(str, "/.")) {
            printf("%s: cannot create %s '.': File exists\n", f_type[0], f_type[1]);
            return true;
        }  
    }
    return false;
}

// cut a token which is the first element in relative path.
void find_name(char *path, char name[NAME_MAX + 1]) {
    int index;
    // find the position for first element.
    for(index = 0; path[index] == '/'; index++) {
    }
    // cut the first element.
    int i = 0;
    while (path[index] != '/' && path[index] != '\0') {
        name[i] = path[index];
        i++;
        index++;
    }
    name[i] = '\0';
}
// cut a new relative path(cut off the first element).
void find_relative_path(char *path, char name[NAME_MAX + 1]) {
    // find the position for first element.
    int i;
    for(i = 0; path[i] == '/'; i++) {

    }
    // ignore the first element.
    int j;
    for(j = i; path[j] != '/'; j++) {

    }
    // cut the new relative path.
    int index;
    for(index = 0; path[j + index] != '\0'; index++) {
        name[index] = path[j + index];
    }
    name[index] = '\0';
}

// find a absolute path(splice the path of Cwd and path in argument).
void find_absolute_path(char absolute_path[NAME_MAX + 1], char *cur_path, char *path) {
    char brunch[NAME_MAX + 1];
    // if the cwd is not root and initial path is not an absolute path.
    if (cur_path != NULL && path != NULL && path[0] != '/') {
        strcpy(brunch, cur_path);
        strcat(brunch, "/");
        strcat(brunch, path);
    } else {
        if(path != NULL) {
            strcpy(brunch, path);
        }
    }
    // use strtok to deal with slashs
    // Consecutive forward slashes
    // are treated as if they were a single forward slash.
    strcpy(absolute_path, "/");
    char *token = strtok(brunch, "/");
    if(token != NULL) {
        strcat(absolute_path, token);
    }
    while (token != NULL) {
        token = strtok(NULL, "/");
        if (token == NULL) {
            break;
        }
        strcat(absolute_path, "/");
        strcat(absolute_path, token);
    }
}
// modify a path for implicit file '.' and '..'
// path[NAME_MAX] is an absolute path.
void modify_path(char path[NAME_MAX]) {
    while (1) {
        // the '..' or '.' before the root, we should ignore that.
        char temp1[NAME_MAX + 1];
        modify_dot_path(temp1, path);
        char *token = strtok(temp1, "/");
        while (token != NULL) {
            if (!compare(token, "..") && !compare(token, ".")) {
                break;
            }
            token = strtok(NULL, "/");
        }
        // Modify the path from the root.
        char temp2[NAME_MAX + 1];
        strcpy(temp2, "/");
        while (token != NULL) {
            strcat(temp2, token);
            token = strtok(NULL, "/");
            if (token == NULL) {
                break;
            }
            strcat(temp2, "/");
        }
        // modify '..' file in path.
        if(modify_double_dot_path(temp2, path)) {
            break;
        }
    }
        
}
// modify '.' file in path.
void modify_dot_path(char temp1[NAME_MAX + 1], char path[NAME_MAX + 1]) {
    // since '.' is refers back to the directory itself,
    // so we need ingore them in a path.
    strcpy(temp1, "/");
    char *token = strtok(path, "/");
    if(token != NULL) {
        strcat(temp1, token);
    }
    while (token != NULL) {
        token = strtok(NULL, "/");
        if (token == NULL) {
            break;
        }
        if(!compare(token,".")) {
            strcat(temp1, "/");
            strcat(temp1, token);
        }
    }
}
// modify '..' file in path.
bool modify_double_dot_path(char temp2[NAME_MAX + 1], char path[NAME_MAX + 1]) {
    strcpy(path, "/");
    char *t1 = strtok(temp2, "/");
    char *t2 = strtok(NULL, "/");
    char *t3 = strtok(NULL, "/");

    if(t1 == NULL) {
        return true;
    }
    else if(t2 == NULL) {
        strcat(path, t1);
        return true;
    }
    // we must assert t2 is not '..'
    else if (!compare(t2, "..")) {
        while (t3 != NULL) {
            if (compare(t3, "..")) {
                break;
            }
            strcat(path, t1);
            strcat(path, "/");
            t1 = t2;
            t2 = t3;
            t3 = strtok(NULL, "/");
        }
        // t3 is NULL can be indicate that we have finished modification.    
        if (t3 == NULL) {
            strcat(path, t1);
            strcat(path, "/");
            strcat(path, t2);
            return true;
        }
        // if t3 is '..' file, we need ingore t2 and splice t1.
        else if (compare(t3, "..")) {
            strcat(path, t1);
            while (t3 != NULL) {
                t3 = strtok(NULL, "/");
                if (t3 == NULL) {
                    break;
                }
                strcat(path, "/");
                strcat(path, t3);
            }
        }
        // if t2 is '..', we need ingore t1, and splice t3.
    } else {
        //t3 is not NULL, we need keep going.
        if(t3 != NULL) {
            strcat(path, t3);
            while(t3 != NULL) {
                t3 = strtok(NULL, "/");
                if (t3 == NULL) {
                    break;
                }
                strcat(path, "/");
                strcat(path, t3);
            }
        } else {
            return true;
        }
    }
    return false;
}

// compare two strings.
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

// return the number of elements in a path.
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



