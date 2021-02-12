# Clone Guide

For using the elastic node middleware as a library see the [WriteOwnProgramGuide.md](WriteOwnProgramGuide.md).

## Set up

We recommend you to use an IDE. We use the IDE CLion for our bazel projects. 
You can easily [Clone](https://docs.github.com/en/free-pro-team@latest/github/creating-cloning-and-archiving-repositories/cloning-a-repository) the ElasticNodeMiddleware repository from there
by using "Get from Version Control". 

### Serial ports

You should now the ports from your elastic node and your programmer from the  [GettingStartedGuide.md](GettingStartedGuide.md). 
For uploading you code change the port in [user.bazelrc](../user.bazelrc) to your programmer.
For flashing bitfiles you also have to change the ports accordingly in the [portsConfigs.py](../scripts/portsConfigs.py) in the scripts folder.

Both files are included in the [.gitignore](../.gitignore), because the ports should only be defined locally.

## Uploading the examples

### Blink Example 
    
The upload script is specified in the [BUILD.bazel](../app/BUILD.bazel) in the app folder. 
For running the upload script you have to run: 

	$ bazel run //app:blinkExample_upload --platforms=@AvrToolchain//platforms:ElasticNode_v4

The four LEDs on your elastic node should blink in sequence.

### Blink Lufa Example

For building and running the blink Lufa Example you have to use the same command like above but exchange "blinkExample" with "blinkLufaExample".
So, the commands look like this:

	$ bazel run //app:blinkLufaExample_upload --platforms=@AvrToolchain//platforms:ElasticNode_v4

We use screen for showing the communication and open screen with the port of the elastic node. 
For printing the communication we use:

    $ sudo screen /dev/ttyACM1
    
By using the Lufa Library, we do not have to specify the baudrate.
Periodically it should be printed "Hello. You debug with Lufa." in the terminal. When you press a button, it should return if it was 'a' or another key.
The right MCU_Led (number 4) should blink all over the time. 

## Uploading your own code

You can extend the main.c with your code and upload it with:

	$ bazel run //app:main_upload --platforms=@AvrToolchain//platforms:ElasticNode_v4
	
or with 	

	$ bazel run //app:mainDEBUG_upload --platforms=@AvrToolchain//platforms:ElasticNode_v4
	
to set the DEBUG flag.

To build the file without uploading it use:

	$ bazel build //app:main_upload --platforms=@AvrToolchain//platforms:ElasticNode_v4

When you create your own C files make sure to include them in the BUILD.bazel, explaind in the [WriteOwnProgramGuide.md](WriteOwnProgramGuide.md#Libraries).

### Libraries

When you include the `ElasticNodeMiddleware.h` you include the libraries as visualised in the [dependency graph](images/dependency_graph_labeld.jpg).
All the libraries are located in the [lib folder](../src). If you do not plan on modifying them consider to use the [WriteOwnProgramGuide.md](WriteOwnProgramGuide.md) instead.

## Uploading bitfiles

For uploading bitfiles it is necessary that the flash functionality is implemented in the code currently uploaded to the elastic node.
This is the case when you uploade the main.c with the DEBUG flag set as explained above.

Note that example bitfiles sX_p1.bit and sX_p2.bit for different FGPA models are in the [bitfiles folder](../bitfiles).
Change the path to them or your own bitfiles in the [bitfileConfigs.py](../scripts/bitfileConfigs.py), which is included in the [.gitignore](../.gitignore).

For uploading the bitfiles run:

    $ bazel run uploadMultiConfigS15
    
After processing the last output should be "ready to proceed with uart".

### Controlmanager

When the controlmanager is included in the code uploaded to the MCU, you can run
    
    $ sudo screen /dev/ttyACM1

exactly like in the section "Blink Lufa Example". 
Then you can write in the terminal your needed char for your specific function you want to use.

- i: see current FPGA user id
- r: reconfigurate the FPGA to user id D1
- R: reconfigurate the FPGA to user id D2
- F: start flashing (used when uploading bitfiles)

If a s15 bitfile is one of your uploaded bitfiles and you selected the corresponding user id:
- L: turn on LED
- l: turn of LED 

## Tests

We wrote some tests for checking the functionalities of our code. 
If you want to run a test of the code (here for example the xmem_Test), you have to run:

    $ bazel test test:xmem_Test

If you want to run all tests:

    $ bazel test test:all
    
If you want to run the integration test you have to use the build and uploads commands as written above.
For example for building the integration test for xmem:

    $ bazel build //test/integration:test_xmem --platforms=@AvrToolchain//platforms:ElasticNode_v4 

and for uploading this integration test

    $ bazel run //test/integration:_test_xmemUpload --platforms=@AvrToolchain//platforms:ElasticNode_v4

The structure of these commands is the same like explained above. 
