from scripts.serial_test import SerialTest
from bitfileConfigs import BitfileConfigs
from portConfigs import Config as portConfigs

def writeexample():
    serialTest = SerialTest(portConfigs.portToElasticnode, portConfigs.portToProgrammer)
    bitfileConfigs = BitfileConfigs()
    assert serialTest.sendConfig(bitfileConfigs.ConfigPart1, flash=True)
    if bitfileConfigs.ConfigPart2 is not None:
        assert serialTest.sendConfig(bitfileConfigs.ConfigPart2, flash=True)

if __name__ == "__main__":
    writeexample()
