

 





























## 9.3. Arithmetic Instructions 


### 9.3.1. Common Arithmetic Instructions 

The A64 ISA implements several instructions that correspond to
arithmetic operations performed by the ALU. [Table
1](#OtherArithmetica64) lists several arithmetic instructions that one
may encounter when reading ARM assembly.


+-----------------------------------+-----------------------------------+
| Instruction                       | Translation                       |
+===================================+===================================+
| `add D, O1, O2`                   | D = O1 + O2                       |
+-----------------------------------+-----------------------------------+
| `sub D, O1, O2`                   | D = O1 - O2                       |
+-----------------------------------+-----------------------------------+
| `neg D, O1`                       | D = -(O1)                         |
+-----------------------------------+-----------------------------------+

: Table 1. Common Instructions

The `add` and `sub` instructions correspond to addition and subtraction
and require two operands in addition to the destination register. In
contrast, the `neg` instruction requires only one operand in addition to
the destination register.


The three instructions in Table 1 also have
*carry* forms that enable the instruction to use the optional carry
condition flag, `C`. The one-bit carry flag is set when an unsigned
operation overflows. We cover other condition control flags in the
following section, but describe the carry flag here to introduce the
additional arithmetic instructions. The carry forms and their rough
translation are shown in Table 2.


+-----------------------------------+-----------------------------------+
| Instruction                       | Translation                       |
+===================================+===================================+
| `adc D, O1, O2`                   | D = O1 + O2 + `C`                 |
+-----------------------------------+-----------------------------------+
| `sbc D, O1, O2`                   | D = O1 - O2 - `~C`                |
+-----------------------------------+-----------------------------------+
| `ngc D, O1`                       | D = -(O1) - `~C`                  |
+-----------------------------------+-----------------------------------+

: Table 2. Carry Forms for Common Instructions

The above instructions also have an optional `s` suffix. When the `s`
suffix is used (e.g., `adds`), it indicates that the arithmetic
operation is setting condition flags.



#### Multiplication and Division 

+-----------------------------------+-----------------------------------+
| Instruction                       | Translation                       |
+===================================+===================================+
| `mul D, O1, O2`                   | D = O1 × O2                       |
+-----------------------------------+-----------------------------------+
| `udiv D, O1, O2`                  | D = O1 / O2 (32-bit unsigned)     |
+-----------------------------------+-----------------------------------+
| `sdiv D, O1, O2`                  | D = O1 / O2 (64-bit signed)       |
+-----------------------------------+-----------------------------------+

: Table 3. Common Multiplication and Division Instructions

The most common multiplication and division instructions are shown in
Table 3. The `mul` instruction operates on two
operands and places the product in the destination D. The division
operation does *not* have a generic form; the `udiv` and `sdiv`
instructions operate on 32-bit and 64-bit data, respectively. Note that
you cannot multiply 32-bit registers with 64-bit registers.


In addition, ARMv8-A provides composite forms for multiplication,
allowing the CPU to perform more sophisticated operations in a single
instruction. These instructions are shown in [Table
4](#CompositeMultiply).


+-----------------------------------+-----------------------------------+
| Instruction                       | Translation                       |
+===================================+===================================+
| `madd D, O1, O2, O3`              | D = O3 + (O1 × O2)                |
+-----------------------------------+-----------------------------------+
| `msub D, O1, O2, O3`              | D = O3 - (O1 × O2)                |
+-----------------------------------+-----------------------------------+
| `mneg D, O1, O2`                  | D = -(O1 × O2)                    |
+-----------------------------------+-----------------------------------+

: Table 4. Composite Multiplication Instructions



### 9.3.2. Bit Shifting Instructions 

Bit shifting instructions enable the compiler to perform bit shifting
operations. Multiplication and division instructions typically take a
long time to execute. Bit shifting offers the compiler a shortcut for
multiplicands and divisors that are powers of 2. For example, to compute
`77 * 4`, most compilers will translate this operation to `77 << 2` to
avoid the use of a `mul` instruction. Likewise, to compute `77 / 4`, a
compiler typically translates this operation to `77 >> 2` to avoid using
the `sdiv` instruction.


Keep in mind that left and right bit shifts translate to different
instructions based on whether the goal is an arithmetic (signed) or
logical (unsigned) shift.


+----------------------+----------------------+-----------------------+
| Instruction          | Translation          | Arithmetic or         |
|                      |                      | Logical?              |
+======================+======================+=======================+
| `lsl D, R, #v`       | D = R `<<` v         | logical or arithmetic |
+----------------------+----------------------+-----------------------+
| `lsr D, R, #v`       | D = R `>>` v         | logical               |
+----------------------+----------------------+-----------------------+
| `asr D, R, #v`       | D = R `>>` v         | arithmetic            |
+----------------------+----------------------+-----------------------+
| `ror D, R, #v`       | D = R `>>>` v        | neither (rotate)      |
+----------------------+----------------------+-----------------------+

: Table 5. Bit Shift Instructions

In addition to the destination register, each shift instruction takes
two operands; one is usually a register (denoted by R) and the other is
a 6-bit shift value (v). On 64-bit systems, the shift value is encoded
as a single byte (since it doesn't make sense to shift past 63). The
shift value v must either be a constant or stored in a component
register.


The last bit shifting instruction, `ror`, requires special discussion.
The `ror` instruction *rotates* the bits, replacing the most significant
bits with the least significant bits. We represent the rotate shift
instruction using the `>>>` symbol.



+-----------------------------------+-----------------------------------+
|                                   |                          |
|                                   | Different versions of             |
|                                   | instructions help us distinguish  |
|                                   | types at an assembly level        |
|                                   | :::                               |
|                                   |                                   |
|                                   | ::: paragraph                     |
|                                   | At the assembly level, there is   |
|                                   | no notion of types. However,      |
|                                   | recall that the compiler can      |
|                                   | choose to use component registers |
|                                   | based on the types present at the |
|                                   | code level. Similarly, recall     |
|                                   | that shift right works            |
|                                   | differently depending on whether  |
|                                   | the value is signed or unsigned.  |
|                                   | At the assembly level, the        |
|                                   | compiler uses separate            |
|                                   | instructions to distinguish       |
|                                   | between logical and arithmetic    |
|                                   | shifts!                           |
|                                   | :::                               |
+-----------------------------------+-----------------------------------+



### 9.3.3. Bitwise Instructions 

Bitwise instructions enable the compiler to perform bitwise operations
on data. One way in which the compiler uses bitwise operations is for
certain optimizations. For example, a compiler may choose to implement
77 mod 4 with the operation `77 & 3` in lieu of the more expensive
`sdiv` instruction.


Table 6 lists common bitwise instructions, and composite
bitwise instructions that utilize negation.


+-----------------------------------+-----------------------------------+
| Instruction                       | Translation                       |
+===================================+===================================+
| `and D, O1, O2`                   | D = O1 `&` O2                     |
+-----------------------------------+-----------------------------------+
| `orr D, O1, O2`                   | D = O1 `|` O2                     |
+-----------------------------------+-----------------------------------+
| `eor D, O1, O2`                   | D = O1 `^` O2                     |
+-----------------------------------+-----------------------------------+
| `mvn D, O`                        | D = `~`O                          |
+-----------------------------------+-----------------------------------+
| `bic D, O1, O2`                   | D = O1 `&` `~`O2                  |
+-----------------------------------+-----------------------------------+
| `orn D, O1, O2`                   | D = O1 `|` `~`O2                  |
+-----------------------------------+-----------------------------------+
| `eon D, O1, O2`                   | D = O1 `^` `~`O2                  |
+-----------------------------------+-----------------------------------+

: Table 6. Bitwise Operations

Remember that bitwise not is distinct from negation (`neg`). The `mvn`
instruction flips the bits of the operand but does not add 1. Be careful
not to confuse these two instructions.



+-----------------------------------+-----------------------------------+
|                                   |                          |
|                                   | Use bitwise operations only when  |
|                                   | needed in your C code!            |
|                                   | :::                               |
|                                   |                                   |
|                                   | ::: paragraph                     |
|                                   | After reading this section, it    |
|                                   | may be tempting to replace common |
|                                   | arithmetic operations in your C   |
|                                   | code with bitwise shifts and      |
|                                   | other operations. This is *not*   |
|                                   | recommended. Most modern          |
|                                   | compilers are smart enough to     |
|                                   | replace simple arithmetic         |
|                                   | operations with bitwise           |
|                                   | operations when it makes sense,   |
|                                   | making it unnecessary for the     |
|                                   | programmer to do so. As a general |
|                                   | rule, programmers should          |
|                                   | prioritize code readability       |
|                                   | whenever possible and avoid       |
|                                   | premature optimization.           |
|                                   | :::                               |
+-----------------------------------+-----------------------------------+






