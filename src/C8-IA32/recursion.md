

 







-   -  
        -  
        -  
        -   [8.3. Additional Arithmetic
            Instructions]()
        -   [8.4. Conditional Control and
            Loops]()
            -  
            -  
            -  
        -  
        -  
        -  
        -  
        -  
        -  
        -  





















## 8.6. Recursion 

Recursive functions are a special class of functions that call
themselves (also known as **self-referential** functions) to compute a
value. Like their nonrecursive counterparts, recursive functions create
new stack frames for each function call. Unlike standard functions,
recursive functions contain function calls to themselves.


Let's revisit the problem of summing up the set of positive integers
from *1* to *n*. In previous sections, we discussed the `sumUp` function
to achieve this task. Table 1 shows a related
function called `sumDown`, which adds the numbers in reverse (*n* to
*1*), and its recursive equivalent `sumr`:


+-----------------------------------+-----------------------------------+
| Iterative                         | Recursive                         |
+===================================+===================================+
|                        |                        |
|                   |                   |
|                        |                        |
| ```     | ```     |
| int sumDown(int n) {              | int sumr(int n) {                 |
|     int total = 0;                |     if (n <= 0) {                 |
|     int i = n;                    |         return 0;                 |
|     while (i > 0) {               |     }                             |
|         total += i;               |     return n + sumr(n-1);         |
|         i--;                      | }                                 |
|     }                             | ```                               |
|     return total;                 | :::                               |
| }                                 | :::                               |
| ```                               | :::                               |
| :::                               |                                   |
| :::                               |                                   |
| :::                               |                                   |
+-----------------------------------+-----------------------------------+

: Table 1. Iterative version (sumDown) and recursive version (sumr)

The base case in the recursive function `sumr` accounts for any values
of *n* that are less than one, and the recursive step adds the current
value of *n* to the result of the function call to `sumr` with the value
*n-1*. Compiling `sumr` with the `-m32` flag and disassembling it with
GDB yields the following assembly code:




    0x0804841d <+0>:  push  %ebp                  # save ebp
    0x0804841e <+1>:  mov   %esp,%ebp             # update ebp (new stack frame)
    0x08048420 <+3>:  sub   $0x8,%esp             # add 8 bytes to stack frame
    0x08048423 <+6>:  cmp   $0x0,0x8(%ebp)        # compare ebp+8 (n) with 0
    0x08048427 <+10>: jg    0x8048430 <sumr+19>   # if (n > 0), goto <sumr+19>
    0x08048429 <+12>: mov   $0x0,%eax             # copy 0 to eax (result)
    0x0804842e <+17>: jmp   0x8048443 <sumr+38>   # goto <sumr+38>
    0x08048430 <+19>: mov   0x8(%ebp),%eax        # copy n to eax (result)
    0x08048433 <+22>: sub   $0x1,%eax             # subtract 1 from n (result--)
    0x08048436 <+25>: mov   %eax,(%esp)           # copy n-1 to top of stack
    0x08048439 <+28>: call  0x804841d <sumr>      # call sumr() function
    0x0804843e <+33>: mov   0x8(%ebp),%edx        # copy n to edx
    0x08048441 <+36>: add   %edx,%eax             # add n to result (result+=n)
    0x08048443 <+38>: leave                       # prepare to leave the function
    0x08048444 <+39>: ret                         # return result


Each line in the preceding assembly code is annotated with its English
translation. Table 2 shows the corresponding
`goto` form and C program without `goto` statements:


+-----------------------------------+-----------------------------------+
| C goto form                       | C version without goto statements |
+===================================+===================================+
|                        |                        |
|                   |                   |
|                        |                        |
| ```     | ```     |
| int sumr(int n) {                 | int sumr(int n) {                 |
|     int result;                   |     int result;                   |
|     if (n > 0) {                  |     if (n <= 0) {                 |
|         goto body;                |         return 0;                 |
|     }                             |     }                             |
|     result = 0;                   |     result = sumr(n-1);           |
|     goto done;                    |     result += n;                  |
| body:                             |     return result;                |
|     result = n;                   | }                                 |
|     result -= 1;                  | ```                               |
|     result = sumr(result);        | :::                               |
|     result += n;                  | :::                               |
| done:                             | :::                               |
|     return result;                |                                   |
| }                                 |                                   |
| ```                               |                                   |
| :::                               |                                   |
| :::                               |                                   |
| :::                               |                                   |
+-----------------------------------+-----------------------------------+

: Table 2. C goto form and translation of sumr assembly code

Although this translation may not initially appear to be identical to
the original `sumr` function, close inspection reveals that the two
functions are indeed equivalent.



### 8.6.1. Animation: Observing How the Call Stack Changes 

As an exercise, we encourage you to draw out the stack and see how the
values change. The animation below depicts how the stack is updated when
we run this function with the value 3.


::: imageblock

![recursion](_images/recursion.gif)






