



























## 4.6. Bitwise Operators 

In addition to the standard arithmetic operations described earlier,
CPUs also support operations that are uncommon outside of binary. These
**bitwise operators** directly apply the behavior of [logic
gates](../C5-Arch/gates.html#_basic_logic_gates) to bit
sequences, making them straightforward to implement efficiently in
hardware. Unlike addition and subtraction, which programmers typically
use to manipulate a variable's numerical interpretation, programmers
commonly use bitwise operators to modify specific bits in a variable.
For example, a program might encode a certain bit position in a variable
to hold a true/false meaning, and bitwise operations allow the program
to manipulate the variable's individual bits to change that specific
bit.



### 4.6.1. Bitwise AND 

The bitwise AND operator (`&`) evaluates two input bit sequences. For
each digit of the inputs, it outputs a 1 in the corresponding position
of the output if *both* inputs are 1 in that position. Otherwise, it
outputs a 0 for the digit. Table 1 shows the truth table
for the bitwise AND of two values, *A* and *B*.


+-----------------+-----------------+-----------------------------------+
| A               | B               | A & B                             |
+=================+=================+===================================+
| 0               | 0               | 0                                 |
+-----------------+-----------------+-----------------------------------+
| 0               | 1               | 0                                 |
+-----------------+-----------------+-----------------------------------+
| 1               | 0               | 0                                 |
+-----------------+-----------------+-----------------------------------+
| 1               | 1               | 1                                 |
+-----------------+-----------------+-----------------------------------+

: Table 1. The Results of Bitwise ANDing Two Values (*A* AND *B*)

For example, to bitwise AND 0b011010 with 0b110110, start by lining up
the two sequences. Checking vertically through each digit, set the
result of the column to 1 if *both* digits are 1. Otherwise, set the
result of the column to 0:




            011010
        AND 110110  Only digits 1 and 4 are 1's in BOTH inputs, so
    Result: 010010  those are the only digits set to 1 in the output.


To perform a bitwise AND in C, place C's bitwise AND operator (`&`)
between two operand variables. Here's the same example again, performed
in C:




```
int x = 26;
int y = 54;

printf("Result: %d\n", x & y);  // Prints 18
```



+-----------------------------------+-----------------------------------+
|                                   |                          |
|                                   | Bitwise Operations versus Logical |
|                                   | Truth Operations                  |
|                                   | :::                               |
|                                   |                                   |
|                                   | ::: paragraph                     |
|                                   | Be careful not to conflate        |
|                                   | bitwise operators with [logical   |
|                                   | truth                             |
|                                   | operat                            |
|                                   | ors](../C1-C_intro/conditionals.h |
|                                   | tml#_boolean_values_in_c). |
|                                   | Despite having similar names      |
|                                   | (AND, OR, NOT, etc.), the two     |
|                                   | *are not* the same:               |
|                                   | :::                               |
|                                   |                                   |
|                                   |                          |
|                                   | -   Bitwise operators consider    |
|                                   |     each bit of their inputs      |
|                                   |     independently and produce an  |
|                                   |     output bit sequence as a      |
|                                   |     function of the specific      |
|                                   |     input bits that are set.      |
|                                   |                                   |
|                                   | -   Logical operators consider    |
|                                   |     only the *truth*              |
|                                   |     interpretation of their       |
|                                   |     operands. To C, a value of    |
|                                   |     zero is *false*, whereas all  |
|                                   |     other values are considered   |
|                                   |     *true*. Logical operators are |
|                                   |     often used when evaluating    |
|                                   |     conditionals (e.g., `if`      |
|                                   |     statements).                  |
|                                   | :::                               |
|                                   |                                   |
|                                   | ::: paragraph                     |
|                                   | Note that C often uses similar    |
|                                   | (but slightly different)          |
|                                   | operators to distinguish between  |
|                                   | the two. For example, you can     |
|                                   | indicate bitwise AND and bitwise  |
|                                   | OR using a single `&` and `|`,    |
|                                   | respectively. Logical AND and     |
|                                   | logical OR correspond to a double |
|                                   | `&&` and `||`. Finally, bitwise   |
|                                   | NOT uses `~`, whereas logical NOT |
|                                   | is expressed by `!`.              |
|                                   | :::                               |
+-----------------------------------+-----------------------------------+



### 4.6.2. Bitwise OR 

The bitwise OR operator (`|`) behaves like the bitwise AND operator
except that it outputs a 1 for a digit if *either or both* of the inputs
is 1 in the corresponding position. Otherwise, it outputs a 0 for the
digit. Table 2 shows the truth table for the bitwise OR
of two values, *A* and *B*.


+-----------------+-----------------+-----------------------------------+
| A               | B               | A \| B                            |
+=================+=================+===================================+
| 0               | 0               | 0                                 |
+-----------------+-----------------+-----------------------------------+
| 0               | 1               | 1                                 |
+-----------------+-----------------+-----------------------------------+
| 1               | 0               | 1                                 |
+-----------------+-----------------+-----------------------------------+
| 1               | 1               | 1                                 |
+-----------------+-----------------+-----------------------------------+

: Table 2. The Results of Bitwise ORing Two Values (*A* OR *B*)

For example, to bitwise OR 0b011010 with 0b110110, start by lining up
the two sequences. Checking vertically through each digit, set the
result of the column to 1 if *either* digit is 1:




            011010
         OR 110110     Only digit 0 contains a 0 in both inputs, so it's
    Result: 111110     the only digit not set to 1 in the result.


To perform a bitwise OR in C, place C's bitwise OR operator (`|`)
between two operands. Here's the same example again, performed in C:




```
int x = 26;
int y = 54;

printf("Result: %d\n", x | y);  // Prints 62
```



### 4.6.3. Bitwise XOR (Exclusive OR) 

The bitwise XOR operator (`^`) behaves like the bitwise OR operator
except that it outputs a 1 for a digit only if *exactly one* (but not
both) of the inputs is 1 in the corresponding position. Otherwise, it
outputs a 0 for the digit. Table 3 shows the truth table
for the bitwise XOR of two values, *A* and *B*.


+-----------------+-----------------+-----------------------------------+
| A               | B               | A \^ B                            |
+=================+=================+===================================+
| 0               | 0               | 0                                 |
+-----------------+-----------------+-----------------------------------+
| 0               | 1               | 1                                 |
+-----------------+-----------------+-----------------------------------+
| 1               | 0               | 1                                 |
+-----------------+-----------------+-----------------------------------+
| 1               | 1               | 0                                 |
+-----------------+-----------------+-----------------------------------+

: Table 3. The Results of Bitwise XORing Two Values (*A* XOR *B*)

For example, to bitwise XOR 0b011010 with 0b110110, start by lining up
the two sequences. Checking vertically through each digit, set the
result of the column to 1 if *only one* digit is 1:




            011010
        XOR 110110     Digits 2, 3, and 6 contain a 1 in exactly one of
    Result: 101100     the two inputs.


To perform a bitwise XOR in C, place C's bitwise XOR operator (`^`)
between two operands. Here's the same example again, performed in C:




```
int x = 26;
int y = 54;

printf("Result: %d\n", x ^ y);  // Prints 44
```



### 4.6.4. Bitwise NOT 

The bitwise NOT operator (`~`) operates on just one operand. For each
bit in the sequence, it simply flips the bit such that a zero becomes a
one or vice versa. Table 4 shows the truth table for the
bitwise NOT operator.


+-----------------------------------+-----------------------------------+
| A                                 | \~ A                              |
+===================================+===================================+
| 0                                 | 1                                 |
+-----------------------------------+-----------------------------------+
| 1                                 | 0                                 |
+-----------------------------------+-----------------------------------+

: Table 4. The Results of Bitwise NOTing a Value (*A*)

For example, to bitwise NOT 0b011010, invert the value of each bit:




        NOT 011010
    Result: 100101


To perform a bitwise NOT in C, place a tilde character (`~`) in front of
an operand. Here's the same example again, performed in C:




```
int x = 26;

printf("Result: %d\n", ~x); // Prints -27
```



+-----------------------------------+-----------------------------------+
|                                   |                          |
|                                   | Bitwise NOT vs. Negation          |
|                                   | :::                               |
|                                   |                                   |
|                                   | ::: paragraph                     |
|                                   | Note that all modern systems      |
|                                   | represent integers using two's    |
|                                   | complement, so bitwise NOT isn't  |
|                                   | quite the same as negation.       |
|                                   | Bitwise NOT *only* flips the bits |
|                                   | and *doesn't* add one.            |
|                                   | :::                               |
+-----------------------------------+-----------------------------------+



### 4.6.5. Bit Shifting 

Another important bitwise operation involves shifting the position of an
operand's bits either to the left (`<<`) or to the right (`>>`). Both
the left and right shifting operators take two operands: the bit
sequence to shift and the number of places it should be shifted.



#### Shifting Left 

Shifting a sequence to the left by *N* places moves each of its bits to
the left *N* times, appending new zeros to the right side of the
sequence. For example, shifting the eight-bit sequence 0b00101101 to the
left by two produces 0b101101**00**. The two zeros at the right are
appended to end of the sequence, since the result still needs to be an
eight-bit sequence.


In the absence of overflow, shifting to the left *increases* the value
of the result because bits move toward digits that contribute larger
powers of two to the value of the number. However, with a fixed number
of bits, any bits that shift into positions beyond the maximum capacity
of the number get truncated. For example, shifting the eight-bit
sequence 0b11110101 (unsigned interpretation 245) to the left by one
produces 0b1110101**0** (unsigned interpretation 234). Here, the
truncation of the high-order bit that shifted out makes the result
smaller.


To perform a left bit shift in C, place two less-than characters (`<<`)
between a value and the number of places to shift that value:




```
int x = 13;  // 13 is 0b00001101

printf("Result: %d\n", x << 3);  // Prints 104 (0b01101000)
```



#### Shifting Right 

Shifting to the right is similar to left shifting --- any bits that are
shifted out of a variable's capacity (e.g., off the end to the right)
disappear due to truncation. However, right shifting introduces an
additional consideration: the new bits prepended to the left side of the
result may need to be either all zeros or all ones depending on the
*type* of the variable being shifted and its high-order bit value.
Conceptually, the choice to prepend zeros or ones resembles that of
[sign extension](signed.html#_sign_extension). Thus, there exist
two distinct variants of right shifting:



-   A **logical right shift** always prepends zeros to the high-order
    bits of the result. Logical shifting is used to shift *unsigned*
    variables, since a leading 1 in the most significant bit of an
    unsigned value isn't intended to mean that the value is negative.
    For example, shifting 0b10110011 to the right by two using a logical
    shift yields 0b**00**101100.

-   An **arithmetic right shift** prepends a copy of the shifted value's
    most significant bit into each of the new bit positions. Arithmetic
    shifting applies to *signed* variables, for which it's important to
    preserve the signedness of the high-order bits. For example,
    shifting 0b10110011 to the right by two using an arithmetic shift
    yields 0b**11**101100.


Fortunately, when programming in C, you don't typically need to worry
about the distinction if you've declared your variables properly. If
your program includes a right shift operator (`>>`), virtually every C
compiler will automatically perform the appropriate type of shifting
according to the type of the shifting variable. That is, if the shifting
variable was declared with the *unsigned* qualifier, the compiler will
perform a logical shift. Otherwise, it will perform an arithmetic shift.



+-----------------------------------+-----------------------------------+
|                                   |                          |
|                                   | C Right Shift Example Program     |
|                                   | :::                               |
|                                   |                                   |
|                                   | ::: paragraph                     |
|                                   | You can test the behavior of      |
|                                   | right shifting with a small       |
|                                   | example program like this one:    |
|                                   | :::                               |
|                                   |                                   |
|                                   |                   |
|                                   |                        |
|                                   | ```     |
|                                   | #include <stdio.h>                |
|                                   |                                   |
|                                   | int main(int argc, char **argv) { |
|                                   |     /*                            |
|                                   | Unsigned integer value: u_val. */ |
|                                   |                                   |
|                                   |  unsigned int u_val = 0xFF000000; |
|                                   |                                   |
|                                   |     /                             |
|                                   | * Signed integer value: s_val. */ |
|                                   |     int s_val = 0xFF000000;       |
|                                   |                                   |
|                                   |     printf("%08X\n", u_va         |
|                                   | l >> 12);  // logical right shift |
|                                   |     printf("%08X\n", s_val >      |
|                                   | > 12);  // arithmetic right shift |
|                                   |                                   |
|                                   |     return 0;                     |
|                                   | }                                 |
|                                   | ```                               |
|                                   | :::                               |
|                                   | :::                               |
|                                   |                                   |
|                                   | ::: paragraph                     |
|                                   | This program declares two 32-bit  |
|                                   | integers: one as an unsigned      |
|                                   | integer (`u_val`), and another as |
|                                   | a signed integer (`s_val`). It    |
|                                   | initializes both integers to the  |
|                                   | same starting value: a sequence   |
|                                   | of eight ones followed by 24      |
|                                   | zeros                             |
|                                   | (`0b1111                          |
|                                   | 111100000000000000000000000000`), |
|                                   | and then it shifts both values 12 |
|                                   | positions to the right. When      |
|                                   | executed, it prints:              |
|                                   | :::                               |
|                                   |                                   |
|                                   |                   |
|                                   |                        |
|                                   |     $ ./a.out                     |
|                                   |     000FF000                      |
|                                   |     FFFFF000                      |
|                                   | :::                               |
|                                   | :::                               |
|                                   |                                   |
|                                   | ::: paragraph                     |
|                                   | Because a leading 1 doesn't       |
|                                   | indicate \"negative\" for the     |
|                                   | unsigned `u_val`, the compiler    |
|                                   | uses instructions to prepend it   |
|                                   | with only zeros. The shifted      |
|                                   | result contains 12 zeros, eight   |
|                                   | ones, and 12 more zeros           |
|                                   | (`0b00                            |
|                                   | 000000000011111111000000000000`). |
|                                   | On the other hand, the leading 1  |
|                                   | **does** indicate \"negative\"    |
|                                   | for `s_val`, so the compiler      |
|                                   | prepends 1's to the front of the  |
|                                   | shifted value, yielding 20 ones   |
|                                   | followed by 12 zeros              |
|                                   | (`0b11                            |
|                                   | 111111111111111111000000000000`). |
|                                   | :::                               |
+-----------------------------------+-----------------------------------+






