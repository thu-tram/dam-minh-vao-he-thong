

 





























## 9.6. Recursion 

Recursive functions are a special class of functions that call
themselves (also known as **self-referential** functions) to compute a
value. Like their nonrecursive counterparts, recursive functions create
new stack frames for each function call. Unlike standard functions,
recursive functions contain function calls to themselves.


Let's revisit the problem of summing up the set of positive integers
from 1 to *n*. In previous sections, we discussed the `sumUp` function
to achieve this task. Table 1 shows a related
function called `sumDown`, which adds the numbers in reverse (*n* to 1),
and its recursive equivalent:


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

: Table 1. Iterative and recursive version of the `sumDown` function.

The base case in the recursive function `sumr` accounts for any values
of *n* that are less than or equal to zero, and the recursive step adds
the current value of *n* to the result of the function call to `sumr`
with the value *n* - 1. Compiling `sumr` and disassembling it with GDB
yields the following assembly code:




    Dump of assembler code for function sumr:
    0x770 <+0>:  stp   x29, x30, [sp, #-32]! // sp = sp-32; store x29,x30 on stack
    0x774 <+4>:  mov   x29, sp               // x29 = sp (i.e. x29 = top of stack)
    0x778 <+8>:  str   w0, [x29, #28]        // store w0 at x29+28 (n)
    0x77c <+12>: ldr   w0, [x29, #28]        // w0 = n
    0x780 <+16>: cmp   w0, #0x0              // compare n to 0
    0x784 <+20>: b.gt  0x790 <sumr+32>       // if (n > 0) goto <sumr+32>
    0x788 <+24>: mov   w0, #0x0              // w0 = 0
    0x78c <+28>: b     0x7a8 <sumr+56>       // goto <sumr+56>
    0x790 <+32>: ldr   w0, [x29, #28]        // w0 = n
    0x794 <+36>: sub   w0, w0, #0x1          // w0 = w0 - 1 (i.e. n-1)
    0x798 <+40>: bl    0x770 <sumr>          // call sumr(n-1) (result)
    0x79c <+44>: mov   w1, w0                // copy result into register w1
    0x7a0 <+48>: ldr   w0, [x29, #28]        // w0 = n
    0x7a4 <+52>: add   w0, w1, w0            // w0 = w0 + w1 (i.e n + result)
    0x7a8 <+56>: ldp   x29, x30, [sp], #32   // restore x29, x30, and sp
    0x7ac <+60>: ret                         // return w0 (result)


Each line in the preceding assembly code is annotated with its English
translation. Table 2 shows the corresponding
`goto` form (left) and C program without `goto` statements (right):


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
|     result--;                     | ```                               |
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

: Table 2. C Goto form and translation of `sumr()` assembly code.

Although this translation may not initially appear to be identical to
the original `sumr` function, close inspection reveals that the two
functions are indeed equivalent.



### 9.6.1. Animation: Observing How the Call Stack Changes 

As an exercise, we encourage you to draw out the stack and see how the
values change. The animation below depicts how the stack is updated when
we run this function with the value 3.


::: imageblock

![recursion](_images/recursion.png)






