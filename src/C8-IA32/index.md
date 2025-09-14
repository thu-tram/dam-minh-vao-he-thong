## 8. 32-bit x86 Assembly (IA32) 

In this chapter, we cover the Intel Architecture 32-bit (IA32)
instruction set architecture. Recall that an [instruction set
architecture](../C5-Arch/index.html#_what_von_neumann_knew_computer_architecture)
(or ISA) defines the set of instructions and binary encodings of a
machine-level program. To run the examples in this chapter, you will
need access to a machine with an x86 processor or a compiler that can
create 32-bit executables. The term \"x86\" is often used synonymously
with the IA32 architecture. The x86, and its 64-bit variant x86-64, are
ubiquitous in modern computers.


Very few modern machines have 32-bit processors; most Intel and AMD
systems produced since 2007 have 64-bit processors. To check what type
of processor you have, use the `uname -m` command:




    $ uname -m
    i686


If typing `uname -m` returns either `i686` or `i386`, your system has a
32-bit processor. However, if the `uname -m` command returns `x86_64`,
your system has a newer 64-bit processor. Please note that since x86-64
is an *extension* of the older IA32 ISA, virtually all 64-bit systems
contain a 32-bit subsystem that allows the execution of 32-bit
executables.


If you have a 64-bit Linux system, additional packages are sometimes
required to allow users to *create* 32-bit executables, like we will be
doing in this chapter. For example, on an Ubuntu machine you will need
to install 32-bit development libraries and additional packages to
augment GCC with cross-compiling features:




    sudo apt-get install libc6-dev-i386 gcc-multilib


>> x86 Syntax Branches


x86 architectures typically follow one of two different syntax branches.
UNIX machines commonly use the AT&T syntax, given that UNIX was
developed at AT&T Bell Labs. The corresponding assembler is GNU
Assembler (GAS). Since we use GCC for most examples in this book, we
cover AT&T syntax in this chapter. Windows machines commonly use Intel
syntax, which is used by Microsoft's Macro Assembler (MASM). The Netwide
Assembler (NASM) is an example of a Linux assembler that uses Intel
syntax. The argument regarding the superiority of one syntax over the
other is one of the \"holy wars\" of the discipline. However, there is
value in being familiar with both syntaxes, as a programmer may
encounter either in various circumstances.






