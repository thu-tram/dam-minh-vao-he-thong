#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

/*First set of functions:
The compiler will never produce the same assembly, not even on O3!
*/
//Multiples a by 10 and adds b to the result
void shiftAdd(int *a, int *b){
    *a = *a * 10; //multiply by 10
    *a += *b; //add b
}

void shiftAddOpt(int *a, int *b){
    *a = (*a * 10) + *b;
}

/*Second set of functions
The difference here is that b is not a pointer
The compiler will optimize these two functiosn to the same assembly
*/
void shiftAdds(int *a, int b){
    *a = *a * 10; //multiply by 10
    *a += b; //add b
}

void shiftAddOpts(int *a, int b){
    *a = (*a * 10) + b;
}


int main(void){
    int x = 5;
    int y = 6;
    shiftAdd(&x, &y); //should produce 56
    //shiftAdd(&x, &x); //should produce 55

    printf("shiftAdd produces: %d\n", x);

    x = 5; //reset x
    shiftAddOpt(&x, &y); //should produce 56
    //shiftAddOpt(&x, &x); //should produce 55

    printf("shiftAddOpt produces: %d\n", x);

    return 0;

}
