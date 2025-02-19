void biosgets(char *str);
void biosputs(char *str);

int main() {
  char str_puts[30] = "puts works";
  char str_biosputs[30] = "biosputs works";
  char str_gets[30];

  gets(str_gets);
  puts(str_gets);
  biosgets(str_gets);
  biosputs(str_gets);
  return 0;
}

//////////////////////////////
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
    } // end asm

  } // end while

  _asm {
			mov ah, 0Eh
			mov al, 10 // newline
			mov bx, 7
			int 10h
			mov al, 13 // carriage return
			int 10h
  } // end asm
}
///////////////////////////////////
void biosgets(char *str) {
  // Write your code here
}
//////////////////////////////
