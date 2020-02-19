//
// Created by annika on 24.09.19.
//

#ifndef ELASTICNODEMIDDLEWARE_FPGAREGISTERS_H
#define ELASTICNODEMIDDLEWARE_FPGAREGISTERS_H

#ifdef TEST

extern uint8_t* DDR_FPGA_PROGRAM_B;
extern uint8_t* PORT_FPGA_PROGRAM_B;

extern uint8_t* DDR_FPGA_POWER_SRAM;
extern uint8_t* PORT_FPGA_POWER_SRAM;
extern uint8_t* DDR_FPGA_POWER_AUX;
extern uint8_t* PORT_FPGA_POWER_AUX;
extern uint8_t* DDR_FPGA_POWER_IO;
extern uint8_t* PORT_FPGA_POWER_IO;
extern uint8_t* DDR_FPGA_POWER_INT;
extern uint8_t* PORT_FPGA_POWER_INT;
extern uint8_t* DDR_FPGA_CCLK;

extern uint8_t* DDR_FPGA_INIT_B;
extern uint8_t* PORT_FPGA_INIT_B;

extern uint8_t* DDR_FPGA_DONE;
extern uint8_t* PORT_FPGA_DONE;
extern uint8_t* PIN_FPGA_DONE;

extern uint8_t* DDR_XMEM_A;
extern uint8_t* DDR_XMEM_AD;
extern uint8_t* DDR_XMEM_WR;
extern uint8_t* PORT_XMEM_RD;
extern uint8_t* DDR_XMEM_ALE;

extern uint8_t* DDR_XMEM_RD;
extern uint8_t* SPCR;

extern uint8_t* XMCRA;
extern uint8_t* XMCRB;

extern uint8_t FPGA_DONE_INT_REG;
extern uint8_t FPGA_DONE_INT_CONTROL_REG;

extern uint8_t SREG;

extern uint8_t* UBRR1H;
extern uint8_t* UBRR1L;
extern uint8_t UCSR1A;
extern uint8_t* UCSR1B;
extern uint8_t* UCSR1C;

extern uint8_t* UDR1;

#else
#include <avr/io.h>

#define DDR_FPGA_PROGRAM_B &DDRF
#define PORT_FPGA_PROGRAM_B &PORTF

#define DDR_FPGA_POWER_SRAM &DDRE
#define PORT_FPGA_POWER_SRAM &PORTE
#define DDR_FPGA_POWER_AUX &DDRE
#define PORT_FPGA_POWER_AUX &PORTE
#define DDR_FPGA_POWER_IO &DDRE
#define PORT_FPGA_POWER_IO &PORTE
#define DDR_FPGA_POWER_INT &DDRB
#define PORT_FPGA_POWER_INT &PORTB

#define DDR_FPGA_CCLK &DDRB

#define DDR_FPGA_INIT_B &DDRF
#define PORT_FPGA_INIT_B &PORTF

#define DDR_FPGA_DONE &DDRB
#define PORT_FPGA_DONE &PORTB
#define PIN_FPGA_DONE &PINB

#define DDR_XMEM_RD &DDRE
#define DDR_XMEM_A &DDRC
#define DDR_XMEM_AD &DDRA
#define DDR_XMEM_WR &DDRE
#define PORT_XMEM_RD &PORTE
#define DDR_XMEM_ALE &DDRE

#define FPGA_DONE_INT_REG PCMSK0
#define FPGA_DONE_INT_CONTROL_REG PCICR

#define FPGA_DONE_INT_VECTOR PCINT0_vect

#endif

#endif //ELASTICNODEMIDDLEWARE_FPGAREGISTERS_H
