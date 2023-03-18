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
#define DNE 2
#define IS_FILE 0
#define INVALID 4
#define IS_DIRECTORY 3

// some helper functions.
void do_FsFree(Node item);
void free_node(Node item);
void FsMkfs(Fs fs, char *path, char **f_type, char absolute_path[PATH_MAX + 1]);
Node do_FsMkfs(Node cur, char *relative_path, char *path, char *absolute_path, char **f_type);
Node insertion(Node cur, char *relative_path, char *path, char *absolute_path, char **f_type);
Node new_fd(char *relative_path, char *absolute_path, char *f_type);
void do_FsTree(Node file, int depth);
int check_file_type(Node file);
Node search(Node cur, char *relative_path, char *path);
void path_modification(Fs fs, char absolute_path[PATH_MAX + 1], char *path);
Node search_file(Fs fs, char absolute_path[PATH_MAX + 1], char *path);
void do_FsLs(Node cur);
void do_FsPut(Node fsput, char *content);
int check_error(Node fsput, char *path);
void do_fsdl(Node cur);
void do_FsCp(Fs fs, Node cur, Node desti, char *cur_path);
// the struct of root in fs.
// first_child: the first element under root
// Cwd: the current working directory.
struct FsRep {
    Node first_child, Cwd;
    FileType type;
    char absolute_path[PATH_MAX + 1];
    char file_name[PATH_MAX + 1];
};

