; #############################
; APEX OS Stage One Bootloader
; @author John Wargo
; #############################

; @func [[noreturn]] void kernel_main()
; The main function of the kernel -- returning hangs the computer
extern kernel_main

; #################
; Multiboot Header
; #################
MB_ALIGN equ 0x01               ; Flag to align modules on pages
MB_MMAP  equ 0x02               ; Flag to provide a memory map
MB_FLAGS equ MB_ALIGN | MB_MMAP ; Final multiboot flags field
MB_MAGIC equ 0x1badb002         ; Multiboot checksum
MB_CHKSUM equ -(MB_MAGIC + MB_FLAGS)    ; Checksum

section .multiboot
align 4
  dd MB_MAGIC
  dd MB_FLAGS
  dd MB_CHKSUM

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
global _boot
_boot:
  mov esp, stack_top    ; Setup the stack
  call kernel_main      ; Invoke kernel
  cli                   ; Clear interrupts to prevent crashes
.hang:
  hlt                   ; Stop the processor!
  jmp .hang             ; Stop the processor again...
.end:
