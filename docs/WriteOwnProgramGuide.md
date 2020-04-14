# Write your own Program

This guide shows you how to write your own program in the elastic node middleware code. 
We assume that for example your implementation is in the folder "myFolder" in the files "myImplementation.h" and "myImplementation.c".
Then you have to create a fitting BUILD.bazel file for building and uploading your implementation.

Alternatively, you can use the example "main.c" file and modify this file for your purposes. 

## Your BUILD.bazel File

Your BUILD.bazel file should be in "myFolder" where your implementation is stored.
The file should begin with the following two lines:
    
    load("@AvrToolchain//:helpers.bzl", "default_embedded_binaries", "default_embedded_binary")
    load("@AvrToolchain//platforms/cpu_frequency:cpu_frequency.bzl", "cpu_frequency_flag")
    
After that you create your own embedded binary.
You should give it a name and define the sources of your implementation as well as the compiler options with the copts command.
Then you define the uploader.
This upload script is defined at the end of this BUILD.bazel file.  
At least you should define your dependencies.
These are the libraries which you want to use in your implementation. 
The libraries are named in the [README](../README.md).
The "//app/setup:Setup" should always be a dependency.

In this example we include all possible libraries to show you the possibilities. 

    default_embedded_binary(
        name = "myImplementation",
        srcs = ["myImplementation.c"],
        copts = cpu_frequency_flag(),
        uploader = "Avr_dude_upload_script",
        deps = [
            "//:BitmanipulationLib",
            "//:ElasticNodeMiddlewareLib",
            "//:Reconfigure_multibootLib",
            "//:UartLib",
            "//:XMemLib",
            "//:Interrupt_ManagerLib",
            "//:ElasticNodeMiddleware_ConfigureFPGALib",
            "//:RegisterDefinitionLibHdr",
            "//app/setup:Setup",
        ],
    ) 
    
    
At the end you define the upload script as a genrule. 
You choose a name for the script as well as the outs. 
Notice that the name of the upload script is used in the above defined embedded binary.
Additionally, you specify your command for the terminal. 

    genrule(
        name = "Avr_dude_upload_script",
        outs = ["upload.sh"],
        cmd = """echo "avrdude -c stk500 -p \$$1 -P /dev/ttyACM0 -D -V -U flash:w:\$$2:i -e" > $@""",
    )
    
Please consider the BUILD.bazel file defined for the [main.c](../app/main.c) in the [app](../app) folder for creating your own BAZEL.build file.

## Your Implementation

Now you can write your own implementation. 
The code is written in C, therefore you should use the programming language C to use the code.
You include the needed libraries in your header or source files with

    #include "lib/neededLibrary/neededLibrary.h"
    
whereby the "neddedLibrary" is replaced with the actual library. 
For example for including the xmem library your include statement looks as the following:
  
    #include "lib/xmem/xmem.h"
    
Notice, that the Bitmanipulation is an external library. 
Therefore, you include this library as follows:

    #include "EmbeddedUtilities/BitManipulation.h"
    
But normally you do not need this library for the functionalities of the elastic node middleware code.
It could only be useful for setting the LEDs on and off for some checks. 


If you want to use uart or reconfigure, you have to implement interrupt service routines. 
Because of implementing a clean library, we do not implement interrupt service routines in the library itself, but the functions that are used in the ISR.
In the following we show which ISR are needed for using the uart and reconfigure library. 

    //the following ISR's have to be comment in by programmer
    
    /* for using uart
     */
    ISR(USART1_RX_vect) {
        uart_ISR_Receive();
    }
    
    ISR(USART1_TX_vect) {
        uart_ISR_Transmit();
    }
    
    /* for using reconfigure
     */
    ISR(FPGA_DONE_INT_VECTOR) {
        reconfigure_interruptSR();
    }

You also can refer to the [main.c](../app/main.c). 

In the end you can use the code and can write your own program.  