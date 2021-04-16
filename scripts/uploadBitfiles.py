from scripts.flashBitfiles import SerialTest
from scripts.bitfileConfigs import BitfileConfigs
from scripts.portConfigs import Config as portConfigs
import time


def test_writesMultiConfigs():
    serialTest = SerialTest(portConfigs.portToElasticnode, portConfigs.portToProgrammer)
    bitfileConfigs = BitfileConfigs()
    # serialTest.fetchBit("")
    assert serialTest.sendConfig(bitfileConfigs.ConfigPart1, flash=True)
    assert serialTest.sendConfig(bitfileConfigs.ConfigPart2, flash=True)


def test_verifyMultiConfigs():
    serialTest = SerialTest(portConfigs.portToElasticnode, portConfigs.portToProgrammer)
    bitfileConfigs = BitfileConfigs()
    assert serialTest.verify(bitfileConfigs.ConfigPart1, mcuFlash=True)
    assert serialTest.verify(bitfileConfigs.ConfigPart2, mcuFlash=True)


def test_verifyMultiConfigsfpga():
    serialTest = SerialTest(portConfigs.portToElasticnode, portConfigs.portToProgrammer)
    bitfileConfigs = BitfileConfigs()
    assert serialTest.verify(bitfileConfigs.ConfigPart1, mcuFlash=False)
    assert serialTest.verify(bitfileConfigs.ConfigPart2, mcuFlash=False)


def test_loadMultiConfigs():
    serialTest = SerialTest(portConfigs.portToElasticnode, portConfigs.portToProgrammer)
    bitfileConfigs = BitfileConfigs()
    result = serialTest.readConfig(bitfileConfigs.ConfigPart1, selectmap=True)
    result = serialTest.readConfig(bitfileConfigs.ConfigPart2, selectmap=True)

    print("sleep 1")
    time.sleep(0.5)
    print("set 1")
    serialTest.setFpgaFlashInterface(1)
    print("sleep 2")
    time.sleep(0.5)
    print("set 0")
    serialTest.setFpgaFlashInterface(0)
    print("after")
    time.sleep(0.5)

    assert result == True


def test_warmbootMultiConfigs():
    serialTest = SerialTest(portConfigs.portToElasticnode, portConfigs.portToProgrammer)
    bitfileConfigs = BitfileConfigs()
    # serialTest.annConfig.loadFile()
    assert serialTest.warmboot(bitfileConfigs.ConfigPart1)
    assert serialTest.warmboot(bitfileConfigs.ConfigPart2)


def test_bootMultiConfigs():
    serialTest = SerialTest(portConfigs.portToElasticnode, portConfigs.portToProgrammer)
    bitfileConfigs = BitfileConfigs()
    serialTest.resetFPGA()
    assert serialTest.multiboot(bitfileConfigs.ConfigPart1)
    assert serialTest.multiboot(bitfileConfigs.ConfigPart1)


def test_hwfidMultiConfigs():
    serialTest = SerialTest(portConfigs.portToElasticnode, portConfigs.portToProgrammer)
    result = int(serialTest.hwfid())

    assert result == 0xdd


def test_writeMultiConfigsfpgaflash():
    serialTest = SerialTest(portConfigs.portToElasticnode, portConfigs.portToProgrammer)
    bitfileConfigs = BitfileConfigs()
    assert serialTest.readConfig(bitfileConfigs.ConfigPart1)
    assert serialTest.readConfig(bitfileConfigs.ConfigPart2)


if __name__ == "__main__":
    test_writesMultiConfigs()
    # test_verifyMultiConfigs()
    # test_loadMultiConfigs()
    # test_writeMultiConfigsfpgaflash()
    # test_verifyMultiConfigsfpga()
    # test_bootMultiConfigs()
    # test_warmbootMultiConfigs()
    # test_hwfidMultiConfigs()
