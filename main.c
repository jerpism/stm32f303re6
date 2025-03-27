#include <gpio.h>
#include <stdint.h>
#include <rcc.h>
#include <usart.h>
#include <common.h>
#include <systick.h>

#define MAX_TASKS 16


// PendSV 13
// SysTick 14
#define NVIC_IPR0 0xE000E400
#define NVIC_IPR13 (*((volatile uint32_t*)(NVIC_IPR0 + (0x4 * 13))))
#define NVIC_IPR14 (*((volatile uint32_t*)(NVIC_IPR0 + (0x4 * 14))))

enum task_status {
    TASK_STATUS_IDLE = 0,
    TASK_STATUS_ACTIVE = 1
};

struct task{
    volatile uint32_t sp;
    void (*handler)(void);
    volatile enum task_status status;
};

static struct {
    struct task tasks[MAX_TASKS];
    volatile uint32_t curr;
    uint32_t size;
}task_table;

volatile struct task *curr_task;
volatile struct task *next_task;


static void task_finished(void){
    while(1);
}

void task_init(void (*handler)(void), uint32_t *p_stack, uint32_t stack_size){

    if(task_table.size >= MAX_TASKS - 1){
        return;
    }

    struct task *p_task = &task_table.tasks[task_table.size];
    p_task->handler = handler;
    p_task->sp = (uint32_t)(p_stack+stack_size-16);
    p_task->status = TASK_STATUS_IDLE;

    p_stack[stack_size-1] = 0x01000000;
    p_stack[stack_size-2] = (uint32_t)handler;
    p_stack[stack_size-3] = (uint32_t)&task_finished;


    task_table.size++;
}


//static volatile int led_state = 0;
void SysTick_Handler(void){
/*    if(led_state == 0){
        led_state = 1;
        led_on();
    }else{
        led_state = 0;
        led_off();
    }*/
    curr_task = &task_table.tasks[task_table.curr];
    curr_task->status = TASK_STATUS_IDLE;

    task_table.curr++;
    if(task_table.curr >= task_table.size){
        task_table.curr = 0;
    }

    next_task = &task_table.tasks[task_table.curr];
    next_task->status = TASK_STATUS_ACTIVE;

    //ICSR 0xE000ED04
    //Sets PendSV to pending
    asm volatile("ldr r0, =0xE000ED04\t\n"
                 "ldr r1, [r0]\t\n"
                 "orr r1, %0\t\n"
                 "str r1, [r0]\t\n"
                 :
                 : "i" (1UL << 28));

}

void task1_handler(){
    while(1){
        spin(999999);
        toggle_led();
    }
}

void task2_handler(){
    while(1){
        spin(99999);
        toggle_led();

    }
}

void main(void){
    static uint32_t stack1[128];
    static uint32_t stack2[128];

    // PendSV to lowest priority
    // SysTick to highest priority
    NVIC_IPR13 = 0xff;
    NVIC_IPR14 = 0x00;

    RCC->CFGR3 |= RCC_CFGR3_USART2SW_HSI;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;


    init_led();
    systick_init(8000000 * 2);

    task_init(&task1_handler, stack1, sizeof(stack1));
    task_init(&task2_handler, stack2, sizeof(stack2));


    curr_task = &task_table.tasks[task_table.curr];

    // Set psp to first task stack top
    asm volatile("msr psp, %0\n\t"
                 :
                 : "r" (curr_task->sp+64));

    // Thread mode unprivileged with SP_process as current stack
    asm volatile("mov r0, #0x3\t\n"
                 "msr control, r0\t\n");

    asm volatile("ISB");

    curr_task->handler();

    for(;;);

    return;
}

__attribute__((naked, noreturn)) void _reset(void) {
    extern long _sbss, _ebss, _sdata, _edata, _sidata;
    for (long *dst = &_sbss; dst < &_ebss; dst++) *dst = 0;
    for (long *dst = &_sdata, *src = &_sidata; dst < &_edata;) *dst++ = *src++;

    main();             
    for (;;) (void) 0;  // Infinite loop in the case if main() returns
}
extern void _estack(void);  
extern void PendSV_Handler(void);


__attribute__((section(".vectors"))) void (*const tab[16 + 91])(void) = {
    _estack, _reset, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, PendSV_Handler, SysTick_Handler
};
