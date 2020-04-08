# Write your own Program

For example your implementation is in the folder "myFolder" in the file "myImplementation".
Then you have to create a fitting BUILD.bazel file for building and uploading your implementation.

## Your BUILD.bazel File

Your BUILD.bazel file should be in "myFolder" where your implementation is stored.
The file should begin with the following two lines:
    
    load("@AvrToolchain//:helpers.bzl", "default_embedded_binaries", "default_embedded_binary")
    load("@AvrToolchain//platforms/cpu_frequency:cpu_frequency.bzl", "cpu_frequency_flag")
    
After that you create your own embedded binary.
You should give it a name and define the sources of your implementation as well as the compiler options with the copts command.
After that you define the uploader.
In our example we use define the upload script below. 
At least you should define your dependencies.
This are the libraries which you are using in your implementation. 
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
    
Notice that the BitmanipulationLib is an external library.
Therefore, we do not name it in the list of libraries.
The used functions of this library are explained in the [getting started guide](docs/GettingStartedGuide.md).

At the end you define the upload script as a genrule. 
You choose a name for the script as well as the outs. 
Notice that the name of the upload script is used in the above defined embedded binary.
Additionally, you specify your command for the terminal. 

    genrule(
        name = "Avr_dude_upload_script",
        outs = ["upload.sh"],
        cmd = """echo "avrdude -c stk500 -p \$$1 -P /dev/ttyACM0 -D -V -U flash:w:\$$2:i -e" > $@""",
    )
    
Please consider the BUILD.bazel file defined for the main.c in the app folder for creating your own BAZEL.build file.

## Your Implementation


