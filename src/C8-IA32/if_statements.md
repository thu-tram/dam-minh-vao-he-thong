### 8.4.2. if Statements in Assembly 

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
      0x8048411 <+6>:   mov    0x8(%ebp),%eax
      0x8048414 <+9>:   cmp    0xc(%ebp),%eax
      0x8048417 <+12>:  jle    0x8048421 <getSmallest+22>
      0x8048419 <+14>:  mov    0xc(%ebp),%eax
      0x804841f <+20>:  jmp    0x8048427 <getSmallest+28>
      0x8048421 <+22>:  mov    0x8(%ebp),%eax
      0x8048427 <+28>:  ret


This is a different view of the assembly code than we have seen before.
Here, we can see the *address* associated with each instruction, but not
the *bytes*. Note that this assembly segment has been lightly edited for
the sake of simplicity. The instructions that are normally part of
function creation/termination (i.e., `push %ebp` and `mov %esp, %ebp`)
and for allocating space on the stack are removed. By convention, GCC
places the first and second parameters of a function at locations
`%ebp+8` and `%ebp+0xc` (or `%ebp+12`), respectively. For the sake of
clarity, we refer to these parameters as `x` and `y` respectively.


Let's trace through the first few lines of the previous assembly code
snippet. Note that we will not draw out the stack explicitly in this
example. We leave this as an exercise for the reader, and encourage you
to practice your stack tracing skills by drawing it out yourself.



-   The `mov` instruction copies the value located at address `%ebp+8`
    (the first parameter, `x`) and places it in register `%eax`. The
    instruction pointer (`%eip`) is set to the address of the next
    instruction, or 0x08048414.

-   The `cmp` instruction compares the value at location `%ebp+12` (the
    second parameter, `y`) to `x` and sets appropriate condition code
    flag registers. Register `%eip` advances to the address of the next
    instruction, or 0x08048417.

-   The `jle` instruction on the third line indicates that if `x` is
    less than or equal to `y`, the next instruction that executes is at
    location `<getSmallest+22>` (or `mov 0x8(%ebp), %eax`) and that
    `%eip` should be set to address 0x8048421. Otherwise, `%eip` is set
    to the next instruction in sequence, or 0x8048419.


The next instructions to execute depend on whether the program follows
the branch (i.e., executes the jump) on line 3 (`<getSmallest+12>`).
Let's first suppose that the branch was *not* followed. In this case,
`%eip` is set to 0x8048419 (i.e., `<getSmallest+14>`) and the following
sequence of instructions executes:



-   The `mov 0xc(%ebp),%eax` instruction at `<getSmallest+14>` copies
    `y` to register `%eax`. Register `%eip` advances to 0x804841f.

-   The `jmp` instruction sets register `%eip` to address 0x8048427.

-   The last instruction to execute is the `ret` instruction, signifying
    the end of the function. In this case, `%eax` contains `y`, and
    `getSmallest` returns `y`.


Now, suppose that the branch was taken at `<getSmallest+12>`. In other
words, the `jle` instruction sets register `%eip` to 0x8048421 (i.e.,
`<getSmallest+22>`). Then, the next instructions to execute are:



-   The `mov 0x8(%ebp),%eax` instruction at address 0x8048421, which
    copies the `x` to register `%eax`. Register `%eip` advances to
    0x8048427.

-   The last instruction that executes is `ret`, signifying the end of
    the function. In this case, `%eax` contains `x`, and `getSmallest`
    returns `x`.


We can then annotate the preceding assembly as follows:




    0x8048411 <+6>:  mov 0x8(%ebp),%eax             #copy x to %eax
    0x8048414 <+9>:  cmp 0xc(%ebp),%eax             #compare x with y
    0x8048417 <+12>: jle 0x8048421 <getSmallest+22> #if x<=y goto<getSmallest+22>
    0x8048419 <+14>: mov 0xc(%ebp),%eax             #copy y to %eax
    0x804841f <+20>: jmp 0x8048427 <getSmallest+28> #goto <getSmallest+28>
    0x8048421 <+22>: mov 0x8(%ebp),%eax             #copy x to %eax
    0x8048427 <+28>: ret                            #exit function (return %eax)


Translating this back to C code yields:


