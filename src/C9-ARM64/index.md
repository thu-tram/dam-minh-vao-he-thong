

 




























## 9. ARM Assembly 

In this chapter, we cover the ARM version 8 application profile
(ARMv8-A) architecture A64 ISA, the latest ARM ISA that is in use on all
Linux OS ARM computers. Recall that an [instruction set
architecture](../C5-Arch/index.html#_what_von_neumann_knew_computer_architecture)
(or ISA) defines the set of instructions and binary encodings of a
machine-level program. To run the examples in this chapter, you will
need access to a machine with an ARMv8-A processor with a 64-bit
operating system installed. The examples in this chapter use a Raspberry
Pi 3B+ running the 64-bit Ubuntu Mate operating system. Note that every
Raspberry Pi released since 2016 can use the A64 ISA. However, Raspberry
Pi OS (the default Raspberry Pi operating system) is still 32-bit as of
this writing.


Readers can confirm that they have a 64-bit version of the OS on their
system by running the `uname -m` command. A system with a 64-bit
operating system will output the following:




    $ uname -m
    aarch64


Although it is possible to *build* ARM binaries on Intel machines using
ARM's [GNU toolchain cross-compilation
tools](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-a/downloads),
you cannot *run* ARM binaries directly on a x86 system. Readers
interested in learning about ARM assembly directly on their laptops are
encouraged to explore [QEMU](https://www.qemu.org/), which can
**emulate** an ARM system. Emulators differ from virtual machines in
that they also simulate the hardware of another system.


Another alternative is to use one of Amazon's recently released [EC2 A1
instances](https://aws.amazon.com/ec2/instance-types/a1/). Each instance
gives users access to a 64-bit Graviton processor, which follows the
ARMv8-A specification.


Keep in mind, however, that the specific assembly instructions produced
by a compiler are highly influenced by the operating system and precise
machine architecture. Therefore, the assembly produced on AWS instances
or through QEMU emulation may differ slightly from the examples shown in
this chapter.


>> RISC and ARM processors


For many years, complex instruction set computer (CISC) architectures
dominated the personal computing and server markets. Common examples of
CISC architectures include Intel and AMD processors. However, reduced
instruction set computer (RISC) architectures gained momentum over the
past decade due to demand from the mobile computing sector. ARM (which
stands for Acorn RISC machine) is an example of a RISC architecture,
along with RISC-V and MIPS. RISC architectures are especially attractive
to mobile computing due to the energy efficiency of their processors,
which prolongs battery life. In recent years, ARM and other RISC
processors have begun making headway in the server and high performance
computing (HPC) markets. For example, Japan's Fugaku supercomputer, the
fastest in the world as of 2020, uses ARM processors.






