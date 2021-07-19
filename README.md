# Elastic Node Middleware

This repository includes the elastic node middleware code as described in the paper 
[The Elastic Node: An Experimental Platform for Hardware Accelerator Research in the Internet of Things](https://ieeexplore.ieee.org/document/8831207).
The included code is written in C and Python. The code is tested under the operating system Ubuntu 18.04.4.

## Hardware

The used hardware is listed in the [Elastic Node Hardware repository](https://github.com/es-ude/ElasticNodeHardware).
We use the elastic node version 4 (v4).
For hardware details please refer to this repository. 

The [getting started guide](docs/GettingStartedGuide.md) explains which hardware components are needed and how to connect the hardware.

## Features

The middleware code includes the following functionalities:
- initialize the FPGA
- turn the FPGA on and off
- write and read to and from the FPGA
- enable and disable the external memory interface for the interconnection between the MCU and the FPGA
- reconfigure the FPGA and switch between different bitfiles
- upload a bitfile to the FPGA

## Documentation

### Installation Guide

For using the elastic node middleware code some tools are a perquisite.
Therefore, please follow the instructions in the [Installation Guide](docs/InstallationGuide.md) in the [docs](docs) directory.

### Getting Started

The [docs](docs) directory contains a [Getting Started Guide](docs/GettingStartedGuide.md) with further information's.

### Write your own application

For writing an own application for the Elastic Node the [Write Own Program Guide](docs/WriteOwnProgramGuide.md) should be used.

### Clone repository 

For using the repository while changing the code of the libraries it can be cloned with the help of the [Clone Guide](docs/CloneGuide.md).

### Docker

How to use a docker container for building and running the elastic node middleware is explained in the file [Docker](docs/Docker.md).

## Interconnection between the MCU and FPGA - the Memory-mapped Interface (Xmem)

The required hardware at the elastic node, which is used in our code, is the MCU and the FPGA.
For interconnection of the MCU and the FPGA we use the MCU's external memory interface (Xmem).

The Xmem interface is an addressable data interface.
To use it, a variable is defined at a specific memory location and the data value is written into it.
The FPGA detects this and reacts accordingly.
The interconnection between MCU and FPGA starts at the internal memory address 0x2000 of the MCU.

## Bitfiles

A FPGA is a type of integrated circuit which is able to instantiate circuits at runtime.
A bitfile stores a specific circuit for the FPGA.
So, a bitfile says how the programmable logic blocks of a FPGA need to be set. 
Therefore, when we using a bifile, the FPGA can be reconfigured to instantiate a different circuit.
As we want to reconfigure the FPGA at the elastic node, we need bitfiles. 
Example bitfiles can be found in the [bitfiles](bitfiles) directory.

When following the [Write Own Program Guide](#write-your-own-application) bitfiles can be uploaded to the elastic node.

## Libraries

The Elastic Node Middleware Library provides all necessary functions for using the Elastic Node 
by including the other libraries internally. 
All function can be found in the [ElasticNodeMiddleware.h](ElasticNodeMiddleware/ElasticNodeMiddleware.h).

### Always included

#### Led 

The Led library can be used for turning the MCU-LEDs at the elastic node on and off. 

#### Xmem

The xmem library initializes, enables and disables the external memory interface. 
The external memory is used to interconnect between the MCU and the FPGA.

In the header file of the xmem library we define the offset of the external memory.
By using the elastic node, the offset is 0x2000.

#### Reconfigure Multiboot Avr

The Reconfigure Multiboot Avr library handles the reconfiguration of the FPGA.
For now the library is specialised for avr, because we use an avr-microcontroller up to the elastic node version 4.
Multiboot is here the type of reconfiguration.
Using multiboot means that we select one configuration 
and provide the address of the next configuration 
to the internal configuration access port (ICAP) interface of the FPGA.

#### Interrupt Manager

The Interrupt manager library clears and sets interrupts.

#### Pin Definitions

The Pin definition library defines all pin and register definitions. 
Therefore, all pin and register definitions are at one point in the code. 

### Only included with the DEBUG target

#### Debug

The debug library provides different debug commands. Debugging is available via UART or LUFA.
It is recommend to debug with LUFA as it needed for uploading a bitfile to the FPGA. 

#### UART

The UART library is one option for communication with the MCU when using the Debug library. 

#### Controlmanager 

The Controlmanager library has different basic elastic node middleware functions, 
especially for starting the for uploading of bitfile to the FPGA.

- FlashFPGA: uploads bitfiles 
- i: print the current User logic ID

#### Configuration

The configuration library configures the FPGA for uploading a bitfile to it.

#### Flash

The Flash library is used for handling with the Flash memory of the FPGA.
The Flash of the FPGA is needed for the communication between the FPGA and the MCU. 

#### Spi

The Spi library implements the [Serial Peripheral Interface](https://en.wikipedia.org/wiki/Serial_Peripheral_Interface) for the FPGA.

### External Libraries

#### Bitmanipulation

The Bitmanipulation library is an library from the [Embedded Utilities repository](https://github.com/es-ude/EmbeddedUtilities).
It is used for setting and clearing bits.

#### LUFA

The Lufa library is an library from the 
[Peripheral Interface repositry](https://github.com/es-ude/PeripheralInterface).
Lufa is a USB library for USB-enabled AVR microcontrollers and explained at 
[fourwalledcubicle](http://www.fourwalledcubicle.com/files/LUFA/Doc/120219/html/index.html).
It is one option for communication with the MCU when using the Debug library. 

## Monitoring

The [Energy Monitoring Guide](docs/EnergyMonitoring.md) explains how to monitor the energy consumption via the monitoring MCU.
It uses the [Elastic Node Monitoring Repository](https://github.com/es-ude/ElasticNodeMonitoring).

## Future Work

As mentioned above the elastic node is described in detail in the paper 
[The Elastic Node: An Experimental Platform for Hardware Accelerator Research in the Internet of Things](https://ieeexplore.ieee.org/document/8831207).
By comparing our implemented features with the features of the paper, we detect some features that are not integrated in our basic version yet.
We list them in the following: 
- Flash: We do not include the code for using the flash memory of the MCU (only of the FPGA).
The flash memory is for additional data storage. 
- Stub/Skeleton Generation: We do not have generation of the abstraction interface on the MCU (Stub) as well as on the FPGA (Skeleton) in this basic code.
- Arm elastic node: We will publish the elastic node middleware code for the elastic node ARM.
This elastic node has an ARM MCU instead of an AVR MCU.
