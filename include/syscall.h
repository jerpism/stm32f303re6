#ifndef SVC_H_
#define SVC_H_

#include <stdint.h>
#include <task.h>

#define N_SYSCALL 3

void *malloc(size_t);
void free(void*);

void exec(struct task *);
void kill(uint32_t);

void fault(const char*, uint32_t, uint32_t, uint32_t);



#endif /* SVC_H_ */
