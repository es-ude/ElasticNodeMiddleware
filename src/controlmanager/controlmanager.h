#ifndef ELASTICNODEMIDDLEWARE_CONTROLMANAGER_H
#define ELASTICNODEMIDDLEWARE_CONTROLMANAGER_H

#include <stdint.h>
#include <stdlib.h>

// due to page sizes, flash must be received differently
typedef enum {
    UART_IDLE,
    UART_CONFIG_ADDRESS,
    UART_CONFIG_SIZE,
    UART_CONFIG_DESTINATION,
    UART_CONFIG_DATA,
    UART_FLASH,
    UART_TEST,
    UART_MULTIBOOT
} uartReceiveMode;

typedef enum {
    LOADING_IDLE,
    LOADING_JTAG,
    LOADING_SELECTMAP,
    LOAD_UART_FLASH,
    LOAD_FLASH_SELECTMAP,
    LOAD_FLASH_JTAG,
    LOADING_FPGA_FLASH,
    LOAD_FLASH_FPGA_FLASH,
    LOADING_FLASH_VERIFY,
    LOADING_FLASH_VERIFY_FPGA,
    LOADING_WEIGHTS_STORE,
    LOADING_WEIGHTS_LOAD,
    LOADING_WEIGHTS_READ
} loadingMode;

#define testingFlashAddress 0x180000

uint8_t control_isUartIdle(void);

void userlogic_enable(void);

void userlogic_read_id(void);

void control_setUserHandle(void (*userHandler)(uint8_t));

void control_handleChar(uint8_t currentData);

#endif //ELASTICNODEMIDDLEWARE_CONTROLMANAGER_H
