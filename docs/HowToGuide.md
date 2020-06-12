# How-to's

## How to upload your own bitfile

If you have a own bitfile and you want to upload it you have to expand the existing pythons scripts.
You need to know the address and desination. (?)
In addition, you need to know the port of your programmer and the port of the elasticnode. 
In the serial_test.py script change the "en4_serial_template" to your port of your elasticnode. 
For example if the elasticnode uses the port /dev/ttyACM1 it should be the following:

    en4_serial_template = "/dev/ttyACM1"
    
The port of your debugger should be defined in the programm_template.
In our example the port for the programmer is /dev/ttyACM0.

    program_template = "/dev/ttyACM0"

Then you have to add your configuration. 
In the test folder there is a script called "test_dummy.py"


## How to configure the FPGA

missing

## How to build a new bitfile for reconfiguring the FPGA

missing

## How to ....