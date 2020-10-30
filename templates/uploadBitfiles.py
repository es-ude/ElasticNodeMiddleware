from scripts.serial_test import SerialTest
from myscripts.bitfileConfigs import BitfileConfigs
from myscripts.portConfigs import Config as portConfigs

def writeexample():
    serialTest = SerialTest(portConfigs.portToElasticnode, portConfigs.portToProgrammer)
    bitfileConfigs = BitfileConfigs()
    assert serialTest.sendConfig(bitfileConfigs.ConfigPart1, flash=True)
    assert serialTest.sendConfig(bitfileConfigs.ConfigPart2, flash=True)

if __name__ == "__main__":
    writeexample()