void biosgets(char *str); // get a string from the user
void biosputs(char *str); // print a string to the screen
void welcome();           // clear the screen and print the welcome message
void help();              // print the help menu
void cls();               // clear the screen
void mem();    // print the size of the conventional memory of the IBM PC
void quit();   // reboot the machine
void exit();   // reboot the machine
void reboot(); // reboot the machine
int Strcmp(char *src, char *dst); // utility function to compare strings

// // struct to hold the commands
// struct command {
//   char *name;
//   void (*func)();
// };
// 
// // array of commands
// struct command commands[] = {
//     {"help", help}, {"cls", cls},   {"mem", mem},
//     {"quit", quit}, {"exit", exit}, {"reboot", reboot},
// };

////////////////////////////////////////////
void main() {
  char prompt[] = ">> ";
  // int i = 0, number_of_commands = sizeof(commands) / sizeof(commands[0]),
  //     command_found = 0;
  welcome();
  while (1) {
    char command[10];

    biosputs(prompt);
    biosgets(command);
    
    if (Strcmp(command, "help"))
      help();
    else if (Strcmp(command, "cls"))
      cls();
    else if (Strcmp(command, "mem"))
      mem();
    else if (Strcmp(command, "quit"))
      quit();
    else if (Strcmp(command, "exit"))
      exit();
    else if (Strcmp(command, "reboot"))
      reboot();
    else {
      char msg[] = "Unknown command\n\r";
      biosputs(msg);
    }

    // for (i = 0; i < number_of_commands; i++) { // 6 commands
    //   if (Strcmp(command, commands[i].name) == 1) {
    //     commands[i].func();
    //     command_found = 1;
    //     break;
    //   }
    // }
    // if (!command_found) {
    //   char msg[] = "Unknown command\n\r";
    //   biosputs(msg);
    // }
  }
}
/////////////////////////////////////////////
void biosputs(char *str) {
  int i = 0;
  char holdc;
  while (str[i]) {
    holdc = str[i++];
    _asm {
			mov ah, 0Eh
			mov al, holdc
			int 10h
    }
  }
}
/////////////////////////////////////////////
void biosgets(char *str) {
  int i, buffer_size = 10;
  char holdc, stop = '\r';

  for (i = 0; i < buffer_size; i++) {
    _asm {
      mov ah ,1
      int 21h
      mov holdc ,al
    }
    if (holdc == stop)
      break;
    str[i] = holdc;
  }
  str[i] = '\0';
}
/////////////////////////////////////////////
void welcome() {
  char msg[] = "Welcome to tiny os\n\r";
  biosputs(msg);
}
/////////////////////////////////////////////
void help() {
  char msg[] = "help - print out this list\n\r"
               "cls - clear the screen\n\r"
               "mem - print the size of memory the computer has\n\r"
               "quit, reboot, exit - reboot the machine\n\r";
  biosputs(msg);
}
/////////////////////////////////////////////
void cls() {
  _asm {
    mov ah, 0
    mov al, 3
    int 10h
  }
}
/////////////////////////////////////////////
void mem() {
  unsigned int mem_size, str_size = 0, i;
  char mem_size_str[6];
  char mem_size_reverse[6];
  _asm {
    int 12h
    mov mem_size, ax
  }

  // convert mem_size to string
  while (mem_size > 0) {
    mem_size_str[str_size++] = mem_size % 10 + '0';
    mem_size /= 10;
  }

  // reverse the string
  for (i = 0; i < str_size; i++) {
    mem_size_reverse[i] = mem_size_str[str_size - i - 1];
  }
  mem_size_reverse[str_size] = '\0';

  biosputs(mem_size_reverse);
  biosputs(" KB\n\r");
}
/////////////////////////////////////////////
void quit() { reboot(); }
/////////////////////////////////////////////
void exit() { 
  reboot();
 }
/////////////////////////////////////////////
void reboot() {
  _asm {
    int 19h
  }
}
/////////////////////////////////////////////
int Strcmp(char *src, char *dst) {
  int index = 0;

  while (src[index] == dst[index] && dst[index] != '\0') {
    index++;
  }

  if (src[index] != dst[index]) {
    return 0;
  }

  return 1;
}
//////////////////////////////////////////////
