

 





























## 9.9. structs in Assembly 

A `struct` is another
way to create a collection of data types in C. Unlike arrays, they
enable different data types to be grouped together. C stores a `struct`
like a single-dimension array, where the data elements (fields) are
stored contiguously.


Let's revisit `struct studentT` from Chapter 1:




```
struct studentT {
    char name[64];
    int  age;
    int  grad_yr;
    float gpa;
};

struct studentT student;
```


Figure 1 shows how the `student` is laid out in
memory. Each a~i~ denotes an offset in memory.




![structArray](_images/structArray.png)


Figure 1. The memory layout of a struct studentT.


Each field is stored contiguously next to one another in memory in the
order in which they are declared. In Figure 1, the
`age` field is allocated at the memory location directly after the
`name` field (at byte offset a~64~) and is followed by the `grad_yr`
(byte offset a~68~) and `gpa` (byte offset a~72~) fields. This
organization enables memory-efficient access to the fields.


To understand how the compiler generates assembly code to work with a
`struct`, consider the function `initStudent`:




```
void initStudent(struct studentT *s, char *nm, int ag, int gr, float g) {
    strncpy(s->name, nm, 64);
    s->grad_yr = gr;
    s->age = ag;
    s->gpa = g;
}
```


The `initStudent` function uses the base address of a `struct studentT`
as its first parameter, and the desired values for each field as its
remaining parameters. The following listing depicts this function in
assembly:




    Dump of assembler code for function initStudent:
    0x7f4 <+0>:  stp  x29, x30, [sp, #-48]!  // sp-=48; store x29, x30 at sp, sp+4
    0x7f8 <+4>:  mov  x29, sp                // x29 = sp (frame ptr = stack ptr)
    0x7fc <+8>:  str  x0, [x29, #40]         // store s at x29 + 40
    0x800 <+12>: str  x1, [x29, #32]         // store nm at x29 + 32
    0x804 <+16>: str  w2, [x29, #28]         // store ag at x29 + 28
    0x808 <+20>: str  w3, [x29, #24]         // store gr at x29 + 24
    0x80c <+24>: str  s0, [x29, #20]         // store g at x29 + 20
    0x810 <+28>: ldr  x0, [x29, #40]         // x0 = s
    0x814 <+32>: mov  x2, #0x40              // x2 = 0x40 (or 64)
    0x814 <+36>: ldr  x1, [x29, #32]         // x1 = nm
    0x818 <+40>: bl   0x6e0 <strncpy@plt>    // call strncpy(s, nm, 64) (s->name)
    0x81c <+44>: ldr  x0, [x29, #40]         // x0 = s
    0x820 <+48>: ldr  w1, [x29, #24]         // w1 = gr
    0x824 <+52>: str  w1, [x0, #68]          // store gr at (s + 68) (s->grad_yr)
    0x828 <+56>: ldr  x0, [x29, #40]         // x0 = s
    0x82c <+60>: ldr  w1, [x29, #28]         // w1 = ag
    0x830 <+64>: str  w1, [x0, #64]          // store ag at (s + 64) (s->age)
    0x834 <+68>: ldr  x0, [x29, #40]         // x0 = s
    0x838 <+72>: ldr  s0, [x29, #20]         // s0 = g
    0x83c <+80>: str  s0, [x0, #72]          // store g at (s + 72) (s->gpa)
    0x844 <+84>: ldp  x29, x30, [sp], #48    // x29 = sp, x30 = sp+4, sp += 48
    0x848 <+88>: ret                         // return (void)


Being mindful of the byte offsets of each field is key to understanding
this code. Here are a few things to keep in mind.



-   The `strncpy` call takes the base address of the `name` field of
    `s`, the address of array `nm`, and a length specifier as its three
    arguments. Recall that since `name` is the first field in
    `struct studentT`, the address of `s` is synonymous with the address
    of `s→name`.




    0x7fc <+8>:  str  x0, [x29, #40]         // store s at x29 + 40
    0x800 <+12>: str  x1, [x29, #32]         // store nm at x29 + 32
    0x804 <+16>: str  w2, [x29, #28]         // store ag at x29 + 28
    0x808 <+20>: str  w3, [x29, #24]         // store gr at x29 + 24
    0x80c <+24>: str  s0, [x29, #20]         // store g at x29 + 20
    0x810 <+28>: ldr  x0, [x29, #40]         // x0 = s
    0x814 <+32>: mov  x2, #0x40              // x2 = 0x40 (or 64)
    0x814 <+36>: ldr  x1, [x29, #32]         // x1 = nm
    0x818 <+40>: bl   0x6e0 <strncpy@plt>    // call strncpy(s, nm, 64) (s->name)



-   The above code snippet contains an undiscussed register (`s0`). The
    `s0` register is an example of a register reserved for floating
    point values.

-   The next part (instructions `<initStudent+44>` thru
    `<initStudent+52>`) places the value of the `gr` parameter at an
    offset of 68 from the start of `s`. Revisiting the memory layout of
    the struct in Figure 1 shows that this address
    corresponds to `s→grad_yr`.




    0x81c <+44>: ldr  x0, [x29, #40]         // x0 = s
    0x820 <+48>: ldr  w1, [x29, #24]         // w1 = gr
    0x824 <+52>: str  w1, [x0, #68]          // store gr at (s + 68) (s->grad_yr)



-   The next section (instructions `<initStudent+56>` thru
    `<initStudent+64>`) copies the `ag` parameter to the `s→age` field,
    which is located at an offset of 64 bytes from the address of `s`.




    0x828 <+56>: ldr  x0, [x29, #40]         // x0 = s
    0x82c <+60>: ldr  w1, [x29, #28]         // w1 = ag
    0x830 <+64>: str  w1, [x0, #64]          // store ag at (s + 64) (s->age)


Lastly, the `g` parameter value is copied to the `s→gpa` field (byte
offset 72). Notice the use of the `s0` register since the data contained
at location `x29 + 20` is single-precision floating point:




    0x834 <+68>: ldr  x0, [x29, #40]         // x0 = s
    0x838 <+72>: ldr  s0, [x29, #20]         // s0 = g
    0x83c <+80>: str  s0, [x0, #72]          // store g at (s + 72) (s->gpa)



### 9.9.1. Data Alignment and structs 

Consider the following modified declaration of `studentT`:




```
struct studentTM {
    char name[63]; //updated to 63 instead of 64
    int  age;
    int  grad_yr;
    float gpa;
};

struct studentTM student2;
```


The size of the `name` field is modified to be 63 bytes, instead of the
original 64 bytes. Consider how this affects the way the `struct` is
laid out in memory. It may be tempting to visualize it as in [Figure
2](#incorrectLayouta64).




![struct2wrong](_images/struct2wrong.png)


Figure 2. An incorrect memory layout for the updated struct studentTM.
Note that the \"name\" field is reduced from 64 to 63 bytes.


In this depiction, the `age` field occurs in the byte immediately
following the `name` field. But this is incorrect. [Figure
3](#correctLayouta64) depicts the actual layout in memory.




![struct2right](_images/struct2right.png)


Figure 3. The correct memory layout for the updated struct studentTM.
Byte a~63~ is added by the compiler to satisfy memory alignment
constraints, but it doesn't correspond to any of the fields.


A64's alignment policy requires that four-byte data types (e.g., `int`)
reside at addresses that are a multiple of four, whereas 64-bit data
types (`long`, `double`, and pointer data) reside at addresses that are
a multiple of eight. For a `struct`, the compiler adds empty bytes as
\"padding\" between fields to ensure that each field satisfies its
alignment requirements. For example, in the `struct` declared in the
previous code snippet, the compiler adds a byte of padding at byte a~63~
to ensure that the `age` field starts at an address that is at a
multiple of four. Values aligned properly in memory can be read or
written in a single operation, enabling greater efficiency.


Consider what happens when the `struct` is defined as the following:




```
struct studentTM {
    int  age;
    int  grad_yr;
    float gpa;
    char name[63];
};

struct studentTM student3;
```


Moving the `name` array to the end moves the byte of padding to the end
of the struct, ensuring that `age`, `grad_yr`, and `gpa` are four-byte
aligned.






