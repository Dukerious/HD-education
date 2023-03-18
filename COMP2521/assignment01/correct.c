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
void FsMkfs(Fs fs, char *path, char *file_type, char absolute_path[PATH_MAX + 1]);
Node do_FsMkfs(Node cur, char *relative_path, char *path, char *absolute_path, char *file_type);
Node insertion(Node cur, char *relative_path, char *path, char *absolute_path, char *file_type);
Node new_fd(char *relative_path, char *absolute_path, char *file_type);
void do_FsTree(Node file, int depth);
int check_file_type(Node file);
Node search(Node cur, char *relative_path, char *path);
void do_FsLs(Node cur);

// the struct of root in fs.
// first_child: the first element under root
// Cwd: the current working directory.
struct FsRep {
    Node first_child, Cwd;
    FileType type;
    char absolute_path[PATH_MAX + 1];
    char file_name[PATH_MAX + 1];
};

// the stuct of elements in fs.
// first_child: the first element under a element
// next_sibling: the next element which in the same generation.
struct Node {
    FileType type;
    char absolute_path[PATH_MAX + 1];
    char file_name[PATH_MAX + 1];
    Node first_child, next_sibling;
};
// create a root in fs.
Fs FsNew(void) {
    Fs new = malloc(sizeof (struct FsRep));
    new -> first_child = new -> Cwd = NULL;
    new -> type = DIRECTORY;
    strcpy(new -> file_name, "/");
    strcpy(new -> absolute_path, "/");
    return new;
}

