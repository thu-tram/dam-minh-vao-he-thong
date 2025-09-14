

 

































### 8.4.3. Loops in assembly 

Like `if` statements, loops in assembly are also implemented using jump
instructions. However, loops enable instructions to be *revisited* based
on the result of an evaluated condition.


The `sumUp` function shown in the following example sums up all the
positive integers from 1 to a user-defined integer. This code is
intentionally written suboptimally to illustrate a `while` loop in C.




```
int sumUp(int n) {
    //initialize total and i
    int total = 0;
    int i = 1;

    while (i <= n) {  //while i is less than or equal to n
        total += i;   //add i to total
        i+=1;          //increment i by 1
    }
    return total;
}
```


Compiling this code with the `-m32` option and disassembling it using
GDB yields the following assembly code:




    (gdb) disas sumUp
    Dump of assembler code for function sumUp:
      0x804840b <+0>:   push   %ebp
      0x804840c <+1>:   mov    %esp,%ebp
      0x804840e <+3>:   sub    $0x10,%esp
      0x8048411 <+6>:   movl   $0x0,-0x8(%ebp)
      0x8048418 <+13>:  movl   $0x1,-0x4(%ebp)
      0x804841f <+20>:  jmp    0x804842b <sumUp+32>
      0x8048421 <+22>:  mov    -0x4(%ebp),%eax
      0x8048424 <+25>:  add    %eax,-0x8(%ebp)
      0x8048427 <+28>:  add   $0x1,-0x4(%ebp)
      0x804842b <+32>:  mov    -0x4(%ebp),%eax
      0x804842e <+35>:  cmp    0x8(%ebp),%eax
      0x8048431 <+38>:  jle    0x8048421 <sumUp+22>
      0x8048433 <+40>:  mov    -0x8(%ebp),%eax
      0x8048436 <+43>:  leave
      0x8048437 <+44>:  ret


Again, we will not draw out the stack explicitly in this example.
However, we encourage readers to draw the stack out themselves. Let's
analyze this assembly segment in parts:



#### The First Five Instructions 

The first five instructions of this function set the stack up for
function execution:




    0x804840b <+0>:   push   %ebp                 # save ebp on stack
    0x804840c <+1>:   mov    %esp,%ebp            # update ebp (new stack frame)
    0x804840e <+3>:   sub    $0x10,%esp           # add 16 bytes to stack frame
    0x8048411 <+6>:   movl   $0x0,-0x8(%ebp)      # place 0 at ebp-0x8 (total)
    0x8048418 <+13>:  movl   $0x1,-0x4(%ebp)      # place 1 at ebp-0x4 (i)


Recall that stack locations store *temporary variables* in a function.
For simplicity we will refer to the location marked by `%ebp - 0x8` as
`total` and `%ebp - 0x4` as `i`. The input parameter to `sumUp` is
located at `%ebp 0x8`.



#### The Heart of the Loop 

The next seven instructions in the `sumUp` function represent the heart
of the loop:




    0x804841f <+20>:  jmp    0x804842b <sumUp+32>  # goto <sumUp+32>
    0x8048421 <+22>:  mov    -0x4(%ebp),%eax       # copy i to eax
    0x8048424 <+25>:  add    %eax,-0x8(%ebp)       # add i to total (total+=i)
    0x8048427 <+28>:  add    $0x1,-0x4(%ebp)       # add 1 to i (i+=1)
    0x804842b <+32>:  mov    -0x4(%ebp),%eax       # copy i to eax
    0x804842e <+35>:  cmp    0x8(%ebp),%eax        # compare i with n
    0x8048431 <+38>:  jle    0x8048421 <sumUp+22>  # if (i <= n) goto <sumUp+22>


The first instruction is a direct jump to `<sumUp+32>`, which sets the
instruction pointer (`%eip`) to address 0x804842b.


The next instructions that execute (`<sumUp+32>` and `<sumUp+35>`) copy
the value of `i` to register `%eax` and compare `i` with the first
parameter to the `sumUp` function (or `n`). The `cmp` instruction sets
the appropriate condition codes in preparation for the `jle` instruction
at `<sumUp+38>`.


The `jle` instruction at `<sumUp+38>` then executes. If `i` is less than
or equal to `n`, the branch is taken and program execution jumps to
`<sumUp+22>`, and `%eip` is set to 0x8048421. The following instructions
then execute in sequence:



