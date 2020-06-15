from serial_test import SerialTest
import time

def test_writedummy():
    serialTest = SerialTest()
    # serialTest.fetchBit("dummy")
    assert serialTest.sendConfig(serialTest.dummyConfig, flash=True)

def test_verifydummy():
    serialTest = SerialTest()
    assert serialTest.verify(serialTest.dummyConfig, mcuFlash=True)

def test_verifydummyfpga():
    serialTest = SerialTest()
    assert serialTest.verify(serialTest.dummyConfig, mcuFlash=False)


def test_loaddummy():
    serialTest = SerialTest()
    result = serialTest.readConfig(serialTest.dummyConfig, selectmap=True)

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

def test_warmbootdummy():
    serialTest = SerialTest()
    # serialTest.annConfig.loadFile()
    assert serialTest.warmboot(serialTest.dummyConfig)

def test_bootdummy():
    serialTest = SerialTest()
    serialTest.resetFPGA()
    assert serialTest.multiboot(serialTest.dummyConfig)

def test_hwfiddummy():
    serialTest = SerialTest()
    result = int(serialTest.hwfid())

    assert result == 0xdd

def test_writedummyfpgaflash():
    serialTest = SerialTest()
    assert serialTest.readConfig(serialTest.dummyConfig)

if __name__ == "__main__":
    test_writedummy()
# test_verifydummy()
# test_loaddummy()
# test_writedummyfpgaflash()
# test_verifydummyfpga()
# test_bootdummy()
# test_warmbootdummy()
# test_hwfiddummy()
