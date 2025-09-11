/*
 * Copyright (c) 2020, Dive into Systems, LLC (https://diveintosystems.org/)
 * An example of a basic while loop.
 */
#include <stdio.h>

int main(void) {
    int i;

    i = 0;
    while (i < 10) {
        printf("i is %d\n", i++);  // i++: increment i's value after using it
    }
    return 0;
}
