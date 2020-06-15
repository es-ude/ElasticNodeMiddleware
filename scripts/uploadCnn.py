from serial_test import SerialTest
import time

def test_writecnn():
    serialTest = SerialTest()
    # serialTest.fetchBit("cnn")
    assert serialTest.sendConfig(serialTest.cnnConfig, flash=True)

def test_verifycnn():
    serialTest = SerialTest()
    assert serialTest.verify(serialTest.cnnConfig, mcuFlash=True)

def test_verifycnnfpga():
    serialTest = SerialTest()
    assert serialTest.verify(serialTest.cnnConfig, mcuFlash=False)


def test_loadcnn():
    serialTest = SerialTest()
    result = serialTest.readConfig(serialTest.cnnConfig, selectmap=True)

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

def test_warmbootcnn():
    serialTest = SerialTest()
    # serialTest.annConfig.loadFile()
    assert serialTest.warmboot(serialTest.cnnConfig)

def test_bootcnn():
    serialTest = SerialTest()
    serialTest.resetFPGA()
    assert serialTest.multiboot(serialTest.cnnConfig)

def test_hwfidcnn():
    serialTest = SerialTest()
    result = int(serialTest.hwfid())

    assert result == 0xdd

def test_writecnnfpgaflash():
    serialTest = SerialTest()
    assert serialTest.readConfig(serialTest.cnnConfig)

if __name__ == "__main__":
    test_writecnn()
# test_verifycnn()
# test_loadcnn()
# test_writecnnfpgaflash()
# test_verifycnnfpga()
# test_bootcnn()
# test_warmbootcnn()
# test_hwfidcnn()
