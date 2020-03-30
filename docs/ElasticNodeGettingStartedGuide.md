# Getting Started Guide

In the following we explain how to use the elastic node middleware.
Therefore, we show how to connect the hardware and how to run a minimal example.

## How to connect the Hardware

To use the Elastic Node Middleware Code, you have to connect your computer with the hardware.
For using the elastic node middleware code you need the elastic node and a programmer.
If you want to use UART for other communication, you need a UART device additionally.
On top of this, you need a USB-to-MicroUSB (?) cable and ... (cable for programmer, GPIO cable?).
You first put the USB port in your USB port at your computer and the MicroUSB port in the port at the elastic node.
In doing so you should use the MicroUSB port for the MCU and not for Monitoring (MON).
Then you connect the ... cable with the MCU program pins for the MCU, whereby on the other side you connect the programmer with it.
The following photo shows the construction.

... foto?

## How to use the Code

In the [main.c](../app/main.c) we show a minimal working example of how to use the elastic node middleware. 
We initialise the middleware and ...

## Build and Upload Command

For building the minimal example in the [main.c](../app/main.c) you have to run the following command: 

    $ bazel build //app:main --platforms=@AvrToolchain//platforms:ElasticNode_v4
    
The upload script is specialized in the [BUILD.bazel](../app/BUILD.bazel) in the app folder. 
For running the upload script you have to run: 

	$ bazel run //app:_mainUpload --platforms=@AvrToolchain//platforms:ElasticNode_v4
    