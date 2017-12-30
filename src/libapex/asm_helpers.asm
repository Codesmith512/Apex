; #########################
; APEX OS Assembly Helpers
; #########################
[bits 32]

; @func void __debugbreak()
; Creates an infinite loop, until cx is set to 0
global __asm_debugbreak
__asm_debugbreak:
  ; Setup stack frame
  push ebp
  mov ebp, esp

  ; Preserve CX
  push cx

  ; Indicate debug status with 0,0 being solid blue
  mov cl, [0xb8001]
  push cx
  mov cl, 0x99
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