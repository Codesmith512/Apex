; #########################
; APEX OS Assembly Helpers
; #########################
[bits 32]

; @func void __asm_debug()
; Creates an infinite loop, until cx is set to 0, sets the pixel at 0,0 to blue
global __asm_debug
__asm_debug:
  ; Setup stack frame
  push ebp
  mov ebp, esp

  ; Preserve CX
  push cx

  ; Indicate debug status with 0,0 being blue
  mov cl, [0xb8001]
  push cx
  mov cl, 0x91
  mov [0xb8001], cl

  ; Loop until cx == 0
  mov cx, 0
.loop:
  cmp cx, 0
  je .loop

  ; Restore formatting
  pop cx
  mov [0xb8001], cl

  ; Restore cx
  pop cx

  ; Restore stack frame
  mov esp, ebp
  pop ebp
  ret
.end:

; @func void __asm_break()
; Creates an infinite loop, sets the pixel at 0,0 to red
global __asm_break
__asm_break:
  ; Setup stack frame
  push ebp
  mov ebp, esp

  ; Preserve CX
  push cx

  ; Indicate debug status with 0,0 being red
  mov cl, [0xb8001]
  push cx
  mov cl, 0xc4
  mov [0xb8001], cl

  ; Loop forever
  jmp $

  ; Restore formatting
  pop cx
  mov [0xb8001], cl

  ; Restore cx
  pop cx

  ; Restore stack frame
  mov esp, ebp
  pop ebp
  ret
.end: