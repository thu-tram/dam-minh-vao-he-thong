

 

































### 9.4.3. Loops in Assembly 

Like `if` statements, loops in assembly are also implemented using
branch instructions. However, loops enable instructions to be
*revisited* based on the result of an evaluated condition.


The `sumUp` function in the following example sums up all the positive
integers from 1 to a user-defined integer *n*. This code is
intentionally written suboptimally to illustrate a `while` loop in C.




```
int sumUp(int n) {
    //initialize total and i
    int total = 0;
    int i = 1;

    while (i <= n) {  //while i is less than or equal to n
        total += i;   //add i to total
        i++;          //increment i by 1
    }
    return total;
}
```


Compiling this code and disassembling it using GDB yields the following
assembly code:




    Dump of assembler code for function sumUp:
    0x0724 <+0>:   sub   sp, sp, #0x20
    0x0728 <+4>:   str   w0, [sp, #12]
    0x072c <+8>:   str   wzr, [sp, #24]
    0x0730 <+12>:  mov   w0, #0x1
    0x0734 <+16>:  str   w0, [sp, #28]
    0x0738 <+20>:  b     0x758 <sumUp+52>
    0x073c <+24>:  ldr   w1, [sp, #24]
    0x0740 <+28>:  ldr   w0, [sp, #28]
    0x0744 <+32>:  add   w0, w1, w0
    0x0748 <+36>:  str   w0, [sp, #24]
    0x074c <+40>:  ldr   w0, [sp, #28]
    0x0750 <+44>:  add   w0, w0, #0x1
    0x0754 <+48>:  str   w0, [sp, #28]
    0x0758 <+52>:  ldr   w1, [sp, #28]
    0x075c <+56>:  ldr   w0, [sp, #12]
    0x0760 <+60>:  cmp   w1, w0
    0x0764 <+64>:  b.le  0x73c <sumUp+24>
    0x0768 <+68>:  ldr   w0, [sp, #24]
    0x076c <+72>:  add   sp, sp, #0x20
    0x0770 <+76>:  ret


Again, we will not draw out the stack explicitly in this example.
However, we encourage readers to draw the stack out themselves.



#### The First Five Instructions 

The first five instructions of this function set the stack up for
function execution and store some temporary values:




    0x0724 <+0>:  sub  sp, sp, #0x20   //grow stack by 32 bytes (new stack frame)
    0x0728 <+4>:  str  w0, [sp, #12]   //store n at sp+12 (n)
    0x072c <+8>:  str  wzr, [sp, #24]  //store 0 at sp+24 (total)
    0x0730 <+12>: mov  w0, #0x1        //w0 = 1
    0x0734 <+16>: str  w0, [sp, #28]   //store 1 at sp+28 (i)


Specifically, they:



-   Grow the call stack by 32 bytes, marking the new frame.

-   Store the first parameter (`n`) at stack location `sp + 12`.

-   Store the value 0 at stack location `sp + 24`, indicating `total`.

-   Copy the value 1 into register `w0`.

-   Store the value 1 at stack location `sp + 28`, indicating `i`.


Recall that stack locations store *temporary variables* in a function.
For simplicity we will refer to the location marked by `sp + 24` as
`total` and `sp + 28` as `i`. The input parameter to `sumUp` (`n`) is
located at stack address `sp + 12`. Despite the placement of temporary
variables on the stack, keep in mind that the stack pointer has not
changed after the execution of the first instruction
(`sub sp, sp, #0x20`).



#### The Heart of the Loop 

The next 12 instructions in the `sumUp` function represent the heart of
the loop:




    0x0738 <+20>: b     0x758 <sumUp+52>  // goto <sumUp+52>
    0x073c <+24>: ldr   w1, [sp, #24]     // w1 = total
    0x0740 <+28>: ldr   w0, [sp, #28]     // w0 = i
    0x0744 <+32>: add   w0, w1, w0        // w0 =  i + total
    0x0748 <+36>: str   w0, [sp, #24]     // store (total + i) in total
    0x074c <+40>: ldr   w0, [sp, #28]     // w0 = i
    0x0750 <+44>: add   w0, w0, #0x1      // w0 = i + 1
    0x0754 <+48>: str   w0, [sp, #28]     // store (i+1) in i (i.e. i++)
    0x0758 <+52>: ldr   w1, [sp, #28]     // w1 = i
    0x075c <+56>: ldr   w0, [sp, #12]     // w0 = n
    0x0760 <+60>: cmp   w1, w0            // compare i and n
    0x0764 <+64>: b.le  0x73c <sumUp+24>  // if (i <= n) goto <sumUp+24>



-   The first instruction is a direct jump to `<sumUp+52>`, which sets
    the program counter register (`pc`) to address 0x758.

-   The next two instructions that execute (at `<sumUp+52>` and
    `<sumUp+56>`) load `i` and `n` into registers `w1` and `w0`,
    respectively.

-   The `cmp` instruction at `<sumUp+60>` compares `i` and `n`, setting
    the appropriate condition flags. The program counter `pc` advances
    to the next instruction, or address 0x764.

-   The `b.le` instruction at `<sumUp+64>` replaces the `pc` register
    with address 0x73c if `i` is less than or equal to `n`.


If the branch is taken (that is, if `i <= n`), program execution jumps
to `<sumUp+24>` and the following instructions execute:



-   The `ldr` instructions at `<sumUp+24>` and `<sumUp+28>` load `total`
    and `i` into registers `w1` and `w0`, respectively.

-   The `add` instruction at `<sumUp+32>` then adds `total` to `i`
    (i.e., `i + total`) and stores the result in `w0`.

-   The `str` instruction at `<sumUp+36>` then updates `total` with the
    value in register `w0` (i.e., `total = total + i`)

-   The `ldr` instruction at `<sumUp+40>` loads `i` into register `w0`.

-   The `add` instruction at `<sumUp+44>` adds 1 to `i` and stores the
    result in register `w0`.

-   The `str` instruction at `<sumUp+48>` updates `i` with the value
    stored in register `w0` (i.e., `i = i + 1`)

-   The `ldr` instructions at `<sumUp+52>` and `<sumUp+56>` load `i` and
    `n` into registers `w1` and `w0`, respectively.

-   The `cmp` instruction at `<sumUp+60>` compares `i` to `n` and sets
    the appropriate condition code flags.

-   The `b.le` instruction then executes. If `i` is less than or equal
    to `n`, program execution jumps back to `<sumUp+24>`, `pc` is set to
    0x73c, and the instructions between `<sumUp+24>` and `<sumUp+64>`
    repeat execution. Otherwise, register `pc` is set to the address of
    the next instruction in sequence, or 0x768 (`<sumUp+68>`).


If the branch is *not* taken (i.e., `i` is greater than `n`), the
following instructions execute:




    0x0768 <+68>:  ldr   w0, [sp, #24]   // w0 = total
    0x076c <+72>:  add   sp, sp, #0x20   // restore stack
    0x0770 <+76>:  ret                   // return w0 (total)


These instructions copy `total` to the return register `w0`, restore the
call stack by shrinking `sp`, and exit the function. Thus, the function
returns `total` upon exit.


Table 1 shows the assembly and C `goto` forms of the
`sumUp` function side by side:


+-----------------------------------+-----------------------------------+
| Assembly                          | Translated Goto Form              |
+===================================+===================================+
|                        |                        |
|                   |                   |
|                        |                        |
| ```     | ```     |
| <sumUp>:                          | int sumUp(int n) {                |
|   <+0>:   sub   sp, sp, #0x20     |     int total = 0;                |
|   <+4>:   str   w0, [sp, #12]     |     int i = 1;                    |
|   <+8>:   str   wzr, [sp, #24]    |     goto start;                   |
|   <+12>:  mov   w0, #0x1          | body:                             |
|   <+16>:  str   w0, [sp, #28]     |     total += i;                   |
|   <+20>:  b     0x758 <sumUp+52>  |     i += 1;                       |
|   <+24>:  ldr   w1, [sp, #24]     | start:                            |
|   <+28>:  ldr   w0, [sp, #28]     |     if (i <= n) {                 |
|   <+32>:  add   w0, w1, w0        |         goto body;                |
|   <+36>:  str   w0, [sp, #24]     |     }                             |
|   <+40>:  ldr   w0, [sp, #28]     |     return total;                 |
|   <+44>:  add   w0, w0, #0x1      | }                                 |
|   <+48>:  str   w0, [sp, #28]     | ```                               |
|   <+52>:  ldr   w1, [sp, #28]     | :::                               |
|   <+56>:  ldr   w0, [sp, #12]     | :::                               |
|   <+60>:  cmp   w1, w0            | :::                               |
|   <+64>:  b.le  0x73c <sumUp+24>  |                                   |
|   <+68>:  ldr   w0, [sp, #24]     |                                   |
|   <+72>:  add   sp, sp, #0x20     |                                   |
|   <+76>:  ret                     |                                   |
| ```                               |                                   |
| :::                               |                                   |
| :::                               |                                   |
| :::                               |                                   |
+-----------------------------------+-----------------------------------+

: Table 1. Translating `sumUp()` into goto C form.

The preceding code is also equivalent to the following C code without
`goto` statements:




```
int sumUp(int n) {
    int total = 0;
    int i = 1;
    while (i <= n) {
        total += i;
        i += 1;
    }
    return total;
}
```



#### for Loops in Assembly 

The primary loop in the `sumUp` function can also be written as a `for`
loop:




```
int sumUp2(int n) {
    int total = 0;             //initialize total to 0
    int i;
    for (i = 1; i <= n; i++) { //initialize i to 1, increment by 1 while i<=n
        total += i;            //updates total by i
    }
    return total;
}
```


This yields identical assembly code to our `while` loop example. We
repeat the assembly code below and annotate each line with its English
translation:




    Dump of assembler code for function sumUp2:
    0x0774 <+0>:  sub   sp, sp, #0x20     // grow stack by 32 bytes (new frame)
    0x0778 <+4>:  str   w0, [sp, #12]     // store n at sp+12 (n)
    0x077c <+8>:  str   wzr, [sp, #24]    // store 0 at sp+24 (total)
    0x0780 <+12>: mov   w0, #0x1          // w0 = 1
    0x0784 <+16>: str   w0, [sp, #28]     // store 1 at sp+28 (i)
    0x0788 <+20>: b     0x7a8 <sumUp2+52> // goto <sumUp2+52>
    0x078c <+24>: ldr   w1, [sp, #24]     // w1 = total
    0x0790 <+28>: ldr   w0, [sp, #28]     // w0 = i
    0x0794 <+32>: add   w0, w1, w0        // w0 = total + i
    0x0798 <+36>: str   w0, [sp, #24]     // store (total+i) in total
    0x079c <+40>: ldr   w0, [sp, #28]     // w0 = i
    0x07a0 <+44>: add   w0, w0, #0x1      // w0 = i + 1
    0x07a4 <+48>: str   w0, [sp, #28]     // store (i+1) in i (i.e. i += 1)
    0x07a8 <+52>: ldr   w1, [sp, #28]     // w1 = i
    0x07ac <+56>: ldr   w0, [sp, #12]     // w0 = n
    0x07b0 <+60>: cmp   w1, w0            // compare i and n
    0x07b4 <+64>: b.le  0x78c <sumUp2+24> // if (i <= n) goto <sumUp2+24>
    0x07b8 <+68>: ldr   w0, [sp, #24]     // w0 = total
    0x07bc <+72>: add   sp, sp, #0x20     // restore stack
    0x07c0 <+76>: ret                     // return w0 (total)


To understand why the `for` loop version of this code results in
identical assembly to the `while` loop version of the code, recall that
the `for` loop has the following representation:




```
for ( <initialization>; <boolean expression>; <step> ){
    <body>
}
```


This is equivalent to the following `while` loop representation:




```
<initialization>
while (<boolean expression>) {
    <body>
    <step>
}
```


Since [every `for` loop can be represented by a `while`
loop](../C1-C_intro/conditionals.html#_for_loops), the following
two C programs are equivalent representations for the previous assembly:


+-----------------------------------+-----------------------------------+
| For loop                          | While loop                        |
+===================================+===================================+
|                        |                        |
|                   |                   |
|                        |                        |
| ```     | ```     |
| int sumUp2(int n) {               | int sumUp(int n){                 |
|     int total = 0;                |     int total = 0;                |
|     int i = 1;                    |     int i = 1;                    |
|     for (i; i <= n; i++) {        |     while (i <= n) {              |
|         total += i;               |         total += i;               |
|     }                             |         i += 1;                   |
|     return total;                 |     }                             |
| }                                 |     return total;                 |
| ```                               | }                                 |
| :::                               | ```                               |
| :::                               | :::                               |
| :::                               | :::                               |
|                                   | :::                               |
+-----------------------------------+-----------------------------------+

: Table 2. Equivalent ways to write the sumUp function.





