/*
 * Copyright (c) 2020, Dive into Systems, LLC (https://diveintosystems.org/)
 * This is an example of pointer arithmetic.
 * Pointer variables are assigned to the base address of arrays
 * and then incremented to iterate over the array elements.
 * Array element values are accessed by dereferencing the pointer.
 */
#include <stdio.h>

#define N 10
#define M 20

int main(void) {
    // array declarations:
    char letters[N];
    int numbers[N], i, j;
    int matrix[N][M];

    // declare pointer variables that will access int or char array elements
    // using pointer arithmetic (the pointer type must match array element type)
    char *cptr = NULL;
    int *iptr = NULL;

    // make the pointer point to the first element in the array
    cptr = &(letters[0]); //  &(letters[0])  is the address of element 0
    iptr = numbers;       // the address of element 0 (numbers is &(numbers[0]))

    // initialized letters and numbers arrays through pointer variables
    for(i = 0; i < N; i++) {
      // dereference each pointer and update the element it currently points to
      *cptr = 'a' + i;
      *iptr = i * 3;

      // use pointer arithmetic to set pointer to point to the next element
      cptr++;  // cptr points to the next char address (next element of letters)
      iptr++;  // iptr points to the next int address  (next element of numbers)
    }

    printf("\n array values using indexing to access: \n");
    // see what the code above did:
    for(i = 0; i < N; i++) {
       printf("letters[%d] = %c, numbers[%d] = %d\n",
           i, letters[i], i, numbers[i]);
    }

    // we could also use pointer arith to print these out:
    printf("\n array values using pointer arith to access: \n");
    // first: initialize pointers to base address of arrays:
    cptr = &(letters[0]); //  letters == &(letters[0]) is the base address
    iptr = numbers;
    for(i = 0; i < N; i++) {
       // dereference pointers to access array element values
       printf("letters[%d] = %c, numbers[%d] = %d\n",
               i, *cptr, i, *iptr);

       // increment pointers to point to the next element
       cptr++;  // cptr points to the next element of letters
       iptr++;  // iptr points to the next next element of numbers
    }

    // pointer arithmetic over a 2D array
    // sets matrix to:
    // row 0:   0,   1,   2, ...,  99
    // row 1: 100, 110, 120, ..., 199
    //        ...
    iptr = &(matrix[0][0]);
    for(i = 0; i < N*M; i++) {
        *iptr = i;
        iptr++;
    }

    // see what the code above did:
    printf("\n 2D array values inited using pointer arith: \n");
    for(i = 0; i < N; i++) {
        for(j = 0; j < M; j++) {
            printf("%3d ", matrix[i][j]);
        }
        printf("\n");
    }

    // pointer arithmetic example starting at different element
    // in the array and changing pointer value by more than 1:
    iptr = &numbers[2];
    *iptr = -13;
    iptr += 4;     // increment by 4 (will point to numbers[6]
    *iptr = 9999;

    printf("\n element 2 and 6 changed using pointer arith: \n");
    for(i = 0; i < N; i++) {
       printf("numbers[%d] = %d\n", i, numbers[i]);
    }


    return 0;
}

