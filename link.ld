ENTRY(_reset);
MEMORY {
  flash(rx)  : ORIGIN = 0x08000000, LENGTH = 512K
  sram(rwx) : ORIGIN = 0x20000000, LENGTH = 64K  /* remaining 16k in a separate address space */
}
_estack     = ORIGIN(sram) + LENGTH(sram);    /* stack points to end of SRAM */

SECTIONS {
  .vectors  : { KEEP(*(.vectors)) }   > flash
  .text     : { *(.text*) }           > flash
  .rodata   : { *(.rodata*) }         > flash

  .data : {
    _sdata = .;   /* .data section start */
    *(.first_data)
    *(.data SORT(.data.*))
    _edata = .;  /* .data section end */
  } > sram AT > flash
  _sidata = LOADADDR(.data);

  .bss : {
    _sbss = .;              /* .bss section start */
    *(.bss SORT(.bss.*) COMMON)
    _ebss = .;              /* .bss section end */
  } > sram

  . = ALIGN(8);
  _end = .;     /* for cmsis_gcc.h  */

  .heap : {
      . = ALIGN(4); 
      _heap_start = .;
      /* . = + (LENGTH(SRAM) - _stackSize - (. - ORIGIN(SRAM))); */
      . = . + 16K;
      . = ALIGN(4);
      _heap_end  = .; 
 } > sram
}
