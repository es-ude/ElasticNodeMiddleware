# Clone Guide

For using the elastic node middleware as a library see the [WriteOwnProgramGuide.md](WriteOwnProgramGuide.md).
The following procedure of cloning the repository is only necessary if the code of the libraries will be changed.

## Cloning

We recommend to use an IDE. We use the IDE CLion for our bazel projects. 
The ElasticNodeMiddleware repository can easily be 
[cloned](https://docs.github.com/en/free-pro-team@latest/github/creating-cloning-and-archiving-repositories/cloning-a-repository) 
there by using "Get from Version Control". 

## Serial ports

The ports of the elastic node and the programmer should be known from the [GettingStartedGuide.md](GettingStartedGuide.md). 
For uploading the code change the port in [user.bazelrc](../user.bazelrc) to the programmer.
For flashing bitfiles the port in the [portsConfigs.py](../scripts/portsConfigs.py) in the scripts folder should also be changed accordingly.

Both files are included in the [.gitignore](../.gitignore), because the ports should only be defined locally.

## Use the Write Own Program Guide

From here onwards the process is the same as the one in the [Write Own Program Guide](WriteOwnProgramGuide.md#Examples) 
starting from the Examples section. The difference being the ability to change libraries or include only specific ones.

## Tests

Unit test for all components and integration tests for some are available

A Unit tests can be run (here for example the xmem_Test):

    $ bazel test test:xmem_Test

For running all unit tests:

    $ bazel test test:all
    
For running the integration test, they need to be uploaded and than checked using screen:

    $ bazel run //test/integration:test_elasticNodeMiddleware_upload --platforms=@AvrToolchain//platforms:ElasticNode_v4