// the struct of elements in fs.
// first_child: the first element under a element
// next_sibling: the next element which in the same generation.
struct Node {
    FileType type;
    char absolute_path[PATH_MAX + 1];
    char file_name[PATH_MAX + 1];
    char *content;
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
Node new_fd(char *relative_path, char *absolute_path, char *f_type) {
    Node new = malloc(sizeof(struct Node));

    //store absolute_path and file name.
    strcpy(new -> absolute_path, absolute_path);
    char file_name[PATH_MAX + 1];
    find_name(relative_path, file_name);
    strcpy(new -> file_name, file_name);

    new -> first_child = new -> next_sibling = NULL;
    new -> content = NULL;
    // set up file_type.
    if(compare("mkdir", f_type)) {
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
    // free files which inside the root.
    do_FsFree(fs -> first_child);
    // free root.
    strcpy(fs->file_name, "");
    strcpy(fs->absolute_path, "");
    free(fs);
}
void do_FsFree(Node item) {
    // base case.
    if (item == NULL) {
        return;
    }
    // recursion.
    do_FsFree(item -> next_sibling);
    do_FsFree(item -> first_child);
    free_node(item);
}
void free_node(Node item) {
    // free content, if not an empty file.
    if(item -> content != NULL) {
        free(item -> content);
    }
    // free file_name and path.
    strcpy(item->file_name, "");
    strcpy(item->absolute_path, "");
    free(item);
}
// create a new directory
void FsMkdir(Fs fs, char *path) {
    char *f_type[] = {"mkdir", "directory", NULL};
    if(check_implicit(path, f_type)) {
        return;
    }
    char absolute_path[PATH_MAX + 1];
    FsMkfs(fs, path, f_type, absolute_path);
}

// find a position for insert a new file or directory.
void FsMkfs(Fs fs, char *path, char **f_type, char absolute_path[PATH_MAX + 1]) {
    if(fs == NULL) {
        return;
    }
    Node cur = fs -> Cwd;
    // the cwd is root.
    if(cur == NULL) {
        // store absolute_path in a string.
        find_absolute_path(absolute_path, NULL, path);
        modify_path(absolute_path);
    } 
    // the cwd is not root.
    if(cur != NULL) {
        // the absolute path is (the path of cwd) + (path). 
        find_absolute_path(absolute_path, cur -> absolute_path, path);
        modify_path(absolute_path);
    }
        // the position of new element is just under root, so directly doing insertion. 
    if(find_position(absolute_path) == 1) {
        fs -> first_child =
insertion(fs->first_child, absolute_path, path, absolute_path, f_type);
        return;
    }
    // find the position of insertion.
    fs -> first_child =
do_FsMkfs(fs->first_child, absolute_path, path, absolute_path, f_type);
}

// find the position of insertion.
Node do_FsMkfs(Node cur, char *relative_path, char *path, char *absolute_path, char **f_type) {
    // cur is NULL, can not find that directory.
    if(cur == NULL) {
        printf("%s: cannot create %s '%s': No such file or directory\n", f_type[0], f_type[1], path);
        return NULL;
    }
    // the parent compare a directory.
    char parent[PATH_MAX + 1];    
    find_name(relative_path, parent);
    bool result = compare(cur->file_name, parent);
    // can not find a directory.
    if(!result) {
        if (cur -> next_sibling == NULL) {
            printf("%s: cannot create %s '%s': No such file or directory\n", f_type[0], f_type[1], path);
            return cur;
        }
        // going to next sibling.
        cur -> next_sibling =
do_FsMkfs(cur->next_sibling, relative_path, path, absolute_path, f_type);
    }
    // find a new relative path.
    char new_path[PATH_MAX + 1];
    find_relative_path(relative_path, new_path);
    // if we meet a file.
    if(find_position(relative_path) != 2 && result) {
        if(check_file_type(cur) == 0) {
            printf("%s: cannot create %s '%s': Not a directory\n", f_type[0], f_type[1], path);
            return cur;
        }
        // going to next generation.
        cur -> first_child =
do_FsMkfs(cur->first_child, new_path, path, absolute_path, f_type);
    }
    // find the insert position and doing insert.
    if(find_position(relative_path) == 2 && result) {
        // if the new element is a file.
        if(check_file_type(cur) == 0) {
            printf("%s: cannot create %s '%s': Not a directory\n", f_type[0], f_type[1], path);
            return cur;
        }
        cur -> first_child =
insertion(cur->first_child, new_path, path, absolute_path, f_type);
    }
    return cur;
}
// inserting a new file or directory.
Node insertion(Node cur, char *relative_path, char *path, char *absolute_path, char **f_type) {
    // if have not any sibling, directly to insert.
    if (cur == NULL) {
        return new_fd(relative_path, absolute_path, f_type[0]);
    }
    // match sibling, if and print error.
    char sibling[PATH_MAX + 1];
    find_name(relative_path, sibling);
    if(compare(sibling, cur->file_name)) {
        printf("%s: cannot create %s '%s': File exists\n", f_type[0], f_type[1], path);
        return cur;
    }
    // if new element greater than sibling then going to next, this step for ordering.
    bool comp_result = compare_insert(sibling, cur -> file_name);
    if(comp_result || compare(cur->file_name, "")) {
        cur -> next_sibling =
insertion(cur->next_sibling, relative_path, path, absolute_path, f_type);
    }
    // doing insert and in order
    if(!comp_result) {
        Node insert = new_fd(relative_path, absolute_path, f_type[0]);
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
    char *f_type[] = {"mkfile", "file", NULL};
    if(check_implicit(path, f_type)) {
        return;
    }
    char absolute_path[PATH_MAX + 1];
    FsMkfs(fs, path, f_type, absolute_path);
}

// change directory.
void FsCd(Fs fs, char *path) {
    // cd to root.
    if(path == NULL) {
        fs -> Cwd = NULL;
        return;
    }
    char absolute_path[PATH_MAX + 1];
    // if we don't have a cwd before.
    Node cwd = fs -> Cwd;
    char *cur_path = NULL;
    if (cwd != NULL) {
        cur_path = cwd -> absolute_path;
    }
    // modify initial path to absolute path.
    find_absolute_path(absolute_path, cur_path, path);
    modify_path(absolute_path);
    // cd to root.
    if (compare(absolute_path, "/")) {
        fs -> Cwd = NULL;
        return;
    }
    // search the position of new cwd.
    Node cur = search(fs -> first_child, absolute_path, path);
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
    if(!comp_result || compare(parent, "")) {
        return search(cur -> next_sibling, relative_path, path);
    } 
    else if(comp_result){
        // if match result is a file
        if(cur -> type == REGULAR_FILE) {
            return cur;
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
    char absolute_path[PATH_MAX + 1];
    // check whether cwd is root.
    Node first = fs -> first_child;
    char *cur_path = NULL;
    if (fs -> Cwd != NULL) {
        cur_path = fs -> Cwd -> absolute_path;
        first = fs -> Cwd -> first_child;
    }
    // list the files in cwd.
    if(path == NULL) {
        do_FsLs(first);
        return;
    }
    // modify initial path to absolute path.
    find_absolute_path(absolute_path, cur_path, path);
    modify_path(absolute_path);
    // list the files in root.
    if (compare(absolute_path, "/")) {
        do_FsLs(fs -> first_child);
        return;
    }
    // list the files by path.
    // search ls position.
    Node ls = search(fs -> first_child, absolute_path, path);
    //printf("ls is %s\n", ls -> file_name);
    if(ls == NULL) {
        printf("ls: cannot access '%s': No such file or directory\n", path);
    } 
    // path not refers to a directory.
    else if (ls -> type == DIRECTORY) {
        do_FsLs(ls -> first_child);
        // path refers to a file.
    } else {
        char *ls_path = strrchr(path, '/');
        if (ls_path == NULL) {
            ls_path = path;
        }
        char ls_name[PATH_MAX + 1];
        find_name(ls_path, ls_name);
        // path refers to a file 
        if (compare(ls_name, ls -> file_name)) {
            printf("%s\n", path);
            // path is invalid
        } else {
            printf("ls: cannot access '%s': Not a directory\n", path);
        }
    } 
}
// print files in cwd.
void do_FsLs(Node cur) {
    if(cur == NULL) {
        return;
    }
    if(!compare(cur->file_name, "")) {
        listFile(cur->file_name, cur->type);
        putchar('\n');
    }
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
        if(!compare(fs->file_name, "")) {
            listFile(fs->file_name, fs->type);
            printf("\n");
        }
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
        do_FsTree(root -> first_child, 1);
    }
}

void do_FsTree(Node file, int depth) {
    // base case.
    if(file == NULL) {
        return;
    }
    // Indentation increase by 4 spaces per level.
    if(!compare(file->file_name, "")) {
        int i;
        for (i = 0; i < depth; i++) {
            printf("    ");
        }
        listFile(file->file_name, file->type);
        printf("\n");
    }
    // recursion.
    if(file -> first_child != NULL) {
        do_FsTree(file -> first_child, depth+1);
    }
    if(file -> next_sibling != NULL) {
        do_FsTree(file -> next_sibling, depth);
    }
}
void FsPut(Fs fs, char *path, char *content) {
    if (fs == NULL || path == NULL) {
        return;
    }
    char absolute_path[PATH_MAX + 1];
    // check the root.
    Node fsput = search_file(fs, absolute_path, path);
    if(compare(absolute_path, "/")) {
        printf("put: '%s': Is a directory\n", path);
        return;
    }
    // check fsput errors and print.
    int check_result = check_error(fsput, path);
    if(check_result == DNE) {
        printf("put: '%s': No such file or directory\n", path);
    }
    else if (check_result == IS_DIRECTORY) {
        printf("put: '%s': Is a directory\n", path);
    }
    else if(check_result == INVALID) {
        printf("put: '%s': Not a directory\n", path);
    }
    else if(check_result == IS_FILE) {
        do_FsPut(fsput, content);
    }
}
void do_FsPut(Node fsput, char *content) {
    if (content == NULL) {
        return;
    }
    // if not empty file, then overwritten.
    if (fsput -> content != NULL) {
        free(fsput -> content);
        fsput -> content = NULL;
    }
    // if an empty file, then calloc.
    fsput -> content = calloc(strlen(content) + 1, sizeof(char));
    strcpy(fsput -> content, content);
}
int check_error(Node fsput, char *path) {
    // cat the end of path.
    char *last = strrchr(path, '/');
    if (last == NULL) {
        last = path;
    }
    char name[PATH_MAX + 1];
    find_name(last, name);
    // file does not exist.
    if(fsput == NULL) {
        return DNE;
    }
    // find a directory.
    else if(fsput -> type == DIRECTORY) {
        return IS_DIRECTORY;
    }
    else if (fsput -> type == REGULAR_FILE) {
        // path is invalid.
        //A proper prefix of the path is a regular file.
        if (!compare(name, fsput -> file_name)) {
            return INVALID;
        }
    }
    // find a file.
    return IS_FILE;
}
// search a file and return, 
// and the file absolute path copy into absolute_path[PATH_MAX + 1].
Node search_file(Fs fs, char absolute_path[PATH_MAX + 1], char *path) {
    path_modification(fs, absolute_path, path);
    return search(fs -> first_child, absolute_path, path);
}
// modify a file path to absolute path.
void path_modification(Fs fs, char absolute_path[PATH_MAX + 1], char *path) {
    // check whether cwd is root.
    char *cur_path = NULL;
    if (fs -> Cwd != NULL) {
        cur_path = fs -> Cwd -> absolute_path;
    }
    // modify initial path to absolute path.
    find_absolute_path(absolute_path, cur_path, path);
    modify_path(absolute_path);
}

void FsCat(Fs fs, char *path) {
    if (fs == NULL || path == NULL) {
        return;
    }
    char absolute_path[PATH_MAX + 1];
    Node fsput = search_file(fs, absolute_path, path);
    // check the root
    if(compare(absolute_path, "/")) {
        printf("put: '%s': Is a directory\n", path);
        return;
    }
    // check fscat errors.
    int check_result = check_error(fsput, path);
    if(check_result == DNE) {
        printf("cat: '%s': No such file or directory\n", path);
    }
    else if (check_result == IS_DIRECTORY) {
        printf("cat: '%s': Is a directory\n", path);
    }
    else if(check_result == INVALID) {
        printf("cat: '%s': Not a directory\n", path);
    }
    else if(check_result == IS_FILE && fsput -> content != NULL) {
        printf("%s", fsput -> content);
    }
}

void FsDldir(Fs fs, char *path) {
    if (fs == NULL || path == NULL) {
        return;
    }
    //modify path to absolute path and search delete target.
    char absolute_path[PATH_MAX + 1];
    Node fsdldir = search_file(fs, absolute_path, path);
    int check_result = check_error(fsdldir, path);
    if(check_result == DNE) {
        printf("dldir: failed to remove '%s': No such file or directory\n", path);
    }
    if(check_result == INVALID || check_result == IS_FILE) {
        printf("dldir: failed to remove '%s': Not a directory\n", path);
    }
    else if (check_result == IS_DIRECTORY) {
        // check directory whether is empty.
        if (fsdldir -> first_child == NULL) {
            strcpy(fsdldir->file_name, "");
            strcpy(fsdldir->absolute_path, "");
            // if the file under directory, we has deleted -> empty.
        } else {
            if(compare(fsdldir->first_child->file_name, "")) {
                strcpy(fsdldir->file_name, "");
                strcpy(fsdldir->absolute_path, "");
                return;
            }
            // if not empty.
            printf("dldir: failed to remove '%s': Directory not empty\n", path);
        }
    }
}

void FsDl(Fs fs, bool recursive, char *path) {
    if (fs == NULL || path == NULL) {
        return;
    }
    //modify path to absolute path and search delete target.
    char absolute_path[PATH_MAX + 1];
    Node fsdl = search_file(fs, absolute_path, path);
    int check_result = check_error(fsdl, path);
    // the target not root, and does not exist.
    if(check_result == DNE && !compare("/", absolute_path)) {
        printf("dl: cannot remove '%s': No such file or directory\n", path);
    }
    // A proper prefix of the path is a regular file.
    else if(check_result == INVALID) {
        printf("dl: cannot remove '%s': Not a directory\n", path);
    }
    // its a file, directly delete. 
    else if(check_result == IS_FILE) {
        strcpy(fsdl->file_name, "");
        strcpy(fsdl->absolute_path, "");
    }
    // if its a directory.
    else if (check_result == IS_DIRECTORY || compare("/", absolute_path)) {
        // check whether its valid.
        if(!recursive) {
            printf("dl: cannot remove '%s': Is a directory\n", path);
        } 
        // delete all files under a target.
        else if(!compare("/", absolute_path)){
            do_fsdl(fsdl -> first_child);
            strcpy(fsdl -> file_name, "");
            strcpy(fsdl -> absolute_path, "");
            // delete all files under root.
        } else {
            do_fsdl(fs -> first_child);
            strcpy(fs -> file_name, "");
            strcpy(fs -> absolute_path, "");
        }
    }
}

void do_fsdl(Node cur) {
    // base case.
    if(cur == NULL) {
        return;
    }
    // do recersively to delete all files under a directory.
    do_fsdl(cur -> next_sibling);
    do_fsdl(cur -> first_child);
    strcpy(cur -> file_name, "");
    strcpy(cur -> absolute_path, "");
}

void FsCp(Fs fs, bool recursive, char *src[], char *dest) {
    // search the dest in fs.
    char dest_path[PATH_MAX + 1];
    Node desti = search_file(fs, dest_path, dest);
    Node dest_cmp = desti;
    
    // if dest path does not exist, we will create it.
    if (desti == NULL) {
        path_modification(fs, dest_path, dest);
        // check first src whether is file or directory.
        char first[PATH_MAX + 1];
        Node first_src = search_file(fs, first, src[0]);
        // the first one is file, then just create a dest file.
        if(first_src -> type == REGULAR_FILE && src[1] == NULL) {
            FsMkfile(fs, dest_path);
            desti = search_file(fs, dest_path, dest);
            do_FsPut(desti, first_src -> content);
            return;
        } else {
            FsMkdir(fs, dest_path);
            desti = search_file(fs, dest_path, dest);
        }
    }
    // copy every src to dest.
    for (int i = 0; src[i] != NULL; i++) {
        // search cp_file in fs.
        char cp_path[PATH_MAX + 1];
        Node fscp = search_file(fs, cp_path, src[i]);

        // since we cp to dest, update the new path.
        char new_path[PATH_MAX + 1];
        find_absolute_path(new_path, dest_path, fscp -> file_name);
        modify_path(new_path);
        // if dest path refers to a file, just overwritten.
        if (desti -> type == REGULAR_FILE) {
            do_FsPut(desti, fscp -> content);
            return;
        }
        // if cp_file is a file.
        if(fscp -> type == REGULAR_FILE) {
            Node fsput = search(fs -> first_child, new_path, new_path);
            // cp_file does not exist, we will create a new one.
            if (fsput == NULL) {
                FsMkfile(fs, new_path);
                // find the real fsput which we just created.
                fsput = search(fs -> first_child, new_path, new_path);
            }
                // copy content.
                do_FsPut(fsput, fscp -> content);
        } 
        // if cp_file is a directory, and if ture we will execute.
        else if(recursive) {
            // the dest path exist, copy of the src directory
            // is made inside the dest directory.
            if(dest_cmp != NULL) {
                Node item = search(fs -> first_child, new_path, new_path);
                // we don't have this directory.
                if(item == NULL) {
                    FsMkdir(fs, new_path);
                    // we already has this directory.
                } else {
                    // not an empty directry.
                    if(item -> first_child != NULL)
                        return;
                }
            } else {
                strcpy(new_path, dest_path);
            }
            // using recursion, copy all files inside the src directory.
            do_FsCp(fs, fscp -> first_child, desti, new_path);
        }
    }
}

void do_FsCp(Fs fs, Node cur, Node desti, char *cur_path) {
    // base case.
    if(cur == NULL) {
        return;
    }
    // find new path for mkdir or mkfile.
    char new_path[PATH_MAX + 1];
    find_absolute_path(new_path, cur_path, cur -> file_name);
    modify_path(new_path);
    // cp_file is a file.
    if (cur -> type == REGULAR_FILE) {
        FsMkfile(fs, new_path);
        Node fsput = search(fs -> first_child, new_path, new_path);
        do_FsPut(fsput, cur -> content);
        // cp_file is a directory.
    } else if(search(fs -> first_child, new_path, new_path) == NULL){
        FsMkdir(fs, new_path);
    }
    // recursion case.
    do_FsCp(fs, cur -> next_sibling, desti, cur_path);
    do_FsCp(fs, cur -> first_child, desti, new_path);
}

// move command actually is copy + delete.
void FsMv(Fs fs, char *src[], char *dest) {
    FsCp(fs, true, src, dest);
    // after copy, delete every src step by step.
    for(int i = 0; src[i] != NULL; i++) {
        char absolute_path[PATH_MAX + 1];
        path_modification(fs, absolute_path, src[i]);
        FsDl(fs, true, absolute_path);
    }
}

