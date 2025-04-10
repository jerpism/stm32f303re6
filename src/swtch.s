.syntax unified
.cpu cortex-m4

.global PendSV_Handler
.type PendSV_Handler,%function

PendSV_Handler:
    cpsid i 

    /* Save r4-r11 for current process */
    mrs     r0, psp

    subs    r0, #32
    stmia   r0, {r4-r11}


    /* SP will always be at task struct base address */
    /* Save current SP */
    ldr     r2, =curr_task
    ldr     r1, [r2]
    str     r0, [r1]

    /* Load next SP */
    ldr     r2, =next_task
    ldr     r1, [r2]
    ldr     r0, [r1]

    /* Load r4-r11 for next process */
    ldmia   r0!,{r4-r11}
    msr     psp, r0

    /* Return to thread mode with PSP */
    ldr     r0, =0xFFFFFFFD

    cpsie i

    bx  r0




    



