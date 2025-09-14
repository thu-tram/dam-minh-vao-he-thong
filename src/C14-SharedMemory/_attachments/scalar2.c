#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct t_arg {
    int *array; // pointer to shared array
    long length; // num elements in array
    long s; //scaling factor
    long numthreads; // total number of threads
    long id; //  logical thread id
};

/*error handling function: prints out error message*/
int print_error(char *msg) {
    fprintf(stderr, "%s\n", msg);
    exit(2);
}

void initialize_array(int *array, long length) {
    int i;
    for (i = 0; i < length; i++) {
        array[i] = 1 + (rand() % 10);
    }
}

void print_array(int *array, long length) {
    int i;
    for (i = 0; i < length; i++){
        printf("%d ", array[i]);
    }
    printf("\n");
}

void *ScalarMultiplication(void *args) {
    struct t_arg *myargs = (struct t_arg *)args;
    long myid =  myargs->id;
    long length = myargs->length;
    long s = myargs->s;
    long nthreads = myargs->numthreads;
    long chunk = length / nthreads;
    long start = myid * chunk;
    long end  = start + chunk;
    if (myid == nthreads - 1) {
        end = length;
    }
    int *ap = myargs->array; //pointer to array in main
    int i;
    for (i = start; i < end; i++) {
        ap[i] *= s;
    }

    return NULL;
}

int main(int argc, char **argv) {

    if (argc != 4) {
        fprintf(stderr, "usage: %s <n> <length> <s>\n", argv[0]);
        fprintf(stderr, "where <n> is the number of threads\n");
        fprintf(stderr, "where <length> is the length of the array\n");
        fprintf(stderr, "and <s> is the scale factor\n");
        return 1;
    }
    long t;
    int nthreads = strtol(argv[1], NULL, 10);
    long length = strtol(argv[2], NULL, 10);
    int *array = malloc(length * sizeof(int));
    if (!array) print_error("ERROR: malloc failed");

    initialize_array(array, length);
    long s = strtol(argv[3], NULL, 10);
    int ret; //for error checking

    //print_array(array, length);
    //allocate space for thread structs and identifiers
    pthread_t *thread_array = malloc(nthreads * sizeof(pthread_t));
    struct t_arg *thread_args = malloc(nthreads * sizeof(struct t_arg));
    
    for (t = 0; t < nthreads; t++) {
        thread_args[t].array = array;
        thread_args[t].length = length;
        thread_args[t].s = s;
        thread_args[t].numthreads = nthreads;
        thread_args[t].id = t;
    }

    for (t = 0; t < nthreads; t++) {
        ret = pthread_create(&thread_array[t], NULL, ScalarMultiplication, &thread_args[t]);
        if (ret) print_error("ERROR: pthread_create failed");
    }

    for (t = 0; t < nthreads; t++) {
        ret = pthread_join(thread_array[t], NULL);
        if (ret) print_error("ERROR: pthread_join failed");
    }

    //print_array(array, length);
    free(thread_array);

    return 0;
}

