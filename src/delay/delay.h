#ifndef ELASTICNODEMIDDLEWARE_DELAY_H
#define ELASTICNODEMIDDLEWARE_DELAY_H

#ifdef TEST

void _delay_ms(uint8_t delay);

#else

#include <util/delay.h>

#endif

#endif //ELASTICNODEMIDDLEWARE_DELAY_H
