
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "list.h"
static List FindNextSubStart(List start, int prev, bool first, int *len);
List flas(List l) {
    List sub = NULL;
    List start = l;
    int maxlen = 1;

    while(start != NULL) {
        int len = 0;
        List end = FindNextSubStart(start, l -> value, true, &len);
        if(len > maxlen) {
            maxlen = len;
            sub = start;
        }

        start = end;
    }
    return sub;
}


static List FindNextSubStart(List start, int prev, bool first, int *len) {
    if(start == NULL) return NULL;
    if(prev >= start -> value && !first) return start;

    *len += 1;
    return FindNextSubStart(start -> next, start -> value, false, len);
}

