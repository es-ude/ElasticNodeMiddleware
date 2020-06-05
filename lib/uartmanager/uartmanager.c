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
            /*uartPtr++ = currentData;

            uartCount++;

            if (uartCount >= 4)
            {
                // don't read data from uart
                if ((currentLoadingMode == LOAD_FLASH_SELECTMAP) | (currentLoadingMode == LOAD_FLASH_JTAG))
                {
                    debugWriteDec32(configDataSize);
                    debugNewLine();
                    debugWaitUntilDone();
                    uartCount = 0;

                    // start reading from flash
                    currentUartReceiveMode = UART_IDLE;

                    // setLed(3,1);
                    debugWriteString("reading from flash... size ");
                    debugWriteDec32(configDataSize);
                    debugWriteString(" address ");
                    debugWriteHex32(configDataAddress);
                    debugNewLine();

                    if (currentLoadingMode == LOAD_FLASH_SELECTMAP)
                    {
                        debugWriteLine("starting selectmap from flash... ");
                        debugWriteHex8(SPCR);
                        debugNewLine();
//						startExperiment(10.0, 0);
                        startSelectMap();
                        readDataFlash(configDataAddress, configDataSize, 1, &selectMapClockDataHelper, &selectMapFinishConfig);
                    }
                    else if (currentLoadingMode == LOAD_FLASH_JTAG)
                    {
                        debugWriteLine("starting jtag...");
                        jtagInitConfig();
                        setLed(1, 0);
                        readDataFlash(configDataAddress, configDataSize, 1, &jtagClockData, &jtagFinishConfig);
                    }

                    // else if (currentLoadingMode == LOAD_FLASH_FPGA_FLASH)
                    // {
                    // 	debugWriteLine("starting soft spi...");
                    // 	softSpiInit();
                    // 	selectFpgaFlash();

                    // 	debugWriteString
                    // 	debugWriteLine("Erasing flash...");
                    // 	numBlocks4K = configDataSize / 0x1000 + 1;
                    // 	for (blockCounter = 0; blockCounter < numBlocks4K; blockCounter++)
                    // 	{
                    // 		uint32_t blockAddress = configDataAddress + ((uint32_t) blockCounter) * 0x1000;
                    // 		eraseSectorFlash(blockAddress, 0);
                    // 	}

                    // 	setLed(1, 0);
                    // 	// this might? work, needs to be queued
                    // 	readDataFlash(configDataAddress, configDataSize, 1, &softSpiWriteHelper, &softSpiFinishedHelper);
                    // }
                    // setLed(3,0);
                    // debugWriteLine("Finished configuration from flash");

                    // debugWriteLine("ready for new uart");
                    // printUartHandler();
                    currentLoadingMode = LOADING_IDLE;
                    currentUartReceiveMode = UART_IDLE;
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
                    debugWriteDec32(configDataSize);
                    debugNewLine();
                    uartCount = 0;

                    setLed(1, 0);

                    uint8_t mcuFlash = currentLoadingMode != LOADING_FLASH_VERIFY_FPGA;
                    debugWriteString("verifying from ");
                    if (mcuFlash)
                        debugWriteString("mcu");
                    else
                        debugWriteString("fpga");
                    debugWriteString(" flash... size ");
                    debugWriteDec32(configDataSize);
                    debugWriteString(" address ");
                    debugWriteHex32(configDataAddress);
                    debugNewLine();

                    // setup soft spi reading
                    if (!mcuFlash)
                        fpgaFlashInit();

                    setLed(2, 1);
                    // TODO: changed verify to synchronous
                    uint8_t *readData;
                    readData = readDataFlash(configDataAddress, configDataSize, mcuFlash, NULL, NULL);
                    if (mcuFlash)
                        deselectMcuFlash();
                    else
                        deselectFpgaFlash();
                    debugWriteString("Done reading... ");
                    debugWriteDec16(configDataSize);
                    debugNewLine();
                    debugReady();
                    debugWaitUntilDone();

                    debugWriteStringLength(readData, configDataSize);
                    debugWaitUntilDone();
                    setLed(2,0);
//                        readData = readDataFlash(configDataAddress, configDataSize, mcuFlash, &debugWriteCharHelper, );
//                    if (mcuFlash)
//                        readData = readDataFlash(configDataAddress, configDataSize, mcuFlash, &debugWriteCharHelper, deselectMcuFlash);
//                    else
//                        readData = readDataFlash(configDataAddress, configDataSize, mcuFlash, &debugWriteCharHelper, deselectFpgaFlash);
                    // debugWriteLine("done verifying");
                    // setLed(1, 1);

                    // debugReady();

                    // start reading from flash
                    currentUartReceiveMode = UART_IDLE;
                    currentLoadingMode = LOADING_IDLE;
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
                        setLed(1, 1);


                        debugWriteLine("Erasing flash...");
                        numBlocks4K = configDataSize / 0x1000 + 1;
                        for (blockCounter = 0; blockCounter < numBlocks4K; blockCounter++)
                        {
                            uint32_t blockAddress = configDataAddress + ((uint32_t) blockCounter) * 0x1000;
                            eraseSectorFlash(blockAddress, mcuFlash);
                        }
                        setLed(1, 0);
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
            break;*/

        case UART_MULTIBOOT:
            /*uartPtr++ = currentData;

            uartCount++;

            if (uartCount >= 3)
            {
                debugWriteString("Multiboot received: ");
                debugWriteHex32(multibootAddress);
                debugNewLine();

                fpgaSetDoneReponse(FPGA_DONE_PRINT);
                startExperiment(0.1, 0);
                fpgaMultiboot(multibootAddress);

                // debugDone();

                currentUartReceiveMode = UART_IDLE;
            }
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

                setLed(2, 1);
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
            setLed((((uint16_t) uartPtr) - (uint16_t) (&configDataAddress)), 1);
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
                jtagClockData(currentData, uartCount == configDataSize);
                // debugAck((uint8_t)(uartCount));
            }
            else if (currentLoadingMode == LOADING_SELECTMAP)
            {
                // clearGpio(2);
                // setGpio(1);
                selectMapClockData(currentData);
                // clearGpio(1);
            }
            else if ((currentLoadingMode == LOAD_UART_FLASH) | (currentLoadingMode == LOADING_FPGA_FLASH))
            {
                if(flashQueueData(currentData))
                {
                    setLed(3, 1);

                    // debugWriteLine("writing to flash...");

                    // this means buffer is full
                    uint8_t mcuFlash = currentLoadingMode == LOAD_UART_FLASH;
                    flashWriteQueue(configDataPtr, mcuFlash);

                    // debugWriteLine("done writing to flash...");

                    setLed(3, 0);

                    configDataPtr += 256;

                    debugAck(currentData);
                }
            }


            // finished
            if (uartCount == configDataSize)
            {

                currentUartReceiveMode = UART_IDLE;
                if (currentLoadingMode == LOADING_JTAG)
                    jtagFinishConfig();
                else if (currentLoadingMode == LOADING_SELECTMAP)
                    selectMapFinishConfig();
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
            ledsSetMode(ledsDemo);
            debugWriteLine("leds demo");
//			enableXmem();
            float latency;
            uint32_t num_blocks, data_size, repeats;

            // int32_t output;

            switch(currentData)
            {
                case 'A':
                    debugWriteString("Testing ANN");
                    debugWaitUntilDone();
                    // startExperimentIndicate();

                    // calculate number of blocks to calculate
                    num_blocks = ceil(((double) BATCH_SIZE_LOCAL) / BLOCK_SIZE_LOCAL);
                    data_size = num_blocks * sizeof(float);
                    enableXmem();
                    fpgaSoftReset();

                    // indicate how many latency bytes need to be received
//					debugWriteStringLength((char *) &data_size, sizeof(uint32_t));

                    for (uint32_t i = 0; i < num_blocks; i++)
                    {
                        latency = annTest(BLOCK_SIZE_LOCAL, 0);
//						debugWriteStringLength((char *) (&latency), sizeof(float));
                        // pushLatency(&latencyBuf, latency);
                    }
                    debugWriteLine("done");
                    debugNewLine();

                    // endExperimentIndicate();
                    break;
                    // small ann test
                case 'a':
                    debugWriteString("Testing ANN without latency");
                    // startExperimentIndicate();

                    // calculate number of blocks to calculate
                    num_blocks = ceil(((float) BATCH_SIZE_LOCAL) / BLOCK_SIZE_LOCAL);
                    data_size = num_blocks * sizeof(float);

                    timerStart(0.01);
                    for (int i = 0; i < 100; i++)
                    {
                        annTest(BLOCK_SIZE_LOCAL, 1);
                    }
                    timerStop();
                    debugWriteString("\nResult: ");
                    debugWriteFloatFull(timerGetFloatValue());
                    debugWriteString(" ");
                    debugWriteFloatFull((float) BLOCK_SIZE_LOCAL / timerGetFloatValue());
                    debugNewLine();
                    debugNewLine();

                    debugDone();

                    // endExperimentIndicate();
                    break;

                case 'c':

                    // cold boot experiment
                    debugWriteLine("FPGA off");
                    fpgaPower(0);
                    _delay_ms(100);
                    debugWriteLine("1");

                    fpgaSetDoneReponse(FPGA_DONE_MULTIBOOT);
                    startExperiment(1, 0);
                    fpgaPower(1);
                    // response in fpga done isr
                    break;
                    // fir filter test
                case 'f':
                    debugWriteLine("Testing FIR Filter HWF");
                    enableXmem();
                    firTest();
                    break;

                    // flash test
                case 'F':
                    debugWriteLine("Testing flash...");

                    debugNewLine();
                    debugWriteLine("MCU:");
                    initFlash();
                    unlockFlash(1);
                    // // // printStatus();
                    debugWaitUntilDone();
                    printJEDEC(1);
                    debugWaitUntilDone();
                    deselectFlash(1);

                    _delay_ms(500);

                    flashTest(1);


                    debugNewLine();
                    debugWriteLine("FPGA:");
                    fpgaFlashInit();
                    deselectFlash(0);
                    _delay_ms(100);
                    unlockFlash(0);
                    // // // // printStatus();
                    uint32_t JEDEC = readJEDEC(0);
                    if (JEDEC == 0xFFFFFFFF || JEDEC == 0)
                    {
                        debugWriteLine("ERROR: Flash could not be reached!!");
                        debugDone();
                    }
                    else
                    {
                        debugWriteString("\nFlash JEDEC: ");
                        debugWriteHex32(JEDEC);
                        debugNewLine();
                        // printJEDEC(0);
                        // deselectFlash(0);

                        // readDataFlash(0, 1000, 0, (void*) &debugWriteHex8, NULL);
                        // debugNewLine();
                        // readDataFlash(340000, 1000, 0, (void*) &debugWriteHex8, NULL);
                        // debugNewLine();
                        // flashTest(0);
                        // softSpiDisable();
                        // softSpiInit();
                        flashTest(0);
                    }
                    fpgaFlashDisable();



                    break;



                    // leds
                case 'L':
                    // fpgaTestLeds();
                    // otherwise leds might be constantly on
                    fpgaEnableFlashInterface();
                    debugWriteLine("start timer");
                    // timerStart(0.002);
                    timerStart(10.0f);
                    ledsFlash(3);
                    timerStop();
                    float timervalue = timerGetFloatValue();
                    debugWriteString("Result: ");
                    debugWriteFloatFull(timervalue*1000.0f);
                    debugNewLine();
                    debugWriteDec16(timerGetValue());
                    debugNewLine();

                    debugDone();
                    break;

                    // // wireless learn
                    // case 'l':
                    // 	debugWriteLine("Wireless learn");
                    // 	wirelessPower(1);
                    // 	startExperiment(4.0f, 1);
                    // 	wirelessLearn(50);
                    // 	float dt = endExperiment(1);
                    // 	debugWriteFloatFull(dt);
                    // 	debugNewLine();

                    // 	debugDone();
                    // 	break;

                    // test matrix multiplication
                case 'M':
                    mmTest();
                    break;

                    // learn some ANN
                case 'm':
                    repeats = 1000;
                    debugWriteString("Running ");
                    debugWriteDec32(repeats);
                    debugWriteString(" repeats of ANN learning...");
                    for (uint32_t i = 0; i < repeats; i++)
                    {
                        stubAnnLearn(0x0F, 0x0);
                    }
                    debugWriteLine("Done...");
                    break;
                    // test local mm
                case 'n':
                    debugAck(currentData);
                    debugWriteLine("testing local mm");
                    matrixA = malloc(MM_INPUT_A_N * MM_INPUT_A_M * sizeof(uint32_t));
                    matrixB = malloc(MM_INPUT_B_N * MM_INPUT_B_M * sizeof(uint32_t));
                    matrixC = malloc(MM_INPUT_A_N * MM_INPUT_B_M * sizeof(uint32_t));

                    for (int k = 0; k < 1000; k++) {
                        for (int i = 0; i < MM_INPUT_A_N; i++) {
                            for (int j = 0; j < MM_INPUT_A_M; j++) {
                                matrixA[i * MM_INPUT_A_M + j] = 1 + (i * MM_INPUT_A_M) + j;
                            }
                        }

                        for (int i = 0; i < MM_INPUT_B_N; i++) {
                            for (int j = 0; j < MM_INPUT_B_M; j++) {
                                matrixB[i * MM_INPUT_B_M + j] = 1 + (i * MM_INPUT_B_M) + j;
                            }
                        }
                        mmLocal(matrixA, matrixB, matrixC);
                    }

                    debugWriteStringLength((uint8_t *) matrixC, sizeof(uint32_t) * MM_OUTPUT_N * MM_OUTPUT_M);


                    free(matrixA);
                    free(matrixB);
                    free(matrixC);

                    debugDone();

                    break;

                    // // wireless
                    // case 'W':
                    // 	wirelessTest(5);
                    // 	debugDone();
                    // 	break;

                    // fpga power cycle
                case 'P':
                    fpgaPowerTest(10);
                    break;

                    // test external flash
                case 'S':
                    fpgaFlashInit();

                    printJEDEC(0);
                    // // flashTest(0);
                    printStatus(0);

                    // debugWriteString("Writing...\n");

                    // eraseSectorFlash(0, 0);

                    // char *data = "test";

                    // // unlockFlash(0);
                    // for (uint16_t i = 0; i < 100; i++)
                    // {
                    // 	// if(flashQueueData(data[i % 4]))
                    // 	if(flashQueueData(i % 0xff))
                    // 	{
                    // 		flashWriteQueue(0, 0);
                    // 		// destination += 256;
                    // 	}
                    // }
                    // debugWriteString("writing to flash: ");
                    // debugWriteDec16(getFlashQueueCount());
                    // debugNewLine();
                    // flashWriteQueue(0, 0);

                    // _delay_ms(200);
                    readDataFlash(testingFlashAddress, 100, 0, (void *) &debugWriteHex8, NULL);
                    // _delay_ms(2000);

                    fpgaFlashDisable();

                    // debugNewLine();
                    debugDone();

                    break;

                    // test transfer rates
                case 'T':
                    beginTransferTest();
                    break;
                case 't':
                    beginLatencyTransferTest();
                    break;
                case 'V':
                    beginVariableTransferTest();
                    break;
                case 'v':
                    beginSectionTransferTest();
                    break;


                case 'x':
//			        fpgaSoftReset();
//                    stubAnnPrintData(1, 4000);
                    stubAnnXor();
                    break;

                case 'X':
                    debugWriteLine("Testing multiboot...");

                    fpgaMultiboot(0);
                    break;

                case 'z':
                    stubAnnPrintAll();
                    break;


                    // // wireless block
                    // case 'w':
                    // 	// debugWriteLine("Wireless block");
                    // 	// pushLatency(&latencyBuf, latency);

                    // 	// startExperimentIndicate();

                    // 	wirelessPower(1);
                    // 	deselectFlash(1);

                    // 	// calculate number of blocks to calculate
                    // 	num_blocks = ceil(((float) BATCH_SIZE_REMOTE) / BLOCK_SIZE_REMOTE);
                    // 	data_size = num_blocks * sizeof(float);

                    // 	// indicate how many latency bytes need to be received
                    // 	debugWriteStringLength((char *) &data_size, sizeof(uint32_t));

                    // 	for (uint32_t i = 0; i < num_blocks; i++)
                    // 	{
                    // 		latency = wirelessSendBlock(BLOCK_SIZE_REMOTE, 1);
                    // 		if (latency != -1)
                    // 			debugWriteStringLength((char *) (&latency), sizeof(float));
                    // 		else
                    // 			// redo this message
                    // 			i--;
                    // 		// pushLatency(&latencyBuf, latency);
                    // 	}

                    // 	// endExperimentIndicate();

                    // 	wirelessPower(0);

                    // 	break;

                default:
                    debugWriteLine("Unknown Test");
                    break;
            }
            ledsSetMode(ledsDebug);
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
            // 	}*/

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

                    // write to fpga flash
                case 'f':
                    debugAck(currentData);
                    // debugWriteLine("flash to flash...");
//					uartPtr = (uint8_t *) &configDataAddress;
//					currentUartReceiveMode = UART_CONFIG_ADDRESS;
//					currentLoadingMode = LOAD_FLASH_FPGA_FLASH;

                    configurationFlashFlash();

                    uartCount = 0;
                    break;

                    // write to flash*/
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
//					// configDataPtr = configDataAddress;
//					uartCount = 0;

                    // acknowledge when ready to receive again
                    debugAck(currentData);

                    configurationUartFlash();

                    break;

                /*case 'g':
                    debugWriteString("mem: ");
                    uint8_t *testPtr = malloc(10);
                    debugWriteHex16((uint16_t )testPtr);
                    debugNewLine();
                    free(testPtr);
                    break;
                    // // fetch all available current measurements
                    // case 'f':
                    // 	debugAck(currentData);
                    // 	printAllCurrentMeasurementsFloat();

                    // 	break;

                    // light sleep
                case 'h':
                    debugWriteLine("Sleep IDLE");

                    setLed(0, 0);
                    setLed(1, 0);
                    setLed(2, 0);
                    setLed(3, 0);

                    debugDisable();
                    set_sleep_mode(SLEEP_MODE_IDLE);
                    sleep_enable();
                    sleep_mode();
                    debugInit(NULL);
                    debugWriteLine("Woke up again...");
                    sleep_disable();

                    break;
                    // deep sleep
                case 'H':
                    debugWriteLine("Sleep DEEP");

                    setLed(0, 0);
                    setLed(1, 0);
                    setLed(2, 0);
                    setLed(3, 0);

                    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
                    sleep_enable();
                    sleep_mode();

                    debugWriteLine("Woke up again...");
                    sleep_disable();

                    break;
                case 'I':
                    debugAck(currentData);
                    jtagPrintIdCode();
                    break;

                    // configure jtag interface from uart data
                case 'J':
                    // jtagPrintIdCode();

                    jtagInitConfig();

                    currentLoadingMode = LOADING_JTAG;
                    currentUartReceiveMode = UART_CONFIG_SIZE;
                    configDataSize = 0;
                    uartPtr = (uint8_t *) &configDataSize;

                    // last = 0x0;
                    uartCount = 0;

                    configured = 0x0;
                    debugAck(currentData);
                    break;

                case 'j':
                    // Program FPGA from flash through JTAG
                    uartPtr = (uint8_t *) &configDataAddress;
                    currentUartReceiveMode = UART_CONFIG_ADDRESS;
                    currentLoadingMode = LOAD_FLASH_JTAG;
                    debugAck(currentData);
                    uartCount = 0;
                    break;

                case 'k':
                    debugAck(currentData);
                    debugWriteLine("Disabling JTAG interface");
                    disableJtagInterface();
                    break;
                case 'K':
                    debugAck(currentData);
                    debugWriteLine("Enabling JTAG interface");
                    enableJtagInterface();
                    break;

                    // control LED allowed or not
                case 'L':
                    enableLeds();
                    debugAck(currentData);
                    break;

                case 'l':
                    disableLeds();
                    debugAck(currentData);
                    break;

                case 'M':
                    startSelectMap();
                    // debugWriteLine("Selectmap started");

                    currentLoadingMode = LOADING_SELECTMAP;

                    debugWriteLine("Starting selectmap...");

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

                case 'm':
                    // multiboot
                    currentUartReceiveMode = UART_MULTIBOOT;
                    uartPtr = (uint8_t *) &multibootAddress;
                    multibootAddress = 0;
                    uartCount = 0;
                    debugAck(currentData);

                    break;


                case 'o':
                    // perform XOR Test
                    debugAck(currentData);
                    testXor();
                    break;

                    // Program FPGA From flash through selectmap
                case 'P':
                    debugAck(currentData);
                    // debugWriteLine("reading flash...");
                    uartPtr = (uint8_t *) &configDataAddress;
                    currentUartReceiveMode = UART_CONFIG_ADDRESS;
                    currentLoadingMode = LOAD_FLASH_SELECTMAP;
                    uartCount = 0;
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

                case 'q':
                    debugWriteLine("testing fir config");
                    fpgaPower(0);
                    fpgaSetDoneReponse(FPGA_DONE_PRINT);
                    fpgaMultibootClearComplete();
                    disableXmem();
                    fpgaEnableFlashInterface();
                    fpgaPower(1);
                    while(!fpgaMultibootComplete());

                    enableXmem();
                    fpgaSoftReset();
//    fpgaPrintHWFID();
                    uint8_t id = fpgaGetHWFID();
                    if (id != 0xdd)
                        debugWriteLine("ERROR: Cannot boot golden image!");
                    else
                        debugWriteLine("Dummy booted");
//    debugWriteHex8(id);
//    debugNewLine();


//        _delay_ms(500);
//    }

//    debugWriteLine("multi");
                    fpgaSoftReset();
                    fpgaMultiboot(FIR_ADDRESS);
                    fpgaSetDoneReponse(FPGA_DONE_PRINT);
                    while(!fpgaMultibootComplete());
                    debugWriteLine("Booted");
//    initXmem();
                    enableXmem();
                    fpgaSoftReset();
                    id = fpgaGetHWFID();
                    if (id != 0xbb) {
                        debugWriteLine("ERROR: Cannot boot FIR image!");
                        debugWriteString("HWF ID: ");
                        debugWriteHex8(id);
                        debugNewLine();
                    }
                    else
                        debugWriteLine("Booted FIR");
                    break;

                case 'r':
                    // perform a reset
                    debugAck(currentData);
                    debugWriteLine("Performing soft reset");
                    enableXmem();
                    fpgaSoftReset();
                    // enableXmem();
                    disableXmem();
                    debugDone();
                    break;

                case 'R':
                    debugAck(currentData);
                    debugWriteLine("Performing hard reset");
                    flashDisableInterface();
                    fpgaHardReset();
                    // give chance for fpga to load config from spi
                    fpgaEnableFlashInterface();
                    debugDone();
                    break;

                case 'S':
                    // suspend
                    debugAck(currentData);
                    debugWriteLine("Suspending FPGA");
                    fpgaSuspend(1);
                    break;

                case 's':
                    // suspend
                    debugAck(currentData);
                    debugWriteLine("Unsuspending FPGA");
                    fpgaSuspend(0);
                    break;

                    // test mw
                case 'T':
                    debugAck(currentData);
                    debugWriteLine("Testing MW...");

                    // go find out which test to run
                    currentUartReceiveMode = UART_TEST;
                    break;

                    // timer value
                case 't':
                    debugAck(currentData);
                    debugWriteLine("getting timer value");
                    float timer = timerGetValue();
                    debugWriteStringLength((char *) (&timer), 4);
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


//			    case 'w':
//			        debugAck(currentData);
//			        warmbootConfigurationFlash(1);
//			        break;

                    // turn wireless chip on/off
                case 'w':
                    debugAck(currentData);
                    debugWriteLine("Wireless off");
                    setUpPeripheral();
                    debugWriteLine("init");
                    initMrf();
                    debugWriteLine("disable");
                    disableWireless();
                    debugWriteLine("done");
                    break;
                case 'W':
                    debugAck(currentData);
                    debugWriteLine("Wireless on");
                    setUpPeripheral();
                    initMrf();
                    sendToCoordinator();
                    break;

//			    case 'w':
//			        debugAck(currentData);
//			        debugWriteLine("Disable wireless");
//                    disableWireless();
//			        break;

                case 'X':
                    debugAck(currentData);
                    _delay_ms(500);
                    fpgaDisableInterface();
                    _delay_ms(500);
                    disableXmem();
                    _delay_ms(500);
                    debugDone();
                    break;
                case 'x':
                    debugAck(currentData);
                    _delay_ms(500);
                    fpgaEnableInterface();
                    _delay_ms(500);
                    enableXmem();
                    _delay_ms(500);

//					for (int i = 0 ; i < 100; i++) {
//                        debugWriteHex8(*((uint8_t *) (XMEM_OFFSET + i)));
//                        debugWriteChar(' ');
//                    }
//                    debugNewLine();

                    debugDone();
                    break;

                    // case 'y':
                    // 	debugAck(currentData);
                    // 	enableXmem();
                    // 	annResetWeights();
                    // 	break;
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

                    // loading initial config
                case '/':
                    debugWriteLine("Loading initial config...");
                    initSelectMap();
                    startSelectMap();
                    readDataFlash(0, 340604, 1, &selectMapClockDataHelper, &selectMapFinishConfig);
                    debugWriteLine("Done with initial");
                    break;

                    // print hwf identifier
                case '\'':
                    debugAck(currentData);
                    fpgaSoftReset();
                    fpgaPrintHWFID();
                    debugDone();
                    break;

                    // return hwf identifier
                case 0x15:
                    debugAck(currentData);
                    fpgaSoftReset();
                    debugWriteChar(fpgaGetHWFID());
                    break;


                    // test mm
                case 0x14:
                    debugAck(currentData);
                    // debugWriteLine("testing fpga mm");
                    matrixA = malloc(MM_INPUT_A_N * MM_INPUT_A_M * sizeof(uint32_t));
                    matrixB = malloc(MM_INPUT_B_N * MM_INPUT_B_M * sizeof(uint32_t));
                    matrixC = malloc(MM_INPUT_A_N * MM_INPUT_B_M * sizeof(uint32_t));


                    for (int i = 0; i < MM_INPUT_A_N; i++)
                    {
                        for (int j = 0; j < MM_INPUT_A_M; j++)
                        {
                            matrixA[i * MM_INPUT_A_M + j] = 1 + (i * MM_INPUT_A_M) + j;
                        }
                    }

                    for (int i = 0; i < MM_INPUT_B_N; i++)
                    {
                        for (int j = 0; j < MM_INPUT_B_M; j++)
                        {
                            matrixB[i * MM_INPUT_B_M + j] = 1 + (i * MM_INPUT_B_M) + j;
                        }
                    }
                    mmOffloadFpga(matrixA, matrixB, matrixC);

                    debugWriteStringLength((uint8_t *) matrixC, sizeof(uint32_t) * MM_OUTPUT_N * MM_OUTPUT_M);


                    free(matrixA);
                    free(matrixB);
                    free(matrixC);

                    debugDone();

                    break;

                    // read softspi jedec
                case 0x16:
                    debugAck(currentData);
                    jedec = readJEDEC(0);
                    debugWriteStringLength((uint8_t *) &jedec, sizeof(uint32_t));
                    debugDone();
                    break;

                    // verify softspi access
                case 0x17:
                    debugAck(currentData);

                    fpgaFlashInit();

                    eraseSectorFlash(testingFlashAddress, 0);

                    // char *data = "test";
                    // unlockFlash(0);
                    flashResetQueue();
                    for (uint16_t i = 0; i < 100; i++)
                    {
                        // assume here buffer won't fill
                        flashQueueData(i % 0xff);
                    }
                    flashWriteQueue(testingFlashAddress, 0);

                    debugReady();

                    // readDataFlash(testingFlashAddress, 100, 0, (void *) &debugWriteCharHelper, &softSpiDisable);
                    data = readDataFlash(testingFlashAddress, 100, 0, NULL, NULL);
                    fpgaFlashDisable();
                    debugWriteStringLength(data, 100);

                    // _delay_ms(2000);

                    // softSpiDisable();

                    // // debugNewLine();
                    // debugDone();

                    break;

                    // verify softspi access
                case 0x18:
                    debugAck(currentData);

                    fpgaFlashInit();

                    eraseSectorFlash(testingFlashAddress, 0);
                    readDataFlash(testingFlashAddress, 100, 0, (void *) &debugWriteChar, &fpgaFlashDisable);

                    break;

                    // verify soft spi lock bytes
                case 0x19:
                    debugAck(currentData);
                    fpgaFlashInit();

                    lockData = readLockFlash(0);
                    debugWriteStringLength((char *)lockData, 18);
                    fpgaFlashDisable();

                    debugDone();

                    break;

                    // test write enable soft spi
                case 0x1A:
                    debugAck(currentData);
                    fpgaFlashInit();
                    // debugWriteChar(readStatus(0));
                    writeEnableFlash(0);
                    debugWriteChar(readStatus(0));

                    fpgaFlashDisable();
                    debugDone();
                    break;

                    // read softspi jedec
                case 0x26:
                    debugAck(currentData);

                    printJEDEC(1);
                    jedec = readJEDEC(1);
                    debugWriteHex32(jedec);
                    debugNewLine();

                    break;

//				// verify softspi access
//				case 0x28:
//					debugAck(currentData);
//
//					eraseSectorFlash(testingFlashAddress, 1);
//
//					// char *data = "test";
//					// unlockFlash(0);
//					flashResetQueue();
//					for (uint16_t i = 0; i < 100; i++)
//					{
//						// assume here buffer won't fill
//						flashQueueData(i % 0xff);
//					}
//					flashWriteQueue(testingFlashAddress, 1);
//
//					debugReady();
//
//					readDataFlash(testingFlashAddress, 100, 1, (void *) &debugWriteCharHelper, NULL);
//					// _delay_ms(2000);
//
//					// softSpiDisable();
//
//					// // debugNewLine();
//					// debugDone();
//
//					break;

                    // verify softspi access
                case 0x28:
                    debugAck(currentData);

                    eraseSectorFlash(testingFlashAddress, 1);
                    readDataFlash(testingFlashAddress, 100, 1, (void *) &debugWriteCharHelper, NULL);

                    break;

                    // verify soft spi lock bytes
                case 0x29:
                    debugAck(currentData);

                    lockData = readLockFlash(1);
                    debugWriteStringLength((char *)lockData, 18);

                    debugDone();

                    break;

                    // test write enable
                case 0x2A:
                    debugAck(currentData);

                    // debugWriteChar(readStatus(1));
                    writeEnableFlash(1);
                    debugWriteChar(readStatus(1));

                    // debugDone();
                    break;

                    // write data for verify test
                case 0x2B:
                    debugAck(currentData);

                    debugWriteLine("Going to write test data...");

                    setLed(1, 1);
                    uint16_t PAGE_SIZE = 256;
                    data = (uint8_t *) malloc(PAGE_SIZE);
                    uint8_t *dataPtr = data;
                    for (int i = 0; i < PAGE_SIZE; i++)
                    {
                        *dataPtr++ = i;
                    }

                    for (uint32_t address = testingFlashAddress; address < (testingFlashAddress + 0x60000); address += PAGE_SIZE)
                    {
                        writeDataFlash(address, data, PAGE_SIZE, 1);
                    }

                    free(data);
                    setLed(1, 0);

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

                case '\\':
                    debugAck(currentData);
                    debugWriteLine("All LEDs high");
                    fpgaControlLeds(0xf);
                    for(int i = 0; i < 4; i++)
                    {
                        setLed(i, 1);
                    }
                    break;

                    // // test mrf integration
                    // case 0x19:
                    // 	debugAck(currentData);

                    // 	setUpPeripheral();
                    // 	// setUpUsbSerial();
                    // 	initMrf();

                    // 	_delay_ms(1000);
                    // 	printTxMemory(18);

                    // 	while(1) {
                    // 		_delay_ms(1000);
                    // 		sendToCoordinator();

                    // 		_delay_ms(50);
                    // 		printTxMemory(18);
                    // 	}

                    // setUpMac();
                    // Mac802154Config config = {
                    // 		.channel = 0xC,
                    // 		.pan_id = {0xcc, 0xdd},
                    // 		.short_source_address = {0xAA, 0xAA},
                    // 		.extended_source_address = {
                    // 			0x11, 0x22,
                    // 			0x33, 0x44,
                    // 			0x55, 0x66,
                    // 			0x77, 0x88,
                    // 		},
                    // };
                    // Mac802154_configure(mac802154, &config);

                    // char payload[32];
                    // Mac802154_setShortDestinationAddress(mac802154, (uint8_t[]){0xAB, 0xCD});

                    // uint16_t number = 0;
                    // for(int i = 0;i < 10; i++) {
                    // number++;
                    // memset(payload, 0, 32);
                    // sprintf(payload, "%d\n", number);
                    // uint8_t payload_length = (uint8_t) strlen(payload);
                    // Mac802154_setPayload(mac802154, payload, payload_length);
                    // Mac802154_sendBlocking(mac802154);
                    // }
                    break;

                    // indicate readiness*/
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


