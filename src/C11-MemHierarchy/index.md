


## 11. Storage and the Memory Hierarchy 

Although designing and implementing efficient algorithms is typically
the *most* critical aspect of writing programs that perform well,
there's another, often overlooked factor that can have a major impact on
performance: memory. Perhaps surprisingly, two algorithms with the same
asymptotic performance (number of steps in the worst case) run on the
same inputs might perform very differently in practice due to the
organization of the hardware on which they execute. Such differences
generally stem from the algorithms\' memory accesses, particularly where
they store data and the kinds of patterns they exhibit when accessing
it. These patterns are referred to as **memory locality**, and to
achieve the best performance, a program's access patterns need to align
well with the hardware's memory arrangement.


For example, consider the following two variations of a function for
averaging the values in an *N*×*N* matrix, as shown in [Table
1](#TabMatrixVersions). Despite both versions accessing the same memory
locations an equal number of times (N^2^), the code on the left executes
about five times faster on real systems than the code on the right. The
difference arises from the patterns in which they access those memory
locations. Toward the end of this chapter we analyze this example using
the memory profiling tool *Cachegrind*.


+-----------------------------------+-----------------------------------+
|                        |                        |
|                   |                   |
|                        |                        |
| ```     | ```     |
| float                             | float                             |
| averageMat_v1(int **mat, int n) { | averageMat_v2(int **mat, int n) { |
|     int i, j, total = 0;          |     int i, j, total = 0;          |
|                                   |                                   |
|     for (i = 0; i < n; i++) {     |     for (i = 0; i < n; i++) {     |
|         for (j = 0; j < n; j++) { |         for (j = 0; j < n; j++) { |
|                                   |                                   |
|          // Note indexing: [i][j] |          // Note indexing: [j][i] |
|             total += mat[i][j];   |             total += mat[j][i];   |
|         }                         |         }                         |
|     }                             |     }                             |
|                                   |                                   |
|                                   |                                   |
|   return (float) total / (n * n); |   return (float) total / (n * n); |
| }                                 | }                                 |
| ```                               | ```                               |
| :::                               | :::                               |
| :::                               | :::                               |
| :::                               | :::                               |
+-----------------------------------+-----------------------------------+

: Table 1. Two versions of a function that accesses every element of an
*N*x*N* matrix. They only differ in their memory indexing into the
matrix.

Storage locations like registers, CPU caches, main memory, and files on
disk all feature remarkably different access times, transfer rates, and
storage capacities. When programming a high-performance application,
it's important to consider where data is stored and how frequently the
program accesses each device's data. For example, accessing a slow disk
once as the program starts is rarely a major concern. On the other hand,
accessing the disk frequently will slow down the program considerably.


This chapter characterizes a diverse set of memory devices and describes
how they're organized in a modern PC. With that context, we'll see how a
collection of varied memory devices can be combined to exploit the
locality found in a typical program's memory access patterns.





