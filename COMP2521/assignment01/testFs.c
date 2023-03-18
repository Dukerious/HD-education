// Main program for testing the File System ADT

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Fs.h"

int main(void) {
	Fs fs = FsNew();
	FsMkdir(fs, "bin");
	FsMkdir(fs, "bin/mv");
	FsMkdir(fs, "etc");
	FsMkdir(fs, "etc/ssh");
	FsMkdir(fs, "etc/uu");
	FsMkfile(fs, "etc/ll");
	FsMkfile(fs, "etc/uu/pp");
	FsMkdir(fs, "home");
	FsMkdir(fs, "home/k");
	FsMkdir(fs, "home/k/ls");
	FsMkfile(fs, "home/file");
	FsMkfile(fs, "home/k/yy");
	FsMkfile(fs, "home/k/ls/ee");
	FsMkdir(fs, "home/jas");
	FsMkfile(fs, "home/jas/init");
	FsTree(fs, NULL);
	printf("-------\n");
	char *src[] = {"home", "etc", NULL};
	FsMv(fs, src, "bin/mv");
	FsTree(fs, NULL);
	FsCd(fs, "/bin/mv/etc/uu");
	FsPwd(fs);
	FsLs(fs, "/bin/mv/etc/.././etc/uu");
	FsFree(fs);
	
	
	
	/*Fs fs = FsNew();
	FsMkdir(fs, "hello");
	FsMkfile(fs, "hello/a.txt");
	FsMkdir(fs, "world");
	FsMkfile(fs, "world/hello");
	FsMkfile(fs, "file");
	FsPut(fs, "/hello/a.txt", "SUCCEED\n");
	//FsMkdir(fs, "cp");
	//FsMkfile(fs, "cp/ss");
	FsTree(fs, NULL);
	printf("--------\n");
	char *src[] = {"hello", NULL};
	FsMv(fs, src, "cp");
	FsTree(fs, NULL);
	FsCat(fs, "cp");
	FsFree(fs);*/
	//FsDl(fs, false, "hello/world");
	/*Fs fs = FsNew();
	//FsTree(fs, NULL);
	FsMkfile(fs, "hello.txt");
	FsMkfile(fs, "world.txt");
	FsMkdir(fs, "/bin");
	FsMkfile(fs, "bin/ls");
	FsMkfile(fs, "bin/pwd");

	FsMkdir(fs, "home");
	FsMkdir(fs, "home/jas");
	//FsMkdir(fs, "home/a");
	//FsMkdir(fs, "bin");
	FsMkdir(fs, "bin/k");
	//printf("error in here?\n");
	FsMkdir(fs, "home/jas");
	//FsMkdir(fs, "home/s/jas");
	//FsMkdir(fs, "bin/kl");
	FsMkfile(fs, "home/jas/todo.txt");
	FsMkfile(fs, "home/jas/mail.txt");
	FsTree(fs, NULL);
	printf("---\n"); // marker to separate output
	FsCd(fs, "bin");
	FsPwd(fs);
	printf("---\n");
	FsDl(fs, true, "ls");
	FsTree(fs, NULL);
	printf("---\n");
	FsPut(fs, "/bin/pwd", "hello\n");
	FsCat(fs, "pwd");
    FsFree(fs);*/
}

