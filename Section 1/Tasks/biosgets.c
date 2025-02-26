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

void biosgets(char *str, int buffer_size, char stop) {
  unsigned int i;
  char holdc = 'a';

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

void main() { 
  const unsigned int buffer_size = 5;
  char* str = (char*)malloc(buffer_size * sizeof(char));
  biosgets(str, buffer_size, '\r');
  biosputs(str);
}
