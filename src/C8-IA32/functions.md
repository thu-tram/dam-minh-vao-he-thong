

 







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





















## 8.5. Functions in Assembly 

In the previous section, we traced through simple functions in assembly.
In this section, we discuss the interaction between multiple functions
in assembly in the context of a larger program. We also introduce some
new instructions involved with function management.


Let's begin with a refresher on how the call stack is managed. Recall
that `%esp` is the **stack pointer** and always points to the top of the
stack. The register `%ebp` represents the base pointer (also known as
the **frame pointer**) and points to the base of the current stack
frame. The **stack frame** (also known as the **activation frame** or
the **activation record**) refers to the portion of the stack allocated
to a single function call. The currently executing function is always at
the top of the stack, and its stack frame is referred to as the **active
frame**. The active frame is bounded by the stack pointer (at the top of
stack) and the frame pointer (at the bottom of the frame). The
activation record typically holds local variables and parameters for a
function.


Figure 1 shows the stack frames for `main` and a
function it calls named `fname`. We will refer to the `main` function as
the *caller* function and `fname` as the *callee* function.




![an illustration of stack frames](_images/stackFrame.png)


Figure 1. Stack frame management


In Figure 1, the current active frame belongs to the
callee function (`fname`). The memory between the stack pointer and the
frame pointer is used for local variables. The stack pointer moves as
local values are pushed and popped from the stack. In contrast, the
frame pointer remains relatively constant, pointing to the beginning
(the bottom) of the current stack frame. As a result, compilers like GCC
commonly reference values on the stack relative to the frame pointer. In
Figure 1, the active frame is bounded below by the base
pointer of `fname`, which contains the stack address 0x418. The value
stored at this address is the \"saved\" `%ebp` value (0x42c), which
itself indicates the bottom of the activation frame for the `main`
function. The top of the activation frame of `main` is bounded by the
**return address**, which indicates the program address at which `main`
resumes execution once the callee function finishes executing.



+-----------------------------------+-----------------------------------+
|                                   |                          |
|                                   | The return address points to      |
|                                   | program memory, not stack memory  |
|                                   | :::                               |
|                                   |                                   |
|                                   | ::: paragraph                     |
|                                   | Recall that the call stack region |
|                                   | (stack memory) of a program is    |
|                                   | different from its code region    |
|                                   | (code memory). Whereas `%ebp` and |
|                                   | `%esp` point to locations in the  |
|                                   | stack memory, `%eip` points to a  |
|                                   | location in *code* memory. In     |
|                                   | other words, the return address   |
|                                   | is an address in code memory, not |
|                                   | stack memory:                     |
|                                   | :::                               |
|                                   |                                   |
|                                   | ::: {#ProgramMe                   |
|                                   | mory632 .imageblock .text-center} |
|                                   |                        |
|                                   | ![The parts of a program's        | |                                   | address                           | |                                   | space.](_                         | |                                   | images/memparts.png) | |                                   | :::                               | |                                   |                                   | |                                   |                          | |                                   | Figure 2. The parts of a          | |                                   | program's address space           | |                                   | :::                               | |                                   | :::                               | +-----------------------------------+-----------------------------------+ :::  Table 1 contains several additional instructions that the compiler uses for basic function management. :::  +-----------------------------------+-----------------------------------+ | Instruction                       | Translation                       | +===================================+===================================+ | `leave`                           |                        | |                                   | ::: paragraph                     | |                                   | Prepares the stack for leaving a  | |                                   | function. Equivalent to:          | |                                   | :::                               | |                                   |                                   | |                                   |                   | |                                   |                        | |                                   |     mov %ebp, %esp                | |                                   |     pop %ebp                      | |                                   | :::                               | |                                   | :::                               | |                                   | :::                               | +-----------------------------------+-----------------------------------+ | `call addr <fname>`               |                        | |                                   | ::: paragraph                     | |                                   | Switches active frame to callee   | |                                   | function. Equivalent to:          | |                                   | :::                               | |                                   |                                   | |                                   |                   | |                                   |                        | |                                   |     push %eip                     | |                                   |     mov addr, %eip                | |                                   | :::                               | |                                   | :::                               | |                                   | :::                               | +-----------------------------------+-----------------------------------+ | `ret`                             |                        | |                                   | ::: paragraph                     | |                                   | Restores active frame to caller   | |                                   | function. Equivalent to:          | |                                   | :::                               | |                                   |                                   | |                                   |                   | |                                   |                        | |                                   |     pop %eip                      | |                                   | :::                               | |                                   | :::                               | |                                   | :::                               | +-----------------------------------+-----------------------------------+  : Table 1. Common Function Management Instructions  For example, the `leave` instruction is a shorthand that the compiler uses to restore the stack and frame pointers as it prepares to leave a function. When the callee function finishes execution, `leave` ensures that the frame pointer is *restored* to its previous value. :::  The `call` and `ret` instructions play a prominent role in the process where one function calls another. Both instructions modify the instruction pointer (register `%eip`). When the caller function executes the `call` instruction, the current value of `%eip` is saved on the stack to represent the return address, or the program address at which the caller resumes executing once the callee function finishes. The `call` instruction also replaces the value of `%eip` with the address of the callee function. :::  The `ret` instruction restores the value of `%eip` to the value saved on the stack, ensuring that the program resumes execution at the program address specified in the caller function. Any value returned by the callee is stored in `%eax`. The `ret` instruction is usually the last instruction that executes in any function. :::   ### 8.5.1. Tracing Through an Example   Using our knowledge of function management, let's trace through the code example first introduced at the beginning of this chapter. :::    ``` #include <stdio.h>  int assign(void) {     int y = 40;     return y; }  int adder(void) {     int a;     return a + 2; }  int main(void) {     int x;     assign();     x = adder();     printf("x is: %d\n", x);     return 0; } ```   We compile the code with the `-m32` flag and use `objdump -d` to view the underlying assembly. The latter command outputs a pretty big file that contains a lot of information that we don't need. Use `less` and the search functionality to extract the `adder`, `assign`, and `main` functions: :::        804840d <assign>:      804840d:       55                      push   %ebp      804840e:       89 e5                   mov    %esp,%ebp      8048410:       83 ec 10                sub    $0x10,%esp      8048413:       c7 45 fc 28 00 00 00    movl   $0x28,-0x4(%ebp)      804841a:       8b 45 fc                mov    -0x4(%ebp),%eax      804841d:       c9                      leave      804841e:       c3                      ret      0804841f <adder>:      804841f:       55                      push   %ebp      8048420:       89 e5                   mov    %esp,%ebp      8048422:       83 ec 10                sub    $0x10,%esp      8048425:       8b 45 fc                mov    -0x4(%ebp),%eax      8048428:       83 c0 02                add    $0x2,%eax      804842b:       c9                      leave      804842c:       c3                      ret      0804842d <main>:      804842d:       55                      push   %ebp      804842e:       89 e5                   mov    %esp,%ebp      8048433:       83 ec 20                sub    $0x14,%esp      8048436:       e8 d2 ff ff ff          call   804840d <assign>      804843b:       e8 df ff ff ff          call   804841f <adder>      8048440:       89 44 24 1c             mov    %eax,0xc(%esp)      8048444:       8b 44 24 1c             mov    0xc(%esp),%eax      8048448:       89 44 24 04             mov    %eax,0x4(%esp)      804844c:       c7 04 24 f4 84 04 08    movl   $0x80484f4,(%esp)      8048453:       e8 88 fe ff ff          call   80482e0 <printf@plt>      8048458:       b8 00 00 00 00          mov    $0x0,%eax      804845d:       c9                      leave      804845e:       c3                      ret   Each function begins with a symbolic label that corresponds to its declared name in the program. For example, `<main>:` is the symbolic label for the `main` function. The address of a function label is also the address of the first instruction in that function. To save space in the figures that follow, we truncate addresses to the lower 12 bits. So, program address 0x804842d is shown as 0x42d.    ### 8.5.2. Tracing Through main   Figure 3 shows the execution stack immediately prior to the execution of `main`. :::    ![slide1](_images/procedures/Slide1.png)


Figure 3. The initial state of the CPU registers and call stack prior to
executing the main function


Recall that the stack grows toward lower addresses. In this example,
`%ebp` is address 0x140, and `%esp` is address 0x130 (both of these
values are made up for this example). Registers `%eax` and `%edx`
initially contain junk values. The red (upper-left) arrow indicates the
currently executing instruction. Initially, `%eip` contains address
0x42d, which is the program memory address of the first line in the
`main` function. Let's trace through the program's execution together.




![slide2](_images/procedures/Slide2.png)


The first instruction pushes the value of `ebp` onto the stack, saving
address 0x140. Since the stack grows toward lower addresses, the stack
pointer `%esp` updates to 0x12c, which is 4 bytes less than 0x130.
Register `%eip` advances to the next instruction in sequence.




![slide3](_images/procedures/Slide3.png)


The next instruction (`mov %esp, %ebp`) updates the value of `%ebp` to
be the same as `%esp`. The frame pointer (`%ebp`) now points to the
start of the stack frame for the `main` function. `%eip` advances to the
next instruction in sequence.




![slide4](_images/procedures/Slide4.png)


The `sub` instruction subtracts 0x14 from the address of our stack
pointer, \"growing\" the stack by 20 bytes. Register `%eip` advances to
the next instruction, which is the first `call` instruction.




![slide5](_images/procedures/Slide5.png)


The `call <assign>` instruction pushes the value inside register `%eip`
(which denotes the address of the *next* instruction to execute) onto
the stack. Since the next instruction after `call <assign>` has the
address 0x43b, that value is pushed onto the stack as the return
address. Recall that the return address indicates the program address
where execution should resume when program execution returns to `main`.


Next, the `call` instruction moves the address of the `assign` function
(0x40d) into register `%eip`, signifying that program execution should
continue into the callee function `assign` and not the next instruction
in `main`.




![slide6](_images/procedures/Slide6.png)


The first two instructions that execute in the `assign` function are the
usual book-keeping that every function performs. The first instruction
pushes the value stored in `%ebp` (memory address 0x12c) onto the stack.
Recall that this address points to the beginning of the stack frame for
`main`. `%eip` advances to the second instruction in `assign`.




![slide7](_images/procedures/Slide7.png)


The next instruction (`mov %esp, %ebp`) updates `%ebp` to point to the
top of the stack, marking the beginning of the stack frame for `assign`.
The instruction pointer (`%eip`) advances to the next instruction in the
`assign` function.




![slide8](_images/procedures/Slide8.png)


The `sub` instruction at address 0x410 grows the stack by 16 bytes,
creating extra space on the stack frame to store local values and
updating `%esp`. The instruction pointer again advances to the next
instruction in the `assign` function.




![slide9](_images/procedures/Slide9.png)


The `mov` instruction at address 0x413 moves the value `$0x28` (or 40)
onto the stack at address `-0x4(%ebp)`, which is four bytes above the
frame pointer. Recall that the frame pointer is commonly used to
reference locations on the stack. `%eip` advances to the next
instruction in the `assign` function.




![slide10](_images/procedures/Slide10.png)


The `mov` instruction at address 0x41a places the value `$0x28` into
register `%eax`, which holds the return value of the function. `%eip`
advances to the `leave` instruction in the `assign` function.




![slide11](_images/procedures/Slide11.png)


At this point, the `assign` function has almost completed execution. The
next instruction that executes is the `leave` instruction, which
prepares the stack for returning from the function call. Recall that
`leave` is analogous to the following pair of instructions:




    mov %ebp, %esp
    pop %ebp


In other words, the CPU overwrites the stack pointer with the frame
pointer. In our example, the stack pointer is initially updated from
0x100 to 0x110. Next, the CPU executes `pop %ebp`, which takes the value
located at 0x110 (in our example, the address 0x12c) and places it in
`%ebp`. Recall that 0x12c is the start of the stack frame for `main`.
`%esp` becomes 0x114 and `%eip` points to the `ret` instruction in the
`assign` function.




![slide12](_images/procedures/Slide12.png)


The last instruction in `assign` is a `ret` instruction. When `ret`
executes, the return address is popped off the stack into register
`%eip`. In our example, `%eip` now advances to the call to the `adder`
function.


Some important things to notice at this juncture:



-   The stack pointer and frame pointer have been restored to their
    values prior to the call to `assign`, reflecting that the stack
    frame for `main` is once again the active frame.

-   The old values on the stack from the prior active stack frame are
    *not* removed. They still exist on the call stack.




![slide13](_images/procedures/Slide13.png)


The call to `adder` *overwrites* the old return address on the stack
with a new return address (0x440). This return address points to the
next instruction to be executed after `adder` returns, or
`mov %eax, 0xc(%ebp)`. `%eip` reflects the first instruction to execute
in `adder`, which is at address 0x41f.




![slide14](_images/procedures/Slide14.png)


The first instruction in the `adder` function saves the caller's frame
pointer (`%ebp` of `main`) on the stack.




![slide15](_images/procedures/Slide15.png)


The next instruction updates `%ebp` with the current value of `%esp`, or
address 0x110. Together, these last two instructions establish the
beginning of the stack frame for `adder`.




![slide16](_images/procedures/Slide16.png)


The `sub` instruction at address 0x422 \"grows\" the stack by 16 bytes.
Notice again that growing the stack does not affect any previously
created values on the stack. Again, old values will litter the stack
until they are overwritten.




![slide20](_images/procedures/Slide17.png)


Pay close attention to the next instruction that executes:
`mov $-0x4(%ebp), %eax`. This instruction moves an *old* value that is
on the stack into register `%eax`! This is a direct result of the fact
that the programmer forgot to initialize `a` in the function `adder`.




![slide18](_images/procedures/Slide18.png)


The `add` instruction at address 0x428 adds 2 to register `%eax`. Recall
that IA32 passes the return value through register `%eax`. Together, the
last two instructions are equivalent to the following code in `adder`:




```
int a;
return a + 2;
```




![slide19](_images/procedures/Slide19.png)


After `leave` executes, the frame pointer again points to the beginning
of the stack frame for `main`, or address 0x12c. The stack pointer now
stores the address 0x114.




![slide20](_images/procedures/Slide20.png)


The execution of `ret` pops the return address off the stack, restoring
the instruction pointer back to 0x440, or the address of the next
instruction to execute in `main`. The address of `%esp` is now 0x118.




![slide21](_images/procedures/Slide21.png)


The `mov %eax, 0xc(%esp)` instruction places the value in `%eax` in a
location 12 bytes (three spaces) below `%esp`.




![slide23](_images/procedures/Slide23.png)


Skipping ahead a little, the `mov` instructions at addresses 0x444 and
0x448 set `%eax` to the value saved at location `%esp+12` (or 0x2A) and
places 0x2A one spot below the top of the stack (address `%esp + 4`, or
0x11c).




![slide24](_images/procedures/Slide24.png)


The next instruction (`mov $0x80484f4, (%esp)`) copies a constant value
that is a memory address to the top of the stack. This particular memory
address, 0x80484f4, contains the string `"x is %d\n"`. The instruction
pointer advances to the call to the `printf` function (which is denoted
with the label `<printf@plt>`).




![slide25](_images/procedures/Slide25.png)


For the sake of brevity, we will not trace the `printf` function, which
is part of `stdio.h`. However, we know from the manual page
(`man -s3 printf`) that `printf` has the following format:




```
int printf(const char * format, ...)
```


In other words, the first argument is a pointer to a string specifying
the format, and the second argument onward specify the values that are
used in that format. The instructions specified by addresses 0x444 -
0x45c correspond to the following line in the `main` function:




```
printf("x is %d\n", x);
```


When the `printf` function is called:



-   A return address specifying the instruction that executes after the
    call to `printf` is pushed onto the stack.

-   The value of `%ebp` is pushed onto the stack, and `%ebp` is updated
    to point to the top of the stack, indicating the beginning of the
    stack frame for `printf`.


At some point, `printf` references its arguments, which are the string
`"x is %d\n"` and the value 0x2A. Recall that the return address is
located directly below `%ebp` at location `%ebp+4`. The first argument
is thus located at `%ebp+8` (i.e., right *below* the return address),
and the second argument is located at `%ebp+12`.


For any function with *n* arguments, GCC places the first argument at
location `%ebp+8`, the second at `%ebp+12`, and the *n*^th^ argument at
location `(%ebp+8) + (4*(n-1))`.


After the call to `printf`, the value 0x2A is output to the user in
integer format. Thus, the value 42 is printed to the screen!




![slide26](_images/procedures/Slide26.png)


After the call to `printf`, the last few instructions clean up the stack
and prepare a clean exit from the `main` function. First, the value 0x0
is placed in register `%eax`, signifying that the value 0 is returned
from `main`. Recall that a program returns 0 to indicate correct
termination.




![slide27](_images/procedures/Slide27.png)


After `leave` and `ret` are executed, the stack and frame pointers
revert to their original values prior to the execution of `main`. With
0x0 in the return register `%eax`, the program returns 0.


If you have carefully read through this section, you should understand
why our program prints out the value 42. In essence, the program
inadvertently uses old values on the stack to cause it to behave in a
way that we didn't expect. While this example was pretty harmless, we
discuss in future sections how hackers have misused function calls to
make programs misbehave in truly malicious ways.






