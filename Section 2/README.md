# Section 2

## Assignment 2

The task here is to write a simple command interpreter (shell) (very tiny one indeed) using C.
The program should start by displaying a welcome message like 

```
Welcome to tiny-os!
Type help if you need it.
```

The program then displays a prompt like `>` and awaits a user command.
The (small) list of the supported commands is:

```
help - print out this list
cls - clear the screen
mem - print the size of memory the computer has
quit - reboot the machine
exit - same as quit
reboot - same as exit
```

If the user enters the “help” command, the previous list of supported commands is to be displayed.
If the user enters anything other than any of the supported commands, a message like `Unknown command` is displayed.
The program should do any I/O using BIOS.
For a reason that will be clear later on, try to make your main function as minimal as possible (i.e., just calling some other function).
An overall sketch of the program is given in the following pseudo-code:

```c
while (true) {
    print (prompt);
    getUserCommand(cmd);
    if (cmd = help) doHelp;
    else if (cmd = cls) doCls;
    else if (cmd = mem) doMEM;
    else if (cmd = quit or exit or reboot) doReboot;
    else print("Unknown command");
}
```

> You may find your code for Assignment 1 useful here.
