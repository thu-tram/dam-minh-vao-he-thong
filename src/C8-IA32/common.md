

 







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





















## 8.2. Common Instructions 

In this section, we discuss several common x86 assembly instructions.
Table 1 lists the most foundational instructions in x86
assembly.


+-----------------------------------+-----------------------------------+
| Instruction                       | Translation                       |
+===================================+===================================+
| `mov S, D`                        | S → D (copies value of S into D)  |
+-----------------------------------+-----------------------------------+
| `add S, D`                        | S + D → D (adds S to D and stores |
|                                   | result in D)                      |
+-----------------------------------+-----------------------------------+
| `sub S, D`                        | D - S → D (subtracts S *from* D   |
|                                   | and stores result in D)           |
+-----------------------------------+-----------------------------------+

: Table 1. Most Common Instructions

Therefore, the sequence of instructions




    mov    0x8(%ebp),%eax
    add    $0x2,%eax


translates to:



-   Copy the value at location `%ebp` + 0x8 in *memory* (or M\[`%ebp` +
    0x8\]) to register `%eax`.

-   Add the value 0x2 to register `%eax`, and store the result in
    register `%eax`.


The three instructions shown in Table 1 also form the
building blocks for instructions that maintain the organization of the
program stack (i.e., the **call stack**). Recall that registers `%ebp`
and `%esp` refer to the *frame* pointer and *stack* pointer,
respectively, and are reserved by the compiler for call stack
management. Recall from our earlier discussion on [program
memory](../C2-C_depth/scope_memory.html#_parts_of_program_memory_and_scope)
that the call stack stores local variables and parameters and helps the
program track its own execution (see Figure 1).




![The parts of a program's address space.](_images/memparts.png)


Figure 1. The parts of a program's address space


On IA32 systems, the execution stack grows toward *lower* addresses.
Like all stack data structures, operations occur at the \"top\" of the
stack. The x86 ISA provides two instructions (Table 2) to
simplify call stack management.


+-----------------------------------+-----------------------------------+
| Instruction                       | Translation                       |
+===================================+===================================+
| `push S`                          |                        |
|                                   | ::: paragraph                     |
|                                   | Pushes a copy of `S` onto the top |
|                                   | of the stack. Equivalent to:      |
|                                   | :::                               |
|                                   |                                   |
|                                   |                   |
|                                   |                        |
|                                   |     sub $4, %esp                  |
|                                   |     mov S, (%esp)                 |
|                                   | :::                               |
|                                   | :::                               |
|                                   | :::                               |
+-----------------------------------+-----------------------------------+
| `pop D`                           |                        |
|                                   | ::: paragraph                     |
|                                   | Pops the top element off the      |
|                                   | stack and places it in location   |
|                                   | `D`. Equivalent to:               |
|                                   | :::                               |
|                                   |                                   |
|                                   |                   |
|                                   |                        |
|                                   |     mov (%esp), D                 |
|                                   |     add $4, %esp                  |
|                                   | :::                               |
|                                   | :::                               |
|                                   | :::                               |
+-----------------------------------+-----------------------------------+

: Table 2. Stack Management Instructions

Notice that while the three instructions in \[Basic\] require
two operands, the `push` and `pop` instructions in \[Stack\]
require only one operand apiece.



### 8.2.1. Putting It All Together: A More Concrete Example 

Let's take a closer look at the `adder2` function:




```
//adds two to an integer and returns the result
int adder2(int a) {
    return a + 2;
}
```


and its corresponding assembly code:




    0804840b <adder2>:
     804840b:       55                      push   %ebp
     804840c:       89 e5                   mov    %esp,%ebp
     804840e:       8b 45 08                mov    0x8(%ebp),%eax
     8048411:       83 c0 02                add    $0x2,%eax
     8048414:       5d                      pop    %ebp
     8048415:       c3                      ret


The assembly code consists of a `push` instruction, followed by a couple
of `mov` instructions, an `add` instruction, a `pop` instruction, and
finally a `ret` instruction. To understand how the CPU executes this set
of instructions, we need to revisit the structure of [program
memory](../C2-C_depth/scope_memory.html#_parts_of_program_memory_and_scope).
Recall that every time a program executes, the operating system
allocates the new program's address space (also known as **virtual
memory**). Virtual memory
and the related concept of
[processes](../C13-OS/processes.html#_processes) are covered in
greater detail in chapter 13; for now, it suffices to think of a process
as the abstraction of a running program and virtual memory as the memory
that is allocated to a single process. Every process has its own region
of memory called the **call stack**. Keep in mind that the call stack is
located in process/virtual memory, unlike registers (which are located
on the CPU).


Figure 2 depicts a sample state of the call stack and
registers prior to the execution of the `adder2` function.




![frame1](_images/ex1_1.png)


Figure 2. Execution stack prior to execution


Notice that the stack grows toward *lower* addresses. Registers `%eax`
and `%edx` currently contain junk values. The addresses associated with
the instructions in the code segment of program memory
(0x804840b-0x8048415) have been shortened to (0x40b-0x415) to improve
figure readability. Likewise, the addresses associated with the call
stack segment of program memory have been shortened to 0x108-0x110 from
0xffffd108-0xffffd110. In truth, call stack addresses occur at higher
addresses in program memory than code segment addresses.


Pay close attention to the initial (made up) values of registers `%esp`
and `%ebp`: they are `0x10c` and `0x12a`, respectively. The call stack
currently has the value `0x28` (or `40`) at stack address `0x110` (why
and how this got here will be covered in our discussion on
[functions](functions.html#_functions_in_assembly)). The
upper-left arrow in the following figures visually indicates the
currently executing instruction. The `%eip` register (or instruction
pointer) shows the next instruction to execute. Initially, `%eip`
contains address `0x40b` which corresponds to the first instruction in
the `adder2` function.




![frame2](_images/ex1_2.png)


The first instruction (`push %ebp`) places a copy of the value in `%ebp`
(or 0x12a) on top of the stack. After it executes, the `%eip` register
advances to the address of the next instruction to execute (or 0x40c).
The `push` instruction decrements the stack pointer by 4 (\"growing\"
the stack by 4 bytes), resulting in a new `%esp` value of `0x108`.
Recall that the `push %ebp` instruction is equivalent to:




    sub $4, %esp
    mov %ebp, (%esp)


In other words, subtract 4 from the stack pointer and place a copy of
the contents of `%ebp` in the location pointed to by the dereferenced
stack pointer, `(%esp)`.




![frame3](_images/ex1_3.png)


Recall that the structure of the `mov` instruction is `mov S,D`, where
`S` is the source location, and `D` is the destination. Thus, the next
instruction (`mov %esp, %ebp`) updates the value of `%ebp` to 0x108. The
register `%eip` advances to the address of the next instruction to
execute, or 0x40e.




![frame4](_images/ex1_4.png)


Next, `mov 0x8(%ebp), %eax` is executed. This is a bit more complicated
than the last `mov` instruction. Let's parse it by consulting the
operand table from the previous section. First, `0x8(%ebp)` translates
to M\[`%ebp` + 0x8\]. Since `%ebp` contains the value 0x108, adding 8 to
it yields 0x110. Performing a (stack) memory lookup on 0x110 yields the
value 0x28 (recall that 0x28 was placed on the stack by previous code).
So, the value 0x28 is copied into register `%eax`. The instruction
pointer advances to address 0x411, the next address to be executed.




![frame5](_images/ex1_5.png)


Afterwards, `add $0x2, %eax` is executed. Recall that the `add`
instruction has the form `add S,D` and places the quantity S + D in the
destination D. So, `add $0x2, %eax` adds the constant value 0x2 to the
value stored in `%eax` (or 0x28), resulting in 0x2A being stored in
register `%eax`. Register `%eip` advances to point to the next
instruction to be executed, or 0x414.




![frame6](_images/ex1_6.png)


The next instruction that executes is `pop %ebp`. This instruction
\"pops\" a value off call stack and places it in destination register
`%ebp`. Recall that this instruction is equivalent to the following
sequence of two instructions:




    mov (%esp), %ebp
    add $4, %esp


After this instruction executes, the value at the top of the stack
`(%esp)` or (M\[0x108\]) is copied into register `%ebp`. Thus, `%ebp`
now contains the value 0x12a. The stack pointer *increments* by 4, since
the stack grows toward lower addresses (and consequently, *shrinks*
toward higher ones). The new value of `%esp` is 0x10c, and `%eip` now
points to the address of the last instruction to execute in this code
snippet (0x415).


------------------------------------------------------------------------

The last instruction executed is `ret`. We will talk more about what
happens with `ret` in future sections when we discuss function calls,
but for now it suffices to know that it prepares the call stack for
returning from a function. By convention, the register `%eax` always
contains the return value (if one exists). In this case, the function
returns the value 0x2A, which corresponds to the decimal value 42.


Before we continue, note that the final values in registers `%esp` and
`%ebp` are 0x10c and 0x12a, respectively, which are the *same values as
when the function started executing*! This is normal and expected
behavior with the call stack. The purpose of the call stack is to store
the temporary variables and data of each function as it executes in the
context of a program. Once a function completes executing, the stack
returns to the state it was in prior to the function call. As a result,
you will commonly see the following two instructions at the beginning of
a function:




    push %ebp
    mov %esp, %ebp


and the following two instructions at the end of every function:




    pop %ebp
    ret






