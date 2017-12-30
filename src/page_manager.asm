; ###################################
; Assembly routines for page_manager
; ###################################
[bits 32]

; @func void __asm_enable_paging(page_directory*)
; Enables paging with the given directory
global __asm_enable_paging
__asm_enable_paging:
  ; Setup new stack frame
  push ebp
  mov ebp, esp

  ; Load page directory
  mov eax, [ebp+8]
  mov cr3, eax

  ; Enable PSE
  mov eax, cr4
  or eax, 0x00000010
  mov cr4, eax

  ; Enable paging
  mov eax, cr0
  or eax, 0x80000001
  mov cr0, eax

  ; Restore stack frame
  mov esp, ebp
  pop ebp
  ret
.end:

; @func void __asm_disable_paging()
; Disables paging entirely
global __asm_disable_paging
__asm_disable_paging:
  ; Setup new stack frame
  push ebp
  mov ebp, esp

  ; Disable paging
  mov eax, cr0
  and eax, 0x7ffffffe
  mov cr0, eax

  ; Restore the stack frame
  mov esp, ebp
  pop ebp
  ret
.end: