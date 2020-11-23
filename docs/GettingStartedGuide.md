# Getting Started Guide

## Hardware

The following photos show the elastic node. 
The left yellow rectangle is the FPGA while the right yellow rectangle is the used MCU. 
The left red rectangle at the top shows the FPGA LEDs. 
The right red rectangle at the top shows the MCU LEDs. 
These MCU LEDs should blink in the blink example whereby the FPGA Leds should blink by using the control manager. 
The right red rectangle shows the connection to the MCU via a programmer whereas the right red circle is the connection of the elastic node MCU to your computer.

<img src="https://github.com/es-ude/ElasticNodeMiddleware/blob/master/docs/images/elasticNodeFrontEdit3.jpg" width="600">
<img src="https://github.com/es-ude/ElasticNodeMiddleware/blob/master/docs/images/elasticNodeBack.jpg" width="600">

The elastic node is connected to a programmer with 6 jumper wires female to female.
In the following pictures this is the grey cable. 
The connected elastic node and programmer are shown in the following image.
Please note which jumper wire connect to a pin at the elastic node is connected to which pin at the programmer.

<img src="https://github.com/es-ude/ElasticNodeMiddleware/blob/master/docs/images/elasticNode.jpg" width="600">
<img src="https://github.com/es-ude/ElasticNodeMiddleware/blob/master/docs/images/programmerEdit.jpg" width="600">

The programmer is connected with your computer with an USB-to-MiniUSB cable. 
The MiniUSB is connected to the programmer and the USB is connected to your computer.
The elastic node itself is connected with your computer via an USB-to-MicroUSB cable.
The micro-USB cable is plugged in the elastic node at the place which is circled red in the first image above.
This place is labeled with MCU_USB. 
The USB is plugged in your computer. 

If you want to communicate via Uart you need a FTDI-adapter.
We don't need this communication, it is only for testing purposes.
This adapter is connected with the elastic node with 3 jumper wires male to female.
The receive (RXI, brown cable) and transmit (TXD, red cable) pins of the FTDI-adapter are connected with the uart pins.
The ground (GND, black cable) pin of the FTDI-adapter is connected to the ground pin of the elastic node (see picture above).
The FTDI-adapter has again a connection to the computer like the programmer with a USB-to-MiniUSB cable. 
The MiniUSB is connected to the FTDI-adapter and the USB is connected to your computer.

**Important:** The communication via Uart with an FTDI-adapter is not explained and used in the following descriptions. 
It is not needed for using the elastic node middleware.

<img src="https://github.com/es-ude/ElasticNodeMiddleware/blob/master/docs/images/ftdiAdapter.jpg" width="600">

The following photo shows the setup after connecting the hardware.
<img src="https://github.com/es-ude/ElasticNodeMiddleware/blob/master/docs/images/construction2.jpg" width="600">
 
### DIP Switches
The four switches on the Elastic Node are usally set as in the picture (OFF,ON,ON,OFF). When you have trouble uplading something to the Elastic Node turn off the third switch while uplading.

## How to use the Code

For using the elastic node middleware code you have to know, which device uses which port.
With Ubuntu and macOS you can run

    $ ls /dev

before and after you plug in the devices and the newly added device is the needed port.
In our case the programmer is "ttyACM0" and the elastic node itself is "ttyACM1", this ports will be set as standard in all files.

### Build your own Program

To use the elastic node middleware as a library, you can write your own program using the [WriteOwnProgramGuide](WriteOwnProgramGuide.md).

### Clone repository

If you want to modify something in the middleware code itself for you project you can clone the code using the [CloneGuide](docs/CloneGuide.md).


