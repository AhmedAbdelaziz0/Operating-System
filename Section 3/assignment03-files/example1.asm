
cseg SEGMENT ;PUBLIC USE16 
assume CS:cseg, DS:cseg 
org 0 

jmp AfterData

;Put any data here! 
somedata db 60 dup(?)
msg      db  'Hello Cyberspace!'
	 db 0

AfterData: 

mov ax, 07C0H
mov ds, ax 


 		mov si, offset msg     ; Print msg
    print:
            lodsb           ; AL=memory contents at DS:SI

            cmp al, 0       ; If AL=0 then hang
            je getkey

            mov ah, 0Eh     ; Print AL
            mov bx, 7
            int 10h

            jmp print       ; Print next character

 getkey:
             mov ah, 0               ; wait for key
             int 016h
            
reboot:
             db 0EAh                 ; machine language to jump to FFFF:0000 (reboot)
             dw 0000h
             dw 0FFFFh

;hang: 	jmp hang ; Hang out... 

		org 510 ; Make the file 512 bytes long 
	dw 0AA55h ; Add the boot signature 
cseg ENDS 
end