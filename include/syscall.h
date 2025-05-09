#ifndef SVC_H_
#define SVC_H_
#include <stdint.h>

#define N_SYSCALL 3

void *malloc(size_t);
void free(void*);

long testcall(long);


#endif /* SVC_H_ */
