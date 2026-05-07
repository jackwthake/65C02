/* Linker script for Ben Eater's 6502 breadboard computer. */

/* Provide imaginary (zero page) registers. */
__rc0 = 0x00;
INCLUDE /home/jwt/llvm-mos/mos-platform/common/lib/imag-regs.ld
ASSERT(__rc31 == 0x1f, "Inconsistent zero page map.")

MEMORY {
  zp : ORIGIN = 0x20, LENGTH = 0xe0 - 0x20
  ram : ORIGIN = 0x200, LENGTH = 0x4000 - 0x200
  rom : ORIGIN = 0x8000, LENGTH = 0x7ffa
}

/* Provide default IRQ and NMI handlers if the program doesn't have them. */
PROVIDE(irq = irq);
PROVIDE(nmi = irq);

REGION_ALIAS("c_readonly", rom)
REGION_ALIAS("c_writeable", ram)

SECTIONS { INCLUDE c.ld }

/* Set initial soft stack address to just above last ram address. (It grows down.) */
__stack = ORIGIN(ram) + LENGTH(ram);

OUTPUT_FORMAT {
  FULL(rom)
  SHORT(nmi)
  SHORT(_start)
  SHORT(irq)
}
