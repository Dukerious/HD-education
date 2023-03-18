// COMP2521 Assignment 1

// Written by: (z5306254)Sicong GAO
// Date: 11/10/2021

#ifndef UTILITY_H
#define UTILITY_H

#define NAME_MAX 4096
// check file whether just is '..' or '.'
bool check_implicit(char *path, char **f_type);

// cut a token from path,
// which is the first element in a relative path.
// such as bin/ls, which will store bin in name[NAME_MAX + 1].
void find_name(char *path, char name[NAME_MAX + 1]);

// cut a new relative path(cut off the first element).
// such as bin/jas/ls, which will store jas/ls in name[NAME_MAX + 1].
void find_relative_path(char *path, char name[NAME_MAX + 1]);

// find a absolute path(splice the path of Cwd and path in argument).
// about a file in fs.
void find_absolute_path(char absolute_path[NAME_MAX + 1], char *cur_path, char *path);

// modify a path for implicit file '.' and '..'
// path[NAME_MAX] is a absolute path, and we need deal with 
// some implicit file problems.
void modify_path(char path[NAME_MAX]);

// modify '.' file in path.
void modify_dot_path(char temp1[NAME_MAX + 1], char path[NAME_MAX + 1]);

// modify '..' file in path.
bool modify_double_dot_path(char temp2[NAME_MAX + 1], char path[NAME_MAX + 1]);

// compare two strings.
// if same return true, otherwise return false.
bool compare(char *item, char *file_name);

// return the number of elements in a path.
// such as bin/jas, which will return 2.
int find_position(char *path);

// compare two strings, if item > file_name, will return 
// true, otherwise will return false.
bool compare_insert(char *item, char *file_name);
#endif

