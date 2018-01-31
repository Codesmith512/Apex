; ##################################
; Low-level assembly port functions
; ##################################
[bits 32]
section .text

; @function void out_8(uint16_t port, uint8_t b)
; Writes the byte to the given port
global out_8
out_8:
  ; standard frame setup
  push ebp
  mov ebp, esp

  ; al is used for the byte to write
  mov eax, [esp + 12]

  ; dx is used for the port
  push edx
  mov edx, [esp+12]

  ; Perform write
  out dx, al

  ; Restore registers
  pop edx

  ; Restore stack frame and return
  mov esp, ebp
  pop ebp
  ret

; @function void out_16(uint16_t port, uint8_t b)
; Writes the word to the given port
global out_16
out_16:
  ; standard frame setup
  push ebp
  mov ebp, esp

  ; al is used for the byte to write
  mov eax, [esp + 12]

  ; dx is used for the port
  push edx
  mov edx, [esp+12]

  ; Perform write
  out dx, ax

  ; Restore registers
  pop edx

  ; Restore stack frame and return
  mov esp, ebp
  pop ebp
  ret


; @function void out_32(uint16_t port, uint8_t b)
; Writes the dword to the given port
global out_32
out_32:
  ; standard frame setup
  push ebp
  mov ebp, esp

  ; al is used for the byte to write
  mov eax, [esp + 12]

  ; dx is used for the port
  push edx
  mov edx, [esp+12]

  ; Perform write
  out dx, eax

  ; Restore registers
  pop edx

  ; Restore stack frame and return
  mov esp, ebp
  pop ebp
  ret

; @function uint8_t in_8(uint16_t port)
; Returns the byte from the given port
global in_8
in_8:
  ; standard frame setup
  push ebp
  mov ebp, esp

  ; dx is used for the port
  push edx
  mov edx, [esp+12]

  ; Perform read
  in dx, al

  ; Restore registers
  pop edx

  ; Restore stack frame and return
  mov esp, ebp
  pop ebp
  ret

; @function uint16_t in_16(uint16_t port)
; Returns the byte from the given port
global in_16
in_16:
  ; standard frame setup
  push ebp
  mov ebp, esp

  ; dx is used for the port
  push edx
  mov edx, [esp+12]

  ; Perform read
  in dx, ax

  ; Restore registers
  pop edx

  ; Restore stack frame and return
  mov esp, ebp
  pop ebp
  ret

; @function uint32_t in_32(uint16_t port)
; Returns the byte from the given port
global in_32
in_32:
  ; standard frame setup
  push ebp
  mov ebp, esp

  ; dx is used for the port
  push edx
  mov edx, [esp+12]

  ; Perform read
  in dx, eax

  ; Restore registers
  pop edx

  ; Restore stack frame and return
  mov esp, ebp
  pop ebp
  ret

; @func void io_wait()
; Performs an IO NOP
global io_wait
io_wait:
  ; Setup stack frame
  push ebp
  mov ebp, esp

  ; Write NOP
  ; note -- port 0x80 is only used for the BIOS, so it's not needed anymore
  out 0x80, 0

  ; Restore stack frame
  mov esp, ebp
  pop ebp
  ret
  