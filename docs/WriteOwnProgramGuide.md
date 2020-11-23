# Write your own Program

Please look up the [Getting Started Guide](GettingStartedGuide.md) first. 

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

After importing you can run the [init.py](../templates/init.py) for setting up the ports mentioned in the [Getting Started Guide](GettingStartedGuide.md).

    $ python init.py

You can run the script again when you have to change a port.

**Important:** Please look up the versions of the repositories declared as dependencies in the WORKSPACE file. The elastic node middleware itself, the [EmbeddedUtilities repository](https://github.com/es-ude/EmbeddedUtilities) and the [PeripheralInterface repositiory](https://github.com/es-ude/PeripheralInterface).

After that you should do a bazel sync:

    $ bazel sync

### Blink Example

First you can try to upload a blink example to the elastic node with:

    $ bazel run //app/examples:blinkExample_upload --platforms=@AvrToolchain//platforms:ElasticNode_v4
    
The four LEDs on your elastic node should blink in sequence.

### Blink Lufa Example

For testing Lufa first uplade the Lufa example:

    $ bazel run //app/examples:blinkLufaExample_upload --platforms=@AvrToolchain//platforms:ElasticNode_v4
    
The fourth LED should blink and when using Lufa with
    
    $ sudo screen /dev/ttyACM1

it should register it when you press 'a'.

## Your Implementation

Now you can write your own implementation by extending the main.c file, which you upload with:

    $ bazel run //app:main_upload --platforms=@AvrToolchain//platforms:ElasticNode_v4

When using the run command you automaticly build the file. For just building it use: 

    $ bazel build //app:main_upload --platforms=@AvrToolchain//platforms:ElasticNode_v4
 
### Libraries

You include the needed libraries in your header or source files with
```c
#include "lib/neededLibrary/neededLibrary.h"
```    
whereby the "neddedLibrary" is replaced with the actual library. 
For example for including the xmem library your include statement looks as the following:
```c  
#include "lib/xmem/xmem.h"
```
Notice, that Bitmanipulation and LufaUsart are external libraries. 
Therefore, you include these libraries as follows:
```c
#include "EmbeddedUtilities/BitManipulation.h"
```
or
```c
#include "PeripheralInterface/LufaUsartImpl.h"
```
When you include libaries you have to add them to the deps in the BUILD.bazel file in the app folder.
The "//app/setup:Setup" should always be a dependency.
Because of the elastic node middleware as an external dependency, you have to add "@ElasticNodeMiddleware" before every used library of the elastic node middleware.
If you write own libraries you do not have to add this before the library (refer to the [BUILD.bazel](../BUILD.bazel) of the elastic node middleware).

Here is an example where have all dependencies from the elastic node middleware are used: 
```bazel
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
```

When working with more C files you need to create new cc_libaries in the BUILD.bazel in the app folder, similar to this:
```bazel
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
```
Wherby you add `:OtherFile` as a deps in the main binary and include all libaries used in the OtherFile as deps for it.

### Debug 

For uploding the main file with the DEBUG macors defined use the mainDEBUG target defined in the [BULD.bazel](../templates/appBUILD.bazel).

    $ bazel run //app:mainDEBUG_upload --platforms=@AvrToolchain//platforms:ElasticNode_v4

## Upload your own bitfiles

Whenever you want to flash bitfiles, upload the main.c with the DEBUG Flag set as exlpained above.
  
You have to write the name of you bitfile, which you can put in the bitfiles folder, in the bitfileConfigs.py.
If you want to upload two bitfiles, comment in the definition of the second bitfile.
Example bitfiles are available in the [bitfiles folder](../bitfiles).

**Important**: the [portConfigs.py](../scripts/portConfigs.py) and [bitfileConfigs.py](../scripts/bitfileConfigs.py) should be defined for your computer. 
Therefore, it does not make sense to put it in the github repository, if you work with a number of people.
Please add this file to your [.gitignore](../.gitignore) like explained [here](https://git-scm.com/docs/gitignore) to not upload it to your github repository.

Remember to check for the DIP-Switches like explained in the [Getting Started Guide](GettingStartedGuide.md#DIP-Switches) Hardware section when you have problems while flashing.

Run the upload script with

    $ bazel run uploadBitfile

For other possible actions to do with the bitfile, like verifying, take a look in the [uploadMultiConfigS15.py](../scripts/uploadMultiConfigS15.py) and the [scripts folder](../scripts) in generall.
