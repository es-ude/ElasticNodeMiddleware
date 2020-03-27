# Elastic Node Middleware

## Getting Started

The [docs](docs/) folder contains a [getting started guide](docs/ElasticNodeGettingStartedGuide.md).

## Hardware

The used hardware is listed in the [Elastic Node Hardware repository](https://github.com/es-ude/ElasticNodeHardware) on github.
-----> v4 gibt es da nicht?! Error!

## Libraries

The elastic node middleware code includes several libraries:
- elastic node middleware
- reconfigure multiboot avr
- xmem
- pin definition
- interrupt Manager
- uart

They are described in the following

### Elastic Node Middleware Lib

The elastic node middleware lib consists of the elastic node middleware interface as well as the elastic node middleware configure FPGA interface and the internal interface.
Application user should only use the elastic node middleware interface and the elastic node middleware configure FPGA interface of the whole code. (?)
The outsourcing in internal interfaces are only for testing purposes.
The elastic node middleware lib contains the following functions:

- for initialsing the elastic node middleware:\
→ elasticnode_initialise
- for turning the FPGA on:\
→ elasticnode_fpgaPowerOn
- for turning the FPGA off:\
→ elasticnode_fpgaPowerOff
- to read and write one Byte or a data stream from the FPGA:\
→ elasticnode_writeOneByteBlockingFromFpga\
→ elasticnode_writeDataBlockingFromFpga\
→ elasticnode_readOneByteBlockingFromFpga\
→ elasticnode_readDataBlockingFromFpga
- to reset the FPGA:\
→ elasticnode_fpgaSoftReset\
→ elasticnode_fpgaHardReset

(difference soft and hard reset -> explain)

The elastic node middleware configure FPGA lib contains the following functions:

- reconfigure FPGA to specific state:\
→ elasticnode_configureFPGA_configureFrom
- get current loaded configuration:\
→ elasticnode_configureFPGA_getLoadedConfiguration

We split these application functions into two interfaces.
Unfortunately, this is necessary to avoid cycle dependencies between the elasticnode middleware lib and the reconfiguration lib.

### reconfigure multiboot avr

The reconfigure multiboot avr lib handles the reconfiguration for the FPGA.
The library is specialised for avr, because we use an avr-microcontroller in the elastic node.
Multiboot is here the type of reconfiguration.
Using multi-boot means that we select one configuration and provide the address of the next configuration to the internal configuration access port (ICAP) interface of the FPGA.

### xmem
### pin definition
### interrupt Manager
### uart

## Additional information

The Elastic Node is descriped in detail in the following [paper](https://ieeexplore.ieee.org/document/8831207).

--> was kann der Code? Was fehlt?


	Auf Elastic Node v4 Hardware verweisen (Github repo?)
	Beschreibung des Speichermodels, How to wie das memory mapped interface benutzt wird um mit dem FPGA zu interagieren.
	Unter anderem : Was kann ich mit der Middleware tun, welche Fähigkeiten hat es.
	I.e. feature list
	Auf ICAC paper verweisen	Was kann ich
	auch beinhalten: Wie stecke ich es an, was muss ich tun
	Wie kommt man an eine Bitfile: gibts existierende, wie sind die aufgebaut
    Documentation