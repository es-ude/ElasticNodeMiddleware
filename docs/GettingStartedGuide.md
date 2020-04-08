# Getting Started Guide

In the following we explain how to use the elastic node middleware code.
Therefore, we show how to connect the hardware and how to run a minimal example as well as the needed commands for building and uploading the target.

## How to connect the Hardware

To use the Elastic Node Middleware Code, you have to connect your computer with the hardware.
You first put the USB port in your USB port at your computer and the MicroUSB port in the port at the elastic node.
In doing so you should use the MicroUSB port for the MCU and not for Monitoring (MON).
The circle in the photo above shows the right MicroUSB port. 
Then you connect the ... cable with the MCU program pins for the MCU (seen in the above image in the bottom right corner), whereby on the other side you connect the programmer with it.

The following photo shows the construction after connect the whole hardware.
 
 --> missing 
 
## How to use the Code

In the [main.c](../app/main.c) we show a minimal working example of how to use the elastic node middleware. 
The implementation uses Bitmanipulation functions. 
These Bitmanipulation functions come from an external library.
The functions just set or clear a bit in the transferred byte_ptr at the transferred offset. 

Missing!

## Using the Bitfile

Missing!

## Build and Upload Command

For building the minimal example in the [main.c](../app/main.c) you have to run the following command: 

    $ bazel build //app:main --platforms=@AvrToolchain//platforms:ElasticNode_v4

Thereby is the second word the command (here: "build") and the third word is the path to the file (here: main), we want to build. 
The leftover command are bazel flags for specializing the platform.  
    
The upload script is specialized in the [BUILD.bazel](../app/BUILD.bazel) in the app folder. 
For running the upload script you have to run: 

	$ bazel run //app:_mainUpload --platforms=@AvrToolchain//platforms:ElasticNode_v4
    
## Tests

If you want to run a test of the code (here for example the xmem_Test), you have to run:

    $ bazel test test:xmem_Test

If you want to run all tests:

    $ bazel test test:all