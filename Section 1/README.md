# Section 1

## Introduction

Before we start you need to powering on your computer.

### Power-On Sequence of a Computer

When you press the power button, several steps occur before you gain control of the system:

1. **Power Stabilization**  
   A brief delay ensures that essential components (CPU, memory, storage, etc.) receive stable power.

2. **CPU Initialization**  
   The CPU begins execution from a predefined location called the *reset vector*. This tells the processor where to start running instructions.

3. **Firmware Execution (BIOS/UEFI)**  
   The reset vector points to firmware stored in ROM (traditionally the BIOS, now often UEFI). The CPU starts executing this firmware directly, rather than copying it into RAM first.  
   - **POST (Power-On Self-Test):** Checks hardware components such as RAM, storage devices, and peripherals.  
   - **Interrupt Vectors:** Provides low-level routines for interacting with hardware.  
   - **Bootstrapping:** Searches for a bootable device containing an operating system.

4. **Loading the Operating System**  
   Once a bootable device is found, the firmware loads the operating system’s bootloader into memory. The bootloader then loads the OS kernel and essential drivers.

5. **Operating System Initialization**  
   The kernel sets up memory management, process scheduling, and hardware drivers. Once initialization is complete, control is handed to the user through the login screen or desktop environment.


### Building Your First OS‑ish

To understand how an operating system works, we’ll experiment with writing code that runs *bare‑metal* — meaning it interacts directly with the hardware (via BIOS interrupts) instead of relying on an existing OS.

Running custom code on modern PCs is complicated, so we’ll use an emulator called **DOSBox**. DOSBox emulates an older x86 environment with simple BIOS interrupts, which are well‑documented and perfect for learning.

In the next sections, we will:

1. **Write C code using BIOS interrupts**  
   Create small programs that rely only on BIOS routines (like printing text or reading input), without any operating system calls.

2. **Build a simple shell**  
   Extend our C project into a minimal shell, adding usability features such as command input and basic responses.

3. **Create a bootable image**  
   Package our shell into a bootable image and run it inside DOSBox, replacing the default DOS environment with our own “OS‑ish.”


### Compilation Process

When you press the **Compile** button in your favorite IDE, several steps happen behind the scenes. The IDE makes this process easier, but it’s important to understand what’s really going on — and you can do it without an IDE.

1. **Write the Code**  
   Open a text editor and write your program in C (or another language).

2. **Compilation**  
   A **compiler** translates your source code (text) into machine code.  
   - The compiler produces **object files** (binary code for each source file).  
   - It also needs access to **headers** and **libraries** that your program uses.

3. **Linking**  
   A **linker** combines all object files and libraries into a single **executable file**.  
   - This step resolves references between files (e.g., function calls across modules).  
   - The result is a program you can run.

4. **Automation Tools**  
   On modern systems, tools like **CMake** or **Makefiles** automate compilation and linking. They manage dependencies and build steps for you.

Since DOSBox emulates an older environment, we won’t have the luxury of using CMake. Instead, we’ll rely on a simple script that compiles and runs C files inside DOSBox. (See the provided video for installation and usage.)

---

## Assignment 1

### Goals:
- To practice `C` Programming
- To be familiar with inline Assembly programming within C
- To enforce understanding of doing I/O with and without operating system support.


A user is supposed to request all required I/O operations from the operating system. However, in some cases 
, a user may have to or wish to avoid using the operating system to perform I/O operations. In such cases, 
the user may try to talk to the hardware directly or easier, to use the BIOS services.  In this assignment, we will 
practice using the BIOS to do some simple I/O operations.

> Why would a user avoid using operating system to perform I/O operations?


### Installation

See the video and follow the instructions.
> Why do we need DosBox?

### First Compilation

Try out simple program to make sure it works.

```c
void main() {
    printf("%s", "Hello World");
}
```
> What are the steps for compiling such a program?

### Task 1
Write a `C` program to display a simple message on the screen.
You may use the C function `puts (char *)`.
Then try to use the BIOS interrupt `10H` to do the same thing.
In particular, we will write our own function, `biosputs(char *)`, that will display a string on the screen.
Below is one way to write this function using inline Assembly from within a C program.

```c
void biosputs (char * str)
{
    int i=0; char holdc;
    while (str[i]) {
        holdc= str[i++];
        _asm {
            mov ah, 0Eh
            mov al, holdc
            mov bx, 7
            int 10h
        }   // end asm
    } // end while
} // end function
```

Write a C program to test the above function.
**Notes:**
    - Write `_asm {` in the same line or you will get an error.
    - All variables **MUST** be initialized at the beginning of the function.

> Explain the role of each line of the assembly code.


### Task 2

Write a C program to read a string from the keyboard then display it again on the screen.
At first, you may use the `C` function `gets (char *)` to get the input from the keyboard.
Then write your own `biosgets (char *)` function that uses the BIOS to do the same task.
Write a C program to test your function.  

> Is it possible to do the same for modern x86 CPU without DosBox? what will be the steps?
