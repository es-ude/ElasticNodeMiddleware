# Write your own Program

Please look up the [Getting Started Guide](GettingStartedGuide.md) first and try the examples before writing your own program.

## Create own Bazel Project

This guide shows you how to write your own program with the elastic node middleware code. 
We recommend you to use the [BazelCProjectCreator](https://github.com/es-ude/BazelCProjectCreator) to build a bazel project in C.
Please follow the instructions and generate a project with:

    $ curl https://raw.githubusercontent.com/es-ude/BazelCProjectCreator/master/create_bazel_project.py \
      | python - MyProject ElasticNodeMiddlewareProject

Note, that you have to use python version 3. 
Required python packages are: requests, numpy, pyserial.

The name MyProject could be replaced with your explicit project name.

We recommend you to use an IDE and import there your new generate project as a bazel project.
We use the IDE CLion for our bazel projects. 

After importing you can run the [init.py](../templates/init.py) for setting up the ports like in the [Getting Started Guide](GettingStartedGuide.md).

    $ python init.py

You can run the scripts again if you want to change a port.

**Important:** Please look up the versions of the repositories declared as dependencies in the WORKSPACE file. The elastic node middleware itself, the [EmbeddedUtilities repository](https://github.com/es-ude/EmbeddedUtilities) and the [PeripheralInterface repositiory](https://github.com/es-ude/PeripheralInterface).

After that you should do a bazel sync:

    $ bazel sync

If you want to check if everything worked the blinkExample.c in the app folder of your project should include a mini example.
It can be build and run or run dircelty like explained in the [Getting Started Guide](GettingStartedGuide.md#blink-example):

    $ bazel build //app:blinkExample --platforms=@AvrToolchain//platforms:ElasticNode_v4
    $ bazel run //app:blinkExample_upload --platforms=@AvrToolchain//platforms:ElasticNode_v4

## Your Implementation

Now you can write your own implementation by extending the main.c file. 
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

When you include libaries you have to add them to the deps in the BUILD.bazel file in the app folder.
The "//app/setup:Setup" should always be a dependency.
Because of the elastic node middleware as an external dependency, you have to add "@ElasticNodeMiddleware" before every used library of the elastic node middleware.
If you write own libraries you do not have to add this before the library (refer to the [BUILD.bazel](../BUILD.bazel) of the elastic node middleware).

Here is an example where have all dependencies from the elastic node middleware are used: 

    default_embedded_binary(
        name = "main",
        srcs = ["main.c"],
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

When working with more C files you need to create new cc_libaries in the BUILD.bazel in the app folder, similar to this:

    cc_library(
        name = "OtherFile",
        srcs = ["OtherFile.c"],
        deps = [":OtherFileHeader",
                "//app/setup:Setup"]
    )

    cc_library(
        name = "OtherFileHeader",
        srcs = ["OtherFile.h"]
    )

Wherby you add ":OtherFile" as a deps in the default_embedded_binaries and include all libaries used in the OtherFile as deps for it.

### Debug 

For uploding the main file with the DEBUG macors defined use the mainDEBUG target defined in the [BULD.bazel](../templates/appBUILD.bazel).

    $ bazel run //app:mainDEBUG_upload --platforms=@AvrToolchain//platforms:ElasticNode_v4

## Upload your own Bitfile

Please try to upload our example bitfile first which is explained in the [Getting Started Guide](GettingStartedGuide.md#uploading-the-example-bitfile).
The uploading of the Bitfile is only possible when you build and run the main.c with the DEBUG Flag set.     
  
You have to write the name of you bitfile, which you can put in the bitfiles folder, in the bitfileConfigs.py.
If you want to upload two bitfiles, comment in the definition of the second bitfile.

**Important**: the [portConfigs.py](../scripts/portConfigs.py) and [bitfileConfigs.py](../scripts/bitfileConfigs.py) should be defined for your computer. 
Therefore, it does not make sense to put it in the github repository, if you work with a number of people.
Please add this file to your [.gitignore](../.gitignore) like explained [here](https://git-scm.com/docs/gitignore) to not upload it to your github repository.

Again, do a bazel sync for synchronising the python scripts and remember to check for the DIP-Switches like explained in the [Getting Started Guide](GettingStartedGuide.md#DIP-Switches) Hardware section. 
Also make sure you uploded the main file while using the mainDEBUG target, when ever you want to upload bitfiles, as this funcionallity will otherwiese not be implementet.

Run the upload script with

    $ bazel run uploadBitfile

For other possible actions to do with the bitfile, like verifying, take a look in the [uploadMultiConfigS15.py](../scripts/uploadMultiConfigS15.py) and the [scripts folder](../scripts) in generall.
