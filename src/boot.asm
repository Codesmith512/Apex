; #############################
; APEX OS Stage One Bootloader
; #############################
[bits 32]

; @func void kernel_init()
; The initialization function for the kernel
extern kernel_init

; @func void kernel_main()
; The main function for the kernel
extern kernel_main

; @func void __asm_break()
; Performs a hard-break
extern __asm_break

; @func void __asm_debug()
; Performs a soft-break
extern __asm_debug

; @func _init
; GCC's initialization function
extern _init

; @func _fini
; GCC's cleanup function
extern _fini

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

section .bss

; ############################################
; Allocate memory for the 4KiB page directory
; ############################################
align 4096
__page_directory:
resb 4096

; Pad some space so the stack doesn't overlap the page directory
resb 0

; ####################################
; Allocate memory for the 16KiB stack
; ####################################
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
  cli                   ; No interrupts yet -- no IDT
  mov esp, stack_top    ; Setup the stack
  call loadGDT          ; Setup the flat GDT and registers
  push ebx              ; Push the pointer to multiboot2 info
  push __page_directory
  call kernel_init      ; Invoke kernel to setup paging+heap
  add esp, 8            ; Cleanup the stack

  call _init            ; Invoke GCC's initialization function

  call kernel_main      ; Invoke kernel_main
  cmp ax, 0x00          ; Hard break if main fails
  je .fini
  call __asm_break

  .fini:
  call _fini            ; Invoke GCC's cleanup function

  call __asm_debug      ; Soft break before hanging processor

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
  .reload_segments:
  mov ax, 0x10
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax
  ret