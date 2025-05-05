.syntax unified
.cpu cortex-m4
.thumb

.global SVCall_Handler
.type SVCall_Handler,%function

SVCall_Handler:
    tst r0, r0      /* Did we get 0 in r0? */
    beq .disable     /* Yes, disable interrupts */

    mov r0, #0x0    /* Otherwise enable them */
    msr basepri, r0 
    isb

    b   .end

.disable:
    mov r0, #0x4
    msr basepri_max, r0
    isb

.end:
    bx lr


