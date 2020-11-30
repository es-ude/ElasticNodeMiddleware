# Getting Started Guide

## Hardware

The following photos show the elastic node front and back. 

<p align="center">
  <img src="https://github.com/es-ude/ElasticNodeMiddleware/blob/master/docs/images/elasticNodelabeled.jpg" width="400" height="400">
  <img src="https://github.com/es-ude/ElasticNodeMiddleware/blob/master/docs/images/elasticNodeBack.jpg" width="400" height="400"> 
</p>

The elastic node is connected to a programmer with 6 jumper wires female to female.
In the following pictures this is the grey cable. 
The connected elastic node and programmer are shown in the following image.
Please note which jumper wire connect to a pin at the elastic node is connected to which pin at the programmer.
The programmer is connected with your computer with an USB-to-MiniUSB cable.

<p align="center">
<img src="https://github.com/es-ude/ElasticNodeMiddleware/blob/master/docs/images/elasticNode.jpg" width="400" height="400">
<img src="https://github.com/es-ude/ElasticNodeMiddleware/blob/master/docs/images/programmerEdit.jpg" width="400" height="400">
</p>
 
If you want to communicate via Uart you need a FTDI-adapter.
We don't need this communication, it is only for testing purposes.
This adapter is connected with the elastic node with 3 jumper wires male to female.
The receive (RXI, brown cable) and transmit (TXD, red cable) pins of the FTDI-adapter are connected with the uart pins.
The ground (GND, black cable) pin of the FTDI-adapter is connected to the ground pin of the elastic node (see picture above).
The FTDI-adapter has again a connection to the computer like the programmer with a USB-to-MiniUSB cable. 

<p align="center"> <img src="https://github.com/es-ude/ElasticNodeMiddleware/blob/master/docs/images/ftdiAdapter.jpg" width="400" height="400"> </p>

The elastic node itself is connected with your computer via an USB-to-MicroUSB cable. You see the MicroUSB labeled in the first image.
The following photo shows the setup after connecting the hardware.

<p align="center"> <img src="https://github.com/es-ude/ElasticNodeMiddleware/blob/master/docs/images/construction2.jpg" width="600"> </p>

### DIP Switches

The four switches on the Elastic Node are usually set as in the first picture (OFF,ON,ON,OFF). When you have trouble uploading something to the Elastic Node turn off the third switch while uploading.

## How to use the Code

For using the elastic node middleware code you have to know, which device uses which serial port. 
In the following they will be refer to as ports.
With Ubuntu and macOS you can run

    $ ls /dev

before and after you plug in the devices and the newly added device is the needed port.
In our case the programmer is "ttyACM0" and the elastic node itself is "ttyACM1", this ports will be set as standard in all files.

### Build your own Program

To use the elastic node middleware as a library, you can write your own program using the [WriteOwnProgramGuide](WriteOwnProgramGuide.md).

### Clone repository

If you want to modify something in the middleware code itself for you project you can clone the code using the [CloneGuide](docs/CloneGuide.md).


