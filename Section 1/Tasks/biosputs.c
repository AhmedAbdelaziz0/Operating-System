void biosputs(char *str) {
  unsigned int i = 0;
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

void main() { 
  biosputs("Hello Wolrd!\n\r");
}
