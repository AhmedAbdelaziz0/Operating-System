
cseg SEGMENT ;PUBLIC USE16 
assume CS:cseg, DS:cseg 
org 0 

jmp AfterData

;Put any data here! 
msg     db  'Hello Cyberspace!'
	  db 0

AfterData: 

push cs
pop ds
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
		 retf
            
reboot:
             

;hang: 	jmp hang ; Hang out... 

cseg ENDS 
end