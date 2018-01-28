; ###########################
; APEX OS Interrupt Services
; ###########################
[bits 32]

; #########################
; IDT descriptor goes here
; #########################
section .bss
idtr:
.size:
resw  1; The size-1 of the IDT
.addr:
resd  1; The linear address of the IDT

; ####################
; Interrupt functions
; ####################
section .text

; @func void load_idt(void*)
; Loads a 256-entry IDT with the given address
global load_idt
load_idt:
  mov [idtr.size], word (8*256)-1
  push eax
  mov eax, [esp+8]
  mov [idtr.addr], eax
  pop eax
  lidt [idtr]
  ret

; @func void enable_int()
; Enables interrupts
global enable_hw_interrupts
enable_hw_interrupts:
  sti
  ret

; @func void disable_int()
; Disables interrupts
global disable_hw_interrupts
disable_hw_interrupts:
  cli
  ret

; @func void int_wrapper
; A function that can be used to generate
; interrupt wrapper code for C/C++ functions
global int_wrapper_f
int_wrapper_f:
  push .return
  jmp 0x8:0xdeadc0de
  .return:
  iret
  .end:

; @uint32_t int_wrapper_size
; The size of the int_wrapper function
global int_wrapper_s
int_wrapper_s: dd (int_wrapper_f.end - int_wrapper_f)

; @func void int_0x03()
; Invokes interrupt 0x03
global int_0x03
int_0x03:
  int 0x03
  ret