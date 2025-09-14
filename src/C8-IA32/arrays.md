

 







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





















## 8.7. Arrays 

Recall that
[arrays](../C1-C_intro/arrays_strings.html#_introduction_to_arrays)
are ordered collections of data elements of the same type that are
contiguously stored in memory. Statically allocated [single-dimension
arrays](../C2-C_depth/arrays.html#_single_dimensional_arrays)
have the form `Type arr[N]`, where `Type` is the data type, `arr` is the
identifier associated with the array, and `N` is the number of data
elements. Declaring an array statically as `Type arr[N]` or dynamically
as `arr = malloc(N*sizeof(Type))` allocates *N* x sizeof(*Type*) total
bytes of memory, with `arr` pointing to it.


To access the element at index *i* in array `arr`, use the syntax
`arr[i]`. Compilers commonly convert array references into [pointer
arithmetic](../C2-C_depth/pointers.html#_pointer_variables) prior
to translating to assembly. So, `arr+i` is equivalent to `&arr[i]`, and
`*(arr+i)` is equivalent to `arr[i]`. Since each data element in `arr`
is of type `Type`, `arr+i` implies that element *i* is stored at address
`arr + sizeof(Type) * i`.


Table 1 outlines some common array operations and their
corresponding assembly instructions. Assume that register `%edx` stores
the address of `arr`, register `%ecx` stores the value `i`, and register
`%eax` represents some variable `x`.


+----------------------+----------------------+-----------------------+
| Operation            | Type                 | Assembly              |
|                      |                      | Representation        |
+======================+======================+=======================+
| `x = arr`            | `int *`              | `movl %edx, %eax`     |
+----------------------+----------------------+-----------------------+
| `x = arr[0]`         | `int`                | `movl (%edx), %eax`   |
+----------------------+----------------------+-----------------------+
| `x = arr[i]`         | `int`                | `movl                 |
|                      |                      | (%edx, %ecx,4), %eax` |
+----------------------+----------------------+-----------------------+
| `x = &arr[3]`        | `int *`              | `                     |
|                      |                      | leal 0xc(%edx), %eax` |
+----------------------+----------------------+-----------------------+
| `x = arr+3`          | `int *`              | `                     |
|                      |                      | leal 0xc(%edx), %eax` |
+----------------------+----------------------+-----------------------+
| `x = *(arr+3)`       | `int`                | `                     |
|                      |                      | movl 0xc(%edx), %eax` |
+----------------------+----------------------+-----------------------+

: Table 1. Common Array Operations and Their Corresponding Assembly
Representations

Pay close attention to the *type* of each expression in
\[ArrayOps\]. In general, the compiler uses `movl`
instructions to dereference pointers and the `leal` instruction to
compute addresses.


Notice that to access element `arr[3]` (or `*(arr+3)` using pointer
arithmetic), the compiler performs a memory lookup on address `arr+3*4`
instead of `arr+3`. To understand why this is necessary, recall that any
element at index *i* in an array is stored at address
`arr + sizeof(Type) * i`. The compiler must therefore multiply the index
by the size of the data type to compute the correct offset. Recall also
that memory is byte-addressable; offsetting by the correct number of
bytes is the same as computing an address.


As an example, consider a sample array (`array`) with five integer
elements (Figure 1):




![Each integer in the array requires four bytes.](_images/arrayFig.png)


Figure 1. The layout of a five-integer array in memory. Each
x~i~-labeled box represents one byte, each int is four bytes.


Notice that since `array` is an array of integers, each element takes up
exactly four bytes. Thus, an integer array with five elements consumes
20 bytes of contiguous memory.


To compute the address of element 3, the compiler multiplies the index 3
by the data size of the integer type (4) to yield an offset of 12. Sure
enough, element 3 in \[FigArray6\] is located at byte
offset x~12~.


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
look at the corresponding assembly for the `sumArray` function:




    <sumArray>:
     <+0>:  push %ebp                    # save ebp
     <+1>:  mov  %esp,%ebp               # update ebp (new stack frame)
     <+3>:  sub  $0x10,%esp              # add 16 bytes to stack frame
     <+6>:  movl $0x0,-0x8(%ebp)         # copy 0 to %ebp-8 (total)
     <+13>: movl $0x0,-0x4(%ebp)         # copy 0 to %ebp-4 (i)
     <+20>: jmp  0x80484ab <sumArray+46> # goto <sumArray+46> (start)
     <+22>: mov  -0x4(%ebp),%eax         # copy i to %eax
     <+25>: lea  0x0(,%eax,4),%edx       # copy i*4 to %edx
     <+32>: mov  0x8(%ebp),%eax          # copy array to %eax
     <+35>: add  %edx,%eax               # copy array+i*4 to %eax
     <+37>: mov  (%eax),%eax             # copy *(array+i*4) to %eax
     <+39>: add  %eax,-0x8(%ebp)         # add *(array+i*4) to total
     <+42>: addl $0x1,-0x4(%ebp)         # add 1 to i
     <+46>: mov  -0x4(%ebp),%eax         # copy i to %eax
     <+49>: cmp  0xc(%ebp),%eax          # compare i with length
     <+52>: jl   0x8048493 <sumArray+22> # if i<length goto <sumArray+22> (loop)
     <+54>: mov  -0x8(%ebp),%eax         # copy total to eax
     <+57>: leave                        # prepare to leave the function
     <+58>: ret                          # return total


When tracing this assembly code, consider whether the data being
accessed represents an address or a value. For example, the instruction
at `<sumArray+13>` results in `%ebp-4` containing a variable of type
`int`, which is initially set to 0. In contrast, the argument stored at
`%ebp+8` is the first argument to the function (`array`) which is of
type `int *` and corresponds to the base address of the array. A
different variable (which we call `total`) is stored at location
`%ebp-8`.


Let's take a closer look at the five instructions between locations
`<sumArray+22>` and `<sumArray+39>`:




    <+22>: mov  -0x4(%ebp),%eax      # copy i to %eax
    <+25>: lea  0x0(,%eax,4),%edx    # copy i*4 to %edx
    <+32>: mov  0x8(%ebp),%eax       # copy array to %eax
    <+35>: add  %edx,%eax            # copy array+i*4 to %eax
    <+37>: mov  (%eax),%eax          # copy *(array+i*4) to %eax
    <+39>: add  %eax,-0x8(%ebp)      # add *(array+i*4) to total (total+=array[i])


Recall that the compiler commonly uses `lea` to perform simple
arithmetic on operands. The operand `0x0(,%eax,4)` translates to
`%eax*4 + 0x0`. Since `%eax` holds the value `i`, this operation copies
the value `i*4` to `%edx`. At this point, `%edx` contains the number of
bytes that must be added to calculate the correct offset of `array[i]`.


The next instruction (`mov 0x8(%ebp), %eax`) copies the first argument
(the base address of `array`) into `%eax`. Adding `%edx` to `%eax` in
the next instruction causes `%eax` to contain `array+i*4`. Recall that
the element at index *i* in `array` is stored at address
`array + sizeof(T) * i`. Therefore, `%eax` now contains the
assembly-level computation of the address `&array[i]`.


The instruction at `<sumArray+37>` *dereferences* the value located at
`%eax`, placing the value `array[i]` into `%eax`. Lastly, `%eax` is
added to the value in `%ebp-8`, or `total`. Therefore, the five
instructions between locations `<sumArray+22>` and `<sumArray+39>`
correspond to the line `total += array[i]` in the `sumArray` function.





