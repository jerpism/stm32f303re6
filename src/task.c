#include <task.h>
#include <systick.h>
#include <common.h>
#include <usart.h>
#include <libc.h>
#include <gpio.h>

#include <stdlib.h>
#include <syscall.h>


#define FREE_TASK(x)    do{ free(x->task->kmem);\
                        x->task->kmem = NULL;   \
                        free(x->task);          \
                        x->task = NULL;         \
                        free(x);                \
                        x = NULL;}while(0)    \


static volatile uint32_t nextpid = 0;

/* These are accessed in PendSV_Handler 
 * swtch will go from curr_task -> next_task */
volatile struct task *curr_task;
volatile struct task *next_task;

static struct task_node *head = NULL;
static struct task_node *last = NULL;
static volatile struct task_node *current = NULL;

static void task_finished(void){
    while(1);
}


struct task *create_task(void (*handler)(void), uint32_t *stack, size_t stack_size, const char *name){

    struct task *new = malloc(sizeof(struct task));

    if(new == NULL){
        fault("Got NULL at create_task:", (uint32_t)stack, stack_size, 0);
    }

    new->handler = handler;
    new->sp = (uint32_t)(stack + stack_size - 16);
    new->kmem = stack;
    new->memsz = stack_size * 4;
    new->pid = nextpid++;
    new->name = name;

    stack[stack_size-1] = 0x01000000;
    stack[stack_size-2] = (uint32_t)handler;
    stack[stack_size-3] = (uint32_t)&task_finished;

    return new;
}


void sched_add(struct task *task){
    struct task_node *new = malloc(sizeof(struct task_node));

    if(new == NULL){
        fault("Got NULL at sched_add:", (uint32_t)task, task->sp, 0);
    }

    if(head == NULL){
        head = new;
        last = head;
    }

    new->task = task;
    new->next = last->next;
    last->next = new;
    last = new;


}


void sched_remove(uint32_t pid){
    // If this was somehow called before we had any scheduler queue just return
    if(head == NULL){ return; }
    // Can't kill that which was never alive
    // and also don't kill pid 0, that's probably something important
    if(pid > nextpid || pid == 0){ return; }

    struct task_node *curr = head;
    struct task_node *prev = head;

    do{
        if(curr->task->pid == pid){

            if(curr->next == head){
                last = prev;
                last->next = curr->next;
            }else{
                prev->next = curr->next;
            }

            FREE_TASK(curr);
            break;
        }
        prev = curr;
        curr = curr->next;
    }while(curr != head && curr != NULL);

    return;
}


void ps(){
    if(head == NULL) return;

    struct task_node *tmp = head;
    char c[20];
    uart_sendstr("pid\tname\tmem\t\tsize (B)\n\n\r");

    do{
        if(tmp->task == NULL) break;
        itoa(tmp->task->pid, c, 10);
        uart_sendstr(c);
        uart_send('\t');
        uart_sendstr(tmp->task->name);
        uart_sendstr("\t0x");
        itoa((uint32_t)(tmp->task->kmem), c, 16);
        uart_sendstr(c);
        uart_send('\t');
        itoa(tmp->task->memsz, c, 10);
        uart_sendstr(c);
        uart_sendstr("\r\n");

        tmp = tmp->next;
    }while (tmp != head);
}


void blinkg(){
    uint32_t *pstack = malloc(sizeof(uint32_t) * 32);
    struct task *blinky = create_task(&blink, pstack,  32, "blinkg");
    exec(blinky);
}


void blinkr(){
    uint32_t *pstack = malloc(sizeof(uint32_t) * 32);
    struct task *blinky = create_task(&blink_red, pstack, 32, "blinkr");
    exec(blinky);
}

void sched_start(){
    current = head;
    curr_task = current->task;


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
