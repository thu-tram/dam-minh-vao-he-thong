

 

































### 9.4.2. if Statements in Assembly 

Let's take a look at the `getSmallest` function in assembly. For
convenience, the function is reproduced here.




```
int getSmallest(int x, int y) {
    int smallest;
    if ( x > y ) {
        smallest = y;
    }
    else {
        smallest = x;
    }
    return smallest;
}
```


The corresponding assembly code extracted from GDB looks similar to the
following:




    (gdb) disas getSmallest
    Dump of assembler code for function getSmallest:
    0x07f4 <+0>:    sub  sp, sp, #0x20
    0x07f8 <+4>:    str  w0, [sp, #12]
    0x07fc <+8>:    str  w1, [sp, #8]
    0x0800 <+12>:   ldr  w1, [sp, #12]
    0x0804 <+16>:   ldr  w0, [sp, #8]
    0x0808 <+20>:   cmp  w1, w0
    0x080c <+24>:   b.le 0x81c <getSmallest+40>
    0x0810 <+28>:   ldr  w0, [sp, #8]
    0x0814 <+32>:   str  w0, [sp, #28]
    0x0818 <+36>:   b    0x824 <getSmallest+48>
    0x081c <+40>:   ldr  w0, [sp, #12]
    0x0820 <+44>:   str  w0, [sp, #28]
    0x0824 <+48>:   ldr  w0, [sp, #28]
    0x0828 <+52>:   add  sp, sp, #0x20
    0x082c <+56>:   ret


This is a different view of the assembly code than we have seen before.
Here, we can see the *address* associated with each instruction, but not
the *bytes*. Note that this assembly segment has been lightly edited for
the sake of simplicity. By convention, GCC places the first and second
parameters of a function in registers `x0` and `x1` respectively. Since
the parameters to `getSmallest` are of type `int`, the compiler places
the parameters in the respective component registers `w0` and `w1`
instead. For the sake of clarity, we refer to these parameters as `x`
and `y` respectively.


Let's trace through the first few lines of the previous assembly code
snippet. Note that we will not draw out the stack explicitly in this
example. We leave this as an exercise for the reader, and encourage you
to practice your stack tracing skills by drawing it out yourself.



-   The `sub` instruction grows the call stack by 32 bytes (0x20).

-   The `str` instructions at `<getSmallest+4>` and `<getSmallest+8>`
    store `x` and `y` at stack locations `sp + 12` and `sp + 8`,
    respectively.

-   The `ldr` instructions at `<getSmallest+12>` and `<getSmallest+16`
    load `x` and `y` into registers `w1` and `w0`, respectively. Note
    that the original contents of `w0` and `w1` have swapped!

-   The `cmp` instruction compares `w1` to `w0` (i.e., `x` to `y`) and
    sets appropriate condition code flag registers.

-   The `b.le` instruction at `<getSmallest+24>` indicates that if `x`
    is less than or equal to `y`, the next instruction that should
    execute should be at location `<getSmallest+40>` (or `pc` = 0x81c).
    Otherwise, `pc` is set to the next instruction in sequence, or
    0x810.


The next instructions to execute depend on whether the program follows
the branch (i.e., executes the jump) at (`<getSmallest+24>`). Let's
first suppose that the branch was *not* followed. In this case, `pc` is
set to 0x810 (i.e., `<getSmallest+28>`) and the following sequence of
instructions executes:



-   The `ldr` instruction at `<getSmallest+28>` loads `y` to register
    `w0`.

-   The `str` instruction at `<getSmallest+32>` stores `y` at stack
    location `sp + 28`.

-   The `b` instruction at `<getSmallest+36>` sets register `pc` to
    address 0x824.

-   The `ldr` instruction at `<getSmallest+48>` loads `y` into register
    `w0`.

-   The last two instructions revert the call stack to its original size
    and return from the function call. In this case, `y` is in the
    return register, `w0`, and `getSmallest` returns `y`.


Now, suppose that the branch *was* taken at `<getSmallest+24>`. In other
words, the `b.le` instruction sets register `pc` to 0x81c (i.e.,
`<getSmallest+40>`). Then, the next instructions to execute are:



-   The `ldr` instruction at `<getSmallest+40>` loads `x` into register
    `w0`.

-   The `str` instruction at `<getSmallest+44>` stores `x` at stack
    location `sp + 28`.

-   The `ldr` instruction at `<getSmallest+48>` loads `x` into register
    `w0`.

-   The last two instructions revert the call stack to its original size
    and return from the function call. In this case, `x` is in the
    return register, `w0`, and `getSmallest` returns `x`.


We can then annotate the preceding assembly as follows:




    0x07f4 <+0>:   sub  sp, sp, #0x20          // grow stack by 32 bytes
    0x07f8 <+4>:   str  w0, [sp, #12]          // store x at sp+12
    0x07fc <+8>:   str  w1, [sp, #8]           // store y at sp+8
    0x0800 <+12>:  ldr  w1, [sp, #12]          // w1 = x
    0x0804 <+16>:  ldr  w0, [sp, #8]           // w0 = y
    0x0808 <+20>:  cmp  w1, w0                 // compare x and y
    0x080c <+24>:  b.le 0x81c <getSmallest+40> // if(x <= y) goto <getSmallest+40>
    0x0810 <+28>:  ldr  w0, [sp, #8]           // w0 = y
    0x0814 <+32>:  str  w0, [sp, #28]          // store y at sp+28 (smallest)
    0x0818 <+36>:  b    0x824 <getSmallest+48> // goto <getSmallest+48>
    0x081c <+40>:  ldr  w0, [sp, #12]          // w0 = x
    0x0820 <+44>:  str  w0, [sp, #28]          // store x at sp+28 (smallest)
    0x0824 <+48>:  ldr  w0, [sp, #28]          // w0 = smallest
    0x0828 <+52>:  add  sp, sp, #0x20          // clean up stack
    0x082c <+56>:  ret                         // return smallest


Translating this back to C code yields:


+-----------------------------------+-----------------------------------+
| Goto Form                         | Translated C code                 |
+===================================+===================================+
|                        |                        |
|                   |                   |
|                        |                        |
| ```     | ```     |
| int getSmallest(int x, int y) {   | int getSmallest(int x, int y) {   |
|     int smallest=y;               |     int smallest=y;               |
|     if (x <= y) {                 |     if (x <= y) {                 |
|         goto assign_x;            |         smallest = x;             |
|     }                             |     }                             |
|     smallest = y;                 |     else {                        |
|     goto done;                    |         smallest = y;             |
|                                   |     }                             |
| assign_x:                         |     return smallest;              |
|     smallest = x;                 | }                                 |
|                                   | ```                               |
| done:                             | :::                               |
|     return smallest;              | :::                               |
| }                                 | :::                               |
| ```                               |                                   |
| :::                               |                                   |
| :::                               |                                   |
| :::                               |                                   |
+-----------------------------------+-----------------------------------+

: Table 1. Translating `getSmallest()` into goto C form and C code.

In Table 1, the variable `smallest` corresponds
to register `w0`. If `x` is less than or equal to `y`, the code executes
the statement `smallest = x`, which is associated with the `goto` label
`assign_x` in our `goto` form of this function. Otherwise, the statement
`smallest = y` is executed. The `goto` label `done` is used to indicate
that the value in `smallest` should be returned.


Notice that the preceding C translation of the assembly code is a bit
different from the original `getSmallest` function. These differences
don't matter; a close inspection of both functions reveals that the two
programs are logically equivalent. However, the compiler first converts
each if statement into an equivalent `goto` form, which results in the
slightly different, but equivalent, version. [Table
2](#StandardGotoIfa64) shows the standard `if` statement format and its
equivalent `goto` form.


+-----------------------------------+-----------------------------------+
| C if statement                    | Compiler's equivalent goto form   |
+===================================+===================================+
|                        |                        |
|                   |                   |
|                        |                        |
| ```     | ```     |
| if (condition) {                  |     if (!condition) {             |
|     then_statement;               |         goto else;                |
| }                                 |     }                             |
| else {                            |     then_statement;               |
|     else_statement;               |     goto done;                    |
| }                                 | else:                             |
| ```                               |     else_statement;               |
| :::                               | done:                             |
| :::                               | ```                               |
| :::                               | :::                               |
|                                   | :::                               |
|                                   | :::                               |
+-----------------------------------+-----------------------------------+

: Table 2. Standard if statement format and its equivalent goto form.

Compilers translating code into assembly designate a branch when a
condition is true. Contrast this behavior with the structure of an `if`
statement, where a \"jump\" (to the `else`) occurs when conditions are
*not* true. The `goto` form captures this difference in logic.


Considering the original `goto` translation of the `getSmallest`
function, we can see that:



-   `x <= y` corresponds to `!(condition)`.

-   `smallest = x` is the `else_statement`.

-   The line `smallest = y` is the `then_statement`.

-   The last line in the function is `return smallest`.


Rewriting the original version of the function with the preceding
annotations yields:




```
int getSmallest(int x, int y) {
    int smallest;
    if (x > y) {     //!(x <= y)
        smallest = y; //then_statement
    }
    else {
        smallest = x; //else_statement
    }
    return smallest;
}
```


This version is identical to the original `getSmallest` function. Keep
in mind that a function written in different ways at the C code level
can translate to the same set of assembly instructions.



#### The Conditional Select Instruction 

The final conditional instruction we cover is the **conditional select**
(`csel`) instruction. The `cmp`, `tst`, and `b` instructions implement a
**conditional transfer of control** in a program. In other words, the
execution of the program branches in many directions. This can be very
problematic for optimizing code, because branch instructions are
typically very expensive to execute, due to the disruption they can
cause to the instruction pipeline (details covered in the [Architecture
section](../C5-Arch/pipelining_advanced.html#_pipelining_hazards_control_hazards)).
In contrast, the `csel` instruction implements a **conditional transfer
of data**. In other words, the CPU executes *both* the `then_statement`
and `else_statement`, and places the data in the appropriate register
based on the result of the condition.


The use of C's **ternary expression** often results in the compiler
generating a `csel` instruction in place of branches. For the standard
if-then-else statement, the ternary expression has the form:




```
result = (condition) ? then_expression : else_expression;
```


Let's use this format to rewrite the `getSmallest` function as a ternary
expression. Keep in mind that this new version of the function behaves
exactly as the original `getSmallest` function:




```
int getSmallest_csel(int x, int y) {
    return x > y ? y : x;
}
```


While this may not seem like a big change, let's look at the resulting
assembly. Recall that the first and second parameters (`x` and `y`) are
stored in registers `w0` and `w1`, respectively:




    (gdb) disas getSmallest_csel
    Dump of assembler code for function getSmallest_csel:
    0x0860 <+0>:  sub  sp, sp, #0x10      // grow stack by 16 bytes
    0x0864 <+4>:  str  w0, [sp, #12]      // store x at sp+12
    0x0868 <+8>:  str  w1, [sp, #8]       // store y at sp+8
    0x086c <+12>: ldr  w0, [sp, #8]       // w0 = y
    0x0870 <+16>: ldr  w2, [sp, #12]      // w2 = x
    0x0874 <+20>: ldr  w1, [sp, #12]      // w1 = x
    0x0878 <+24>: cmp  w2, w0             // compare x and y
    0x087c <+28>: csel w0, w1, w0, le     // if (x <= y) w0 = x, else w0=y
    0x0880 <+32>: add  sp, sp, #0x10      // restore sp
    0x0884 <+36>: ret                     // return (w0)


This assembly code has no jumps. After the comparison of `x` and `y`,
`x` moves into the return register `w0` only if `x` is less than or
equal to `y`.


The structure of the `csel` instruction is:




    csel D, R1, R2, C // if (C) D = R1 else D = R2


where `D` denotes the destination register, `R1` and `R2` are the two
registers containing the values to be compared, and `C` is the condition
to be evaluated.


As for the branch instructions, the `C` component of the `csel`
instructions indicates the condition on which the conditional select
occurs. They are identical to those shown in the [conditional branch
suffix
table](conditional_control_loops.html#_conditional_branch_instruction_suffixes).


In the case of the original `getSmallest` function, the compiler's
internal optimizer (see Chapter 12) will replace the `b` instructions
with a `csel` instruction if level 1 optimizations are turned on (i.e.,
`-O1`):




    // compiled with: gcc -O1 -o getSmallest getSmallest.c
    Dump of assembler code for function getSmallest:
    0x0734 <+0>:  cmp  w0, w1            // compare x and y
    0x0738 <+4>:  csel w0, w0, w1, le    // if (x<=y)  w0=x, else w0=y
    0x073c <+8>:  ret                    // return (w0)


In general, the compiler is very cautious about optimizing branch
instructions into `csel` instructions, especially in cases where side
effects and pointer values are involved. Table 3 shows
two equivalent ways of writing a function called `incrementX`:


+-----------------------------------+-----------------------------------+
| C code                            | C ternary form                    |
+===================================+===================================+
|                        |                        |
|                   |                   |
|                        |                        |
| ```     | ```     |
| int incrementX(int * x) {         | int incrementX2(int * x){         |
|     if                            |     return x ? (*x)++ : 1;        |
|  (x != NULL) { //if x is not NULL | }                                 |
|                                   | ```                               |
|      return (*x)++; //increment x | :::                               |
|     }                             | :::                               |
|     else { //if x is NULL         | :::                               |
|         return 1; //return 1      |                                   |
|     }                             |                                   |
| }                                 |                                   |
| ```                               |                                   |
| :::                               |                                   |
| :::                               |                                   |
| :::                               |                                   |
+-----------------------------------+-----------------------------------+

: Table 3. Two functions that attempt to increment the value of integer
`x`.

Each function takes a pointer to an integer as input and checks whether
it is `NULL`. If `x` is not `NULL`, the function increments and returns
the dereferenced value of `x`. Otherwise, the function returns the value
1.


It is tempting to think that `incrementX2` uses a `csel` instruction
because it uses a ternary expression. However, both functions yield the
exact same assembly code:




    // parameter x is in register x0
    Dump of assembler code for function incrementX2:
    0x0774 <+0>:  mov  w1, #0x1                   // w1 = 0x1
    0x0778 <+4>:  cbz  x0, 0x788 <incrementX2+20> // if(x==0) goto<incrementX2+20>
    0x077c <+8>:  ldr  w1, [x0]                   // w1 = *x
    0x0780 <+12>: add  w2, w1, #0x1               // w2 = w1 + 1
    0x0784 <+16>: str  w2, [x0]                   // *x = w2
    0x0788 <+20>: mov  w0, w1                     // w0 = *x
    0x078c <+24>: ret                             // return (w0)


Recall that the `csel` instruction *executes both branches of the
conditional*. In other words, `x` gets dereferenced no matter what.
Consider the case where `x` is a null pointer. Recall that dereferencing
a null pointer leads to a null pointer exception in the code, causing a
segmentation fault. To prevent any chance of this happening, the
compiler takes the safe road and uses a branch.





