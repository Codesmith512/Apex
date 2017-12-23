; #############################
; APEX OS Stage One Bootloader
; #############################

; @func [[noreturn]] void kernel_main()
; The main function of the kernel -- returning hangs the computer
extern kernel_main

; #################
; Multiboot Header
; https://www.gnu.org/software/grub/manual/multiboot2/multiboot.html#dir
; #################

MB_MAGIC equ 0xe85250d6             ; Magic number
MB_ARCH equ  0x00000000             ; Defines 32b i386 architecture
MB_SIZE equ (header.end - header)   ; Size of the header
MB_CHKSUM equ -(MB_MAGIC + MB_ARCH + MB_SIZE)

section .multiboot
header:
; Standard header 
  dd MB_MAGIC
  dd MB_ARCH
  dd MB_SIZE
  dd MB_CHKSUM

; Null tag
  dd 0
  dd 0
  dd 8
.end:

; ####################################
; Allocate memory for the 16KiB stack
; ####################################

section .bss
align 16
stack_bottom:
resb 16384
stack_top:

; ##################################
; Actual executable code goes here!
; ##################################
section .text

; Define a flat GDT that spans the whole 4GiB space
GDT:
  dw .table_end - .table_start - 1
  dd .table_start
.table_start:
  ; 0x00 Null selector -- unusable
  dq 0
  ; 0x08 Code selector
  dw 0xffff     ;  0:15b Limit
  dw 0x0000     ;  0:15b Base
  db 0x00       ; 16:23b Base
  db 10011010b  ;  0: 7b Access Flags
  db 0xcf       ; 16:19b Limit
                ;  0: 4b Descriptor Flags
  db 0x00       ; 24:31b Base
  ; 0x10 Data selector
  dw 0xffff     ;  0:15b Limit
  dw 0x0000     ;  0:15b Base
  db 0x00       ; 16:23b Base
  db 10010010b  ;  0: 7b Access Flags
  db 0xcf       ; 16:19b Limit
                ;  0: 4b Descriptor Flags
  db 0x00       ; 24:31b Base
.table_end:

; @func [[noreturn]] void _boot(void)
; The initial entry point -- returning is UB
global _boot
_boot:
  mov esp, stack_top    ; Setup the stack
  call loadGDT          ; Setup the flat GDT and registers
  push ebx              ; Push the pointer to multiboot2 info
  call kernel_main      ; Invoke kernel
  cli                   ; Clear interrupts to prevent crashes
.hang:
  hlt                   ; Stop the processor!
  jmp .hang             ; Stop the processor again...
.end:

; @func void loadGDT(void)
; Loads the GDT and sets the new segments accordingly
loadGDT:
  lgdt [GDT]
  jmp 0x0008:.reload_segments
  .reload_segments
  mov ax, 0x10
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax
  ret