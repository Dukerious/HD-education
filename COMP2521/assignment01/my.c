#include<stdio.h>
#include<string.h>
#include <stdbool.h>
#define NAME_MAX 100 
bool compare(char *item, char *file_name);
void modify_path(char path[NAME_MAX]);
void modify_dot_path(char temp1[NAME_MAX + 1], char path[NAME_MAX + 1]);
bool modify_double_dot_path(char temp2[NAME_MAX + 1], char temp3[NAME_MAX + 1]);
int main(void) {
    char path1[NAME_MAX] = "/k/s/bin";  //    /k/s
    char path2[NAME_MAX] = "/k/s//k/s/bin"
    return 0;
}



bool compare(char *item, char *file_name) {
    if(strcmp(item, file_name) == 0) {
        return true;
    }
    return false;
}

/*
/* if (fs == NULL) {
        return;
    }
    Node cur = fs -> Cwd;
    Node cur_path = NULL;
    if (cur != NULL) {
        cur_path = fs -> Cwd -> absolute_path;
    }
    // modify initial path to absolute path.
    find_absolute_path(absolute_path, cur_path, path);
    modify_path(absolute_path);
    Node fsput = search(first, absolute_path, path);*/


*/
void modify_path(char path[NAME_MAX]) {
    while (1) {
        char temp1[NAME_MAX + 1];
        modify_dot_path(temp1, path);
        //printf("temp1 is %s\n", temp1);
        char *token = strtok(temp1, "/");
        while (token != NULL) {
            if (!compare(token, "..") && !compare(token, ".")) {
                break;
            }
            token = strtok(NULL, "/");
        }
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
        //printf("the temp2 is %s\n", temp2);
        if(modify_double_dot_path(temp2, path)) {
            break;
        }
    }
        
}
void modify_dot_path(char temp1[NAME_MAX + 1], char path[NAME_MAX + 1]) {
    strcpy(temp1, "/");
    char *token = strtok(path, "/");
    strcat(temp1, token);
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
        if (t3 == NULL) {
            strcat(path, t1);
            strcat(path, "/");
            strcat(path, t2);
            return true;
        }
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
    } else {
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



/*void modify_path(char path[NAME_MAX]) {
    while (1) {
        char temp1[NAME_MAX + 1];
        strcpy(temp1, path);
        printf("temp1: %s\n", temp1);
        char *token = strtok(temp1, "/");
        //printf("token start: %s\n", token);
        while (token != NULL) {
            if (!compare(token, "..") && !compare(token, ".")) {
                break;
            }
            //printf("token: %s\n", token);
            token = strtok(NULL, "/");
        }
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
        printf("the temp2 is %s\n", temp2);
        //////////////////////
        //path[NAME_MAX + 1] = {0};
        strcpy(path, "/");
        char *t1 = strtok(temp2, "/");
        char *t2 = strtok(NULL, "/");
        char *t3 = strtok(NULL, "/");
        printf("t1 is %s\n", t1);
        printf("t2 is %s\n", t2);
        printf("t3 is %s\n", t3);
        if(t1 == NULL) {
            break;
        }
        else if(t2 == NULL) {
            strcat(path, t1);
            break;
        }

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
            //printf("now t1 is %s\n", t1);
            //printf("now t2 is %s\n", t2);
            if (t3 == NULL) {
                strcat(path, t1);
                strcat(path, "/");
                strcat(path, t2);
                break;
            }
            else if (compare(t3, "..")) {
                strcat(path, t1);
                //printf("the path is %s\n", path);
                //strcpy(path, "/");
                while (t3 != NULL) {
                    t3 = strtok(NULL, "/");
                    if (t3 == NULL) {
                        break;
                    }
                    strcat(path, "/");
                    strcat(path, t3);
                }
            }
            //printf("the path is %s\n", path);
        } else {
            if(t3 != NULL) {
                strcat(path, t3);
            }
            while(t3 != NULL) {
                t3 = strtok(NULL, "/");
                if (t3 == NULL) {
                    break;
                }
                strcat(path, "/");
                strcat(path, t3);
            }

        }
    }
}*/
/*void FsMkdir(char *path) {
    char *out = strtok(path, "/");
    printf("%s\n", out);
}





    if (fs == NULL) {
        No such file or directory
        return;
    }
    if (compare != 0)
        FsMkdir(fs -> next_sibling, path);
    else
        if (fs -> type == REGULAR_FILE) {
            Not a directory
            return;
        }
        if (fs -> first_child == NULL) {
            fs -> first_child = fsnew;
        }
        FsMkdir(fs -> first_child, path);







        if(root == NULL) {
        return NULL;
    }
    if(compare(root->name, path))
    do_FsMkdir(root -> next_sibling, path);






    Fs find_parent(Fs fs, parent) {
    if (fs == NULL) {
        return NULL;
    }
    if(compare(fs -> file_name, parent)) {
        return fs;
    }
    fs = find_parent(fs -> next_sibling, parent);
    return fs;
}*/




/*void print_fs(Fs fs) {
    if(fs == NULL) {
        return;
    }
    printf("   ");
    listFile(fs->file_name, fs->type);
    print_fs(fs -> next_sibling);
    printf("\n");
    print_fs(fs -> first_child);
}*/