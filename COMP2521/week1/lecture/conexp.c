// (a)
#include <stdio.h>
int main (void) {
    int x = 0;
    int y;
   /* if (x > 0)
    y = x - 1;
    else
    y = x + 1;*/

    y = (x > 0) ? x-1 : x+1;
    printf("the x is %d\n", x);
    printf("the y is %d\n", y);

   /* // (b)
    if (x > 0)
    y = x - 1;
    else
    z = x + 1;

    Can't use conditional expr
    Different vars in each branch of if

    // (c)
    if (x > 0) {
    y = x - 1;  z = x + 1;
    } else {
    y = x + 1;  z = x - 1;
    }

    y = (x > 0) ? x-1 : x+1;
    z = x > 0 ? x+1 : x-1;*/
}