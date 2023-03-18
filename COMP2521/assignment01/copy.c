// Implementation of the File System ADT
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
#include "Fs.h"
#include "utility.h"
typedef struct Node *Node;
void do_FsFree(Node item);
void FsMkfs(Fs fs, char *path, char *file_type);
Node do_FsMkfs(Node cur, char *relative_path, char *path, char *absolute_path, char *file_type);
Node insertion(Node cur, char *relative_path, char *path, char *absolute_path, char *file_type);
Node new_fd(char *relative_path, char *absolute_path, char *file_type);
void do_FsTree(Node file, int depth);
int check_file_type(Node file);
Node search(Node cur, char *path);
void do_FsLs(Node cur);
struct FsRep {
    Node first_child, Cwd;
    FileType type;
    char *path_name;
    char *file_name;
};
struct Node {
    FileType type;
    char *path_name;
    char *file_name;
    Node first_child, next_sibling;
};
Fs FsNew(void) {
    Fs new = malloc(sizeof (struct FsRep));
    new -> first_child = new -> Cwd = NULL;
    new -> type = DIRECTORY;
    new -> path_name = new -> file_name = "/";
    return new;
}

Node new_fd(char *relative_path, char *absolute_path, char *file_type) {
    Node new = malloc(sizeof(struct Node));
    //printf("here\n");
    new -> path_name = absolute_path;
    new -> file_name = find_name(relative_path);
    // printf("the absolute path is %s nad file_name is %s\n", new -> path_name, new -> file_name);
    new -> first_child = new -> next_sibling = NULL;
    if(compare("mkdir", file_type)) {
        new -> type = DIRECTORY;
    } else {
        new -> type = REGULAR_FILE;
    }
    return new;
}
void FsGetCwd(Fs fs, char cwd[PATH_MAX + 1]) {
    assert(fs != NULL);
    Node cur = fs -> Cwd;
    if(cur == NULL) {
        strcpy(cwd, fs -> path_name);
    } else {
        strcpy(cwd, cur -> path_name);
    }
}
void FsFree(Fs fs) {
    do_FsFree(fs -> first_child);
    free(fs);
}
void do_FsFree(Node item) {
    if (item == NULL) {
        return;
    }
    do_FsFree(item -> next_sibling);
    do_FsFree(item -> first_child);
}
void FsMkdir(Fs fs, char *path) {
    FsMkfs(fs, path, "mkdir"); 
   
}
void FsMkfile(Fs fs, char *path) {
    FsMkfs(fs, path, "mkfile"); 
}
void FsMkfs(Fs fs, char *path, char *file_type) {
    if(fs == NULL) {
        return;
    }
    Node cur = fs -> Cwd;
    //printf("the path is %s\n", path);    
    if(cur == NULL) {
        if(find_position(path) == 1) { 
            //printf("the path is %s\n", path);     
            fs -> first_child = insertion(fs->first_child, path, path, path, file_type);
            //printf("here\n");
            return;
        }
        //printf("the path is %s\n", path);    
        fs -> first_child = do_FsMkfs(fs->first_child, path, path, path, file_type);
    } 
    if(cur != NULL) {
        char absolute_path[PATH_MAX + 1];
        strcpy(absolute_path, cur -> path_name);
        strcat(absolute_path, "/");
        strcat(absolute_path, path);
        //printf("the absolute path is %s\n", absolute_path);  
        if(find_position(path) == 1) { 
                //printf("here\n");     
                cur -> first_child = insertion(cur->first_child, path, path, absolute_path, file_type);
                //printf("here\n");
                return;
            }
            cur -> first_child = do_FsMkfs(cur->first_child, path, path, absolute_path, file_type);
        }
}

Node do_FsMkfs(Node cur, char *relative_path, char *path, char *absolute_path, char *file_type) {
    if(cur == NULL) {
        printf("%s: cannot create directory '%s': No such file or directory\n", file_type, path);
        return NULL;
    }    
    char *parent = find_name(relative_path);
    bool result = compare(cur->file_name, parent);
    if(!result) {
        if (cur -> next_sibling == NULL) {
            printf("%s: cannot create directory '%s': No such file or directory\n", file_type, path);
            free(parent);
            return cur;
        }
        cur -> next_sibling = do_FsMkfs(cur->next_sibling, relative_path, path, absolute_path, file_type);
    }
    char *new_path = find_relative_path(relative_path);
    if(find_position(relative_path) != 2 && result) {
        if(check_file_type(cur) == 0) {
            printf("%s: cannot create directory '%s': Not a directory\n", file_type, path);
            free(parent);
            free(new_path); 
            return cur;
        }
        cur -> first_child = do_FsMkfs(cur->first_child, new_path, path, absolute_path, file_type);
    }
    if(find_position(relative_path) == 2 && result) {
        if(check_file_type(cur) == 0) {
            printf("%s: cannot create directory '%s': Not a directory\n", file_type, path);
            free(parent);
            free(new_path); 
            return cur;
        }
        cur -> first_child = insertion(cur->first_child, new_path, path, absolute_path, file_type);
    }
    free(parent);
    free(new_path);
    return cur;
}
Node insertion(Node cur, char *relative_path, char *path, char *absolute_path, char *file_type) {
    if (cur == NULL) {
        return new_fd(relative_path, absolute_path, file_type);
    }
    char *sibling = find_name(relative_path);
    if(compare(sibling, cur->file_name)) {
        printf("%s: cannot create directory '%s': File exists\n", file_type, path);
        return cur;
    }
    bool comp_result = compare_insert(sibling, cur -> file_name);
    //printf("the compare is %s and %s, the value is %d\n", sibling, cur->file_name, comp_result);
    if(comp_result) {
        cur -> next_sibling = insertion(cur->next_sibling, relative_path, path, absolute_path, file_type);
    }
    if(!comp_result) {
        Node insert = new_fd(relative_path, absolute_path, file_type);
        insert -> next_sibling = cur;
        return insert;
    }
    return cur;
}
int check_file_type(Node file) {
    if(file ->type == REGULAR_FILE) {
        return 0;
    }
    return 1;
}

