.segment "STARTUP"
.import __DATA_LOAD__, __DATA_RUN__, __DATA_SIZE__
.import __BSS_RUN__, __BSS_SIZE__
.importzp sp
.import __RAM_START__, __RAM_SIZE__
.import _main

init:
  ldx #$ff
  txs       ; initialise stack pointer to top of stack

  lda #$FF
  sta sp
  lda #$5F
  sta sp+1

  ldx #0

data_loop:
  lda __DATA_LOAD__,x
  sta __DATA_RUN__,x
  inx
  cpx __DATA_SIZE__
  bne data_loop

  ldx #0

bss_loop:
  lda #$0
  sta __BSS_RUN__,x
  inx
  cpx __BSS_SIZE__
  bne bss_loop

  jsr _main ; jump to c land

.segment "VECTORS"
  .word init
  .word init