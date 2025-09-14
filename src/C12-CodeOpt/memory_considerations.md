
















## 12.3. Memory Considerations 

Programmers should pay special attention to memory use, especially when
employing memory-intensive data structures such as matrices and arrays.
Although compilers offer powerful optimization features, the compiler
cannot always make optimizations that improve a program's memory use. In
this section, we use an implementation of a matrix-vector program
([matrixVector.c](_attachments/matrixVector.c)) to guide discussion of
techniques and tools for improving memory use.


The `main` function of the program performs two steps. First, it
allocates and initializes the input matrix, the input vector, and the
output matrix. Next, it performs matrix-vector multiplication. Running
the code on matrix-vector dimensions of 10,000 × 10,000 reveals that the
`matrixVectorMultiply` function takes up the majority of the time:




```
$ gcc -o matrixVector matrixVector.c
$ ./matrixVector 10000 10000
Time to allocate and fill matrices: 1.2827
Time to allocate vector: 9.6e-05
Time to matrix-vector multiply: 1.98402
```


Our discussion will thus focus on the `matrixVectorMultiply` function.



### 12.3.1. Loop Interchange 

Loop interchange optimizations switch the order of inner and outer loops
in nested loops in order to maximize cache locality. Automatically
performing this task is difficult for compilers to do. In GCC, the
`-floop-interchange` compiler flag exists but is currently not available
by default. Therefore, it is a good idea for programmers to pay
attention to how their code is accessing memory-composite data
structures like arrays and matrices. As an example, let's take a closer
look at the `matrixVectorMultiply` function in
[matrixVector.c](_attachments/matrixVector.c):


+-----------------------------------+-----------------------------------+
| Original Version                  | Loop interchange version          |
| ([matrixVector                    | ([matrixVector2.                  |
| .c](_attachments/matrixVector.c)) | c](_attachments/matrixVector2.c)) |
+===================================+===================================+
|                        |                        |
|                   |                   |
|                        |                        |
| ```     | ```     |
| v                                 | v                                 |
| oid matrixVectorMultiply(int **m, | oid matrixVectorMultiply(int **m, |
|                           int *v, |                           int *v, |
|                                   |                                   |
|                        int **res, |                        int **res, |
|                                   |                                   |
|                          int row, |                          int row, |
|                                   |                                   |
|                        int col) { |                        int col) { |
|     int i, j;                     |     int i, j;                     |
|     //c                           |     //c                           |
| ycles through every matrix column | ycles through every row of matrix |
|     /                             |     //in inner-most loop          |
| /in inner-most loop (inefficient) |     for (i = 0; i < row; i++){    |
|     for (j = 0; j < col; j++){    |                                   |
|                                   |        for (j = 0; j < col; j++){ |
|        for (i = 0; i < row; i++){ |                                   |
|                                   |       res[i][j] = m[i][j] * v[j]; |
|       res[i][j] = m[i][j] * v[j]; |         }                         |
|         }                         |     }                             |
|     }                             | }                                 |
| }                                 | ```                               |
| ```                               | :::                               |
| :::                               | :::                               |
| :::                               | :::                               |
| :::                               |                                   |
+-----------------------------------+-----------------------------------+

: Table 1. Loop interchange on the `matrixVectorMultiply()` function.

