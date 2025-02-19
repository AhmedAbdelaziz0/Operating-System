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

////////////////////////////////////////////
void main() {
  welcome();
  while (1) {
    char str[10];
    biosgets(str);

    // Write your code here
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
			mov bx, 7
			int 10h
    }
  }
}
/////////////////////////////////////////////
void biosgets(char *str) {
  // Write your code here
}
/////////////////////////////////////////////
void welcome() {
  // Write your code here
}
/////////////////////////////////////////////
void help() {
  // Write your code here
}
/////////////////////////////////////////////
void cls() {
  // Write your code here
}
/////////////////////////////////////////////
void mem() {
  // Write your code here
}
/////////////////////////////////////////////
void quit() {
  // Write your code here
}
/////////////////////////////////////////////
void exit() {
  // Write your code here
}
/////////////////////////////////////////////
void reboot() {
  // Write your code here
}
/////////////////////////////////////////////
int Strcmp(char *src, char *dst) {
  // Write your code here
}
//////////////////////////////////////////////
