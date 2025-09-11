



-   [Dive Into Systems](../index-2.html)
-   [13. The Operating System](index.html)
-   [13.4. Interprocess Communication](ipc.html)
-   [13.4.3. Shared Memory](ipc_shm.html)
:::

::: content
::: sect2
### [](#_shared_memory){.anchor}13.4.3. Shared Memory {#_shared_memory}

::: paragraph
Message passing using sockets is useful for bidirectional communication
between processes running on the same machine and between processes
running on different machines. However, when two processes are running
on the same machine, they can take advantage of shared system resources
to communicate more efficiently than by using message passing.
:::

::: paragraph
For example, an operating system can support interprocess communication
by allowing processes to share all or part of their virtual address
spaces. One process can read and write values to the shared portion of
its address space to communicate with other processes sharing the same
memory region.
:::

::: paragraph
One way that the OS can implement partial address space sharing is by
setting entries in the page tables of two or more processes to map to
the same physical frames. [Figure 1](#FigShm) illustrates an example
mapping. To communicate, one process writes a value to an address on a
shared page, and another process subsequently reads the value.
:::

::: {#FigShm .imageblock .text-center}
::: content
![ipc through shared memory pages](_images/shm.png){width="650"}
:::

::: title
Figure 1. The OS can support sharing pages of virtual address space by
setting entries in the page tables of sharing processes to the same
physical frame number (e.g., frame 100). Note that processes do not need
to use the same virtual address to refer to the shared page of physical
memory.
:::
:::

::: paragraph
If the OS supports partial shared memory, then it implements an
interface to the programmer for creating and attaching to shared pages
(or shared regions/segments) of memory. In Unix systems, the system call
`shmget` creates or attaches to a shared memory segment. Each shared
memory segment corresponds to a contiguous set of virtual addresses
whose physical mappings are shared with other processes attaching to the
same shared memory segment.
:::

::: paragraph
Operating systems also typically support sharing a single, full virtual
address space. A **thread** is the OS abstraction of an execution
control flow. A process has a single thread of execution control flow in
a single virtual address space. A multithreaded process has multiple
concurrent threads of execution control flow in a single, shared virtual
address space --- all threads share the full virtual address space of
their containing process.
:::

::: paragraph
Threads can easily share execution state by reading and writing to
shared locations in their common address space. For example, if one
thread changes the value of a global variable, all other threads see the
result of that change.
:::

::: paragraph
On a multiprocessor systems (SMP or multicore), individual threads of a
multithreaded process can be scheduled to run simultaneously, *in
parallel*, on the multiple cores. In the [Shared Memory
Chapter](../C14-SharedMemory/index.html#_leveraging_shared_memory_in_the_multicore_era){.page},
we discuss threads and parallel multi-threaded programming in more
detail.
:::
:::

::: toc-menu
:::
:::
:::
:::

Copyright (C) 2020 Dive into Systems, LLC.

*Dive into Systems,* is licensed under the Creative Commons
[Attribution-NonCommercial-NoDerivatives 4.0
International](https://creativecommons.org/licenses/by-nc-nd/4.0/) (CC
BY-NC-ND 4.0).