+-----------------------------------+-----------------------------------+
| goto Form                         | Translated C code                 |
+===================================+===================================+
|                        |                        |
|                   |                   |
|                        |                        |
| ```     | ```     |
| int getSmallest(int x, int y) {   | int getSmallest(int x, int y) {   |
|     int smallest;                 |     int smallest;                 |
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

: Table 1. Translating `getSmallest` into goto C form and C code.

In Table 1, the variable `smallest` corresponds
to register `%eax`. If `x` is less than or equal to `y`, the code
executes the statement `smallest = x`, which is associated with the
`goto` label `assign_x` in our `goto` form of this function. Otherwise,
the statement `smallest = y` is executed. The `goto` label `done` is
used to indicate that the value in `smallest` should be returned.


Notice that the preceding C translation of the assembly code is a bit
different from the original `getSmallest` function. These differences
don't matter; a close inspection of both functions reveals that the two
programs are logically equivalent. However, the compiler first converts
any `if` statement into an equivalent `goto` form, which results in the
slightly different, but equivalent, version. [Table
2](#StandardGotoIf32) shows the standard `if` statement format and its
equivalent `goto` form:


+-----------------------------------+-----------------------------------+
| C `if`-statement                  | Compiler's equivalent `goto` form |
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

: Table 2. Standard `if`-statement format and its equivalent `goto`
form.

Compilers translating code into assembly designate a jump when a
condition is true. Contrast this behavior with the structure of an `if`
statement, where a \"jump\" (to the `else`) occurs when conditions are
*not* true. The `goto` form captures this difference in logic.


Considering the original `goto` translation of the `getSmallest`
function, we can see that:



-   `x <= y` corresponds to `!condition`.

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
in mind that a function written in different ways in the C language can
translate to the same set of assembly instructions.



#### The cmov Instructions 

The last set of conditional instructions we cover are **conditional
move** (`cmov`) instructions. The `cmp`, `test`, and `jmp` instructions
implement a *conditional transfer of control* in a program. In other
words, the execution of the program branches in many directions. This
can be very problematic for optimizing code, because these branches are
very expensive.


In contrast, the `cmov` instruction implements a *conditional transfer
of data*. In other words, both the `then`-statement\` and
`else`-statement\` of the conditional are executed, and the data is
placed in the appropriate register based on the result of the condition.


The use of C's **ternary expression** often results in the compiler
generating a `cmov` instruction in place of jumps. For the standard
if-then-else statement, the ternary expression has the form:




```
result = (condition) ? then_statement : else_statement;
```


Let's use this format to rewrite the `getSmallest` function as a ternary
expression. Keep in mind that this new version of the function behaves
exactly as the original `getSmallest` function:




```
int getSmallest_cmov(int x, int y) {
    return x > y ? y : x;
}
```


Although this may not seem like a big change, let's look at the
resulting assembly. Recall that the first and second parameters (`x` and
`y`) are stored at stack addresses `%ebp+0x8` and `%ebp+0xc`,
respectively.




    0x08048441 <+0>:   push   %ebp              #save ebp
    0x08048442 <+1>:   mov    %esp,%ebp         #update ebp
    0x08048444 <+3>:   mov    0xc(%ebp),%eax    #copy y to %eax
    0x08048447 <+6>:   cmp    %eax,0x8(%ebp)    #compare x with y
    0x0804844a <+9>:   cmovle 0x8(%ebp),%eax    #if (x <= y) copy x to %eax
    0x0804844e <+13>:  pop    %ebp              #restore %ebp
    0x0804844f <+14>:  ret                      #return %eax


This assembly code has no jumps. After the comparison of `x` and `y`,
`x` moves into the return register only if `x` is less than or equal to
`y`. Like the jump instructions, the suffix of the `cmov` instructions
indicates the condition on which the conditional move occurs. [Table
3](#CmovInstr32) lists the set of conditional move instructions.


+----------------------+----------------------+-----------------------+
| Signed               | Unsigned             | Description           |
+======================+======================+=======================+
| `cmove` (`cmovz`)    |                      | move if equal (==)    |
+----------------------+----------------------+-----------------------+
| `cmovne` (`cmovnz`)  |                      | move if not equal     |
|                      |                      | (!=)                  |
+----------------------+----------------------+-----------------------+
| `cmovs`              |                      | move if negative      |
+----------------------+----------------------+-----------------------+
| `cmovns`             |                      | move if non-negative  |
+----------------------+----------------------+-----------------------+
| `cmovg` (`cmovnle`)  | `cmova` (`cmovnbe`)  | move if greater (\>)  |
+----------------------+----------------------+-----------------------+
| `cmovge` (`cmovnl`)  | `cmovae` (`cmovnb`)  | move if greater than  |
|                      |                      | or equal (\>=)        |
+----------------------+----------------------+-----------------------+
| `cmovl` (`cmovnge`)  | `cmovb` (`cmovnae`)  | move if less (\<)     |
+----------------------+----------------------+-----------------------+
| `cmovle` (`cmovng`)  | `cmovbe` (`cmovna`)  | move if less than or  |
|                      |                      | equal (\<=)           |
+----------------------+----------------------+-----------------------+

: Table 3. The cmov Instructions.

The compiler is very cautious about converting jump instructions into
`cmov` instructions, especially in cases where side effects and pointer
values are involved. Table 4 shows two equivalent ways
of writing a function, `incrementX`:


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

: Table 4. Two functions that attempt to increment the value of integer
`x`.

Each function takes a pointer to an integer as input and checks whether
it is `NULL`. If `x` is not `NULL`, the function increments and returns
the dereferenced value of `x`. Otherwise, the function returns the value
1.


It is tempting to think that `incrementX2` uses a `cmov` instruction
because it uses a ternary expression. However, both functions yield the
exact same assembly code:




    0x80484cf <+0>:   push   %ebp
    0x80484d0 <+1>:   mov    %esp,%ebp
    0x80484d2 <+3>:   cmpl   $0x0,0x8(%ebp)
    0x80484d6 <+7>:   je     0x80484e7 <incrementX2+24>
    0x80484d8 <+9>:   mov    0x8(%ebp),%eax
    0x80484db <+12>:  mov    (%eax),%eax
    0x80484dd <+14>:  lea    0x1(%eax),%ecx
    0x80484e0 <+17>:  mov    0x8(%ebp),%edx
    0x80484e3 <+20>:  mov    %ecx,(%edx)
    0x80484e5 <+22>:  jmp    0x80484ec <incrementX2+29>
    0x80484e7 <+24>:  mov    $0x1,%eax
    0x80484ec <+29>:  pop    %ebp
    0x80484ed <+30>:  ret


Recall that the `cmov` instruction *executes both branches of the
conditional*. In other words, `x` gets dereferenced no matter what.
Consider the case where `x` is a null pointer. Recall that dereferencing
a null pointer leads to a null pointer exception in the code, causing a
segmentation fault. To prevent any chance of this happening, the
compiler takes the safe road and uses jumps.





