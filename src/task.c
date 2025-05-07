#include <task.h>
#include <systick.h>
#include <common.h>
#include <usart.h>
#include <libc.h>
#include <gpio.h>
#include <linkedListAllocator.h>

#define FREE_TASK(x)    free(x->task);  \
                        x->task = NULL; \
                        free(x);        \
                        x = NULL;       \


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

    new->handler = handler;
    new->sp = (uint32_t)(stack+stack_size-16);
    new->pid = nextpid++;
    new->name = name;

    stack[stack_size-1] = 0x01000000;
    stack[stack_size-2] = (uint32_t)handler;
    stack[stack_size-3] = (uint32_t)&task_finished;

    return new;
}

void sched_add(struct task *task){
    disable_irq();

    struct task_node *new = malloc(sizeof(struct task_node));

    if(head == NULL){
        head = new;
        last = head;
    }


    new->task = task;
    new->next = last->next;
    last->next = new;
    last = new;

    enable_irq();

}

void sched_remove(uint32_t pid){
    // If this was somehow called before we had any scheduler queue just return
    if(head == NULL){ return; }
    // Can't kill that which was never alive
    // and also don't kill pid 0, that's probably something important
    if(pid > nextpid || pid == 0){ return; }

    disable_irq();

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

    enable_irq();

    return;
}

void ps(){
    if(head == NULL) return;

    struct task_node *tmp = head;
    char c[3];
    uart_sendstr("pid\tname\n\n\r");

    do{
        if(tmp->task == NULL) break;
        itoa(tmp->task->pid, c, 10);
        uart_sendstr(c);
        uart_send('\t');
        uart_sendstr(tmp->task->name);
        uart_sendstr("\r\n");

        tmp = tmp->next;
    }while (tmp != head);
}

void kill(uint32_t pid){
    //I don't feel like parsing any input so just kill 1-20 every time, one of them will be a blinky hopefully
    //0 should always be shell so don't kill that (not that you can anyways)
    for(int i = 1; i <= 10; ++i){
        sched_remove(i);
    }
}

void blinkg(){
    uint32_t *pstack = malloc(64);
    struct task *blinky = create_task(&blink, pstack, 64, "blinkg");
    sched_add(blinky);
}


void blinkr(){
    uint32_t *pstack = malloc(64);
    struct task *blinky = create_task(&blink_red, pstack, 64, "blinkr");
    sched_add(blinky);
}

void sched_start(){
    disable_irq();

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

    enable_irq();


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
