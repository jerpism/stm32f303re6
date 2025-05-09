.syntax unified
.cpu cortex-m4
.thumb

.global SVCall_Handler
.type SVCall_Handler,%function

.equ N_SYSCALL, 3

SVCall_Handler:
    cpsid if    // Disable interrupts

    /* See if we have a valid syscall # */
    cmp     r7, #0
    blt     .end
    cmp     r7, N_SYSCALL
    bgt     .end

    /* We do. Check which SP was in use */
    tst     lr, #4                  // check bit #4
    ite     eq                      // was 1?
    mrseq   r0, msp                 // MSP was in use
    mrsne   r0, psp                 // Otherwise PSP (should usually be this)

    push    {r0, lr}                // save SP and EXC_RETURN
    ldm     r0,{r0-r3}              // Load arguments from stack frame

    /* Calculate syscall jump */
    ldr     r12, table_addr
    ldr     r12, [r12, +r7, lsl #2] // r7 has syscall #
    bic     r12, #0x1               // clear bit 0 so we don't fault
    mov     pc, r12                 // go run syscall

svc_end:
    pop     {r12, lr}               // restore SP and EXC_RETURN
    stm     r12, {r0}               // store return value on stack

    .end:
    cpsie if    // Enable interrupts
    bx lr

call0:  bl syscall0
        b svc_end

call1: bl syscall1

b svc_end



.align 4
syscall_table:
    .word call0
    .word call1
.align 4
table_addr: .word syscall_table
