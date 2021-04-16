#ifndef ELASTICNODEMIDDLEWARE_FPGAREGISTERS_H
#define ELASTICNODEMIDDLEWARE_FPGAREGISTERS_H

#ifdef TEST

#include <stdint.h>

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
extern uint8_t *SPCR_SPI;

extern uint8_t* XMEM_XMCRA;
extern uint8_t* XMEM_XMCRB;

extern uint8_t FPGA_DONE_INT_REG;
extern uint8_t FPGA_DONE_INT_CONTROL_REG;

extern uint8_t SREG;

extern uint8_t* UBRR1H;
extern uint8_t* UBRR1L;
extern uint8_t UCSR1A;
extern uint8_t* UCSR1B;
extern uint8_t* UCSR1C;

extern uint8_t* UDR1;

extern uint8_t* DDR_LED;
extern uint8_t* PORT_LED;

// spi
extern uint8_t* DDR_SPI;
extern uint8_t* PORTB_SPI;
extern uint8_t* DDRE_SPI;
extern uint8_t* PORTE_SPI;

extern uint8_t* SPSR_SPI;
extern uint8_t* SPDR_SPI;

extern uint8_t* SPCR_XMEM;

//flash
extern uint8_t* DDR_FLASH_CS;
extern uint8_t* PORT_FLASH_CS;
extern uint8_t* SPCR_FLASH;
extern uint8_t* DDRB_FLASH;
extern uint8_t* PORTB_FLASH;

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

// spi
#define DDR_SPI &DDRB
#define PORTB_SPI &PORTB
#define SPCR_SPI &SPCR
#define DDRE_SPI &DDRE
#define PORTE_SPI &PORTE
#define SPSR_SPI &SPSR
#define SPDR_SPI &SPDR

#define SPCR_XMEM &SPCR

//flash
#define SPCR_FLASH &SPCR
#define DDRB_FLASH &DDRB
#define PORTB_FLASH &PORTB

#define FPGA_DONE_INT_REG PCMSK0
#define FPGA_DONE_INT_CONTROL_REG PCICR

#define FPGA_DONE_INT_VECTOR PCINT0_vect

#define DDR_FLASH_CS DDRB
#define PORT_FLASH_CS PORTB

#define DDR_WIRELESS_CS DDRE
#define PORT_WIRELESS_CS PORTE

//leds
#define DDR_LED &DDRD
#define PORT_LED &PORTD

// xmem
#define XMEM_XMCRA &XMCRA
#define XMEM_XMCRB &XMCRB

#endif

#endif //ELASTICNODEMIDDLEWARE_FPGAREGISTERS_H
