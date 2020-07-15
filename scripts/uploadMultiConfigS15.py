from serial_test import SerialTest
from bitfileConfigs import BitfileConfigs
import time

def test_writesMultiConfigs_s15():
    serialTest = SerialTest()
    bitfileConfigs = BitfileConfigs()
    # serialTest.fetchBit("s15")
    assert serialTest.sendConfig(bitfileConfigs.s15ConfigPart1, flash=True)
    assert serialTest.sendConfig(bitfileConfigs.s15ConfigPart2, flash=True)

def test_verifyMultiConfigs_s15():
    serialTest = SerialTest()
    bitfileConfigs = BitfileConfigs()
    assert serialTest.verify(bitfileConfigs.s15ConfigPart1, mcuFlash=True)
    assert serialTest.verify(bitfileConfigs.s15ConfigPart2, mcuFlash=True)

def test_verifyMultiConfigs_s15fpga():
    serialTest = SerialTest()
    bitfileConfigs = BitfileConfigs()
    assert serialTest.verify(bitfileConfigs.s15ConfigPart1, mcuFlash=False)
    assert serialTest.verify(bitfileConfigs.s15ConfigPart2, mcuFlash=False)

def test_loadMultiConfigs_s15():
    serialTest = SerialTest()
    bitfileConfigs = BitfileConfigs()
    result = serialTest.readConfig(bitfileConfigs.s15ConfigPart1, selectmap=True)
    result = serialTest.readConfig(bitfileConfigs.s15ConfigPart2, selectmap=True)

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

def test_warmbootMultiConfigs_s15():
    serialTest = SerialTest()
    bitfileConfigs = BitfileConfigs()
    # serialTest.annConfig.loadFile()
    assert serialTest.warmboot(bitfileConfigs.s15ConfigPart1)
    assert serialTest.warmboot(bitfileConfigs.s15ConfigPart2)

def test_bootMultiConfigs_s15():
    serialTest = SerialTest()
    bitfileConfigs = BitfileConfigs()
    serialTest.resetFPGA()
    assert serialTest.multiboot(bitfileConfigs.s15ConfigPart1)
    assert serialTest.multiboot(bitfileConfigs.s15ConfigPart1)

def test_hwfidMultiConfigs_s15():
    serialTest = SerialTest()
    result = int(serialTest.hwfid())

    assert result == 0xdd

def test_writeMultiConfigs_s15fpgaflash():
    serialTest = SerialTest()
    bitfileConfigs = BitfileConfigs()
    assert serialTest.readConfig(bitfileConfigs.s15ConfigPart1)
    assert serialTest.readConfig(bitfileConfigs.s15ConfigPart2)

if __name__ == "__main__":
    test_writesMultiConfigs_s15()
# test_verifyMultiConfigs_s15()
# test_loadMultiConfigs_s15()
# test_writeMultiConfigs_s15fpgaflash()
# test_verifyMultiConfigs_s15fpga()
# test_bootMultiConfigs_s15()
# test_warmbootMultiConfigs_s15()
# test_hwfidMultiConfigs_s15()
