# Energy Monitoring

By using the monitoring MCU on the elastic node you can measure the power the elastic node consumes during an experiment.
You should be familiar with the the [WriteOwnProgramGuide](WriteOwnProgramGuide.md).

## Monitoring MCU

The code for the monitoring MCU usually does not change.
For uploading code to the monitoring MCU you need to connect the programmer to it. The connections are labeled [here](images/elasticNodelabeled.jpg).
The platform changes to 'ElasticNode_v4_monitor' as it is another type of MCU.

    $ bazel run //app:currentSenseApp --platforms=@AvrToolchain//platforms:ElasticNode_v4_monitor

//TODO How to include it (or use other project for the moment?)
//TODO Code does not change?

## Example

In the [monitoring example](../app/monitoringExample.c) each second one led more blinks. After that all turn off and the experiment starts again.
This happens 3 times and than the experiment is stopped.

You can upload the code for the main MCU as usual with

    $ bazel run //app/examples:monitoringExample_upload --platforms=@AvrToolchain//platforms:ElasticNode_v4
    
To check if everything works see 'Get results' underneath.

## Inclusion in own code

Everything explained in this section can be found implemented in the [monitoring example](../app/monitoringExample.c).

The code for the main MCU needs the inclusion
```c
#include "src/sub-system/sub-system.h"
```

and the dependency in bazel

```bazel
"@ElasticNodeMiddleware//:sub-system",
```

A slave must be initialized at the beginning of the code:

```c
IIC_slave_init(MY_IIC_ADDRESS);
```

### MCU running states

There are different mcu running states, these are captured when reading the measurement result to connect each measurement with a state of the experiment.
Therefore you need to declare a uint8_t:

```c
uint8_t state_of_the_mcu;
```

For easy use it is recommended to define your own running states with a typdef like in the monitoring example. 
There are 16 different states possible, whereby the sleep mode should always be 0 and the end of measurement 15:

```c
typedef enum {
    SLEEP_MODE = 0,
    OWN_STATE_1 = 1,
    //possible states from 2 to 14
    END_MEASUREMENT = 15,
}mcu_running_state;
```

When the state is changed to 'SLEEP_MODE' the sending of measurement data is paused until it is changed to another state.
The 'END_MEASUREMENT' state stops the sending of measurement data completely.

Now you can change the state to the corresponding state of your experiment:

```c
change_running_state(OWN_STATE_1, &state_of_the_mcu);
```

### Sample rate

//TODO: what does the sample rate do

You can change the sample rate with:

```c
change_sample_rate(CURRENT_SAMPLE_TIME_10ms);
```

The different sample rates are defined in [sub-system.c](../src/sub-system/sub-system.h)

## Get results

You need to set the serial port to your elastic node and programmer and the path to your data folder in the config.py.
The micro-USB as well as the programmer should be connected to the monitoring MCU.
Now you can run the python script with

    $ bazel run analysis argument
    
Table of arguments:

| argument | function                                                |
|----------|---------------------------------------------------------|
| capture  | the output is written in a .csv file in the data folder |
| print    | prints the current output to the console                |
| live     | draws a graph of the power consumption                  |
| debug    | writes debug output in hex to the console               |
| reset    | resets the monitoring MCU                               |