The input and output matrices are dynamically allocated (second method
discussed in C chapter). As a result, the rows in the matrices are not
contiguous to one another, whereas the elements in every row are
contiguous. The current ordering of the loops causes the program to
cycle through each column instead of every row. Recall that [data is
loaded into cache in
*blocks*](../C11-MemHierarchy/caching.html#_direct_mapped_caches)
not elements. As a result, when an element *x* in an array in either
`res` or `m` is accessed, the *elements adjacent to x* are also loaded
into cache. Cycling through every \"column\" of the matrix causes more
cache misses, as the cache is forced to load new blocks with every
access. Table 2 shows that adding optimization flags does
not decrease the runtime of the function. However, simply switching the
order of the loops (as shown in the previous code examples and in
[matrixVector2.c](_attachments/matrixVector2.c)) makes the function
nearly eight times faster and allows the compiler to perform additional
optimizations.


+----------+----------+----------+----------+----------+-----------+
| Version  | Program  | Uno      | `-O1`    | `-O2`    | `-O3`     |
|          |          | ptimized |          |          |           |
+==========+==========+==========+==========+==========+===========+
| Original | `matri   | 2.01     | 2.05     | 2.07     | 2.08      |
|          | xVector` |          |          |          |           |
+----------+----------+----------+----------+----------+-----------+
| With     | `matrix  | 0.27     | 0.08     | 0.06     | 0.06      |
| Loop     | Vector2` |          |          |          |           |
| Int      |          |          |          |          |           |
| erchange |          |          |          |          |           |
+----------+----------+----------+----------+----------+-----------+

: Table 2. Time in Seconds to Perform Matrix Multiplication on 10,000 ×
10,000 Elements

The Valgrind tool `cachegrind` (discussed in [Chapter
11](../C11-MemHierarchy/cachegrind.html#_cache_analysis_and_valgrind))
is a great way to identify data locality issues, and reveals the cache
access differences in the two versions of the `matrixVectorMultiply`
function shown in the previous example.



### 12.3.2. Some Other Compiler Optimizations for Improving Locality: Fission and Fusion 

Rerunning the improved program on 10,000 × 10,000 elements yields the
following runtime numbers:




```
$ gcc -o matrixVector2 matrixVector2.c
$ ./matrixVector2 10000 10000
Time to allocate and fill matrices: 1.29203
Time to allocate vector: 0.000107
Time to matrix-vector multiply: 0.271369
```


Now, matrix allocation and filling takes the most time. Additional
timing reveals that it is the filling of the matrices that in fact takes
the most time. Let's take a closer look at that code:




```
//fill matrices
for (i = 0; i < rows; i++){
    fillArrayRandom(matrix[i], cols);
    fillArrayZeros(result[i], cols);
}
```


To fill the input and output matrices, a `for` loop cycles through all
the rows, and calls the `fillArrayRandom` and `fillArrayZeros` functions
on each matrix. In some scenarios, it may be advantageous for the
compiler to split the single loop into two separate loops (known as
**loop fission**), as shown in Table 3.


+-----------------------------------+-----------------------------------+
| Original Version                  | With loop fission                 |
+===================================+===================================+
|                        |                        |
|                   |                   |
|                        |                        |
| ```     | ```     |
| for (i = 0; i < rows; i++) {      | for (i = 0; i < rows; i++) {      |
|                                   |                                   |
| fillArrayRandom(matrix[i], cols); | fillArrayRandom(matrix[i], cols); |
|                                   | }                                 |
|  fillArrayZeros(result[i], cols); |                                   |
| }                                 | for (i = 0; i < rows; i++) {      |
| ```                               |                                   |
| :::                               |  fillArrayZeros(result[i], cols); |
| :::                               | }                                 |
| :::                               | ```                               |
|                                   | :::                               |
|                                   | :::                               |
|                                   | :::                               |
+-----------------------------------+-----------------------------------+

: Table 3. Loop fission example on fill matrices for loop in `main`.

The process of taking two loops that operate over the same range and
combining their contents into a single loop (i.e., the opposite of loop
fission) is called **loop fusion**. Loop fission and fusion are examples
of optimizations a compiler might perform to try to improve data
locality. Compilers for multicore processors may also use loop fission
or fusion to enable loops to execute efficiently on multiple cores. For
example, a compiler may use loop fission to assign two loops to
different cores. Likewise, a compiler may use loop fusion to combine
together dependent operations into the body of the loop and distribute
to each core a subset of the loop iterations (assuming data between
iterations are independent).


In our case, applying loop fission manually does not directly improve
program performance; there is virtually no change in the amount of time
required to fill the array. However, it may make clear a more subtle
optimization: the loop containing `fillArrayZeros` is not necessary. The
`matrixVectorMultiply` function assigns values to each element in the
`result` array; a prior initialization to all zeros is unnecessary.


+-----------------------------------+-----------------------------------+
| Previous version                  | Updated version with `calloc()`   |
| ([matrixVector2.                  | ([matrixVector3.                  |
| c](_attachments/matrixVector2.c)) | c](_attachments/matrixVector3.c)) |
+===================================+===================================+
|                        |                        |
|                   |                   |
|                        |                        |
| ```     | ```     |
| for (i = 0; i < rows; i++) {      | for (i = 0; i < rows; i++) {      |
|                                   |                                   |
|  matrix[i] = allocateArray(cols); |  matrix[i] = allocateArray(cols); |
|                                   |                                   |
|  result[i] = allocateArray(cols); |  result[i] = allocateArray(cols); |
| }                                 | }                                 |
|                                   |                                   |
| for (i = 0; i < rows; i++) {      | for (i = 0; i < rows; i++) {      |
|                                   |                                   |
| fillArrayRandom(matrix[i], cols); | fillArrayRandom(matrix[i], cols); |
|                                   |     //fillArrayZeros(res          |
|  fillArrayZeros(result[i], cols); | ult[i], cols); //no longer needed |
| }                                 | }                                 |
| ```                               | ```                               |
| :::                               | :::                               |
| :::                               | :::                               |
| :::                               | :::                               |
+-----------------------------------+-----------------------------------+

: Table 4. Removing unneeded call to `fillArrayZeros`.



### 12.3.3. Memory Profiling with Massif 

Making the previous change results in only a slight decrease in runtime.
Although it eliminates the step of filling in all elements in the result
matrix with zeros, a significant amount of time is still required to
fill the input matrix with random numbers:




```
$ gcc -o matrixVector3 matrixVector3.c
$ ./matrixVector3 10000 10000
Time to allocate matrices: 0.049073
Time to fill matrices: 0.946801
Time to allocate vector: 9.3e-05
Time to matrix-vector multiply: 0.359525
```


Even though each array is stored noncontiguously in memory, each one
takes up 10,000 × `sizeof(int)` bytes, or 40,000 bytes. Because there is
a total of 20,000 (10,000 each for the initial matrix and the result
matrix) arrays allocated, this corresponds to 800 million bytes, or
roughly 762 MB of space. Filling 762 MB with random numbers
understandably takes a lot of time. With matrices, memory use increases
quadratically with the input size, and can play a large role in
performance.


Valgrind's `massif` tool can help you profile memory use. Like the other
Valgrind tools we covered in this book
([memcheck](../C3-C_debug/valgrind.html#_debugging_memory_with_valgrind),
[cachegrind](../C11-MemHierarchy/cachegrind.html#_cache_analysis_and_valgrind),
and callgrind),
`massif` runs as a wrapper around a program's executable. Specifically,
`massif` takes snapshots of program memory use throughout the program,
and profiles how memory usage fluctuates. Programmers may find the
`massif` tool useful for tracking how their programs use heap memory,
and for identifying opportunities to improve memory use. Let's run the
`massif` tool on the `matrixVector3` executable:




    $ valgrind --tool=massif ./matrixVector3 10000 10000
    ==7030== Massif, a heap profiler
    ==7030== Copyright (C) 2003-2015, and GNU GPL'd, by Nicholas Nethercote
    ==7030== Using Valgrind-3.11.0 and LibVEX; rerun with -h for copyright info
    ==7030== Command: ./matrixVector3 10000 10000
    ==7030==
    Time to allocate matrices: 0.049511
    Time to fill matrices: 4.31627
    Time to allocate vector: 0.001015
    Time to matrix-vector multiply: 0.62672
    ==7030==


Running `massif` produces a `massif.out.xxxx` file, where `xxxx` is a
unique id number. If you are typing along, type `ls` to reveal your
corresponding massif file. In the example that follows, the
corresponding file is `massif.out.7030`. Use the `ms_print` command to
view the `massif` output:




    $ ms_print massif.out.7030
    -----------------------------------------------------------------------------
    Command:            ./matrixVector3 10000 10000
    Massif arguments:   (none)
    ms_print arguments: massif.out.7030
    -----------------------------------------------------------------------------

        MB
    763.3^                                                ::::::::::::::::::::::#
         |:::::::::::::::::::::::::::::::::::::::::::::::::                     #
         |:                                               :                     #
         |@                                               :                     #
         |@                                               :                     #
         |@                                               :                     #
         |@                                               :                     #
         |@                                               :                     #
         |@                                               :                     #
         |@                                               :                     #
         |@                                               :                     #
         |@                                               :                     #
         |@                                               :                     #
         |@                                               :                     #
         |@                                               :                     #
         |@                                               :                     #
         |@                                               :                     #
         |@                                               :                     #
         |@                                               :                     #
         |@                                               :                     #
       0 +-------------------------------------------------------------------->Gi
         0                                                                  9.778

    Number of snapshots: 80
     Detailed snapshots: [3, 12, 17, 22, 49, 59, 69, 79 (peak)]


At the top of the output is the memory use graph. The *x*-axis shows the
number of instructions executed. The *y*-axis shows memory use. The
graph above indicates that a total of 9.778 billion (Gi) instructions
executed during our run of `matrixVector3`. During execution, `massif`
took a total of 80 snapshots to measure use on the heap. Memory use
peaked in the last snapshot (79). Peak memory use for the program was
763.3 MB, and stayed relatively constant throughout the program.


Summaries of all the snapshots occur after the graph. For example, the
following table corresponds to the snapshots around snapshot 79:




    ....

    ------------------------------------------------------------------------------
      n        time(i)         total(B)   useful-heap(B) extra-heap(B)   stacks(B)
    ------------------------------------------------------------------------------
     70      1,081,926      727,225,400      727,080,000       145,400          0
     71      1,095,494      737,467,448      737,320,000       147,448          0
     72      1,109,062      747,709,496      747,560,000       149,496          0
     73      1,122,630      757,951,544      757,800,000       151,544          0
     74      1,136,198      768,193,592      768,040,000       153,592          0
     75      1,149,766      778,435,640      778,280,000       155,640          0
     76      1,163,334      788,677,688      788,520,000       157,688          0
     77      1,176,902      798,919,736      798,760,000       159,736          0
     78  7,198,260,935      800,361,056      800,201,024       160,032          0
     79 10,499,078,349      800,361,056      800,201,024       160,032          0
    99.98% (800,201,024B) (heap allocations) malloc/new/new[], --alloc-fns, etc.
    ->99.96% (800,040,000B) 0x40089D: allocateArray (in matrixVector3)


Each row corresponds to a particular snapshot, the time it was taken,
the total heap memory consumption (in bytes) at that point, the number
of bytes requested by the program (\"useful-heap\") at that point, the
number of bytes allocated in excess of what the program asked for, and
the size of the stack. By default, stack profiling is off (it slows
`massif` down significantly). To enable stack profiling, use the
`--stacks=yes` option when running `massif`.


The `massif` tool reveals that 99.96% of the program's heap memory use
occurred in the `allocateArray` function and that a total of 800 million
bytes were allocated, consistent with the back-of-the-envelope
calculation we performed earlier. Readers will likely find `massif` a
useful tool for identifying areas of high heap memory use in their
programs, which often slows a program down. For example, **memory
leaks** can occur in programs when programmers frequently call `malloc`
without calling `free` at the first correct opportunity. The `massif`
tool is incredibly useful for detecting such leaks.






