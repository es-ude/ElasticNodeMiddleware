//#include "uartManager.h"
#include "lib/uartmanager/uartmanager.h"
//#include <fpgaPins.h>
#include "lib/pinDefinition/fpgaRegisters.h"
#include "lib/pinDefinition/fpgaPins.h"
//#include "lib/fpgaFlash/fpgaFlash.h"
#include "lib/fpgaFlash_new/fpgaFlash.h"
#include "lib/fpgaFlash_new/fpgaFlashSelect.h"
#include "lib/xmem/xmem.h"
//#include "lib/timer/timer.h"
//#include "lib/flash/flash.h"
#include "lib/flash_new/flash.h"
#include "lib/spi_new/spi.h"
//#include "lib/fpga/fpga.h"
#include "lib/elasticNodeMiddleware/elasticNodeMiddleware.h"
#include "lib/elasticNodeMiddleware/elasticNodeMiddleware_internal.h"
//#include "lib/leds/leds.h"
//#include "lib/spiArbitration/spiArbitration.h"
//#include "lib/selectmap/selectmap.h"
//#include "lib/jtag/jtag.h"
//#include "lib/experiments/experiments.h"
//#include "lib/experiments/latency.h"
// #include <wireless.h>
//#include "lib/transfer/transfer.h"
//#include "lib/multiboot/multiboot.h"
#include "lib/reconfigure_multiboot_avr/reconfigure_multiboot_avr.h"
#include "lib/reconfigure_multiboot_avr/reconfigure_multiboot_internal_avr.h"
//#include "lib/selectmap/selectmap.h"
//#include "lib/io/pinio.h"

//#include "lib/stubsLocal/stubMmLocal.h"
//#include "lib/stubs/mm/mm.h"
//#include "lib/stubs/fir/fir.h"
//#include "lib/stubs/ann/ann.h"

//#include "lib/xor/xor.h"
//#include "mcu-ann/Ann.h"
//#include "lib/configuration/configuration.h"
#include "lib/configuration_new/configuration.h"
//#include "lib/wireless/HelpersForUsageWithoutCommModule.h"
#include "EmbeddedUtilities/BitManipulation.h"
#include <avr/sleep.h>
#include <math.h>

//from me!!!
#include "lib/debug/debug.h"

uartReceiveMode currentUartReceiveMode = UART_IDLE;
loadingMode currentLoadingMode = LOADING_IDLE;

volatile uint32_t uartCount;
uint8_t *uartPtr;
uint32_t multibootAddress;
uint32_t configDataSize;
uint32_t configDataAddress;
uint32_t configDataDestination;
uint32_t configDataPtr;

int configured = 0x0;

uint8_t numBlocks4K;
uint32_t numPages;
uint8_t blockCounter;

uint32_t *matrixA, *matrixB, *matrixC;

uint8_t *data;

uint8_t isUartIdle(void)
{
    return currentUartReceiveMode != UART_IDLE;
}

void uartProcessInput(uint8_t currentData)
{
    uint8_t *lockData;
    uint32_t jedec;

    switch(currentUartReceiveMode)
    {

        // receive 32 bit size
        case UART_CONFIG_SIZE:
            // configDataSize |= ((uint32_t)(currentData) << (8 * (uartCount)));
            *uartPtr++ = currentData;

            uartCount++;

            if (uartCount >= 4)
            {
                // don't read data from uart
                if ((currentLoadingMode == LOAD_FLASH_SELECTMAP) | (currentLoadingMode == LOAD_FLASH_JTAG))
                {
//                    debugWriteDec32(configDataSize);
//                    debugNewLine();
//                    debugWaitUntilDone();
//                    uartCount = 0;
//
//                    // start reading from flash
//                    currentUartReceiveMode = UART_IDLE;
//
//                    // setLed(3,1);
//                    debugWriteString("reading from flash... size ");
//                    debugWriteDec32(configDataSize);
//                    debugWriteString(" address ");
//                    debugWriteHex32(configDataAddress);
//                    debugNewLine();
//
//                    if (currentLoadingMode == LOAD_FLASH_SELECTMAP)
//                    {
//                        debugWriteLine("starting selectmap from flash... ");
//                        debugWriteHex8(SPCR);
//                        debugNewLine();
////						startExperiment(10.0, 0);
//                        startSelectMap();
//                        readDataFlash(configDataAddress, configDataSize, 1, &selectMapClockDataHelper, &selectMapFinishConfig);
//                    }
//                    else if (currentLoadingMode == LOAD_FLASH_JTAG)
//                    {
//                        debugWriteLine("starting jtag...");
//                        jtagInitConfig();
//                        setLed(1, 0);
//                        readDataFlash(configDataAddress, configDataSize, 1, &jtagClockData, &jtagFinishConfig);
//                    }
//
//                    // else if (currentLoadingMode == LOAD_FLASH_FPGA_FLASH)
//                    // {
//                    // 	debugWriteLine("starting soft spi...");
//                    // 	softSpiInit();
//                    // 	selectFpgaFlash();
//
//                    // 	debugWriteString
//                    // 	debugWriteLine("Erasing flash...");
//                    // 	numBlocks4K = configDataSize / 0x1000 + 1;
//                    // 	for (blockCounter = 0; blockCounter < numBlocks4K; blockCounter++)
//                    // 	{
//                    // 		uint32_t blockAddress = configDataAddress + ((uint32_t) blockCounter) * 0x1000;
//                    // 		eraseSectorFlash(blockAddress, 0);
//                    // 	}
//
//                    // 	setLed(1, 0);
//                    // 	// this might? work, needs to be queued
//                    // 	readDataFlash(configDataAddress, configDataSize, 1, &softSpiWriteHelper, &softSpiFinishedHelper);
//                    // }
//                    // setLed(3,0);
//                    // debugWriteLine("Finished configuration from flash");
//
//                    // debugWriteLine("ready for new uart");
//                    // printUartHandler();
//                    currentLoadingMode = LOADING_IDLE;
//                    currentUartReceiveMode = UART_IDLE;
                }
                else if (currentLoadingMode == LOAD_FLASH_FPGA_FLASH)
                {
                    uartPtr = (uint8_t *) &configDataDestination;
                    currentUartReceiveMode = UART_CONFIG_DESTINATION;

                    debugWriteDec32(configDataSize);
                    debugNewLine();
                    uartCount = 0;
                }
                else if ((currentLoadingMode == LOADING_FLASH_VERIFY) | (currentLoadingMode == LOADING_FLASH_VERIFY_FPGA))
                {
//                    debugWriteDec32(configDataSize);
//                    debugNewLine();
//                    uartCount = 0;
//
//                    setLed(1, 0);
//
//                    uint8_t mcuFlash = currentLoadingMode != LOADING_FLASH_VERIFY_FPGA;
//                    debugWriteString("verifying from ");
//                    if (mcuFlash)
//                        debugWriteString("mcu");
//                    else
//                        debugWriteString("fpga");
//                    debugWriteString(" flash... size ");
//                    debugWriteDec32(configDataSize);
//                    debugWriteString(" address ");
//                    debugWriteHex32(configDataAddress);
//                    debugNewLine();
//
//                    // setup soft spi reading
//                    if (!mcuFlash)
//                        fpgaFlashInit();
//
//                    setLed(2, 1);
//                    // TODO: changed verify to synchronous
//                    uint8_t *readData;
//                    readData = readDataFlash(configDataAddress, configDataSize, mcuFlash, NULL, NULL);
//                    if (mcuFlash)
//                        deselectMcuFlash();
//                    else
//                        deselectFpgaFlash();
//                    debugWriteString("Done reading... ");
//                    debugWriteDec16(configDataSize);
//                    debugNewLine();
//                    debugReady();
//                    debugWaitUntilDone();
//
//                    debugWriteStringLength(readData, configDataSize);
//                    debugWaitUntilDone();
//                    setLed(2,0);
////                        readData = readDataFlash(configDataAddress, configDataSize, mcuFlash, &debugWriteCharHelper, );
////                    if (mcuFlash)
////                        readData = readDataFlash(configDataAddress, configDataSize, mcuFlash, &debugWriteCharHelper, deselectMcuFlash);
////                    else
////                        readData = readDataFlash(configDataAddress, configDataSize, mcuFlash, &debugWriteCharHelper, deselectFpgaFlash);
//                    // debugWriteLine("done verifying");
//                    // setLed(1, 1);
//
//                    // debugReady();
//
//                    // start reading from flash
//                    currentUartReceiveMode = UART_IDLE;
//                    currentLoadingMode = LOADING_IDLE;
                }
                else
                {
                    currentUartReceiveMode = UART_CONFIG_DATA;
                    debugWriteDec32(configDataSize);
                    debugNewLine();


                    uint8_t mcuFlash = currentLoadingMode != LOADING_FPGA_FLASH;
                    if (currentLoadingMode == LOADING_FPGA_FLASH)
                    {
                        debugWriteString("Writing to fpga flash: ");
                        debugWriteDec32(configDataAddress);
                        debugNewLine();
                        fpgaFlashInit();
                    }

                    // erase correct chunks of flash memory
                    if ((currentLoadingMode == LOAD_UART_FLASH) | (currentLoadingMode == LOADING_FPGA_FLASH))
                    {
//                        setLed(1, 1);


                        debugWriteLine("Erasing flash...");
                        numBlocks4K = configDataSize / 0x1000 + 1;
                        for (blockCounter = 0; blockCounter < numBlocks4K; blockCounter++)
                        {
                            uint32_t blockAddress = configDataAddress + ((uint32_t) blockCounter) * 0x1000;
                            eraseSectorFlash(blockAddress, mcuFlash);
                        }
//                        setLed(1, 0);
                    }

                    // debugWriteLine("received size");
                    // for (int i = 0; i < 4; i++)
                    // {
                    // 	debugWriteDec8(configDataSize >> (8 * i));
                    // 	debug
                    // }
                    // debugNewLine();
                    // debugWriteDec32(configDataSize);
                    // debugNewLine();

                    debugReady();

                    // ready to receive data again

                    uartCount = 0;
                }
            }
            break;

        case UART_MULTIBOOT:
            // *uartPtr++ = currentData;

            // uartCount++;

            // if (uartCount >= 3)
            // {
            //     debugWriteString("Multiboot received: ");
            //     debugWriteHex32(multibootAddress);
            //     debugNewLine();

            //     fpgaSetDoneReponse(FPGA_DONE_PRINT);
            //     startExperiment(0.1, 0);
            //     fpgaMultiboot(multibootAddress);

            //     // debugDone();

            //     currentUartReceiveMode = UART_IDLE;
            // }
            break;

            // receive 32 bit destination addresses
        case UART_CONFIG_DESTINATION:

            *uartPtr++ = currentData;

            uartCount++;

            // load flash to flash
            if (uartCount >= 4)
            {
                debugWriteDec32(configDataDestination);
                debugNewLine();
                uartCount = 0;

                // start reading from flash
                currentUartReceiveMode = UART_IDLE;

                // setLed(3,1);
                debugWriteString("reading from flash... size ");
                debugWriteDec32(configDataSize);
                debugWriteString(" address ");
                debugWriteHex32(configDataAddress);
                debugWriteString(" to ");
                debugWriteHex32(configDataDestination);
                debugNewLine();

                // softSpiInit();
                // // flashTest(0);
                // softSpiDisable();

                debugWriteLine("starting soft spi...");
                fpgaFlashInit();
                fpgaDisableFlashInterface();
                // softSpiInit();
                // selectFpgaFlash();
                unlockFlash(0);
                _delay_ms(100);
                fpgaFlashSetWritingAddress(configDataDestination);

                // uint32_t destination = 0x10000;
                // write to test erasing
                // for (uint16_t i = 0; i < 500; i++)
                // {
                // 	if(flashQueueData(i % 10))
                // 	{
                // 		flashWriteQueue(destination, 0);
                // 		readDataFlash(destination, 0x100, 0, (void*) &debugWriteHex8, NULL);
                // 		debugNewLine();

                // 		destination += 256;
                // 	}
                // }
                // flashWriteQueue(destination, 0);

//				fpgaEnableFlashInterface();

                debugWriteLine("Erasing flash...");
                _delay_ms(1000);
                numBlocks4K = configDataSize / 0x1000 + 1;
                // numBlocks4K = 20;
                for (blockCounter = 0; blockCounter < numBlocks4K; blockCounter++)
                {
                    uint32_t blockAddress = configDataDestination + ((uint32_t) blockCounter) * 0x1000;
                    eraseSectorFlash(blockAddress, 0);
                    deselectFlash(0);
//					debugWriteString("Erase at ");
//					debugWriteHex32(blockAddress);
//					debugNewLine();
//					_delay_ms(100);
                }
//				debugWriteString("Erased ");
//				debugWriteDec16(numBlocks4K);
//				debugWriteLine(" blocks...");
                flashResetQueue();
                flashResetWriteCount();

                // readDataFlash(0, 1000, 0, (void*) &debugWriteHex8, NULL);
                // debugNewLine();
                // readDataFlash(340000, 1000, 0, (void*) &debugWriteHex8, NULL);
                // debugNewLine();
                debugWriteLine("Reading from flash...");

                debugWaitUntilDone();

                // setLed(2, 1);
                // this might? work, needs to be queued

                uint8_t *flashPtr;
                numPages = (uint32_t) ceilf((float) configDataSize / 256.0f);
                debugWriteString("num pages: ");
                debugWriteDec32(numPages);
                debugWriteChar(' ');
                debugWriteDec32(configDataSize);
                debugWriteChar(' ');
                debugWriteFloat((float) configDataSize / 256.0f);

                debugNewLine();
                unlockFlash(0);
                _delay_ms(100);
                // numPages = 10;
                uint32_t pageAddress = configDataAddress;
                for (uint32_t page = 0; page < numPages; page++)
                {
//					debugWriteDec32(page);
//					debugWriteString(" of ");
//					debugWriteDec32(numPages);
//					debugWriteString(" to ");
//					debugWriteHex32(pageAddress);
//					debugNewLine();
                    flashPtr = readDataFlash(pageAddress, 256, 1, NULL, NULL);

                    fpgaDisableFlashInterface();
                    writeDataFlash(pageAddress, flashPtr, 256, 0);
                    pageAddress += 256;
//					_delay_ms(100);
//					 for (int16_t i = 0; i < 256; i++)
//					 {
//					 	debugWriteHex8(flashPtr[i]);
//					 	debugWriteChar(' ');
//					 }
//					 debugNewLine();
                    // readDataFlash(configDataAddress, configDataSize, 1, &softSpiWriteHelper, &softSpiFinishedHelper);
                }
                fpgaFlashFinishedHelper();


                currentLoadingMode = LOADING_IDLE;
                currentUartReceiveMode = UART_IDLE;

                // debugNewLine();

            }  
            break;

            // receive 32 bit address
            
        case UART_CONFIG_ADDRESS:

            
            // setLed((((uint16_t) uartPtr) - (uint16_t) (&configDataAddress)), 1);
            *uartPtr++ = currentData;

            uartCount++;

            if (uartCount >= 4)
            {
                // // not actually loading a config
                // if ((currentLoadingMode == LOADING_WEIGHTS_STORE) | (currentLoadingMode == LOADING_WEIGHTS_LOAD) | (currentLoadingMode == LOADING_WEIGHTS_READ))
                // {
                // 	currentUartReceiveMode = UART_IDLE;

                // 	debugWriteDec32(configDataAddress);
                // 	debugNewLine();

                // 	// debugWriteLine("")
                // 	enableXmem();
                // 	int8_t *weights;
                // 	switch  (currentLoadingMode)
                // 	{
                // 		case LOADING_WEIGHTS_STORE:
                // 			annStoreFlash(configDataAddress);
                // 			break;
                // 		case LOADING_WEIGHTS_LOAD:
                // 			annLoadFlash(configDataAddress);
                // 			break;
                // 		case LOADING_WEIGHTS_READ:
                // 			// uint8_t *weights = (uint8_t *) malloc(96);
                // 			debugWriteString("Reading weights from ");
                // 			debugWriteHex32(configDataAddress);
                // 			debugNewLine();

                // 			weights = (int8_t *) readDataFlash(configDataAddress, 120, 0, NULL, NULL);

                // 			uint16_t value;
                // 			int16_t littleEndian;
                // 			for (int16_t *w = (int16_t *) weights; w < (int16_t *)(weights + 120); w++)
                // 			{
                // 				value = *w;
                // 				littleEndian = (value & 0xff) << 8 | ((value & 0xff00) >> 8);

                // 				debugWriteHex16(littleEndian);
                // 				debugWriteChar(' ');

                // 				// debugWriteFloatFull(littleEndian / 1024.0);
                // 				// debugNewLine();
                // 			}

                // 			debugNewLine();

                // 			debugWriteStringLength((char *) weights, 120);
                // 			debugNewLine();
                // 			// readDataFlash(0x3AFF00, 96, 0, (void*) &debugWriteHex8, NULL);

                // 			break;
                // 		default:
                // 			debugWriteLine("Incompatible loading weights mode");
                // 			break;

                // 	}
                // 	// if (currentLoadingMode == LOADING_WEIGHTS_STORE)
                // 	// elsif
                // 	// else
                // 	currentLoadingMode = LOADING_IDLE;

                // }
                // else
                // {
                debugWriteLine("received address");
                debugReady();
                uartPtr = (uint8_t *) &configDataSize;
                currentUartReceiveMode = UART_CONFIG_SIZE;
                configDataPtr = configDataAddress;
                debugWaitUntilDone();

                debugWriteDec32(configDataAddress);
                debugNewLine();
                debugWaitUntilDone();
                uartCount = 0;

                // }
            }
            
            break;

            
           
        case UART_CONFIG_DATA:

            uartCount++;

            if (currentLoadingMode == LOADING_JTAG)
            {
                // jtagClockData(currentData, uartCount == configDataSize);
                // debugAck((uint8_t)(uartCount));
            }
            else if (currentLoadingMode == LOADING_SELECTMAP)
            {
                // clearGpio(2);
                // setGpio(1);
                // selectMapClockData(currentData);
                // clearGpio(1);
            }
            else if ((currentLoadingMode == LOAD_UART_FLASH) | (currentLoadingMode == LOADING_FPGA_FLASH))
            {
                if(flashQueueData(currentData))
                {
                    // setLed(3, 1);

                    // debugWriteLine("writing to flash...");

                    // this means buffer is full
                    uint8_t mcuFlash = currentLoadingMode == LOAD_UART_FLASH;
                    flashWriteQueue(configDataPtr, mcuFlash);

                    // debugWriteLine("done writing to flash...");

                    // setLed(3, 0);

                    configDataPtr += 256;

                    debugAck(currentData);
                }
            }


            // finished
            if (uartCount == configDataSize)
            {

                currentUartReceiveMode = UART_IDLE;
                if (currentLoadingMode == LOADING_JTAG)
                {
                    // jtagFinishConfig();
                }
                else if (currentLoadingMode == LOADING_SELECTMAP)
                {
                    // selectMapFinishConfig();
                }
                else if ((currentLoadingMode == LOAD_UART_FLASH) | (currentLoadingMode == LOADING_FPGA_FLASH))
                {
                    uint8_t mcuFlash = currentLoadingMode == LOAD_UART_FLASH;
                    flashWriteQueue(configDataPtr, mcuFlash);

                    debugAck(currentData);

                    if (currentLoadingMode == LOADING_FPGA_FLASH)
                        fpgaFlashDisable();
                }

                debugWriteLine("Received all the data");


                // debugWriteLine("config data done ");

                // debugNewLine();
                configured = 0x1;
                currentLoadingMode = LOADING_IDLE;

                // setLed(2, 0);
                debugDone();
            }

            break;

            // receive which test should be done
        case UART_TEST:


            debugAck(currentData);
//            ledsSetMode(ledsDemo);
            debugWriteLine("leds demo");
//			enableXmem();
            float latency;
            uint32_t num_blocks, data_size, repeats;

            // int32_t output;

            switch(currentData)
            {

                default:
                    debugWriteLine("Unknown Test");
                    break;
            }
//            ledsSetMode(ledsDebug);
//			debugWriteLine("leds debug");
            currentUartReceiveMode = UART_IDLE;
            break;


            // case UART_FLASH:
            // 	uartCount++;
            // 	uint8_t done = configDataSize == uartCount;

            // 	// put new data in flash buffer
            // 	if(flashQueueData(currentData) || done)
            // 	{
            // 		// this means buffer is full
            // 		flashWriteQueue(configDataPtr);
            // 		configDataPtr += 256;

            // 		debugAck(currentData);
            // 	}

        case UART_IDLE:
        default:

            // debugWriteString("Command: ");
            // debugWriteChar(currentData);
            // debugNewLine();
            switch(currentData)
            {
                // fetching latency
                /*case 'a':
                    debugAck(currentData);
                    float lat;

                    // prepare buffer for sending
                    uint16_t bufferLength = getLatencyBufferLength();

                    uint8_t *buffer = (uint8_t *) malloc(bufferLength + sizeof(uint16_t));
                    if (!buffer)
                    {
                        debugWriteLine("Could not allocate buffer for latency!");
                        debugNewLine();

                        // indicate to pc that no latency data is coming
                        debugWriteChar(0x00);
                        debugWriteChar(0x00);
                    }
                    else
                    {
                        uint8_t *ptr = buffer;
                        *((uint16_t *) ptr) = bufferLength;

                        ptr += sizeof(uint16_t);

                        // put latencies into buffer for sending
                        while(popLatency(&lat))
                        {
                            *((float *) ptr) = lat;
                            ptr += sizeof(float);
                        }
                        debugWriteStringLength((char *) buffer, bufferLength + sizeof(uint16_t));

                        free(buffer);
                    }
                    break;

                    // case 'A':
                    // 	debugAck(currentData);
                    // 	enableXmem();
                    // 	annStoreFlashOff();
                    // 	break;
                case 'b':
                    debugWriteLine("Storing weights to flash...");
                    currentUartReceiveMode = UART_CONFIG_ADDRESS;


                    uartPtr = (uint8_t *) &configDataAddress;
                    currentUartReceiveMode = UART_CONFIG_ADDRESS;
                    currentLoadingMode = LOADING_WEIGHTS_STORE;
                    debugAck(currentData);
                    uartCount = 0;

                    break;

                case 'B':
                    // debugAck(currentData);
                    debugWriteLine("Reading weights from flash...");

                    currentUartReceiveMode = UART_CONFIG_ADDRESS;


                    uartPtr = (uint8_t *) &configDataAddress;
                    currentUartReceiveMode = UART_CONFIG_ADDRESS;
                    currentLoadingMode = LOADING_WEIGHTS_READ;
                    debugAck(currentData);
                    uartCount = 0;


                    break;

                case 'c':
                    debugAck(currentData);
                    debugWriteLine("Turning FPGA off");
                    disableXmem();
                    fpgaPower(0);
                    debugDone();
                    break;

                case 'C':
                    debugAck(currentData);
                    debugWriteLine("Turning FPGA on");
                    fpgaPower(1);
                    flashDisableInterface();
                    debugDone();
                    break;

                    // control debug output
                case 'D':
                    debugEnable();
                    debugAck(currentData);
                    break;
                case 'd':
                    debugDisable();
                    debugAck(currentData);
                    break;

                    // experiment beginning and ending
                case 'E':
                    debugAck(currentData);
                    startExperimentIndicate();
                    debugNewLine();
                    break;
                case 'e':
                    debugAck(currentData);
                    endExperimentIndicate();
                    debugNewLine();
                    break;
*/
                    // write to fpga flash
                case 'f':
                    debugAck(currentData);
                    BitManipulation_setBit(&PORTD, PD6);
                    BitManipulation_setBit(&PORTD, PD7);
                    uint16_t numBlocks4K = ceil((float)(196776) / 0x1000);
                    uint32_t blockAddress;
                    for (uint16_t blockCounter = 0; blockCounter < numBlocks4K; blockCounter++)
                    {
                        blockAddress = 0 + ((uint32_t) blockCounter) * 0x1000;
                        eraseSectorFlash(blockAddress, 1);
                    }
                    BitManipulation_clearBit(&PORTD, PD6);
                    BitManipulation_clearBit(&PORTD, PD7);
                    break;

                    // write to flash
                case 'F':
                    // certain assumptions made about addresses aligning
                    // addresses must be aligned to 4K blocks 0xFFF000

                    // calculate what blocks need to be erased
                    // TODO: use larger erase blocks
                    // TODO: using non async communication
//					ledsFlash(5);
//					uartPtr = (uint8_t *) &configDataAddress;
//					currentUartReceiveMode = UART_CONFIG_ADDRESS;
//					currentLoadingMode = LOAD_UART_FLASH;
//					 configDataPtr = configDataAddress;
//					uartCount = 0;

                    // acknowledge when ready to receive again
                    debugAck(currentData);

                    configurationUartFlash();

                    break;

                case 'g':
                    debugWriteString("mem: ");
                    uint8_t *testPtr = malloc(10);
                    debugWriteHex16((uint16_t )testPtr);
                    debugNewLine();
                    free(testPtr);
                    break;

                case 'j':
                    // Program FPGA from flash through JTAG
                    uartPtr = (uint8_t *) &configDataAddress;
                    currentUartReceiveMode = UART_CONFIG_ADDRESS;
                    currentLoadingMode = LOAD_FLASH_JTAG;
                    debugAck(currentData);
                    uartCount = 0;
                    break;

                case 'm':
                    // multiboot
                    currentUartReceiveMode = UART_MULTIBOOT;
                    uartPtr = (uint8_t *) &multibootAddress;
                    multibootAddress = 0;
                    uartCount = 0;
                    debugAck(currentData);

                    break;

                    // write to fpga flash
                case 'p':
                    currentLoadingMode = LOADING_FPGA_FLASH;

                    debugWriteLine("Writing to fpga flash...");

                    currentUartReceiveMode = UART_CONFIG_ADDRESS;
                    // uartReceiveUint32Blocking(&configDataSize);
                    // configDataSize = 0;
                    uartPtr = (uint8_t *) &configDataAddress;
                    // debugWriteDec32(configDataSize);
                    // debugNewLine();
                    uartCount = 0;
                    // last = 0x0;
                    debugAck(currentData);

                    configured = 0x0;
                    break;

                case 'r':
                    // perform a reset
                    debugAck(currentData);
                    debugWriteLine("Performing soft reset");
                    xmem_enableXmem();
                    // fpgaSoftReset();
                    // enableXmem();
                    xmem_disableXmem();
                    debugDone();
                    break;

                case 'R':
                    debugAck(currentData);
                    debugWriteLine("Performing hard reset");
//                    flashDisableInterface();
                    // fpgaHardReset();
                    // give chance for fpga to load config from spi
                    fpgaEnableFlashInterface();
                    debugDone();
                    break;

                    // test mw
                case 'T':
                    debugAck(currentData);
                    debugWriteLine("Testing MW...");

                    // go find out which test to run
                    currentUartReceiveMode = UART_TEST;
                    break;


                case 'v':
                    // Verify and readback data from flash to uart
                    // debugWriteLine("time to verify again...");
//					uartPtr = (uint8_t *) &configDataAddress;
//					currentUartReceiveMode = UART_CONFIG_ADDRESS;
//					currentLoadingMode = LOADING_FLASH_VERIFY_FPGA;

                    debugAck(currentData);
//					uartCount = 0;
                    verifyConfigurationFlash(0);

                    break;

                case 'V':
                    // Verify and readback data from flash to uart
                    // debugWriteLine("time to verify again...");
//					uartPtr = (uint8_t *) &configDataAddress;
//					currentUartReceiveMode = UART_CONFIG_ADDRESS;
//					currentLoadingMode = LOADING_FLASH_VERIFY;
                    debugAck(currentData);
//					uartCount = 0;
                    verifyConfigurationFlash(1);
                    break;
                case 'Y':
                    // debugAck(currentData);
                    debugWriteLine("Loading weights from flash...");
                    currentUartReceiveMode = UART_CONFIG_ADDRESS;

                    uartPtr = (uint8_t *) &configDataAddress;
                    currentUartReceiveMode = UART_CONFIG_ADDRESS;
                    currentLoadingMode = LOADING_WEIGHTS_LOAD;
                    debugAck(currentData);
                    uartCount = 0;
                    break;

                    // control fpga flash interface
                case ',':
                    debugAck(currentData);
                    debugWriteLine("FPGA Flash On");
                    fpgaEnableFlashInterface();
                    debugDone();
                    break;
                case '.':
                    debugAck(currentData);
                    debugWriteLine("FPGA Flash Off");
                    fpgaDisableFlashInterface();
                    debugDone();
                    break;

                    // change fpga flash interface
                case '[':
                    debugAck(currentData);

                    fpgaEnableFlashInterface();
                    debugWriteLine("enabled flash interface");
                    debugDone();
                    break;
                case ']':
                    debugAck(currentData);

                    fpgaDisableFlashInterface();
                    debugWriteLine("disabled flash interface");
                    debugDone();
                    break;

                    // indicate readiness
                default:
                    // use this to check that mcu is receiving
                    debugAck(currentData + 1);
                    // debugWriteLine("echo");
                    // uint8_t *ptr = malloc(1);
                    // debugWriteHex16((uint16_t) ptr);
                    // free(ptr);
                    // debugWriteChar(' ');

                    break;
            }
            break;

        case UART_FLASH:break;
    }
}


