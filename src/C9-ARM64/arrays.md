

 





























## 9.7. Arrays 

Recall that
[arrays](../C1-C_intro/arrays_strings.html#_introduction_to_arrays)
are ordered collections of data elements of the same type that are
contiguously stored in memory. Statically allocated [single-dimension
arrays](../C2-C_depth/arrays.html#_single_dimensional_arrays)
have the form `Type arr[N]`, where `Type` is the data type, `arr` is the
identifier associated with the array, and `N` is the number of data
elements. Declaring an array statically as `Type arr[N]` or dynamically
as `arr = malloc(N*sizeof(Type))` allocates *N* × `sizeof`(*Type*) total
bytes of memory.


To access the element at index `i` in array `arr`, use the syntax
`arr[i]`. Compilers commonly convert array references into [pointer
arithmetic](../C2-C_depth/pointers.html#_pointer_variables) prior
to translating to assembly. So ,`arr+i` is equivalent to `&arr[i]`, and
`*(arr+i)` is equivalent to `arr[i]`. Since each data element in `arr`
is of type `Type`, `arr+i` implies that element `i` is stored at address
`arr + sizeof(Type) * i`.


Table 1 outlines some common array operations and their
corresponding assembly instructions. In the examples that follow,
suppose we declare an `int` array of length 10 (e.g., `int arr[10]`).
Assume that register `x1` stores the address of `arr`, register `x2`
stores the `int` value `i`, and register `x0` represents some variable
`x` (also of type `int`). Recall that `int` variables take up four bytes
of space, whereas `int *` variables take up eight bytes.


+----------------------+----------------------+-----------------------+
| Operation            | Type                 | Assembly              |
|                      |                      | Representation        |
+======================+======================+=======================+
| `x = arr`            | `int *`              | `mov x0, x1`          |
+----------------------+----------------------+-----------------------+
| `x = arr[0]`         | `int`                | `ldr w0, [x1]`        |
+----------------------+----------------------+-----------------------+
| `x = arr[i]`         | `int`                | `ldr w                |
|                      |                      | 0, [x1, x2, LSL, #2]` |
+----------------------+----------------------+-----------------------+
| `x = &arr[3]`        | `int *`              | `add x0, x1, #12`     |
+----------------------+----------------------+-----------------------+
| `x = arr+3`          | `int *`              | `add x0, x1, #12`     |
+----------------------+----------------------+-----------------------+
| `x = *(arr+5)`       | `int`                | `ldr w0, [x1, #20]`   |
+----------------------+----------------------+-----------------------+

: Table 1. Common Array Operations and Their Corresponding Assembly
Representations

Notice that to access element `arr[5]` (or `*(arr+5)` using pointer
arithmetic), the compiler performs a memory lookup on address `arr+5*4`
instead of `arr+5`. To understand why this is necessary, recall that any
element at index `i` in an array is stored at address
`arr + sizeof(Type) * i`. The compiler must therefore multiply the index
by the size of the data type (in this case 4, since `sizeof(int) = 4`)
to compute the correct offset. Recall also that memory is
byte-addressable; offsetting by the correct number of bytes is the same
as computing an address.


As an example, consider a sample array (`array`) with 10 integer
elements (\[FigArray6\]).




![Each integer in the array requires four bytes.](_images/arrayFig.png)


Figure 1. The layout of a ten-integer array in memory. Each a~i~-labeled
box represents an offset of four bytes, as each integer requires four
bytes to store.


Notice that since `array` is an array of integers, each element takes up
exactly four bytes. Thus, an integer array with 10 elements consumes 40
bytes of contiguous memory.


To compute the address of element 3, the compiler multiplies the index 3
by the data size of the integer type (4) to yield an offset of 12 (or
0xc). Sure enough, element 3 in Figure 1 is located at
byte offset a~12~.


Let's take a look at a simple C function called `sumArray` that sums up
all the elements in an array:




```
int sumArray(int *array, int length) {
    int i, total = 0;
    for (i = 0; i < length; i++) {
        total += array[i];
    }
    return total;
}
```


The `sumArray` function takes the address of an array and the array's
associated length and sums up all the elements in the array. Now take a
look at the corresponding assembly of the `sumArray` function:




    Dump of assembler code for function sumArray:
    0x874 <+0>:   sub    sp, sp, #0x20       // grow stack by 32 bytes (new frame)
    0x878 <+4>:   str    x0, [sp, #8]        // store x0 at sp + 8 (array address)
    0x87c <+8>:   str    w1, [sp, #4]        // store w1 at sp + 4 (length)
    0x880 <+12>:  str    wzr, [sp, #28]      // store 0 at sp + 28  (total)
    0x884 <+16>:  str    wzr, [sp, #24]      // store 0 at sp + 24 (i)
    0x888 <+20>:  b      0x8b8 <sumArray+68> // goto <sumArray+68>
    0x88c <+24>:  ldrsw  x0, [sp, #24]       // x0 = i
    0x890 <+28>:  lsl    x0, x0, #2          // left shift i by 2 (i << 2, or i*4)
    0x894 <+32>:  ldr    x1, [sp, #8]        // x1 = array
    0x898 <+36>:  add    x0, x1, x0          // x0 = array + i*4
    0x89c <+40>:  ldr    w0, [x0]            // w0 = array[i]
    0x8a0 <+44>:  ldr    w1, [sp, #28]       // w1 = total
    0x8a4 <+48>:  add    w0, w1, w0          // w0 = total + array[i]
    0x8a8 <+52>:  str    w0, [sp, #28]       // store (total + array[i]) in total
    0x8ac <+56>:  ldr    w0, [sp, #24]       // w0 = i
    0x8b0 <+60>:  add    w0, w0, #0x1        // w0 = w0 + 1 (i+1)
    0x8b4 <+64>:  str    w0, [sp, #24]       // store (i + 1) in i (i.e. i+=1)
    0x8b8 <+68>:  ldr    w1, [sp, #24]       // w1 = i
    0x8bc <+72>:  ldr    w0, [sp, #4]        // w0 = length
    0x8c0 <+76>:  cmp    w1, w0              // compare i and length
    0x8c4 <+80>:  b.lt   0x88c <sumArray+24> // if (i < length) goto <sumArray+24>
    0x8c8 <+84>:  ldr    w0, [sp, #28]       // w0 = total
    0x8cc <+88>:  add    sp, sp, #0x20       // revert stack to original state
    0x8d0 <+92>:  ret                        // return (total)


When tracing this assembly code, consider whether the data being
accessed represents a pointer or a value. For example, the instruction
at `<sumArray+12>` results in stack location `sp + 28` containing a
variable of type `int`, which is initially set to `0`. In contrast, the
argument stored at location `sp + 8` is the first argument to the
function (`array`), which is of type `int *` and corresponds to the base
address of the array. A different variable (which we call `i`) is stored
at location `sp + 24` and is initially set to 0.


The astute reader will notice a previously unseen instruction at line
`<sumArray+30>` called `ldrsw`. The `ldrsw` instruction stands for
\"load register signed word\" and converts the 32-bit `int` value stored
at `sp + 24` to a 64-bit integer value and stores it in `x0`. This
operation is necessary because the instructions that follow perform
pointer arithmetic. Recall that on 64-bit systems, pointers take up
eight bytes of space. The compiler's use of `ldrsw` simplifies the
process by ensuring that all data are stored in full 64-bit registers
instead of their 32-bit components.


Let's take a closer look at the seven instructions between locations
`<sumArray+28>` and `<sumArray+52>`:




    0x890 <+28>:  lsl    x0, x0, #2             // left shift i by 2 (i << 2, or i*4)
    0x894 <+32>:  ldr    x1, [sp, #8]           // x1 = array
    0x898 <+36>:  add    x0, x1, x0             // x0 = array + i*4
    0x89c <+40>:  ldr    w0, [x0]               // w0 = array[i]
    0x8a0 <+44>:  ldr    w1, [sp, #28]          // w1 = total
    0x8a4 <+48>:  add    w0, w1, w0             // w0 = total + array[i]
    0x8a8 <+52>:  str    w0, [sp, #28]          // store (total + array[i]) in total


The compiler uses `lsl` to perform a left shift on the value `i` stored
in `x0`. When this instruction completes execution, register `x0`
contains `i << 2`, or `i * 4`. At this point, `x0` contains the number
of bytes to calculate the correct offset of `array[i]` (or
`sizeof(int) = 4`).


The next instruction (`ldr x1, [sp, #8]`) loads the first argument to
the function (i.e., the base address of `array`) into register `x1`.
Adding `x1` to `x0` in the next instruction causes `x0` to contain
`array + i * 4`. Recall that the element at index `i` in `array` is
stored at address `array + sizeof(T) * i` Therefore, `x0` now contains
the assembly-level computation of address `&array[i]`.


The instruction at `<sumArray+40>` *dereferences* the value located at
`x0`, placing the value `array[i]` into `w1`. Notice the use of the
component register `w1`, because `array[i]` contains a 32-bit `int`
value! In contrast, the variable `i` was changed to a 64-bit integer on
line `<sumArray+24>` since `i` was about to be used for *address
computation*. Again, addresses (pointers) are stored as 64-bit words.


The last three instructions between `<sumArray+44>` and `<sumArray+52>`
load the current value of `total` into component register `w1`, add
`array[i]` to it, and store the result in component register `w0`,
before updating `total` at location `sp + 28` with the new sum.
Therefore, the seven instructions between `<sumArray+28>` and
`<sumArray+52>` are equivalent to the line `total += array[i]` in the
`sumArray` function.





