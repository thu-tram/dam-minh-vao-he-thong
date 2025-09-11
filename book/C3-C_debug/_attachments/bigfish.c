/* Copyright (c) 2020, Dive into Systems, LLC (https://diveintosystems.org/)  */
#include <stdio.h>
#include <stdlib.h>
/* print size elms of array p with name name */
void print_array(int *p, int size, char *name) ;

int main(int argc, char *argv[]) {
    int *bigfish, *littlefish, i;

    // allocate space for two int arrays
    bigfish = (int *)malloc(sizeof(int)*10);
    littlefish = (int *)malloc(sizeof(int)*10);
    if (!bigfish || !littlefish) {
        printf("Error: malloc failed\n");
        exit(1);
    }
    for (i=0; i < 10; i++) {
        bigfish[i] = 10+i;
        littlefish[i] = i;
    }
    print_array(bigfish,10, "bigfish");
    print_array(littlefish,10, "littlefish");

    // here is a bad Heap memory access
    // (write beyond bounds of allocated memory):
    for (i=0; i < 13; i++) {
        bigfish[i] = 66+i;
    }
    printf("\nafter loop:\n");
    print_array(bigfish,10, "bigfish");
    print_array(littlefish,10, "littlefish");

    free(bigfish);
    free(littlefish);  // program will crash here
    return 0;
}
/***********************************/
/*
 * print out an array
 * p: the array, size: num elms, name: name
 */
void print_array(int *p, int size, char *name) {

    int i;
    printf("%s array:\n", name);
    for(i=0; i< size; i++) {
	printf("%3d ",p[i]);
    }
    printf("\n");
}
