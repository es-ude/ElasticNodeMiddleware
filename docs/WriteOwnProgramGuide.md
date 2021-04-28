# Write your own program

Please look up the [Getting Started Guide](GettingStartedGuide.md) first. 

## Create own Bazel project

This guide explains how to write an own program with the elastic node middleware code. 
We strongly recommend to use the [BazelCProjectCreator](https://github.com/es-ude/BazelCProjectCreator) to build a bazel project in C.
Please follow the instructions and generate a project with:

    $ curl https://raw.githubusercontent.com/es-ude/BazelCProjectCreator/master/create_bazel_project.py \
      | python - MyProject ElasticNodeMiddlewareProject

Note, that have to use python version 3 has to be used. 
Required python packages are: requests, numpy, pyserial.

The name MyProject could be replaced with an explicit project name.

We recommend to use an IDE and import the new generate project as a bazel project.
In CLion this is possible with the bazel plugin for it.

After importing run the [init.py](../templates/init.py) for creating some necessary config files and set the ports 
mentioned in the [Getting Started Guide](GettingStartedGuide.md).

    $ bazel run init

The ports can be changed by running the command again.

**Important:** Please look up the versions of the repositories declared as dependencies in the WORKSPACE file. 
The elastic node middleware itself, the [EmbeddedUtilities repository](https://github.com/es-ude/EmbeddedUtilities) 
and the [PeripheralInterface repositiory](https://github.com/es-ude/PeripheralInterface).

After that a bazel sync is recommended:

    $ bazel sync

## Examples

### Blink Example

An blink example can be uploaded to the elastic node with:

    $ bazel run //app/examples:blinkExample_upload --platforms=@AvrToolchain//platforms:ElasticNode_v4
    
The four LEDs on the elastic node should blink in sequence.

### Blink LUFA Example

For testing LUFA first uplade the LUFA example:

    $ bazel run //app/examples:blinkLufaExample_upload --platforms=@AvrToolchain//platforms:ElasticNode_v4
    
The fourth LED should blink and when using LUFA with
    
    $ sudo screen /dev/ttyACM1

it should register it when 'a' is pressed. (`/dev/ttyACM1 being the serial port to the elastic node)

## Your Implementation

Now an own implementation can be written by extending the main.c file, which can be upload with

    $ bazel run //app:main_upload --platforms=@AvrToolchain//platforms:ElasticNode_v4

When using the run command the file is build automatically. For just building it use

    $ bazel build //app:main_upload --platforms=@AvrToolchain//platforms:ElasticNode_v4
 
### Elastic Node Middleware header

As can be seen in the example [main.c](../app/main.c) file, the the middleware header needs to be included as following

```c
#include "ElasticNodeMiddleware/ElasticNodeMiddleware.h"
```    

and the external dependency for the middleware in the [BUILD.bazel](../templates/appBUILD.bazel) file

```bazel
"@ElasticNodeMiddleware//:ElasticNodeMiddlewareLib",
```

### Include other source files with bazel

When working with multiple C files new cc_libraries need to be created the BUILD.bazel in the app folder, similar to this
    
```bazel
cc_library(
    name = "OtherFile",
    srcs = ["OtherFile.c"],
    hdrs = ["OtherFile.h"],
    deps = ["//app/setup:Setup"]
)
```

`:OtherFile` could for example be added to the deps of the main and/or mainDEBUG targets and then `OtherFile.h` included.

### Debug 

The standard debugging is performed using LUFA.
For uploading the main file with the `DEBUG` macros defined use the `mainDEBUG` target defined in the [BUILD.bazel](../templates/appBUILD.bazel)

    $ bazel run //app:mainDEBUG_upload --platforms=@AvrToolchain//platforms:ElasticNode_v4

This includes the middleware with the `DEBUG` flag set

```bazel
"@ElasticNodeMiddleware//:ElasticNodeMiddlewareDEBUGLib",
```

#### Debug with UART

For debugging with UART a FTDI-adapter is needed. See the [Getting Started Guide](GettingStartedGuide.md#Hardware) for more information.
A UART example similar to the LUFA example is provided, which can be upload with

    $ bazel run //app/examples:blinkUartExample_upload --platforms=@AvrToolchain//platforms:ElasticNode_v4

For using UART in the [main.c](../app/main.c) the build target in the [BUILD.bazel](../app/BUILD.bazel) needs to be changed from 
"@ElasticNodeMiddleware//:ElasticNodeMiddlewareDEBUGLib" to "@ElasticNodeMiddleware//:ElasticNodeMiddlewareDEBUG_UARTLib". 
Also the both interrupts at the beginning of the file needs to be included, this can be done defining UART with " ["-DUART"] " as with DEBUG.

#### Debug commands

When the mainDEBUG is uploaded to the MCU, screen can be run exactly like in the section "Blink LUFA Example":
    
    $ sudo screen /dev/ttyACM1

Then specif chars for specific commands can be written to the command line.

    - i: see current FPGA user id
    - u: goes into user mode if not already in it
    - e: exits user mode if in it
    - FlashFPGA: start flashing (used when uploading bitfiles)
    - default: given char + 1 is returned

In the example main following custom commands are included when in user mode

    - t: output's test string
    - r: reconfigures the FPGA to address 0x0
    - R: reconfigures the FPGA to address 0x9000
    - default: unknown mode command received
    
    If an example bitfile is one of the uploaded bitfiles on the FPGA 
    and it is configured to the corresponding adress:
    - L: turn on LED
    - l: turn of LED

## Uploading bitfiles to the FPGA

***Importent:*** Uploading the bitfiles via UART is currently not supported.

Whenever bitfiles are to be uploaded, the main.c with the DEBUG Flag set as explained above needs to be on the MCU.

The path to the bitfiles which should be uploaded needs to be changed in the [bitfileConfigs.py](../templates/bitfileConfigs.py), 
there the second definition can be commented in for uploading two bitfiles together.
Example bitfiles are available in the [bitfiles folder](../bitfiles).

Remember to check for the DIP-Switches like explained in the [Getting Started Guide](GettingStartedGuide.md#DIP-Switches) 
Hardware section when problems with the flashing occur.

Run the upload script with

    $ bazel run uploadBitfiles

After the flashing restart the MCU.

For other possible actions to perform with the bitfiles, like verifying, take a look in the 
[uploadMultiConfigS15.py](../scripts/uploadBitfiles.py) and the [scripts folder](../scripts) in general.
