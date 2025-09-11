/* Copyright (c) 2020, Dive into Systems, LLC (https://diveintosystems.org/)
 *
 * An example of a more complex for loop which uses multiple variables.
 * Note: it's unusual to have for loops with multiple statements in the
 * init and step parts, but C supports it and there are times when it
 * is useful...don't go nuts with this just because you can!
 */
#include <stdio.h>

int main(void) {
    int i, j;

    for (i = 0, j = 0; i < 10; i += 1, j += 10) {
        printf("i + j = %d\n", i + j);
    }

    return 0;
}

// the rules for evaluating a for loop are the same no mater how
// simple or complex each part is:
// (1) evaluate the initialization statements once on the  first 
//     evaluation of the for loop:  i=0 and j=0 
// (2) evaluate the boolean condition: i < 10
//     if false (if i == 0), drop out of the for loop
// (3) execute the statements inside the for loop body: printf
// (4) evaluate the step statements:  i += 1, j += 10
// (5) goto (2)
