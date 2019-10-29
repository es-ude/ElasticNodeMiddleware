//
// Created by annika on 28.10.19.
//

#ifndef ELASTICNODEMIDDLEWARE_CONFIGURATION_FLASH_H
#define ELASTICNODEMIDDLEWARE_CONFIGURATION_FLASH_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint32_t configAddress, configSize, configDestination, configRemaining;
uint16_t* ptr_numBlock4K;

void configuration_flash_Flash();
void readValue(uint32_t *destination);
void readData(uint8_t *buffer, uint16_t num);

#endif //ELASTICNODEMIDDLEWARE_CONFIGURATION_FLASH_H
