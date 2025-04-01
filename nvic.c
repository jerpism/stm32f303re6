#include <nvic.h>

void nvic_set_priority(unsigned int n, uint8_t prio){
    if(n > NVIC_IMPLEMENTED){
        return;
    }

    uint8_t *nvic_ipr = (uint8_t*)NVIC_IPR_BASE + n;
    *nvic_ipr = prio;
}
