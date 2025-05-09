#ifndef DEBUG_H_
#define DEBUG_H_
#include <stdint.h>

enum base{
    HEX = 16,
    DEC = 10,
};

void dbg_print1(const char*, enum base, uint32_t a);
void dbg_print2(const char*, enum base, uint32_t a, uint32_t b);
void dbg_print3(const char*, enum base, uint32_t a, uint32_t b, uint32_t c);

#endif /* DEBUG_H_ */

