#include "lib/reconfigure_multiboot_avr/reconfigure_multiboot_internal_avr.h"

volatile uint8_t fpgaMultibootCompleteState = 0;

void reconfigure_fpgaMultibootClearComplete_internal(void) {
    fpgaMultibootCompleteState = 0;
}

void reconfigure_fpgaSetDoneReponse_internal(uint8_t response) {
    fpgaMultibootCompleteState = response;
}

//wofür?
uint8_t reconfigure_fpgaMultibootComplete_internal(void) {
    return fpgaMultibootCompleteState;
}
