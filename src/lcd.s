.section .text,"ax",@progbits

; llvm calling convention seems to be:
; arg fits in single byte: -> A reg
; arg is 16 bit pointer:   -> __RC2/__RC3 (spans both registers)
; arg is 16 bit (not pointer) -> A, low byte, X. high byte
; LLVM seems to use __rc0 and __rc1 as scratch for the current operation     

.globl long_delay
long_delay:
    ldy #$FF
outer:
    ldx #$FF
inner:
    dex
    bne inner
    dey
    bne outer
    rts

.globl put_c
put_c:
    sta $6000
    lda #%00100000
    sta $6001
    lda #%10100000
    sta $6001
    ldx #$FF
1:  dex
    bne 1b
    lda #%00100000
    sta $6001
    rts

.globl print_str
print_str:
    ; pointer already arrives in __rc2/__rc3
next_char:
    ldy #0
    lda (__rc2),y   ; dereference pointer
    beq done        ; null terminator, exit
    jsr put_c       ; A already has the character, put_c expects A
    inc __rc2       ; increment pointer low byte
    bne next_char
    inc __rc3       ; carry into high byte
    bne next_char
done:
    rts

.globl print_hex_byte
print_hex_byte:
    pha
    lsr a
    lsr a
    lsr a
    lsr a
    jsr print_nibble
    pla
    and #$0f
print_nibble:
    cmp #10
    bcc .is_digit
    adc #6          ; Convert to 'A'-'F' (6502 carry math)
.is_digit:
    adc #48         ; Convert to '0'-'9'
    jsr put_c
    rts

.globl print_page
print_page:
    sta __rc2       ; Save Low Byte [cite: 16]
    stx __rc3       ; Save High Byte [cite: 17]
    

    ; 1. Print Address Label (e.g., 0200)
    txa             
    jsr print_hex_byte
    lda __rc2       
    jsr print_hex_byte
    
    lda #' '        ; Space after address
    jsr put_c

    ; 2. Print 4 Data Bytes
    ldy #0          
next_byte:
    tya             
    pha             ; Save Y on stack [cite: 31, 32]
    
    lda (__rc2),y   ; Read actual memory [cite: 18, 27]
    jsr print_hex_byte
    
    lda #' '        
    jsr put_c
    
    pla             ; Restore Y [cite: 31, 32]
    tay             
    iny             
    cpy #4          
    bne next_byte   
    rts