;--------------------
; Asembly Code
;--------------------

.global RotateASM             ;declare global variables so it can be accessed from INO file
.global RotateASM0Offset
.global RotateASM8Offset
.global RotateASM16Offset
.global RotateASM24Offset




;--------------------


RotateASM:
  ; (x % 32) / 8
  mov r21, r20    ; MOV(Move) copies 8-bits

  andi  r20, 0b00111  ; ANDI(Logical AND immediate) takes lowest 3 bits (sets n)
  ldi r18, 8          ; LDI(Load immediate) sets 8
  sub r18, r20        ; SUB(Subtract) (8 - n) sets otherN

  andi  r21, 0b11000  ; ANDI(Logical AND immediate) takes 2 bits

  cpi r20, 0             ; CPI(Compare immediate)
  breq RotateASMByteOffset    ; BRNE(Branch if equal)

  cpi r21, 0b00000  ; 0  ; CPI(Compare immediate)
  breq  GoToRotateASM0Offset ; BRNE(Branch if equal)

  cpi r21, 0b01000  ; 1  ; CPI(Compare immediate)
  breq  GoToRotateASM8Offset ; BRNE(Branch if equal)

  cpi r21, 0b10000  ; 2  ; CPI(Compare immediate)
  breq  GoToRotateASM16Offset ; BRNE(Branch if equal)

  cpi r21, 0b11000  ; 3  ; CPI(Compare immediate)
  breq  GoToRotateASM24Offset ; BRNE(Branch if equal)

  ret

GoToRotateASM0Offset:  ; relative branch is too far away, need to be JMP(Jump)
  jmp RotateASM0Offset

GoToRotateASM8Offset:  ; relative branch is too far away, need to be JMP(Jump)
  jmp RotateASM8Offset

GoToRotateASM16Offset:  ; relative branch is too far away, need to be JMP(Jump)
  jmp RotateASM16Offset

GoToRotateASM24Offset:  ; relative branch is too far away, need to be JMP(Jump)
  jmp RotateASM24Offset



  ;--------------------




RotateASMByteOffset:
  cpi r21, 0b01000  ; 1  ; CPI(Compare immediate)
  breq  RotateASM1ByteOffset ; BRNE(Branch if equal)

  cpi r21, 0b10000  ; 2  ; CPI(Compare immediate)
  breq  RotateASM2ByteOffset ; BRNE(Branch if equal)

  cpi r21, 0b11000  ; 3  ; CPI(Compare immediate)
  breq  RotateASM3ByteOffset ; BRNE(Branch if equal)

  ret
  


;--------------------



RotateASM1ByteOffset:
  mov r18, r22

  mov r22, r23
  mov r23, r24
  mov r24, r25

  mov r25, r18

  ret

;-----

RotateASM2ByteOffset:
  movw r18, r22

  movw r22, r24
  
  movw r24, r18

  ret

;-----

RotateASM3ByteOffset:
  mov r18, r25

  mov r25, r24
  mov r24, r23
  mov r23, r22

  mov r22, r18

  ret




;--------------------




RotateASM0Offset:
  push r29        ; pushes registers that are gonna be used

  mov r29, r22;   ; MOV(Move) copies 8-bit from r22 to r29

  mov r21, r20    ; sets n

RotateASM0OffsetRightLoop:
  lsr	r25				  ; LSR(Logical Shift Right)
  ror	r24				  ; ROR(Shift Rigth with Carry)
  ror	r23				  ; ROR(Shift Rigth with Carry)
  ror	r22				  ; ROR(Shift Rigth with Carry)

  dec	r21				  ; DEC(Decrement) r21 (n)
  brne RotateASM0OffsetRightLoop     	; BRNE(Branch if not equal zero) if is not zero
  mov r21, r18    ; sets otherN

RotateASM0OffsetLeftLoop:
  lsl	r29     		; LSR(Logical Shift Left)

  dec r21       ; DEC(Decrement) r21 (otherN)
  brne RotateASM0OffsetLeftLoop       ; BRNE(Branch if not equal zero) if is not zero

  or	r25, r29		; OR(OR) and save to output

  pop r29
  ret





;--------------------






RotateASM8Offset:
  push r28
  push r29        ; pushes registers that are gonna be used

  movw	r28, r22	; MOVW(Move Word) copies 16-bit word from r22:r23 to r28:r29

  mov r21, r20    ; sets n

  mov r22, r23
  mov r23, r24
  mov r24, r25

RotateASM8OffsetRightLoop:
  lsr	r24         ; LSR(Logical Shift Right)
  ror	r23				  ; ROR(Shift Rigth with Carry)
  ror	r22				  ; ROR(Shift Rigth with Carry)

  dec	r21				  ; DEC(Decrement) r21 (n)
  brne RotateASM8OffsetRightLoop     	; BRNE(Branch if not equal zero) if is not zero
  mov r21, r18    ; sets otherN

RotateASM8OffsetLeftLoop:
  lsl	r28     		; LSL(Logical Shift Left)
  rol	r29     		; ROL(Shift left with Carry)

  dec r21       ; DEC(Decrement) r21 (otherN)
  brne RotateASM8OffsetLeftLoop       ; BRNE(Branch if not equal zero) if is not zero


  or	r24, r28		; OR(OR) and save to output
  mov r25, r29

  pop r29
  pop r28
  ret





;--------------------




RotateASM16Offset:
  push r26
  push r27        ; pushes registers that are gonna be used

  movw r26, r24   ; MOVW(Move Word) copies 16-bit from r24:r25 to r26:r27

  mov r21, r20    ; sets n

RotateASM16OffsetRightLoop:
  lsr	r27				  ; LSR(Logical Shift Right)
  ror	r26				  ; ROR(Shift Rigth with Carry)

  dec	r21				  ; DEC(Decrement) r21 (n)
  brne RotateASM16OffsetRightLoop     	; BRNE(Branch if not equal zero) if is not zero
  mov r21, r18    ; sets otherN

  mov r25, r24
  mov r24, r23
  mov r23, r22

RotateASM16OffsetLeftLoop:
  lsl	r23     		; LSL(Logical Shift Left)
  rol	r24     		; ROL(Shift left with Carry)
  rol	r25     		; ROL(Shift left with Carry)

  dec r21         ; DEC(Decrement) r21 (otherN)
  brne RotateASM16OffsetLeftLoop       ; BRNE(Branch if not equal zero) if is not zero

  mov r22, r26    ; MOV(Move) copies 8-bit from r26 to r22
  or	r23, r27		; OR(OR) and save to output

  pop r27
  pop r26
  ret




;--------------------




RotateASM24Offset:
  push r26        ; pushes registers that are gonna be used

  mov r26, r25    ; MOV(Move) copies 8-bit from r22 to r27

  mov r21, r20    ; MOV(MOVE) sets N value

RotateASM24OffsetRightLoop:
  lsr r26         ; LSR(Logical Shift Right)

  dec r21
  brne RotateASM24OffsetRightLoop     	; BRNE(Branch if not equal zero) if is not zero
  mov r21, r18    ; MOV(MOVE) sets otherN value

RotateASM24OffsetLeftLoop:
  lsl r22         ; LSL(Logical Shift Left)
  rol	r23     		; ROL(Shift left with Carry) 
  rol	r24     		; ROL(Shift left with Carry) 
  rol	r25     		; ROL(Shift left with Carry) 

  dec r21
  brne RotateASM24OffsetLeftLoop     	  ; BRNE(Branch if not equal zero) if is not zero

  or r22, r26

  pop r26
  ret

;--------------------






