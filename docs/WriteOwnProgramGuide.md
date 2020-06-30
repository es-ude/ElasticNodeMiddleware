# Write your own Program

This guide shows you how to write your own program in the elastic node middleware code. 
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
        copts = cpu_frequency_flag(),
        uploader = "Avr_dude_upload_script",
        deps = [
            "//:CircularBufferLib",
            "//:ConfigurationLib",
            "//:DebugLufaLib",
            "//:DebugUartLib",
            "//:ElasticNodeMiddleware_ConfigureFPGALib",
            "//:ElasticNodeMiddlewareLib",
            "//:FlashLib",
            "//:Interrupt_ManagerLib",
            "//:Reconfigure_multibootLib",
            "//:RegisterDefinitionLibHdr",
            "//:SpiLib",
            "//:UartLib",
            "//:UartmanagerLib",
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
For build and run commands refer to the [Getting Started Guide](GettingStartedGuide.md).
In the end you can use the code and can write your own program.  

## Upload your own Bitfile

Please try to upload our example bitfile first which is explained in the [Getting Started Guide](GettingStartedGuide.md).
It is important that you set your ports right. 
For your own bitfile you have to set your configurations in [serial_test.py](../scripts/serial_test.py).
First define your address for your bitfile. 
For this, you have to add to the following your address:

    TEST_ADDRESS = 0
    ANN_WEIGHTS_ADDRESS = 0x121000
    ANN_ADDRESS = 0x90000
    CNN_ADDRESS = 0x0
    BSCAN_ADDRESS = 0xC0000
    FIR_ADDRESS = 0x180000
    MM_ADDRESS = 0xC0000
    VDP_ADDRESS = 0x120000
    SMALL_ADDRESS = 0x0
    DUMMY_ADDRESS = 0x0
    
For example you add under this:

    EXAMPLE_ADDRESS = 0x0
    
After this add to the following line in [serial_test.py](../scripts/serial_test.py) your configuration

    dummyConfig, smallConfig, testConfig, bscanConfig, annConfig, firConfig, mmConfig, vdpConfig = None, None, None, None, None, None, None, None

So add your configuration like this:

    exampleConfig, dummyConfig, smallConfig, testConfig, bscanConfig, annConfig, firConfig, mmConfig, vdpConfig = None, None, None, None, None, None, None, None, None

Note the added "None".
Af least you have to setup your configuration. 
For this you look at the following code snippet:

    # setup configurations
    self.dummyConfig = Configuration("../dummy.bit", DUMMY_ADDRESS, DUMMY_ADDRESS) #, mini=True)
    self.smallConfig = Configuration("small.bit", SMALL_ADDRESS, SMALL_ADDRESS, special=True)
    self.testConfig = Configuration("test.bit", TEST_ADDRESS, TEST_ADDRESS)
    self.bscanConfig = Configuration("bit_file_bscan.bit", BSCAN_ADDRESS)
    self.annConfig = Configuration("ann.bit", ANN_ADDRESS, ANN_ADDRESS)
    self.cnnConfig = Configuration("../cnnProjectBlockRAM.bit", CNN_ADDRESS, CNN_ADDRESS)
    self.firConfig = Configuration("fir.bit", FIR_ADDRESS, FIR_ADDRESS)
    self.mmConfig = Configuration("mm.bit", MM_ADDRESS, MM_ADDRESS)
    self.vdpConfig = Configuration("vdp.bit", VDP_ADDRESS, VDP_ADDRESS
    
Add at the end your configuration: 
    
    self.exampleConfig = Configuration("path/to/your/bitfile.bit", EXAMPLE_ADDRESS, EXAMPLE_ADDRESS)     

Exchange the "path/to/your/bitfile.bit" with the actual path to your bitfile. 

Now, create a new python file in the [scripts](../scripts) folder. 
Name it like "uploadExample", whereby you exchange "Example" with the name of your bitfile.
Import SerialTest from serial_test like this

    from serial_test import SerialTest

For writing your bitfile to the FPGA, define a method "writeExample()" like this.
Exchange Example with the name of the bitfile.
    
    def writeexample():
        serialTest = SerialTest()
        assert serialTest.sendConfig(serialTest.exampleConfig, flash=True)
        
Note that you use here your exampleConfig, which we defined in [serial_test.py](../scripts/serial_test.py) before.
This code creates a object of the class SerialTest. 
It runs the method "sendConfig" with your specified config and "flash=True" because it communicates over the flash of the FPGA.
This method return a boolean value, which is compared to "True" with the "assert" statement.
At least you have to run your defined "writeexample()" method in the main function:

    if __name__ == "__main__":
        writeexample()
        
Add this code snippet to your "uploadExample.py".
All in all your "uploadExample.py" code should look like this:

    from serial_test import SerialTest
    
    def writeexample():
        serialTest = SerialTest()
        assert serialTest.sendConfig(serialTest.exampleConfig, flash=True)
    
    if __name__ == "__main__":
        writeexample()

To synchronise your python scipt in bazel you have to add a python library for it. 
Go to the end of the [BUILD.bazel](../BUILD.bazel) in the elasticnodemiddleware folder and add the following code:

    py_library(
        name = "uploadExamplePyLib",
        srcs = ["scripts/uploadExample.py"],
        deps = [],
    )

Thereby you can five it a arbitrary name. 
The scrs should include the path of your implemented "uploadExample.py".

For uploading your bitfile you first have to build and run our main.c like explained in the [Getting Started Guide](GettingStartedGuide.md).
Then you run your "uploadExample.py" with python3.
For example you open your terminal and route to the [scripts](../scripts) folder. 
There you run your script with

    python uploadExample.py
    
For other possible actions to do with the bitfile, like verifying please look in the [uploadDummy.py](../scripts/uploadDummy.py).
