/* Copyright (c) 2020, Dive into Systems, LLC (https://diveintosystems.org/)
 *
 * An example of a typical while loop used to force the user to
 * enter valid input.
 */
#include <stdio.h>

int main(void) {
    int data;

    while (1) {  // an infinite loop (1 is always true)
        printf("Enter a value beween 0 and 100: ");
        scanf("%d", &data);
        if ((data >= 0) && (data <= 100)){
            break;  // break out of a loop
        }
        printf("Hey, %d isn't between 0 and 100...try again\n", data);
    }

    printf("The value read in is %d\n", data);
    return 0;
}
