cseg SEGMENT ;PUBLIC USE16 
assume CS:cseg, DS:cseg, ES:cseg 

          ORG 0
          jmp     START
     
     OEM_ID                db "QUASI-OS"
     BytesPerSector        dw 0200h
     SectorsPerCluster     db 01h
     ReservedSectors       dw 0001h
     TotalFATs             db 02h
     MaxRootEntries        dw 00E0h
     TotalSectorsSmall     dw 0B40h
     MediaDescriptor       db 0F0h
     SectorsPerFAT         dw 0009h
     SectorsPerTrack       dw 0012h
     NumHeads              dw 0002h
     HiddenSectors         dd 00000000h
     TotalSectorsLarge     dd 00000000h
     DriveNumber           db 00h
     Flags                 db 00h
     Signature             db 29h
     VolumeID              dd 0FFFFFFFFh
     VolumeLabel           db "QUASI  BOOT"
     SystemID              db "FAT12   "
     
     START:
     ; code located at 0000:7C00, adjust segment registers
          cli
          mov     ax, 07C0h
          mov     ds, ax
          mov     es, ax
          
     ; create stack
          mov     ax, 0000h
          mov     ss, ax
          mov     sp, 0FFFFh
          sti
     ; post message
          mov     si, offset msgLoading
          call    DisplayMessage
     LOAD_ROOT:
     ; compute size of root directory and store in `cx'
          xor     cx, cx
          xor     dx, dx
          mov     ax, 0020h                          ; 32 byte directory entry
          mul     WORD PTR [MaxRootEntries]               ; total size of directory
          div     WORD PTR [BytesPerSector]               ; sectors used by directory
          xchg    ax, cx
     ; compute location of root directory and store in `ax'
          mov     al, BYTE PTR [TotalFATs]                ; number of FATs
          mul     WORD PTR [SectorsPerFAT]                ; sectors used by FATs
          add     ax, WORD PTR [ReservedSectors]          ; adjust for bootsector
          mov     WORD PTR [datasector], ax               ; base of root directory
          add     WORD PTR [datasector], cx
     ; read root directory into memory (7C00:0200)
          mov     bx, 0200h                          ; copy root dir above bootcode
          call    ReadSectors
     ; browse root directory for binary image
          mov     cx, WORD PTR [MaxRootEntries]           ; load loop counter
          mov     di, 0200h                          ; locate first root entry
     xLOOP:
          push    cx
          mov     cx, 000Bh                          ; eleven character name
          mov     si, offset ImageName                       ; image name to find
          push    di
     rep  cmpsb                                       ; test for entry match
          pop     di
          je      LOAD_FAT
          pop     cx
          add     di, 0020h                          ; queue next directory entry
          loop    xLOOP
          jmp     FAILURE
     LOAD_FAT:
     ; save starting cluster of boot image
          mov     si, offset msgCRLF
          call    DisplayMessage
          mov     dx, WORD PTR [di + 001Ah]
          mov     WORD PTR [cluster], dx                  ; file's first cluster
     ; compute size of FAT and store in `cx'
          xor     ax, ax
          mov     al, BYTE PTR [TotalFATs]                ; number of FATs
          mul     WORD PTR [SectorsPerFAT]                ; sectors used by FATs
          mov     cx, ax
     ; compute location of FAT and store in `ax'
          mov     ax, WORD PTR [ReservedSectors]          ; adjust for bootsector
     ; read FAT into memory (7C00:0200)
          mov     bx, 0200h                          ; copy FAT above bootcode
          call    ReadSectors
     ; read image file into memory (0050:0000)
          mov     si, offset msgCRLF
          call    DisplayMessage
          mov     ax, 0050h
          mov     es, ax                              ; destination for image
          mov     bx, 0000h                          ; destination for image
          push    bx
     LOAD_IMAGE:
          mov     ax, WORD PTR [cluster]                  ; cluster to read
          pop     bx                                  ; buffer to read into
          call    ClusterLBA                          ; convert cluster to LBA
          xor     cx, cx
          mov     cl, BYTE PTR [SectorsPerCluster]        ; sectors to read
          call    ReadSectors
          push    bx
     ; compute next cluster
          mov     ax, WORD PTR [cluster]                  ; identify current cluster
          mov     cx, ax                              ; copy current cluster
          mov     dx, ax                              ; copy current cluster
          shr     dx, 0001h                          ;
     ; divide by two
          add     cx, dx                              ; sum for (3/2)
          mov     bx, 0200h                          ; location of FAT in memory
          add     bx, cx                              ; index into FAT
          mov     dx, WORD PTR [bx]                       ; read two bytes from FAT
          test    ax, 0001h
          jnz     ODD_CLUSTER
     EVEN_CLUSTER:
          and     dx, 0000111111111111b               ; take low twelve bits
         jmp      xDONE
     ODD_CLUSTER:
          push    cx
          mov     cl, 0004h
          shr     dx, cl                          ; take high twelve bits
          pop     cx
     xDONE:
          mov     WORD PTR [cluster], dx                  ; store new cluster
          cmp     dx, 0FF0h                          ; test for end of file
          jb      LOAD_IMAGE
     DONE:
          mov     si, offset msgCRLF
          call    DisplayMessage
	    mov 	ax, 0050h
          push    ax
	    mov     ax, 0000h
          push    ax
          retf
     FAILURE:
          mov     si, offset msgFailure
          call    DisplayMessage
          mov     ah, 00h
          int     16h                                ; await keypress
          int     19h                                ; warm boot computer
     
     ;*************************************************************************
     ; PROCEDURE DisplayMessage
     ; display ASCIIZ string at ds:si via BIOS
     ;*************************************************************************
     DisplayMessage:
          lodsb                                       ; load next character
          or      al, al                              ; test for NUL character
          jz      yDONE
          mov     ah, 0Eh                            ; BIOS teletype
          mov     bh, 00h                            ; display page 0
          mov     bl, 07h                            ; text attribute
          int     10h                                ; invoke BIOS
          jmp     DisplayMessage
     yDONE:
          ret
     
     ;*************************************************************************
     ; PROCEDURE ReadSectors
     ; reads `cx' sectors from disk starting at `ax' into
     ; memory location `es:bx'
     ;*************************************************************************
     ReadSectors:
     yMAIN:
          mov     di, 0005h                          ; five retries for error
     ySECTORLOOP:
          push    ax
          push    bx
          push    cx
          call    LBACHS
          mov     ah, 02h                            ; BIOS read sector
          mov     al, 01h                            ; read one sector
          mov     ch, BYTE PTR [absoluteTrack]            ; track
          mov     cl, BYTE PTR [absoluteSector]           ; sector
          mov     dh, BYTE PTR [absoluteHead]             ; head
          mov     dl, BYTE PTR [DriveNumber]              ; drive
          int     13h                                ; invoke BIOS
          jnc     xSUCCESS                            ; test for read error
          xor     ax, ax                              ; BIOS reset disk
          int     13h                                ; invoke BIOS
          dec     di                                  ; decrement error counter
          pop     cx
          pop     bx
          pop     ax
          jnz     ySECTORLOOP                         ; attempt to read again
          int     18h
     xSUCCESS:
          mov     si, offset msgProgress
          call    DisplayMessage
          pop     cx
          pop     bx
          pop     ax
          add     bx, WORD PTR [BytesPerSector]           ; queue next buffer
          inc     ax                                  ; queue next sector
          loop    yMAIN                               ; read next sector
          ret
     
     ;*************************************************************************
     ; PROCEDURE ClusterLBA
     ; convert FAT cluster into LBA addressing scheme
     ; LBA = (cluster - 2) * sectors per cluster
     ;*************************************************************************
     ClusterLBA:
          sub     ax, 0002h                          ; zero base cluster number
          xor     cx, cx
          mov     cl, BYTE PTR [SectorsPerCluster]        ; convert byte to word
          mul     cx
          add     ax, WORD PTR [datasector]               ; base data sector
          ret
     
     ;*************************************************************************
     ; PROCEDURE LBACHS
     ; convert `ax' LBA addressing scheme to CHS addressing scheme
     ; absolute sector = (logical sector / sectors per track) + 1
     ; absolute head   = (logical sector / sectors per track) MOD number of heads
     ; absolute track  = logical sector / (sectors per track * number of heads)
     ;*************************************************************************
     LBACHS:
          xor     dx, dx                              ; prepare dx:ax for operation
          div     WORD PTR [SectorsPerTrack]              ; calculate
          inc     dl                                  ; adjust for sector 0
          mov     BYTE PTR [absoluteSector], dl
          xor     dx, dx                              ; prepare dx:ax for operation
          div     WORD PTR [NumHeads]                     ; calculate
          mov     BYTE PTR [absoluteHead], dl
          mov     BYTE PTR [absoluteTrack], al
          ret

     absoluteSector db 00h
     absoluteHead   db 00h
     absoluteTrack  db 00h
     
     datasector  dw 0000h
     cluster     dw 0000h
     ImageName   db "LOADER  BIN"
     msgLoading  db 0Dh, 0Ah, "Loading Boot Image ", 0Dh,0Ah, 00h
     msgCRLF     db 0Dh, 0Ah, 00h
     msgProgress db ".", 00h
     msgFailure  db 0Dh, 0Ah, "ERROR : Press Any Key to Reboot", 00h
     
          
     org 510 ; Make the file 512 bytes long 
     dw 0AA55h ; Add the boot signature 
cseg ENDS 
end
