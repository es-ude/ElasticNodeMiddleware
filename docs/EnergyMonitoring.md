# Energy Monitoring

By using the second MCU on the elastic node you can measure the power the elastic node consumes during an experiment.
You should be familiar with the the [WriteOwnProgramGuide](WriteOwnProgramGuide.md).

## Monitoring MCU

The code for the monitoring MCU usually does not change.
For uploading code to the monitoring MCU you need to connect the programmer to it. The connections are labeled [here](images/elasticNodelabeled.jpg).
The platform changes to 'ElasticNode_v4_monitor' as it is another MCU.

    $ bazel run //app:currentSenseApp --platforms=@AvrToolchain//platforms:ElasticNode_v4_monitor

//TODO How to include it (or use other project for the moment?)

//TODO Code does not change?

## Example

In the example each second one led more blinks. After that all turn off and the experiment starts again.

You can upload the code for the main MCU as usual with

    $ bazel run //app/examples:monitoringExample_upload --platforms=@AvrToolchain//platforms:ElasticNode_v4
    
To check if everything works see 'Get results' underneath.

## Inclusion in own code

The code for the main MCU needs the inclusion
```c
#include "src/sub-system/sub-system.h"
```

and the dependency in bazel

```bazel
"@ElasticNodeMiddleware//:sub-system",
```

## Get results

You need to set the serial port to your elastic node and programmer and the path to your data folder in the config.py.
The micro-USB as well as the programmer should be connected to the monitoring MCU.
Now you can run the python script with

    $ bazel run analysis argument

List of arguments

capture: The output is written in a .csv file in the data folder.

print: Prints the current output to the console

live: draws a graph of the power consumption

debug: Puts out everything in hex

reset: Resets the monitoring MCU

With no arguments, the script is in capture mode. 
