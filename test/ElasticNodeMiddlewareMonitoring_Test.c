#include "unity.h"

#include "ElasticNodeMiddleware/ElasticNodeMiddlewareMonitoring.h"

#include "test/header_replacements/EmbeddedUtilities/MockBitManipulation.h"

#include "src/interruptManager/MockinterruptManager.h"

uint8_t twcr_mon;
uint8_t twar_mon;
uint8_t twdr_mon;

uint8_t *TWCR_MON = &twcr_mon;
uint8_t *TWAR_MON = &twar_mon;
uint8_t *TWDR_MON = &twdr_mon;

void test_elasticnode_monitoring_change_running_state(void) {

}

void test_elasticnode_monitoring_change_sample_rate(void) {

}

void test_IIC_slave_init(void) {

    uint8_t address= 12;

    interruptManager_clearInterrupt_Expect();

    *TWAR_MON  = address;

    BitManipulation_setBit_Expect(TWCR_MON, TWIE_MON);
    BitManipulation_setBit_Expect(TWCR_MON, TWEA_MON);
    BitManipulation_setBit_Expect(TWCR_MON, TWINT_MON);
    BitManipulation_setBit_Expect(TWCR_MON, TWEN_MON);

    interruptManager_setInterrupt_Expect();

    IIC_slave_init(address);

    TEST_ASSERT_EQUAL(address, *TWAR_MON);
}

void test_elasticnode_monitoring_ISR(void) {

}
