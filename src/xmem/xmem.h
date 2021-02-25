#ifndef ELASTICNODEMIDDLEWARE_XMEM_H
#define ELASTICNODEMIDDLEWARE_XMEM_H

void xmem_initXmem(void);

void xmem_enableXmem(void);

void xmem_disableXmem(void);

#include <stdint.h>

#ifdef TEST

extern uint8_t* XMEM_ENABLE_REG;

//iom64: SRE = 7
#define XMEM_ENABLE_P 7

extern const uint8_t* externalMockMemory;
#define XMEM_OFFSET &externalMockMemory
#define XMEM_USERLOGIC_OFFSET ((&externalMockMemory) + 0x200)

#else

#include <avr/io.h>

#if defined (__AVR_ATmega2560__)
#define XMEM_ENABLE_REG &XMCRA
#define XMEM_ENABLE_P SRE
#define XMEM_OFFSET 0x2200
#elif defined (__AVR_ATmega64__)
#define XMEM_ENABLE_REG &MCUCR
#define XMEM_ENABLE_P SRE
#define XMEM_OFFSET 0x2000
#elif defined (__AVR_AT90USB1287__)
#define XMEM_ENABLE_REG &XMCRA
#define XMEM_ENABLE_P SRE
#define XMEM_OFFSET 0x2100
#define XMEM_USERLOGIC_OFFSET 0x2200
#else
#define XMEM_ENABLE_REG &MCUCR
#define XMEM_ENABLE_P SRE
#define XMEM_OFFSET 0x2000
#define XMEM_USERLOGIC_OFFSET 0x2200

#endif

#endif

#endif //ELASTICNODEMIDDLEWARE_XMEM_H
