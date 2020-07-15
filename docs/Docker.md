# Docker Container Version

A dockerfile is provided here to simplify development in Visual Studio Code. This image is also uploaded to [Docker Hub](https://hub.docker.com/repository/docker/alwynburger/bazel) for general use, and can be freely used by anyone. 

An example usage for uploading the Lufa example looks as follows:

```bash
docker run --device /dev/ttyACM0 --device /dev/ttyACM1 -v /absolutePathToYourProject/ElasticNodeMiddleware:/app alwynburger/bazel:latest bazel run //app:_blinkLufaExampleUpload --platforms=@AvrToolchain//platforms:ElasticNode_v4
```

This binds the devices used (/dev/ttyACM0 and /dev/ttyACM1) into the running container, note these can easily be renamed when mounting. An example of that is

```bash
... run --device /dev/tty.usbmodem101:/dev/ttyACM0 ...
```

The scripts require the programmer port to be mounted at ttyACM0, and the serial port at ttyACM1. If that is not the case, these variables need to be changed in the scripts. 

Additionally, your base directory with the project code should be mounted at /app as shown. 
So, you have to change "/absolutePathToYourProject/" to your own path.

The last part of the run command can be changed for any other building command as presented in the main Readme. 