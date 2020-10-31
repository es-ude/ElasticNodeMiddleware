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
When asked if you want to create an elastic node middleware project confirm with "y".

We recommend you to use an IDE and import there your new generate project as a bazel project.
We use the IDE CLion for our bazel projects. 

After importing you can run the [init.py](../templates/init.py) for creating all necessary files.

    § python init.py

You will be asked to declare the port of your programmer and elastic node, which you should now form the [Getting Started Guide](GettingStartedGuide.md).

**Important:** Please look up the versions of the repositories declared as dependencies in the WORKSPACE file. The elastic node middalware itself, the [EmbeddedUtilities repository](https://github.com/es-ude/EmbeddedUtilities) and the [PeripheralInterface repositiory](https://github.com/es-ude/PeripheralInterface).

After that you should do a bazel sync:

    $ bazel sync

The blink.c in the app folder of your project should include a mini example.
Now build and upload the blink example again like explained in the [Getting Started Guide](GettingStartedGuide.md):

    $ bazel build //app:blink --platforms=@AvrToolchain//platforms:ElasticNode_v4
    $ bazel run //app:_blinkUpload --platforms=@AvrToolchain//platforms:ElasticNode_v4

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

When working with more c files you need to create new cc_libaries in the BUILD.bazel in the app folder, similar to this:

    cc_library(
        name = "OtherFile",
        srcs = ["OtherFile.c"],
        deps = [":OtherFileHeader",
                "//app/setup:Setup"]
    )

    cc_library(
        name = "OthrerFileHeader",
        srcs = ["OtherFile.h"]
    )

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
     
  
Exchange the "path/to/your/bitfile.bit" with the actual path to your bitfile. This path has to be the absolute path to your bitfile. Relative path will lead to errors.

**Important**: the [portConfigs.py](../scripts/portConfigs.py) and [bitfileConfigs.py](../scripts/bitfileConfigs.py) should be defined for your computer. 
Therefore, it does not make sense to put it in the github repository, if you work with a number of people.
Please add this file to your [.gitignore](../.gitignore) like explained [here](https://git-scm.com/docs/gitignore) to not upload it to your github repository.

**Important:** Please refer to the [scripts folder](../scripts) in the elastic node middleware code. 
The configurations for the bitfile and for the ports and the upload script should be look similar to the files in this folder! 


Again, do a bazel sync for synchronising the python scripts and remember to check for the DIP-Switches like explained in the [Getting Started Guide](GettingStartedGuide.md) Hardware section. 

For uploading your bitfile you first have to build and run our [main.c](../app/main.c) or your own implementation when it implements the flashing of Bitfiles.
Otherwise use the main to flash the Bitfile and then uplaod your own Implementation again. 
Run your script with

    $ bazel run uploadBitFile

whereby the term "uploadBitFile" is the name of your defined python binary above. 
    
If you want to upload multiple bitfiles please consider to the [uploadMultiConfigS15.py](../scripts/uploadMultiConfigS15.py).
This scripts also uses two parts. 
Please note that there are also two addresses and configurations in the [bitfileConfigs.py](../scripts/bitfileConfigs.py). 
For other possible actions to do with the bitfile, like verifying, take a look in the [uploadMultiConfigS15.py](../scripts/uploadMultiConfigS15.py) and the [scripts folder](../scripts).
