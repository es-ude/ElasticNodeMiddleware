//
// Created by annika on 27.11.19.
//

#ifndef ELASTICNODEMIDDLEWARE_INTERRUPTMANAGER_AVR_MOCK_H
#define ELASTICNODEMIDDLEWARE_INTERRUPTMANAGER_AVR_MOCK_H

#ifdef TEST
void cli();
void sei();
#else
#include <avr/interrupt.h>
#endif

#endif //ELASTICNODEMIDDLEWARE_INTERRUPTMANAGER_AVR_MOCK_H
