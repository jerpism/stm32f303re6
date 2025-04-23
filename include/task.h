#ifndef TASK_H_
#define TASK_H_
#include <stdint.h>

#define MAX_TASKS 16
#define NULL (void*)0

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
    uint32_t pid;
};

struct task_node{
    struct task *task;
    struct task_node *next;
};


void sched_start(struct task_node *head);
struct task_node *sched_add(struct task_node *last, struct task *task);

struct task *create_task(void (*handler)(void), uint32_t *stack, size_t stack_size);


#endif /* TASK_H_ */
