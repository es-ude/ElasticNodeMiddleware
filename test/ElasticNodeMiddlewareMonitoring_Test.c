#include "unity.h"

#include "ElasticNodeMiddleware/ElasticNodeMiddlewareMonitoring.h"

#include "test/header_replacements/EmbeddedUtilities/MockBitManipulation.h"

#include "src/interruptManager/MockinterruptManager.h"

uint8_t twcr_mon;
uint8_t twar_mon;
uint8_t twdr_mon;
uint8_t state_and_sample_rate_of_the_mcu;

uint8_t *TWCR_MON = &twcr_mon;
uint8_t *TWAR_MON = &twar_mon;
uint8_t *TWDR_MON = &twdr_mon;

uint8_t *STATE_AND_SAMPLE_RATE_OF_THE_MCU = &state_and_sample_rate_of_the_mcu;

void test_elasticnode_monitoring_change_running_state(void) {
    uint8_t new_state = 4;
    BitManipulation_setByte_Expect(STATE_AND_SAMPLE_RATE_OF_THE_MCU, 0b00001111, new_state);

    elasticnode_monitoring_change_running_state(new_state);

    new_state = 16;
    elasticnode_monitoring_change_running_state(new_state);

}

void test_elasticnode_monitoring_change_sample_rate(void) {
    uint8_t new_sample_rate = 4;
    BitManipulation_setByte_Expect(STATE_AND_SAMPLE_RATE_OF_THE_MCU, 0b11110000, new_sample_rate);

    elasticnode_monitoring_change_sample_rate(new_sample_rate);

    new_sample_rate = 16;
    elasticnode_monitoring_change_sample_rate(new_sample_rate);
}

void test_IIC_slave_init(void) {

    uint8_t address = 12;

    interruptManager_clearInterrupt_Expect();

    *TWAR_MON = address;

    BitManipulation_setBit_Expect(TWCR_MON, TWIE_MON);
    BitManipulation_setBit_Expect(TWCR_MON, TWEA_MON);
    BitManipulation_setBit_Expect(TWCR_MON, TWINT_MON);
    BitManipulation_setBit_Expect(TWCR_MON, TWEN_MON);

    interruptManager_setInterrupt_Expect();

    IIC_slave_init(address);

    TEST_ASSERT_EQUAL(address, *TWAR_MON);
}

void test_elasticnode_monitoring_ISR(void) {

    // TODO: if

    BitManipulation_setBit_Expect(TWCR_MON, TWIE_MON);
    BitManipulation_setBit_Expect(TWCR_MON, TWEA_MON);
    BitManipulation_setBit_Expect(TWCR_MON, TWINT_MON);
    BitManipulation_setBit_Expect(TWCR_MON, TWEN_MON);

    elasticnode_monitoring_ISR();
}
