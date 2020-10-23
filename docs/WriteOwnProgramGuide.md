# Write your own Program

Please look up the [Getting Started Guide](GettingStartedGuide.md) first and try the examples before writing your own program.

## Create own Bazel Project

This guide shows you how to write your own program with the elastic node middleware code. 
We recommend you to use the [BazelCProjectCreator](https://github.com/es-ude/BazelCProjectCreator) to build a bazel project in C.
Please follow the instructions and generate a project with:

    $ curl https://raw.githubusercontent.com/es-ude/BazelCProjectCreator/master/create_bazel_project.py \
      | python - MyProject

Note, that you have to use python version 3. 
The name MyProject could be replaced with your explicit project name.
We recommend you to use an IDE and import there your new generate project as a bazel project.
We use the IDE CLion for our bazel projects. 
After importing the project you should do a bazel sync:

    $ bazel sync

The main.c in the app folder of your project should include a mini example.
For testing this example please add to BUILD.bazel file in the app folder your upload script.
You should add to this:

    default_embedded_binaries(
        main_files = glob(["*.c"]),
        copts = cpu_frequency_flag(),
        deps = [
            "//app/setup:Setup",
            "//:Library",
            "//myProject:HdrOnlyLib",
            ],
    )

the upload script like this:

    default_embedded_binaries(
        main_files = glob(["*.c"]),
        copts = cpu_frequency_flag(),
        uploader = "Avr_dude_upload_script",
        deps = [
            "//app/setup:Setup",
            "//:Library",
            "//myProject:HdrOnlyLib",
            ],
    )

In addition, you have to add to the BUILD.bazel file the upload script:

    genrule(
        name = "Avr_dude_upload_script",
        outs = ["upload.sh"],
        cmd = """echo "avrdude -c stk500 -p \$$1 -P /dev/ttyACM0 -D -V -U flash:w:\$$2:i -e" > $@""",
    )

Change the ports to your ports like explained in the [Getting Started Guide](GettingStartedGuide.md).
Now build and upload the main again like explained in the [Getting Started Guide](GettingStartedGuide.md).
It should blink the fifth MCU-Led of your elastic node. 
If the Led does not blink you possibly have to change the given main a little bit. 
Change in the main the "DDRB" to "DDRD" and the "PORTB" to "PORTD". 
Your main should then look like this:

      DDRD = _BV(5);
      while (true)
      {
        _delay_ms(500);
        PORTD ^= _BV(5);
      }

Now you should add the elastic node middleware as a dependency for using it in your own project.
Therefore, go into the WORKSPACE file in your project folder and add to it at the end of the file:

    es_github_archive(
        name = "ElasticNodeMiddleware",
        version = "1.1"
    )
    
    es_github_archive(
        name = "EmbeddedUtilities",
        version = "0.3.1",
    )
    
    es_github_archive(
        name = "PeripheralInterface",
        version = "0.7.1"
    )

You have to add the [EmbeddedUtilities repository](https://github.com/es-ude/EmbeddedUtilities) and the [PeripheralInterface repositiory](https://github.com/es-ude/PeripheralInterface) because it is used in the elastic node middleware. 
After adding the repositories you should again do a bazel sync like above. 

**Important:** Please look up the repositories. 
You possibly have to change the version numbers because of new releases.

We assume that for example your implementation is in the folder "app" in the file "myImplementation.c".
Then you have to create a fitting BUILD.bazel file for building and uploading your implementation.

Alternatively, you can use the example main.c file in the app folder and modify this file for your purposes. 

Of course, you can use the elastic node middleware repository itself and add your implementation in this app folder. 
But if you want to implement a huge project, we do not recommend it. 

## The BUILD.bazel File

You have to extend the file in the app folder of your own project.
For your implementation you create your own embedded binary.
You should give it a name and define the sources of your implementation as well as the compiler options with the copts command.
The uploader is already defined at the end of this BUILD file by you in the section above. 
You can just use it directly. 
At least you should define your dependencies.
There are different libraries which you can use in your implementation. 
The libraries are named in the [README](../README.md).
The "//app/setup:Setup" should always be a dependency.
Because of the elastic node middleware as an external dependency, you have to add "@ElasticNodeMiddleware" before every used library of the elastic node middleware.
If you write own libraries you do not have to add this before the library (refer to the [BUILD.bazel](../BUILD.bazel) of the elastic node middleware).

In this example we include all possible libraries to show you the possibilities. 

    default_embedded_binary(
        name = "myImplementation",
        srcs = ["myImplementation.c"],
        copts = cpu_frequency_flag(),
        uploader = "Avr_dude_upload_script",
        deps = [
            "@ElasticNodeMiddleware//:CircularBufferLib",
            "@ElasticNodeMiddleware//:ConfigurationLib",
            "@ElasticNodeMiddleware//:ControlmanagerLib",            
            "@ElasticNodeMiddleware//:DebugLufaLib",
            "@ElasticNodeMiddleware//:DebugUartLib",
            "@ElasticNodeMiddleware//:ElasticNodeMiddleware_ConfigureFPGALib",
            "@ElasticNodeMiddleware//:ElasticNodeMiddlewareLib",
            "@ElasticNodeMiddleware//:FlashLib",
            "@ElasticNodeMiddleware//:Interrupt_ManagerLib",
            "@ElasticNodeMiddleware//:LedLib",
            "@ElasticNodeMiddleware//:Reconfigure_multibootLib",
            "@ElasticNodeMiddleware//:RegisterDefinitionLibHdr",
            "@ElasticNodeMiddleware//:SpiLib",
            "@ElasticNodeMiddleware//:UartLib",
            "@ElasticNodeMiddleware//:XMemLib",
            "@ElasticNodeMiddleware//:BitmanipulationLib",
            "@ElasticNodeMiddleware//app/setup:Setup",
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
    
For build and run commands refer to the [Getting Started Guide](GettingStartedGuide.md).
In the end you can use the code and can write your own program.  

## Upload your own Bitfile

Please try to upload our example bitfile first which is explained in the [Getting Started Guide](GettingStartedGuide.md).
It is important that you know your ports.
Please copy the [main.c](../app/main.c) of our project in the main.c or your own myImplementation.c to upload a bitfile.  
Then, add the needed external libraries. 
For example, you add the C code for uploading a bitfile in the myImplementation.c in the app folder of your own project.
Then add to the BUILD.bazel file of the app folder:

    default_embedded_binary(
        name = "myImplementation",
        srcs = ["myImplementation.c"],
        copts = cpu_frequency_flag(),
        uploader = "Avr_dude_upload_script",
        deps = [
            "//app/setup:Setup",
            "@ElasticNodeMiddleware//:LedLib",
            "@ElasticNodeMiddleware//:ElasticNodeMiddlewareLib",
            "@ElasticNodeMiddleware//:Reconfigure_multibootLib",
            "@ElasticNodeMiddleware//:XMemLib",
            "@ElasticNodeMiddleware//:DebugLufaLib",
            "@ElasticNodeMiddleware//:ControlmanagerLib",
            "@ElasticNodeMiddleware//:FlashLib",
        ],
    )

The uploader scipts is added by you before (refer to the section before).
"@ElasticNodeMiddleware" marks an external dependency. 
All these libraries are needed for uploading a bitfile.

First, create a new folder for your python implementations, e.g. myscripts.
In this folder you create a file for your port configurations, e.g. portConfigs.py.
In this python script you define your ports as follows:

    from scripts.Configuration import Configuration
    
    class Config:
        # change to your ports
        portToProgrammer = "/dev/ttyACM0"
        portToElasticnode = "/dev/ttyACM1"
    
You have to import the Configuration script. 
Please refer to the [portConfigs.py](../scripts/portConfigs.py) of the elastic node middleware. 
If you want, you can just copy this file and change the ports to your port.  

After that you create a python file for the bitfile configurations, e.g. bitfileConfigs.py.
Please refer to the [bitfileConfigs.py](../scripts/bitfileConfigs.py) of the elastic node middleware.
Your bitfile Configurations should be similar to this: 

    from scripts.Configuration import Configuration

    # define your address for your bitfiles
    EXAMPLE_ADDRESS = 0x0
    
    class BitfileConfigs:
    
        exampleConfig = None
    
        def __init__(self):
            self.exampleConfig = Configuration("path/to/your/bitfile.bit", EXAMPLE_ADDRESS, EXAMPLE_ADDRESS)     
  
Exchange the "path/to/your/bitfile.bit" with the actual path to your bitfile. This path has to be the absolute path to your bitfile. Relative path will lead to errors.

**Important**: the [portConfigs.py](../scripts/portConfigs.py) and [bitfileConfigs.py](../scripts/bitfileConfigs.py) should be defined for your computer. 
Therefore, it does not make sense to put it in the github repository, if you work with a number of people.
Please add this file to your [.gitignore](../.gitignore) like explained [here](https://git-scm.com/docs/gitignore) to not upload it to your github repository.

Now, create a new python file in your python scripts folder, which we will call "uploadBitFile".

    from scripts.serial_test import SerialTest
    from myscripts.bitfileConfigs import BitfileConfigs
    from myscripts.portConfigs import Config as portConfigs
    
    def writeexample():
        serialTest = SerialTest(portConfigs.portToElasticnode, portConfigs.portToProgrammer)
        bitfileConfigs = BitfileConfigs()
        assert serialTest.sendConfig(bitfileConfigs.exampleConfig, flash=True)
    
    if __name__ == "__main__":
        writeexample()

Here we import the other python files and write the code for uploading the bitfile.


**Important:** Please refer to the [scripts folder](../scripts) in the elastic node middleware code. 
The configurations for the bitfile and for the ports and the upload script should be look similar to the files in this folder! 

To synchronise your python scripts in bazel you have to add a python library or python binary for it. 
Go to the BUILD.bazel file in your top folder, the MyProject folder where MyProject is the name of your project.
Add for your bitfile Configurations and for your port Configurations python libraries like this.

    py_library(
        name = "bitfileConfigs",
        srcs = ["myscripts/bitfileConfigs.py"],
        deps = ["@ElasticNodeMiddleware//:Configuration"],
    )
    
    py_library(
        name = "portConfigs",
        srcs = ["myscripts/portConfigs.py"],
        deps = ["@ElasticNodeMiddleware//:Configuration"],
    )

Exchange the srcs paths with your sources path. 
Note the external dependency which is shown with "@ElasticNodeMiddleware".
This dependency refers to the [Configuration.py](../scripts/Configuration.py) file of the elastic node middleware code.
        
Then add for your uploadExample.py file following code: 

    py_binary(
        name = "uploadBitFile",
        srcs = ["myscripts/uploadBitFile.py"],
        deps = [
            "portConfigs",
            "@ElasticNodeMiddleware//:serial_test",
            "bitfileConfigs"
        ],
    )
    
Thereby you can give the library an arbitrary name. 
The scrs should include the path of your implemented "uploadBitFile.py".
This is a python binary instead of a python library because the script is an excutable script. 
The dependencies portConfigs and bitfileConfigs are the ones defined above. 
You also need the dependency to the [serial_test.py script](../scripts/serial_test.py).

Again, do a bazel sync for synchronising the python scripts and remember to check for the DIP-Switches like explained in the [Getting Started Guide](GettingStartedGuide.md) Hardware section. 

For uploading your bitfile you first have to build and run our [main.c](../app/main.c) or your own implementation when it implements the flashing of Bitfiles.
Otherwise use the main to flash the Bitfile and then uplaod your own Implementation again. 
Run your script with

    bazel run uploadBitFile

whereby the term "uploadBitFile" is the name of your defined python binary above. 
    
If you want to upload multiple bitfiles please consider to the [uploadMultiConfigS15.py](../scripts/uploadMultiConfigS15.py).
This scripts also uses two parts. 
Please note that there are also two addresses and configurations in the [bitfileConfigs.py](../scripts/bitfileConfigs.py). 
For other possible actions to do with the bitfile, like verifying, take a look in the [uploadMultiConfigS15.py](../scripts/uploadMultiConfigS15.py) and the [scripts folder](../scripts).
