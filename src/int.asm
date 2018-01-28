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
global enable_int
enable_int:
  sti
  ret

; @func void disable_int()
; Disables interrupts
global disable_int
disable_int:
  cli
  ret

; @func void __asm_debug()
; The standard debug function (from libapex)
extern __asm_debug

; @func int int_debug()
; A dummy interrupt for use on 0x3
global int_debug
int_debug:
  call __asm_debug
  iret

; @func void int_0x03()
; Invokes interrupt 0x03
global int_0x03
int_0x03:
  int 0x03
  ret