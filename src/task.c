#include <task.h>

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

int task_init(void (*handler)(void), uint32_t *stack, uint32_t stack_size){
    /* TODO: Maybe define proper error codes at some point for these */
    if(task_table.size >= MAX_TASKS - 1){
        return -1;
    }

    /* Each task needs a minimum stack size of 64 bytes */
    if(stack_size < 64){
        return -2;
    }

    struct task *task = &task_table.tasks[task_table.size];
    task->handler = handler;
    task->sp = (uint32_t)(stack+stack_size-16);

    stack[stack_size-1] = 0x01000000;
    stack[stack_size-2] = (uint32_t)handler;
    stack[stack_size-3] = (uint32_t)&task_finished;

    task_table.size++;

    return 0;
}

void sched_start(){
    curr_task = &task_table.tasks[task_table.curr];

    // Set psp to first task stack top
    asm volatile("msr psp, %0\n\t"
                 :
                 : "r" (curr_task->sp+64));

    // Thread mode unprivileged with SP_process as current stack
    asm volatile("mov r0, #0x3\n\t"
                 "msr control, r0");

    asm volatile("ISB");

    curr_task->handler();
}

void SysTick_Handler(void){
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
