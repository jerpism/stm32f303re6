#ifndef TASK_H_
#define TASK_H_
#include <stdint.h>

#define MAX_TASKS 16

enum task_status {
    TASK_STATUS_DISABLED = -2,
    TASK_STATUS_READY = -1,
    TASK_STATUS_IDLE = 0,
    TASK_STATUS_ACTIVE = 1
};

struct task{
    volatile uint32_t sp;
    void (*handler)(void);
    volatile enum task_status status;
    uint32_t *kmem;
    size_t memsz;
    uint32_t pid;
    const char *name;
};

struct task_node{
    struct task *task;
    struct task_node *next;
};


struct task *create_task(void (*handler)(void), uint32_t *stack, size_t stack_size, const char *name);

void sched_start();
void sched_add(struct task *task);
void sched_remove(uint32_t pid);

void ps();
void blinkg();
void blinkr();



#endif /* TASK_H_ */
