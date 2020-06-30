from serial_test import SerialTest
import time

def test_writes15():
    serialTest = SerialTest()
    # serialTest.fetchBit("s15")
    assert serialTest.sendConfig(serialTest.s15Config, flash=True)

def test_verifys15():
    serialTest = SerialTest()
    assert serialTest.verify(serialTest.s15Config, mcuFlash=True)

def test_verifys15fpga():
    serialTest = SerialTest()
    assert serialTest.verify(serialTest.s15Config, mcuFlash=False)


def test_loads15():
    serialTest = SerialTest()
    result = serialTest.readConfig(serialTest.s15Config, selectmap=True)

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

def test_warmboots15():
    serialTest = SerialTest()
    # serialTest.annConfig.loadFile()
    assert serialTest.warmboot(serialTest.s15Config)

def test_boots15():
    serialTest = SerialTest()
    serialTest.resetFPGA()
    assert serialTest.multiboot(serialTest.s15Config)

def test_hwfids15():
    serialTest = SerialTest()
    result = int(serialTest.hwfid())

    assert result == 0xdd

def test_writes15fpgaflash():
    serialTest = SerialTest()
    assert serialTest.readConfig(serialTest.s15Config)

if __name__ == "__main__":
    test_writes15()
# test_verifys15()
# test_loads15()
# test_writes15fpgaflash()
# test_verifys15fpga()
# test_boots15()
# test_warmboots15()
# test_hwfids15()
