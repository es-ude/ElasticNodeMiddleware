# Write your own Program

Please look up the [Getting Started Guide](GettingStartedGuide.md) first and try the examples before writing your own program.

This guide shows you how to write your own program in the Elastic Node middleware code. 
We assume that for example your implementation is in the folder "app" in the file "myImplementation.c".
Then you have to create a fitting BUILD.bazel file for building and uploading your implementation.

Alternatively, you can use the example ["main.c"](../app/main.c) file and modify this file for your purposes. 
Another option is to add a case to the switch case in the controlmanager and run it with building and running the [main.c](../app/main.c).

## The BUILD.bazel File

You have to extend the [BUILD.bazel](../app/BUILD.bazel) file in the app folder. 
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
        copts = [
            "-DF_CPU=8000000UL",
            "-DBAUD=9600UL",
        ],
        uploader = "Avr_dude_upload_script",
        deps = [
            "//:CircularBufferLib",
            "//:ConfigurationLib",
            "//:ControlmanagerLib",            
            "//:DebugLufaLib",
            "//:DebugUartLib",
            "//:ElasticNodeMiddleware_ConfigureFPGALib",
            "//:ElasticNodeMiddlewareLib",
            "//:FlashLib",
            "//:Interrupt_ManagerLib",
            "//:LedLib
            "//:Reconfigure_multibootLib",
            "//:RegisterDefinitionLibHdr",
            "//:SpiLib",
            "//:UartLib",
            "//:XMemLib",
            "//:BitmanipulationLib",
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
    
Notice, that Bitmanipulation and LufaUsart are external libraries. 
Therefore, you include these libraries as follows:

    #include "EmbeddedUtilities/BitManipulation.h"
    
or

    #include "PeripheralInterface/LufaUsartImpl.h"
    
If you want to use uart, you have to implement interrupt service routines. 
Because of implementing a clean library, we do not implement interrupt service routines in the library itself, but the functions that are used in the ISR.
In the following we show which ISR are needed for using the uart library. 

    //the following ISR's have to be comment in by programmer
    
    /* for using uart
     */
    ISR(USART1_RX_vect) {
        uart_ISR_Receive();
    }
    
    ISR(USART1_TX_vect) {
        uart_ISR_Transmit();
    }

You also can refer to the [main.c](../app/main.c). 
For build and run commands refer to the [Getting Started Guide](GettingStartedGuide.md).
In the end you can use the code and can write your own program.  

## Upload your own Bitfile

Please try to upload our example bitfile first which is explained in the [Getting Started Guide](GettingStartedGuide.md).
It is important that you set your ports right in the [portConfigs.py](../scripts/portConfigs.py). 

**Important**: the [portConfigs.py](../scripts/portConfigs.py) should be defined for your computer. 
Therefore, it does not make sense to put it in the github repository, if you work with a number of people.
Please add this file to your [.gitignore](../.gitignore) like explained [here](https://git-scm.com/docs/gitignore) to not upload it to your github repository.

For your own bitfile you have to set your configurations in [bitfileConfigs.py](../scripts/bitfileConfigs.py).
First you specify your address for your bitfile. 
For this, you have to add to the following your address:

    S15_ADDRESS_1 =0x0
    S15_ADDRESS_2 =0x90000
    
For example you add this:

    EXAMPLE_ADDRESS = 0x0
    
After that you have to setup your configuration.
Add to the class BitfileConfigs beneath this:

    s15ConfigPart1 = None
    s15ConfigPart2 = None

your configuration like this:

    exampleConfig = None
    
Then you look at the following code snippet:

    def __init__(self):
        self.s15ConfigPart1 = Configuration("../bitfiles/s15_p1.bit", S15_ADDRESS_1, S15_ADDRESS_1)
        self.s15ConfigPart2 = Configuration("../bitfiles/s15_p2.bit", S15_ADDRESS_2, S15_ADDRESS_2)    

Add at the end your configuration: 
    
        self.exampleConfig = Configuration("path/to/your/bitfile.bit", EXAMPLE_ADDRESS, EXAMPLE_ADDRESS)     

Exchange the "path/to/your/bitfile.bit" with the actual path to your bitfile. 

Now, create a new python file in the [scripts](../scripts) folder. 
Name it like "uploadExample", whereby you exchange "Example" with the name of your bitfile.
Import SerialTest from serial_test  and BitfileConfigs from bitfileConfigs like this

    from serial_test import SerialTest
    from bitfileConfigs import BitfileConfigs

For writing your bitfile to the FPGA, define a method "writeexample()" like this.
Exchange Example with the name of the bitfile.
    
    def writeexample():
        serialTest = SerialTest()
        bitfileConfigs = BitfileConfigs()
        assert serialTest.sendConfig(bitfileConfigs.exampleConfig, flash=True)
        
Note that you use here your exampleConfig, which we defined in [bitfileConfigs.py](../scripts/bitfileConfigs.py) before.
This code creates a object of the class SerialTest. 
It runs the method "sendConfig" with your specified config and "flash=True" because it communicates over the flash of the FPGA.
This method return a boolean value, which is compared to "True" with the "assert" statement.
At least you have to run your defined "writeexample()" method in the main function:

    if __name__ == "__main__":
        writeexample()
        
Add this code snippet to your "uploadExample.py".
All in all your "uploadExample.py" code should look like this:

    from serial_test import SerialTest
    from bitfileConfigs import BitfileConfigs
    
    def writeexample():
        serialTest = SerialTest()
        bitfileConfigs = BitfileConfigs()
        assert serialTest.sendConfig(bitfileConfigs.exampleConfig, flash=True)
    
    if __name__ == "__main__":
        writeexample()

To synchronise your python script in bazel you can add a python library for it. 
Go to the end of the [BUILD.bazel](../BUILD.bazel) in the elasticnodemiddleware folder and add the following code:

    py_library(
        name = "uploadExamplePyLib",
        srcs = ["scripts/uploadExample.py"],
        deps = [],
    )

Thereby you can give it an arbitrary name. 
The scrs should include the path of your implemented "uploadExample.py".

For uploading your bitfile you first have to build and run our main.c like explained in the [Getting Started Guide](GettingStartedGuide.md).
Then you run your "uploadExample.py" with python3.
For example you open your terminal and route to the [scripts](../scripts) folder. 
There you run your script with

    python uploadExample.py
    
If you want to upload multiple bitfiles please consider to the [uploadMultiConfigS15.py](../scripts/uploadMultiConfigS15.py).
This scripts also uses two parts. 
Please note that there are also two addresses and configurations in the [bitfileConfigs.py](../scripts/bitfileConfigs.py). 
For other possible actions to do with the bitfile, like verifying please again look in the [uploadMultiConfigS15.py](../scripts/uploadMultiConfigS15.py).
