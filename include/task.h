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
};

int task_init(void (*handler)(void), uint32_t *stack, uint32_t stack_size);
void task_disable(uint32_t n);
void task_enable(uint32_t n);
void sched_start();



#endif /* TASK_H_ */
