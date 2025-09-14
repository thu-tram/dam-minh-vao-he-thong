

 

































### 9.4.1. Preliminaries 

### Conditional Comparison Instructions {#_conditional_comparison_instructions .discrete}

Comparison instructions perform an arithmetic operation for the purpose
of guiding the conditional execution of a program. [Table
1](#ConditionalControla64) lists the basic instructions associated with
conditional control.


+-----------------------------------+-----------------------------------+
| Instruction                       | Translation                       |
+===================================+===================================+
| `cmp O1, O2`                      | Compares O1 with O2 (computes     |
|                                   | O1 - O2)                          |
+-----------------------------------+-----------------------------------+
| `tst O1, O2`                      | Computes O1 `&` O2                |
+-----------------------------------+-----------------------------------+

: Table 1. Conditional Control Instructions

The `cmp` instruction compares the value of two operands, O1 and O2.
Specifically, it subtracts O2 from O1. The `tst` instruction performs
bitwise AND. It is common to see an instruction like:




    tst x0, x0


In this example, the bitwise AND of `x0` with itself is zero only when
`x0` contains zero. In other words, this is a test for a zero value and
is equivalent to:




    cmp x0, #0


Unlike the arithmetic instructions covered thus far, `cmp` and `tst` do
not modify a destination register. Instead, both instructions modify a
series of single-bit values known as **condition code flags**. For
example, `cmp` will modify condition code flags based on whether the
value O1 - O2 results in a positive (greater), negative (less), or zero
(equal) value. Recall that [condition
code](../C5-Arch/cpu.html#_the_alu) values encode information
about an operation in the ALU. The condition code flags are part of the
ARM processor state (`PSTATE`), which replaces the current program
status register (`CPSR`) from ARMv7-A systems.


+-----------------------------------+-----------------------------------+
| Flag                              | Translation                       |
+===================================+===================================+
| `Z`                               | Is equal to zero (1: yes, 0: no)  |
+-----------------------------------+-----------------------------------+
| `N`                               | Is negative (1: yes, 0: no)       |
+-----------------------------------+-----------------------------------+
| `V`                               | Signed overflow has occurred (1:  |
|                                   | yes, 0: no)                       |
+-----------------------------------+-----------------------------------+
| `C`                               | Arithmetic carry/unsigned         |
|                                   | overflow has occurred (1: yes, 0: |
|                                   | no)                               |
+-----------------------------------+-----------------------------------+

: Table 2. Common Condition Code Flags

Table 2 depicts the common flags used for
condition code operations. Revisiting the `cmp O1, O2` instruction:



-   The `Z` flag is set to 1 if O1 and O2 are equal.

-   The `N` flag is set to 1 if O1 is *less* than O2 (i.e. O1 - O2
    results in a negative value).

-   The `V` flag is set to 1 if the operation O1 - O2 results in
    overflow (useful for signed comparisons).

-   The `C` flag is set to 1 if the operation O1 - O2 results in an
    arithmetic carry operation (useful for unsigned comparisons).


While an in-depth discussion of condition code flags is beyond the scope
of this book, the setting of these registers by `cmp` and `tst` enables
the next set of instructions we cover (the *branch* instructions) to
operate correctly.


### The Branch Instructions {#_the_branch_instructions .discrete}

A branch instruction enables a program's execution to \"jump\" to a new
position in the code. In the assembly programs we have traced through
thus far, `pc` always points to the next instruction in program memory.
The branch instructions enable `pc` to be set to either a new
instruction not yet seen (as in the case of an `if` statement) or to a
previously executed instruction (as in the case of a loop).


#### Direct branch instructions {#_direct_branch_instructions .discrete}

+-----------------------------------+-----------------------------------+
| Instruction                       | Description                       |
+===================================+===================================+
| `b addr L`                        | `pc` = addr                       |
+-----------------------------------+-----------------------------------+
| `br A`                            | `pc` = A                          |
+-----------------------------------+-----------------------------------+
| `cbz R, addr L`                   | If R is equal to 0, `pc` = addr   |
|                                   | (conditional branch)              |
+-----------------------------------+-----------------------------------+
| `cbnz R, addr L`                  | If R is not equal to 0, `pc` =    |
|                                   | addr (conditional branch)         |
+-----------------------------------+-----------------------------------+
| `b.c addr L`                      | If c, `pc` = addr (conditional    |
|                                   | branch)                           |
+-----------------------------------+-----------------------------------+

: Table 3. Common Branch Instructions

Table 3 lists the set of common branch instructions; `L`
refers to a **symbolic label**, which serves as an identifier in the
program's object file. All labels consist of some letters and digits
followed by a colon. Labels can be *local* or *global* to an object
file's scope. Function labels tend to be *global* and usually consist of
the function name and a colon. For example, `main:` (or `<main>:`) is
used to label a user-defined `main` function. By convention, labels
whose scope are *local* are typically preceded by a period. One may
encounter a local label with a name like `.L1` in the context of an `if`
statement or loop.


All labels have an associated address (`addr` in Table 3).
When the CPU executes a `b` instruction, it sets the `pc` register to
`addr`. The `b` instruction enables the program counter to change within
128 MB of its current location; a programmer writing assembly can also
specify a particular address to branch to by using the `br` instruction.
Unlike the `b` instruction, there are no restrictions on the address
range of `br`.


Sometimes, local labels also are shown as an offset from the start of a
function. Therefore, an instruction whose address is 28 bytes away from
the start of `main` may be represented with the label `<main+28>`. For
example, the instruction `b 0x7d0 <main+28>` indicates a branch to
address 0x7d0, which has the associated label `<main+28>`, meaning that
it is 28 bytes away from the starting address of the `main` function.
Executing this instruction sets `pc` to 0x7d0.


The last three instructions are **conditional branch instructions**. In
other words, the program counter register is set to `addr` only if the
given condition evaluates to true. The `cbz` and `cbnz` instructions
require a register in addition to an address. In the case of `cbz`, if R
is zero, the branch is taken and `pc` is set to `addr`. In the case of
`cbnz`, if R is nonzero, the branch is taken and `pc` is set to `addr`.


The most powerful of the conditional branch instructions are the `b.c`
instructions, which enable the compiler or assembly writer to pick a
custom suffix that indicates the condition on which a branch is taken.


#### Conditional branch instruction suffixes {#_conditional_branch_instruction_suffixes .discrete}

Table 4 lists the set of common conditional branch
suffixes (c). When used in conjunction with a branch, each instruction
starts with the letter `b` and a dot, denoting that it is a branch
instruction. The suffix of each instruction (c) indicates the
**condition** for the branch. The branch instruction suffixes also
determine whether to interpret numerical comparisons as signed or
unsigned. Note that conditional branch instructions have a much more
limited range (1 MB) than the `b` instruction. These suffixes are also
used for the conditional select instruction (`csel`), which is covered
in the next section.


+----------------------+----------------------+-----------------------+
| Signed Comparison    | Unsigned Comparison  | Description           |
+======================+======================+=======================+
| `eq`                 | `eq`                 | branch if equal (==)  |
|                      |                      | or branch if zero     |
+----------------------+----------------------+-----------------------+
| `ne`                 | `ne`                 | branch if not equal   |
|                      |                      | (!=)                  |
+----------------------+----------------------+-----------------------+
| `mi`                 | `mi`                 | branch if minus       |
|                      |                      | (negative)            |
+----------------------+----------------------+-----------------------+
| `pl`                 | `pl`                 | branch if             |
|                      |                      | non-negative (\>= 0)  |
+----------------------+----------------------+-----------------------+
| `gt`                 | `hi`                 | branch if greater     |
|                      |                      | than (higher) (\>)    |
+----------------------+----------------------+-----------------------+
| `ge`                 | `cs` (`hs`)          | branch if greater     |
|                      |                      | than or equal (\>=)   |
+----------------------+----------------------+-----------------------+
| `lt`                 | `lo` (`cc`)          | branch if less than   |
|                      |                      | (\<)                  |
+----------------------+----------------------+-----------------------+
| `le`                 | `ls`                 | branch if less than   |
|                      |                      | or equal (\<=)        |
+----------------------+----------------------+-----------------------+

: Table 4. Conditional Branch Instruction Suffixes (synonyms shown in
parentheses)

### The goto Statement {#_the_goto_statement .discrete}

In the following subsections, we look at conditionals and loops in
assembly and reverse engineer them back to C. When translating assembly
code of conditionals and loops back into C, it is useful to understand
their corresponding C language `goto` forms. The `goto` statement is a C
primitive that forces program execution to switch to another line in the
code. The assembly instruction associated with the `goto` statement is
`b`.


The `goto` statement consists of the `goto` keyword followed by a **goto
label**, a type of program label that indicates that execution should
continue at the corresponding label. So, `goto done` means that the
program execution should branch to the line marked by label `done`.
Other examples of program labels in C include the [switch statement
labels](../C2-C_depth/advanced_switch.html#_c_switch_stmt_)
previously covered in Chapter 2.


The following code listings depict a function `getSmallest` written in
regular C code (left) and its associated `goto` form in C (right). The
`getSmallest` function compares the value of two integers (`x` and `y`),
and assigns the smaller value to variable `smallest`.


+-----------------------------------+-----------------------------------+
| Regular C version                 | Goto version                      |
+===================================+===================================+
|                        |                        |
|                   |                   |
|                        |                        |
| ```     | ```     |
| int getSmallest(int x, int y) {   | int getSmallest(int x, int y) {   |
|     int smallest;                 |     int smallest;                 |
|                                   |                                   |
| if ( x > y ) { //if (conditional) |     i                             |
|                                   | f (x <= y ) { //if (!conditional) |
|    smallest = y; //then statement |         goto else_statement;      |
|     }                             |     }                             |
|     else {                        |                                   |
|                                   |    smallest = y; //then statement |
|    smallest = x; //else statement |     goto done;                    |
|     }                             |                                   |
|     return smallest;              | else_statement:                   |
| }                                 |                                   |
| ```                               |    smallest = x; //else statement |
| :::                               |                                   |
| :::                               | done:                             |
| :::                               |     return smallest;              |
|                                   | }                                 |
|                                   | ```                               |
|                                   | :::                               |
|                                   | :::                               |
|                                   | :::                               |
+-----------------------------------+-----------------------------------+

: Table 5. Comparison of a C function and its associated goto form.

The `goto` form of this function may seem counterintuitive, but let's
discuss what exactly is going on. The conditional checks to see whether
variable `x` is less than or equal to `y`.



-   If `x` is less than or equal to `y`, the program transfers control
    to the label marked by `else_statement`, which contains the single
    statement `smallest = x`. Since the program executes linearly, the
    program continues on to execute the code under the label `done`,
    which returns the value of `smallest` (`x`).

-   If `x` is greater than `y`, then `smallest` is set to `y`. The
    program then executes the statement `goto done`, which transfers
    control to the `done` label, which returns the value of `smallest`
    (`y`).


While `goto` statements were commonly used in the early days of
programming, their use in modern code is considered bad practice,
because it reduces the overall readability of code. In fact, computer
scientist Edsger Dijkstra wrote a famous paper lambasting the use of
`goto` statements called *Go To Statement Considered Harmful*^1^.


In general, well-designed C programs do not use `goto` statements and
programmers are discouraged from using it to avoid writing code that is
difficult to read, debug, and maintain. However, the C `goto` statement
is important to understand, as GCC typically changes C code with
conditionals into a `goto` form prior to translating it to assembly,
including code that contains `if` statements and loops.


The following subsections cover the assembly representation of `if`
statements and loops in greater detail.



-   [If
    Statements](if_statements.html#_if_statements_in_assembly)

-   Loops


### References {#_references .discrete}


1.  Edsger Dijkstra. \"Go To Statement Considered Harmful\".
    *Communications of the ACM* 11(3) pp. 147---​148. 1968.





