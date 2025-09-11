



-   [Dive Into Systems](../index-2.html)
-   [13. The Operating System](index.html)
-   [13.4. Interprocess Communication](ipc.html)
:::

::: content
::: sect1
## [](#_interprocess_communication){.anchor}13.4. Interprocess Communication {#_interprocess_communication}

::: sectionbody
::: paragraph
Processes are one of the primary abstractions implemented by the OS.
Private virtual address spaces are an important abstraction in
multiprogrammed systems, and are one way in which the OS prevents
processes from interfering with one another's execution state. However,
sometimes a user or programmer may want their application processes to
communicate with one another (or to share some of their execution state)
as they run.
:::

::: paragraph
Operating systems typically implement support for several types of
interprocess communication, or ways in which processes can communicate
or share their execution state. **Signals** are a very restricted form
of interprocess communication by which one process can send a signal to
another process to notify it of some event. Processes can also
communicate using **message passing**, in which the OS implements an
abstraction of a message communication channel that is used by a process
to exchange messages with another process. Finally, the OS may support
interprocess communication through **shared memory** that allows a
process to share all or part of its virtual address space with other
processes. Processes with shared memory can read or write to addresses
in shared space to communicate with one another.
:::
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
