# Write your own Program

This guide shows you how to write your own program in the elastic node middleware code. 
We assume that for example your implementation is in the folder "app" in the file "myImplementation.c".
Then you have to create a fitting BUILD.bazel file for building and uploading your implementation.

Alternatively, you can use the example "main.c" file and modify this file for your purposes. 

## The BUILD.bazel File

You have to extend the BUILD.bazel file in the app folder. 
You should see there the definition for our main.c and the blinking example.
For your implementation you create your own embedded binary.
You should give it a name and define the sources of your implementation as well as the compiler options with the copts command.
The uploader is already defined at the end of this BUILD file. 
You can just use it directly. 
At least you should define your dependencies.
There are different libraries which you can use in your implementation. 
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
    
For using this you just have to change this code with your implementation and your desired dependencies.

## Your Implementation

Now you can write your own implementation. 
The code is written in C, therefore you should use the programming language C to use the code.
You include the needed libraries in your header or source files with

    #include "lib/neededLibrary/neededLibrary.h"
    
whereby the "neddedLibrary" is replaced with the actual library. 
For example for including the xmem library your include statement looks as the following:
  
    #include "lib/xmem/xmem.h"
    
Notice, that the Bitmanipulation and the PeripheralInterface are external libraries. 
Therefore, you include these libraries as follows:

    #include "EmbeddedUtilities/BitManipulation.h"
    
or

    #include "PeripheralInterface/LufaUsartImpl.h"
    
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