#include <task.h>
#include <alloc.h>
#include <systick.h>
#include <common.h>
#include <usart.h>
#include <libc.h>


static volatile uint32_t nextpid = 0;

volatile struct task *curr_task;
volatile struct task *next_task;
static volatile struct task_node *current = NULL;

static void task_finished(void){
    while(1);
}

struct task *create_task(void (*handler)(void), uint32_t *stack, size_t stack_size){
    struct task *new = malloc(sizeof(struct task));

    new->handler = handler;
    new->sp = (uint32_t)(stack+stack_size-16);
    new->pid = nextpid++;

    stack[stack_size-1] = 0x01000000;
    stack[stack_size-2] = (uint32_t)handler;
    stack[stack_size-3] = (uint32_t)&task_finished;

    return new;
}

struct task_node *new_queue(struct task_node *last, struct task *task){

}

struct task_node *sched_add(struct task_node *last, struct task *task){
    disable_irq();
    if(last == NULL) return 0;

    struct task_node *new = malloc(sizeof(struct task_node));
    new->task = task;
    new->next = last;

    last->next = new;

    return last;

    enable_irq();
}

void sched_start(struct task_node *head){
    // Head just points to first task and doesn't actually contain a task itself  
    current = head->next;
    curr_task = current->task;

    // Set psp to first task stack top
    asm volatile("msr psp, %0\n\t"
            :
            : "r" (current->task->sp+64));


    // Thread mode unprivileged with SP_process as current stack
    asm volatile("mov r0, #0x3\n\t"
            "msr control, r0");

    asm volatile("ISB");


    curr_task->handler();

}

void SysTick_Handler(void){
    curr_task = current->task;

    if(curr_task == NULL || current->next == NULL){
        return;
    }

    current = current->next;
    next_task = current->task;

    //ICSR 0xE000ED04
    //Sets PendSV to pending
    asm volatile("ldr r0, =0xE000ED04\t\n"
            "ldr r1, [r0]\t\n"
            "orr r1, %0\t\n"
            "str r1, [r0]\t\n"
            :
            : "i" (1UL << 28));

}
