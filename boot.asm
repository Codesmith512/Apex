; #############################
; APEX OS Stage One Bootloader
; @author John Wargo
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
global _boot
_boot:
  mov esp, stack_top    ; Setup the stack
  call kernel_main      ; Invoke kernel
  cli                   ; Clear interrupts to prevent crashes
.hang:
  hlt                   ; Stop the processor!
  jmp .hang             ; Stop the processor again...
.end:
