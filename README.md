# Elastic Node Middleware

This repository includes the elastic node middleware code  as described in the paper [The Elastic Node: An Experimental Platform for Hardware Accelerator Research in the Internet of Things](https://ieeexplore.ieee.org/document/8831207).
The included code is written in C and Python. The code is tested under the operating system Ubuntu 18.04.4.

## Hardware

The used hardware is listed in the [Elastic Node Hardware repository](https://github.com/es-ude/ElasticNodeHardware) on github.
-----> v4 nonexistent?! Error!

In the [getting started guide](docs/GettingStartedGuide.md) is explained which hardware is needed as well as the connection to each other.

## Features

The middleware code includes the following functionalities:
- initialize the FPGA
- turn the FPGA on and off
- write and read to and from the FPGA
- enable and disable the external memory interface for the interconnection between the MCU and the FPGA
- reconfigure the FPGA
- upload a bitfile to the FPGA

## Installation Guide

For using the elastic node middleware code you need to install some tools.
Therefore, please follow the [installation guide](docs/InstallationGuide.md) in the [docs](docs) folder.

## Getting Started

The [docs](docs) folder contains a [getting started guide](docs/GettingStartedGuide.md) with mini examples of how to use the code.

## Write your own program

To write your own program code you should look into the [write your own program - guide](docs/WriteOwnProgramGuide.md).

## How-to's

The ["How-to" - guide](docs/HowToGuide.md) includes several short explanations about how to:
- configure the FPGA
- build a new bitfile for reconfiguring the FPGA
- missing .....

## Interconnection between the MCU and FPGA - the Memory-mapped Interface (Xmem)

The required hardware at the elastic node, which is used in our code, is the MCU and the FPGA.
For interconnection of the MCU and the FPGA we use the MCU's external memory interface (xmem).

The Xmem interface is an addressable data interface.
For using it, you define a variable at a specific memory location and write the data value into it.
The FPGA detects this and reacts accordingly.
The interconnection between MCU and FPGA starts in our code at the internal memory address 0x2000 from the MCU.

The using of the Xmem interface is shown in example in the [getting started guide](docs/GettingStartedGuide.md).

## Bitfile

A FPGA is an type of integrated circuit which is able to instantiate circuits at runtime.
A bitfile stores a specific circuit for the FPGA.
So, a bitfile says how the programmable logic blocks of a FPGA need to be set. 
Therefore, when we using a bifile, the FPGA can be reconfigured to instantiate a different circuit.
As we want to reconfigure the FPGA at the Elastic Node, we need bitfiles. 
An example bitfile is given in the project: [dummy.bit](bitfiles/dummy.bit).

The using of a bitfile is again shown in the [getting started guide](docs/GettingStartedGuide.md).

## Libraries

The elastic node middleware code includes several libraries:
- configuration
- debug
- elastic node middleware
- flash
- fpga flash
- interrupt Manager
- pin definition
- reconfigure multiboot avr
- spi
- uart
- uartmanger?!
- xmem
- Bitmanipulation
- Lufa Usart

In the file [graph_uartmanager.png](graph_uartmanager.png) you can see the dependencies between these libraries. 

They are described in the following.

### Configuration

The configuration library configures the FPGA for uploading a bitfile to it. 

### Debug

The debug library can be used for debugging the code.
For example you can print a something for your output.
You can debug via Uart or Lufa.
We recommend you to debug over Lufa because Lufa is needed for uploading a bitfile to the FPGA. 

### Elastic Node Middleware

The elastic node middleware library consists of the elastic node middleware interface as well as the elastic node middleware configure FPGA interface and the internal interface.
The outsourcing in internal interfaces are only for testing purposes.
The elastic node middleware interface contains the following functions:

- for initialising the fpga:\
→ elasticnode_initialise
- for turning the FPGA on:\
→ elasticnode_fpgaPowerOn
- for turning the FPGA off:\
→ elasticnode_fpgaPowerOff
- to read and write one byte or a data stream from the FPGA:\
→ elasticnode_writeOneByteBlockingFromFpga\
→ elasticnode_writeDataBlockingFromFpga\
→ elasticnode_readOneByteBlockingFromFpga\
→ elasticnode_readDataBlockingFromFpga
- to reset the FPGA:\
→ elasticnode_fpgaSoftReset\
→ elasticnode_fpgaHardReset

A soft reset refreshes the address of the external memory offset, whereby a hard reset refreshs the program pins. 

The elastic node middleware configure FPGA interface contains the following functions:

- reconfigure FPGA to specific state:\
→ elasticnode_configureFPGA_configureFrom
- get current loaded configuration:\
→ elasticnode_configureFPGA_getLoadedConfiguration

We split these application functions into two interfaces.
Unfortunately, this is necessary to avoid cycle dependencies between the elasticnode middleware library and the reconfiguration multiboot avr library.

### Flash

The Flash library is used for handling with the Flash of the FPGA.
The Flash of the FPGA is needed for the communication between the FPGA and the MCU. 

### Interrupt Manager

The Interrupt manager library clears and set interrupts. 
Our interrupts are again implemented for avr. 

### Pin Definition

The Pin definition library defines all pin and register definitions.
Therefore, all pin and register definitions are at one point in the code. 

### Reconfigure Multiboot Avr

The Reconfigure Multiboot Avr library handles the reconfiguration for the FPGA.
The library is specialised for avr, because we use an avr-microcontroller in the elastic node.
Multiboot is here the type of reconfiguration.
Using multiboot means that we select one configuration and provide the address of the next configuration to the internal configuration access port (ICAP) interface of the FPGA.

The reconfigure multiboot avr library contains an reconfigure_multiboot_avr interface and for this an internal interface. 
The functions are:

- for initialising the reconfiguration:\
→ reconfigure_initMultiboot
- for reconfiguring the FPGA with multiboot technique:\
→ reconfigure_fpgaMultiboot
- for the interrupt service routine, which checks for reconfiguration:\
→ reconfigure_interruptSR
- for returning the current offset from the address in the memory:\
→ reconfigure_getMultibootAddress
- for returning if the reconfiguration is complete:\
→ reconfigure_fpgaMultibootComplete

### Spi

The Spi library implements the [Serial Peripheral Interface](https://de.wikipedia.org/wiki/Serial_Peripheral_Interface) for the FPGA.

### Uart

The uart library is used for communication with the MCU. 
It is only used for testing purposes. 

### Uartmanager 

The Uartmanager library can be used for using different elasticnode middleware function, e.g. for uploading a bitfile to the FPGA.

### Xmem

The xmem library initializes, enables and disables the external memory interface. 
The external memory is used to interconnect between the MCU and the FPGA as discussed below.
The functions of the xmem library are:

- for initialising the external memory:\
→ xmem_initXmem
- for enabling the external memory:\
→ xmem_enableXmem
- for disabling the external memory:\
→ xmem_disableXmem

In the header file of the xmem library we define the offset of the external memory.
By using the elastic node, the offset is 0x2000. 

### Bitmanipulation

The Bitmanipulation library is an external library from the [Embedded Utilities repository](https://github.com/es-ude/EmbeddedUtilities).
It is used for setting and clearing bits.
Please note that the include statements looks different to the other libraries (see [Write your own program guide](docs/WriteOwnProgramGuide.md)).

### Lufa Usart 

The Lufa Usart library is also an external library from the [Peripheral Interface repositry](https://github.com/es-ude/PeripheralInterface).
Lufa is a USB library for USB-enabled AVR microcontrollers and explained at [fourwalledcubicle](http://www.fourwalledcubicle.com/files/LUFA/Doc/120219/html/index.html).
Again, please note that the include statements looks different to the other libraries (see [Write your own program guide](docs/WriteOwnProgramGuide.md)).

## Future Work

As mentioned above the Elastic Node is described in detail in the paper [The Elastic Node: An Experimental Platform for Hardware Accelerator Research in the Internet of Things](https://ieeexplore.ieee.org/document/8831207).
By comparing our implemented features with the features of the paper, we detect some features that are not integrated in our basic version yet.
We list them in the following: 
- Monitoring: We do not have include the monitoring of the current usage yet. 
The monitoring includes measuring the energy/power consumption. 
- Flash: We do not include the code for using the flash memory of the MCU (only of the Fpga).
The flash memory is for additional data storage. 
- Stub/Skeleton Generation: We do not have generation of the abstraction interface on the MCU (Stub) as well as on the FPGA (Skeleton) in this basic code.