void FsCd(Fs fs, char *path) {
    if (path == NULL || compare(path, "/")) {
        fs -> Cwd = NULL;
        return;
    }
    Node first = NULL;
    if (fs -> Cwd == NULL) {
        first = fs -> first_child; 
    } else {
        first = fs -> Cwd -> first_child;
    }
    Node cur = search(first, path);
    if (cur == NULL) {
         printf("cd: '%s': No such file or directory\n", path);
    }
    else if (cur -> type == REGULAR_FILE) {
        printf("cd: '%s': Not a directory\n", path);
    } else {
        fs -> Cwd = cur;
    }
}
Node search(Node cur, char *path) {
    if(cur == NULL) {
        return NULL;
    }
    char *parent = find_name(path);
    bool comp_result = compare(parent, cur->file_name);
    //printf("the cur is %s\n", cur -> file_name);
    if(!comp_result) {
        free(parent);
        return search(cur -> next_sibling, path);
    } 
    else if(comp_result){
        if(cur -> type == REGULAR_FILE) {
            free(parent);
            return NULL;
        }
        int position = find_position(path);
        if(position != 1) {
            char *new_path = find_relative_path(path);
           Node find = search(cur -> first_child, new_path);
            free(parent);
            free(new_path);
            return find;
        }
    }
    free(parent);
    return cur;
}
void FsLs(Fs fs, char *path) {
    if (fs == NULL) {
        return;
    }
    Node first = NULL;
    if (path == NULL || compare(path, "/")) {
        if(fs -> Cwd == NULL) {
            first = fs -> first_child;
        } else {
            first = fs -> Cwd -> first_child;
        }
        do_FsLs(first);
        return;
    }
    Node ls = search(first, path);
    if(ls == NULL) {
        printf("ls: cannot access '%s': No such file or directory\n", path);
    } 
    else if (ls -> type == DIRECTORY) {
        do_FsLs(ls);
    } else {
        char *ls_path = strrchr(path, '/');
        char *ls_name = find_name(ls_path);
        if (compare(ls_name, ls -> file_name)) {
            listFile(ls_name, REGULAR_FILE);
        } else {
            printf("ls: cannot access '%s': Not a directory", path);
        }
        free(ls_name);
    } 
}
void do_FsLs(Node cur) {
    if(cur == NULL) {
        return;
    }
    listFile(cur->file_name, cur->type);
    putchar('\n');
    do_FsLs(cur -> next_sibling);
}
void FsPwd(Fs fs) {
    char cwd[PATH_MAX + 1];
    FsGetCwd(fs, cwd);
    printf("%s\n", cwd);
}

void FsTree(Fs fs, char *path) {
    if(path == NULL || compare(path, "/")) {
        listFile(fs->file_name, fs->type);
        printf("\n");
        do_FsTree(fs -> first_child, 1);
        return;
    }
    Node first = NULL;
    if (fs -> Cwd == NULL) {
        first = fs -> first_child;
    } else {
        first = fs -> Cwd -> first_child;
    }
    //printf("the first is %s\n", first -> file_name);
    Node root = search(first, path);
    if (root == NULL) {
        printf("tree: '%s': No such file or directory\n", path);
        return;
    }
    if (root -> type == REGULAR_FILE) {
        printf("tree: '%s': Not a directory\n", path);
        return;
    } else {
        printf("%s\n", path);
        printf("\n");
        do_FsTree(root -> first_child, 1);
    }
}

void do_FsTree(Node file, int depth) {
    if(file == NULL) {
        return;
    }
    int i;
    for (i = 0; i < depth; i++) {
        printf("\t");
    }
    listFile(file->file_name, file->type);
    printf("\n");
    //printf("i'm %s\n", file->file_name);
    if(file -> first_child != NULL) {
        //printf("i'm %s\n", file->file_name);
       // printf("the child is %s, hello!\n", file -> first_child -> file_name);
        do_FsTree(file -> first_child, depth+1);
        //printf("hello!\n");
    }
    if(file -> next_sibling != NULL) {
        do_FsTree(file -> next_sibling, depth);
    }
}
void FsPut(Fs fs, char *path, char *content) {
    // TODO
}

void FsCat(Fs fs, char *path) {
    // TODO
}

void FsDldir(Fs fs, char *path) {
    // TODO
}

void FsDl(Fs fs, bool recursive, char *path) {
    // TODO
}

void FsCp(Fs fs, bool recursive, char *src[], char *dest) {
    // TODO
}

void FsMv(Fs fs, char *src[], char *dest) {
    // TODO
}