// create a new file or directory.
Node new_fd(char *relative_path, char *absolute_path, char *file_type) {
    Node new = malloc(sizeof(struct Node));

    //store absolute_path and file name.
    strcpy(new -> absolute_path, absolute_path);
    char file_name[PATH_MAX + 1];
    find_name(relative_path, file_name);
    strcpy(new -> file_name, file_name);

    new -> first_child = new -> next_sibling = NULL;
    // set up file_type.
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
    // cur is NULL, which means that the cwd is root.
    if(cur == NULL) {
        strcpy(cwd, fs -> absolute_path);
    } else {
        strcpy(cwd, cur -> absolute_path);
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

// create a new directory
void FsMkdir(Fs fs, char *path) {
    char absolute_path[PATH_MAX + 1];
    FsMkfs(fs, path, "mkdir", absolute_path);
}

// find a position for insert a new file or directory.
void FsMkfs(Fs fs, char *path, char *file_type, char absolute_path[PATH_MAX + 1]) {
    if(fs == NULL) {
        return;
    }
    Node cur = fs -> Cwd;
    // the cwd is root.
    if(cur == NULL) {
        // store absolute_path in a string.
        find_absolute_path(absolute_path, NULL, path);
        modify_path(absolute_path);
        //printf("the ab path is %s\n", absolute_path);
        // the position of new element is just under root, so directly doing insertion. 
        if(find_position(path) == 1) {    
            fs -> first_child = insertion(fs->first_child, path, path, absolute_path, file_type);
            return;
        }
        // find the position of insertion.
        fs -> first_child = do_FsMkfs(fs->first_child, path, path, absolute_path, file_type);
    } 
    // the cwd is not root.
    if(cur != NULL) {
        // the absolute path is (the path of cwd) + (path). 
        find_absolute_path(absolute_path, cur -> absolute_path, path);
        modify_path(absolute_path);
        //printf("the ab path is %s\n", absolute_path);
        if(find_position(path) == 1) {  
            cur -> first_child = insertion(cur->first_child, path, path, absolute_path, file_type);
            return;
        }
        cur -> first_child = do_FsMkfs(cur->first_child, path, path, absolute_path, file_type);
    }
}

// find the position of insertion.
Node do_FsMkfs(Node cur, char *relative_path, char *path, char *absolute_path, char *file_type) {
    // cur is NULL, can not find that directory.
    if(cur == NULL) {
        printf("%s: cannot create directory '%s': No such file or directory\n", file_type, path);
        return NULL;
    }
    // the parent compare a directory.
    char parent[PATH_MAX + 1];    
    find_name(relative_path, parent);
    bool result = compare(cur->file_name, parent);
    // can not find a directory.
    if(!result) {
        if (cur -> next_sibling == NULL) {
            printf("%s: cannot create directory '%s': No such file or directory\n", file_type, path);
            return cur;
        }
        // going to next sibling.
        cur -> next_sibling = do_FsMkfs(cur->next_sibling, relative_path, path, absolute_path, file_type);
    }
    // find a new relative path.
    char new_path[PATH_MAX + 1];
    find_relative_path(relative_path, new_path);
    // if we meet a file.
    if(find_position(relative_path) != 2 && result) {
        if(check_file_type(cur) == 0) {
            printf("%s: cannot create directory '%s': Not a directory\n", file_type, path);
            return cur;
        }
        // going to next generation.
        cur -> first_child = do_FsMkfs(cur->first_child, new_path, path, absolute_path, file_type);
    }
    // find the insert position and doing insert.
    if(find_position(relative_path) == 2 && result) {
        // if the new element is a file.
        if(check_file_type(cur) == 0) {
            printf("%s: cannot create directory '%s': Not a directory\n", file_type, path);
            return cur;
        }
        cur -> first_child = insertion(cur->first_child, new_path, path, absolute_path, file_type);
    }
    return cur;
}
// inserting a new file or directory.
Node insertion(Node cur, char *relative_path, char *path, char *absolute_path, char *file_type) {
    // if have not any sibling, directly to insert.
    if (cur == NULL) {
        return new_fd(relative_path, absolute_path, file_type);
    }
    // match sibling, if same print error.
    char sibling[PATH_MAX + 1];
    find_name(relative_path, sibling);
    if(compare(sibling, cur->file_name)) {
        printf("%s: cannot create directory '%s': File exists\n", file_type, path);
        return cur;
    }
    // if new element greater than sibling then going to next, this step for ordering.
    bool comp_result = compare_insert(sibling, cur -> file_name);
    if(comp_result) {
        cur -> next_sibling = insertion(cur->next_sibling, relative_path, path, absolute_path, file_type);
    }
    // doing insert and in order
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
// create a new file.
void FsMkfile(Fs fs, char *path) {
    char absolute_path[PATH_MAX + 1];
    FsMkfs(fs, path, "mkfile", absolute_path);
}

// change directory.
void FsCd(Fs fs, char *path) {
    // if new cwd is root.
    if (path == NULL || compare(path, "/")) {
        fs -> Cwd = NULL;
        return;
    }
    Node first = NULL;
    // if we don't have a cwd before.
    if (fs -> Cwd == NULL) {
        first = fs -> first_child; 
        // if we have a cwd before.
    } else {
        first = fs -> Cwd -> first_child;
    }
    // search the position of new cwd.
    Node cur = search(first, path, path);
    if (cur == NULL) {
        printf("cd: '%s': No such file or directory\n", path);
    }
    else if (cur -> type == REGULAR_FILE) {
        printf("cd: '%s': Not a directory\n", path);
        // update new cwd.
    } else {
        fs -> Cwd = cur;
    }
}

// search a element in fs and return that.
Node search(Node cur, char *relative_path, char *path) {
    if(cur == NULL) {
        return NULL;
    }
    // matching branch and through.
    char parent[PATH_MAX + 1];    
    find_name(relative_path, parent);
    bool comp_result = compare(parent, cur->file_name);
    // if not match keep going to match sibling.
    if(!comp_result) {
        return search(cur -> next_sibling, relative_path, path);
    } 
    else if(comp_result){
        // if match result is a file
        if(cur -> type == REGULAR_FILE) {
            return NULL;
        }
        int position = find_position(relative_path);
        // if not the last generation.
        // keeping search child.
        if(position != 1) {
            char new_path[PATH_MAX + 1];
            find_relative_path(relative_path, new_path);
            Node find = search(cur -> first_child, new_path, path);
            return find;
        }
    }
    return cur;
}

void FsLs(Fs fs, char *path) {
    if (fs == NULL) {
        return;
    }
    Node first = NULL;
    // list the files in cwd.
    if (path == NULL || compare(path, "/")) {
        if(fs -> Cwd == NULL) {
            first = fs -> first_child;
        } else {
            first = fs -> Cwd -> first_child;
        }
        do_FsLs(first);
        return;
    }
    // list the files by path.
    // search ls position.
    Node ls = search(first, path, path);
    if(ls == NULL) {
        printf("ls: cannot access '%s': No such file or directory\n", path);
    } 
    // path not refers to a directory.
    else if (ls -> type == DIRECTORY) {
        do_FsLs(ls);
        // path refers to a file.
    } else {
        char *ls_path = strrchr(path, '/');
        char ls_name[PATH_MAX + 1];
        find_name(ls_path, ls_name);
        if (compare(ls_name, ls -> file_name)) {
            listFile(ls_name, REGULAR_FILE);
        } else {
            printf("ls: cannot access '%s': Not a directory", path);
        }
    } 
}
// print files in cwd.
void do_FsLs(Node cur) {
    if(cur == NULL) {
        return;
    }
    listFile(cur->file_name, cur->type);
    putchar('\n');
    do_FsLs(cur -> next_sibling);
}
// print cwd.
void FsPwd(Fs fs) {
    char cwd[PATH_MAX + 1];
    FsGetCwd(fs, cwd);
    printf("%s\n", cwd);
}
// print fs.
void FsTree(Fs fs, char *path) {
    // print files under root.
    if(path == NULL || compare(path, "/")) {
        listFile(fs->file_name, fs->type);
        printf("\n");
        do_FsTree(fs -> first_child, 1);
        return;
    }
    // search a directory and will print files under this.
    Node root = search(fs -> first_child, path, path);
    // can not search
    if (root == NULL) {
        printf("tree: '%s': No such file or directory\n", path);
        return;
    }
    // path refers to a file.
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
    // Indentation increase by 4 spaces per level.
    int i;
    for (i = 0; i < depth; i++) {
        printf("\t");
    }
    listFile(file->file_name, file->type);
    printf("\n");
    if(file -> first_child != NULL) {
        do_FsTree(file -> first_child, depth+1);
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

