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

After importing you can run the [init.py](../templates/init.py) for creating some necessary config files and change the ports mentioned in the [Getting Started Guide](GettingStartedGuide.md).

    $ bazel run init

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

Now you can write your own implementation by extending the main.c file, which you upload with

    $ bazel run //app:main_upload --platforms=@AvrToolchain//platforms:ElasticNode_v4

When using the run command you automatically build the file. For just building it use

    $ bazel build //app:main_upload --platforms=@AvrToolchain//platforms:ElasticNode_v4
 
### Elastic Node Middleware header

As you see in the example [main.c](../app/main.c) file, you need to include the middleware header as follows

```c
#include "ElasticNodeMiddleware/ElasticNodeMiddleware.h"
```    

and the external dependency for the middleware in the [BULD.bazel](../templates/appBUILD.bazel) file

```bazel
"@ElasticNodeMiddleware//:ElasticNodeMiddlewareLib",
```

### Include other source files with bazel

When working with more C files you need to create new cc_libraries in the BUILD.bazel in the app folder, similar to this

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

Wherby you add `:OtherFile` as a deps in the main binary and include all libraries used in the OtherFile as deps for it.

### Debug 

For uploading the main file with the `DEBUG` macros defined use the `mainDEBUG` target defined in the [BULD.bazel](../templates/appBUILD.bazel)

    $ bazel run //app:mainDEBUG_upload --platforms=@AvrToolchain//platforms:ElasticNode_v4

This includes the middleware with the `DEBUG` flag set

```bazel
"@ElasticNodeMiddleware//:ElasticNodeMiddlewareDEBUGLib",
```

## Upload your own bitfiles

Whenever you want to flash bitfiles, upload the main.c with the DEBUG Flag set as explained above.
  
You have to write the name of you bitfile, which you can put in the bitfiles folder, in the bitfileConfigs.py.
If you want to upload two bitfiles, comment in the definition of the second bitfile.
Example bitfiles are available in the [bitfiles folder](../bitfiles).

Remember to check for the DIP-Switches like explained in the [Getting Started Guide](GettingStartedGuide.md#DIP-Switches) Hardware section when you have problems while flashing.

Run the upload script with

    $ bazel run uploadBitfiles

For other possible actions to perform with the bitfiles, like verifying, take a look in the [uploadMultiConfigS15.py](../scripts/uploadMultiConfigS15.py) and the [scripts folder](../scripts) in general.
