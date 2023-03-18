
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Fs.h"
int CountUndersize(FileList cur);
int CountSamesize(FileList cur);
int totalTextSize(Fs fs) {
	if(fs == NULL || fs -> root == NULL) return 0;
	return CountSamesize(fs -> root -> files);
}

int CountUndersize(FileList cur) {
	if(cur == NULL) return 0;
	int size = 0;
	int under = 0;
	if(cur -> file -> type == TEXT_FILE) {
		size = strlen(cur -> file -> text);
	} else {
		under = CountSamesize(cur -> file -> files);
	}
	return size + under;
}

int CountSamesize(FileList cur) {
	if(cur == NULL) return 0;
	return CountUndersize(cur) + CountSamesize(cur -> next);
}

