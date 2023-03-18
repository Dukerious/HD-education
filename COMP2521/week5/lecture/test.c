#include <stdio.h>
#include <stdbool.h>
bool check() {
    return(1 < 2);
    printf("here\n");
    return false;
}
int main(void) {
    bool result = check();
    if(result) printf("true\n");
    return 0;
}