-   `mov -0x4(%ebp),%eax` copies `i` to register `%eax`.

-   `add %eax,-0x8(%ebp)` adds `i` to `total` (`total+=i`).

-   `add $0x1,-0x4(%ebp)` increments `i` by 1 (`i+=1`).

-   `mov -0x4(%ebp),%eax` copies `i` to register `%eax`.

-   `cmp 0x8(%ebp),%eax` compares `i` to `n`.

-   `jle 0x8048421 <sumUp+22>` jumps back to the beginning of this
    instruction sequence if `i` is less than or equal to `n`.


If the branch is not taken at `<sumUp+38>` (i.e., `i` is *not* less than
or equal to `n`), `total` is placed in the return register, and the
function exits.


Table 1 shows the assembly and C `goto` forms of the
`sumUp` function:


+-----------------------------------+-----------------------------------+
| Assembly                          | Translated goto Form              |
+===================================+===================================+
|                        |                        |
|                   |                   |
|                        |                        |
| ```     | ```     |
| <sumUp>:                          | int sumUp(int n) {                |
| <+0>:   push   %ebp               |     int total = 0;                |
| <+1>:   mov    %esp,%ebp          |     int i = 1;                    |
| <+3>:   sub    $0x10,%esp         |     goto start;                   |
| <+6>:   movl   $0x0,-0x8(%ebp)    | body:                             |
| <+13>:  movl   $0x1,-0x4(%ebp)    |     total += i;                   |
| <+20>:  jmp    <sumUp+32>         |     i += 1;                       |
| <+22>:  mov    -0x4(%ebp),%eax    | start:                            |
| <+25>:  add    %eax,-0x8(%ebp)    |     if (i <= n) {                 |
| <+28>:  addl   $0x1,-0x4(%ebp)    |         goto body;                |
| <+32>:  mov    -0x4(%ebp),%eax    |     }                             |
| <+35>:  cmp    0x8(%ebp),%eax     |     return total;                 |
| <+38>:  jle    <sumUp+22>         | }                                 |
| <+40>:  mov    -0x8(%ebp),%eax    | ```                               |
| <+43>:  leave                     | :::                               |
| <+44>:  ret                       | :::                               |
| ```                               | :::                               |
| :::                               |                                   |
| :::                               |                                   |
| :::                               |                                   |
+-----------------------------------+-----------------------------------+

: Table 1. Translating sumUp into goto C form.

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


This version yields assembly code identical to our `while` loop example.
We repeat the assembly code below and annotate each line with its
English translation:




    0x8048438 <+0>:  push   %ebp                  # save ebp
    0x8048439 <+1>:  mov    %esp,%ebp             # update ebp (new stack frame)
    0x804843b <+3>:  sub    $0x10,%esp            # add 16 bytes to stack frame
    0x804843e <+6>:  movl   $0x0,-0x8(%ebp)       # place 0 at ebp-0x8 (total)
    0x8048445 <+13>: movl   $0x1,-0x4(%ebp)       # place 1 at ebp-0x4 (i)
    0x804844c <+20>: jmp    0x8048458 <sumUp2+32> # goto <sumUp2+32>
    0x804844e <+22>: mov    -0x4(%ebp),%eax       # copy i to %eax
    0x8048451 <+25>: add    %eax,-0x8(%ebp)       # add %eax to total (total+=i)
    0x8048454 <+28>: addl   $0x1,-0x4(%ebp)       # add 1 to i (i+=1)
    0x8048458 <+32>: mov    -0x4(%ebp),%eax       # copy i to %eax
    0x804845b <+35>: cmp    0x8(%ebp),%eax        # compare i with n
    0x804845e <+38>: jle    0x804844e <sumUp2+22> # if (i <= n) goto <sumUp2+22>
    0x8048460 <+40>: mov    -0x8(%ebp),%eax       # copy total to %eax
    0x8048463 <+43>: leave                        # prepare to leave the function
    0x8048464 <+44>: ret                          # return total


To understand why the `for` loop version of this code results in
identical assembly to the `while` loop version of the code, recall that
the `for` loop has the following representation:




```
for ( <initialization>; <boolean expression>; <step> ){
    <body>
}
```


and is equivalent to the following `while` loop representation:




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





