#!/usr/bin/env python
from warnings import warn
import os
import serial
import sys
import shlex
import time
import glob
import subprocess
import time
import string
import numpy as np
import warnings
import threading
import struct
import queue as queue
from threading import Thread
# from serial_plotter import SerialPlotter

import sched
from scripts.Configuration import Configuration

from scripts.portConfigs import Config as portConfigs

PLOT_DELAY = 0.5
# AMOUNT = 256

TIME_TOTAL = 1
# import os
# # print(os.environ['BAUD'])
# if os.environ['BAUD'] is None:
# 	baudrate = 115200
# else:
# 	baudrate = os.environ['BAUD']
# print "Baudrate:", baudrate
CURRENT_MEASUREMENT_FLOAT = 20
CURRENT_BUFFER_SIZE = 20

TEST_ADDRESS = 0
ANN_WEIGHTS_ADDRESS = 0x121000
ANN_ADDRESS = 0x90000
CNN_ADDRESS = 0x0
BSCAN_ADDRESS = 0xC0000
FIR_ADDRESS = 0x180000
MM_ADDRESS = 0xC0000
VDP_ADDRESS = 0x120000
SMALL_ADDRESS = 0x0
DUMMY_ADDRESS = 0x0

SKIP = None # (538844 - 256) # 4096 * 5

cpuName = "at90usb1287"
en4_serial_template = portConfigs.portToElasticnode
#en4_serial_template = "/dev/ttyACM1"
en3_serial_template = "/dev/tty.usbserial-EN*"
# serial_template = "/dev/ttyS256*"
if 'posix' not in os.name:
    serial_default = "/dev/ttys4"
else:
    serial_default = None
program_template = portConfigs.portToProgrammer
#program_template = "/dev/ttyACM0"
if 'posix' not in os.name:
    program_default = "/dev/ttys3" # "/dev/ttyS5" # "COM5"
else:
    program_default = None
# serial_template = "/dev/tty.usbserial-AL03E2KG"

jtag = False
selectmap = False
flash = False
plot = True
baudrate = None

class SerialTest:
    ser = None
    monitoring = False
    plotter = None
    monitoringThread = None
    debug = False
    # selectmap = /False
    busy = False # dont request current because interface in use
    debugQ = None
    programPort = None


    baud, port, bitFile, testFile, bscanFile, size, testsize, bscansize = None, None, None, None, None, None, None, None
    skip, testskip, bscanskip = None, None, None

    dummyConfig, smallConfig, testConfig, bscanConfig, annConfig, firConfig, mmConfig, vdpConfig = None, None, None, None, None, None, None, None

    backgroundDebug = False

    elasticNodeVersion = None

    def __init__(self, enVersion=4, backgroundDebug=False):
        self.elasticNodeVersion = enVersion

        if self.elasticNodeVersion == 4:
            # change to 500000
            baudrate = 500000
        else:
            baudrate = 115200

        self.backgroundDebug = backgroundDebug

        if self.backgroundDebug:
            self.debugQ = Queue()
            debugThread = Thread(target=self.debugWorker)
            debugThread.daemon = True
            debugThread.start()

        # setup configurations
        self.dummyConfig = Configuration("../bitfiles/dummy.bit", DUMMY_ADDRESS, DUMMY_ADDRESS)
        self.smallConfig = Configuration("small.bit", SMALL_ADDRESS, SMALL_ADDRESS, special=True)
        self.testConfig = Configuration("test.bit", TEST_ADDRESS, TEST_ADDRESS)
        self.bscanConfig = Configuration("bit_file_bscan.bit", BSCAN_ADDRESS)
        self.annConfig = Configuration("ann.bit", ANN_ADDRESS, ANN_ADDRESS)

        # now is made in own config!
        
        try:
            # if plot:
            # 	self.plotter = SerialPlotter()

            try:
                self.baud = int(sys.argv[2])
                self.port = sys.argv[1]
                self.bitFile = sys.argv[3]
            except Exception:
                # print 'finding serial...'
                if self.elasticNodeVersion == 4:
                    serial_template = en4_serial_template
                else:
                    serial_template = en3_serial_template

                self.baud = baudrate
                if serial_default is None:
                    ports = glob.glob(serial_template)
                    # ports = glob.glob("/dev/tty.usbserial-A51ZP5TJ")

                    if len(ports) == 0:
                        print("no serial ports...")
                        time.sleep(2)
                        ports = glob.glob(serial_template)
                        if len(ports) == 0:
                            print("still no serial ports...")
                            sys.exit(0)
                        else:
                            self.port = ports[0]
                    elif len(ports) == 1:
                        self.port = ports[0]
                    else:
                        print("too many serial ports...")
                        print(ports)
                        sys.exit(0)
                else:
                    self.port = serial_default

            print('Opening serial...', self.port, '@', self.baud)
            exists = os.path.exists(self.port)
            print("port", exists)
            if program_default is None:
                self.programPort = glob.glob(program_template)[0]
            else:
                self.programPort = program_default

            self.resetMcu()

            if exists:
                retries = 0
                while not os.path.exists(self.port) and retries < 10:
                    retries += 1
                    time.sleep(0.5)
                    print("waiting for port...")

            self.ser = serial.Serial(self.port, self.baud,)
            self.remainingMonitor = 0;

            self.ser.read(self.ser.inWaiting())
            # print("FLUSHING")
            self.readSerial()
            # print("DONE")
            self.ser.reset_input_buffer()
            response = '0'
            self.ser.timeout = 0.5#  * 10
            request = '0'



            # read any old data
            self.ser.read(1000) # self will always cause a timeout delay


            self.ser.timeout = 2.5#  * 10

            expectedResponse = 'x'
            while ord(response) != ord(expectedResponse):
                self.ser.reset_input_buffer();

                expectedResponse = chr(ord(request) + 1)
                self.ser.write(request.encode())
                response = self.ser.read(1)

                if len(response) > 0:
                    # print('request:', ord(request), 'response:', ord(response), 'expected:', ord(expectedResponse))
                    continue
                else:
                    print('nothing received')
                    response = 'x'

                # set up next request:
                request = expectedResponse
                if ord(request) > ord('9'): request = '0'
            # self.ser.reset_input_buffer();
            # self.ser.timeout = None

            print('Serial available')
        except KeyboardInterrupt:
            print('Interrupted initialisation')
            sys.exit(0)


    # @staticmethod
    def resetMcu(self, stk=True):
        print("Resetting MCU",)
        if stk:

            print('port', self.programPort)
            if self.programPort is None:
                print("no program port for resetting MCU!")
                sys.exit(0)

            cmd = "avrdude -c stk500 -p " + cpuName + " -P %s -n" % self.programPort
            # print(cmd)

            process = subprocess.Popen(shlex.split(cmd), stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            (output, err) = process.communicate()
            exitCode = process.wait()

            time.sleep(2)
            print('done')
        # subprocess.call(['avrdude', '-c stk500 -p atmega64 -P /dev/tty.usbmodem* -n'])
        else:
            print("other programmers not implemented")


    # @staticmethod
    def fetchBit(self, configName):
        warn("fetching disabled!")
    # print("Fetching", configName)
    # cmd = "./scripts/fetch-bitfile.sh %d %s" % (self.elasticNodeVersion, configName)
    # print("cmd", cmd)
    # process = subprocess.Popen(shlex.split(cmd), stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    # (output, err) = process.communicate()
    # exitCode = process.wait()
    # print(output)
    # print(err)
    # print(exitCode)

    remainingCache = ""
    debugOpen = False
    debugCache = ""

    def debugWorker(self):
        print("\tDEBUG worker active")
        while True:
            # print("\n:\tDEBUG: %s" % self.debugQ.get(),
            sys.stdout.write("%s" % self.debugQ.get())
            self.debugQ.task_done()

    def readSerialBlocking(self, number):
        bk = self.ser.timeout
        self.ser.timeout = None
        readData = self.ser.read(number)
        self.ser.timeout = bk
        return readData
    # print("serialblocking", number)
    # readData = self.ser.read(max(self.ser.in_waiting,number)) # read at least number bytes
    # remaining = self.filterDebug(readData, number)

    # # limit number of results
    # if len(remaining):
    # 	warn("Too many bytes received")
    # 	remaining = remaining[:number];
    # return remaining

    def filterDebug(self, data, number=None):
        DEBUG = True

        # if len(data) > 0:
        # 	if DEBUG:
        # 		print("DEBUG INPUT '%s'" % data)
        # remaining = ""

        # # check for ##
        # if "#" in data:
        # 	begin = data.find("#")
        # 	if DEBUG:
        # 		print('DEBUG BEGIN:', begin)

        # 	# if begin >= 0:
        # 	# self is close
        # 	if self.debugOpen:
        # 		if DEBUG:
        # 			print("DEBUG OPEN")
        # 			print("DEBUG CACHE", self.debugCache)
        # 			print("@", begin, data[:begin])
        # 		debugLine = "%s%s" % (self.debugCache, data[:begin])
        # 		# print("\tDEBUGLINE '%s'" % debugLine)
        # 		self.debugQ.put(debugLine)
        # 		self.debugOpen = False

        # 		remaining = "%s%s" % (self.remainingCache, data[begin+1:])
        # 		if DEBUG:
        # 			print("REMAINING '%s'" % remaining)
        # 		remaining = self.filterDebug(remaining)

        # 	# self is open
        # 	else:
        # 		waitForMore = True
        # 		if len(data) > begin+1:
        # 			if DEBUG:
        # 				print("DEBUG LOOKING FOR OPEN")
        # 			# full debug
        # 			if "#" in data[begin+1:]:
        # 				close = data[begin+1:].find("#")
        # 				if DEBUG:
        # 					print("DEBUGCACHE", self.debugCache)
        # 					print("INDEX", begin, close)
        # 					print("DATA '", data, "'")
        # 					print("BOTH #?", data[begin], data[begin + close])
        # 				debugLine = "%s%s" % (self.debugCache, data[begin+1:begin+close+1])
        # 				self.debugQ.put(debugLine)
        # 				if DEBUG:
        # 					print("\tDEBUGLINE '%s'" % debugLine)
        # 				self.debugOpen = False
        # 				self.debugCache = ""

        # 				waitForMore = False

        # 				if DEBUG:
        # 					print("'%s' '%s'" % (data[:begin], data[begin + close + 2:])
        # 				remaining = "%s%s" % (data[:begin], data[begin + close + 2:])
        # 				if DEBUG:
        # 					print("REMAINING %s" % remaining)
        # 				time.sleep(.01)
        # 				remaining = self.filterDebug(remaining)



        # 		if waitForMore:
        # 			self.debugOpen = True
        # 			if DEBUG:
        # 				print("@", begin, data[begin:])

        # 				print
        # 				print("DEBUG WAITING")
        # 			if self.ser.in_waiting:
        # 				self.debugCache = data[begin+1:]
        # 				self.remainingCache = data[:begin]
        # 				remaining = self.filterDebug(self.ser.read(self.ser.in_waiting))
        # 			else:
        # 				if number is not None:
        # 					remaining = self.filterDebug(self.ser.read(number))
        # 				else:
        # 					if DEBUG:
        # 						print("NO DEBUG AND NO NUMBER")
        # else:
        # 	if len(data) > 0:
        # 		if DEBUG:
        # 			print("REMAINING IS EVERYTHING", data)
        # 	remaining = data

        self.remainingCache = ""
        return data

    def readSerialLine(self):
        # print('reading line')
        data = ""
        incoming = 0x00
        get_line_ended_flag=False
        while get_line_ended_flag==False:
            incoming = self.ser.read(1)
            # print('incoming', incoming, ord(incoming))
            data = data + str(incoming, encoding='utf-8')
            get_line_ended_flag = (ord(incoming)== ord('\n'))
            # print('[chao_debug] readSerialLine, incoming: ', incoming, "str so far get: ", data, 'flag: ',flag )
        if data[-1] == '\n':
            data = data[:-1]
        if len(data) > 0:
            if data[-1] == '\r':
                data = data[:-1]
        return data
    # data = ""
    # while not data.endswith('\n'):
    # 	data = self.readSerial()
    # 	if data != "":
    # 		print("data: %s" % data)
    # return data
    # return self.ser.readline()

    def readSerial(self):
        read = self.ser.read(self.ser.in_waiting)
        # if len(read) > 0:
        # 	if DEBUG:
        # 		print("READ %s" % read)
        remaining = self.filterDebug(read)
        return remaining

    def waitSerialDone(self):
        # print("waiting...")
        data = ""
        while "$$" not in data:
            data = self.readSerialLine()
            # print('[chao_debug] waitSerialDone, data is:',data)
            if len(data) > 0:
                if self.backgroundDebug:
                    # print(data)
                    self.debugQ.put(data)
                elif len(data) > 2:
                    # copy = str(data)
                    # print((copy.replace("$$", "")))
                    print(data)
        # print("data:", data)
        # print("sleeping...")
        # time.sleep(1)

        # print(data)

        # print("DONE")
        if self.backgroundDebug: sys.exit(0)

        # print(self.ser.in_waiting)

        return True

    def waitSerialReady(self, quiet=False):
        # print("waiting for ready...")
        data = ""
        while "%%" not in data:
            data = self.readSerialLine()
            # print('[chao_debug] waitSerialReady, data is:',data)
            # data = self.ser.readline()
            if len(data) > 0:
                if "%%" not in data:
                    if not quiet:
                        if self.backgroundDebug:
                            self.debugQ.put(data)
                        else:
                            print(data)
        # print("data:", data)
        # print("sleeping...")
        # time.sleep(1)

    # print(data)

    # print("READY")


    def sendzero(self, num):
        # send zeros
        ba = bytearray([0, num & 0xff, (num >> 8) & 0xff, (num >> 16) & 0xff, (num >> 24) & 0xff])
        # ba = ba[::-1]
        # print('sending', num, 'zeros',)
        for i in range(len(ba)):
            self.ser.write([ba[i]])

        # print('confirming...', num)

        self.confirmCommand(0x00)
    # print('done')

    def receiveData(self):
        t0 = time.time()
        try:
            print('receiving for %d s' % TIME_TOTAL)
            while (time.time() - t0 < TIME_TOTAL or self.remainingMonitor > 0):
                if (self.ser.in_waiting > 0): #if incoming bytes are waiting to be read from the serial input buffer
                    data_in = self.ser.read(self.ser.inWaiting())
                    data_str = data_in.decode('ascii') #read the bytes and convert from binary array to ASCII
                    sys.stdout.write(data_str), #print(the incoming string without putting a new-line ('\n') automatically after every print())
                    sys.stdout.flush()

                    # if data_str[:2] == "#M":
                    # 	self.remainingMonitor -= 1
                    # 	print('Remaining:', self.remainingMonitor)

                    t0 = time.time()
        #Put the rest of your code you want here
        except UnicodeDecodeError:
            print(data_in)
        finally:

            print('done')

    def sendValue32(self, value, name="value"):
        # sending address
        ba = bytearray([value & 0xff, (value >> 8) & 0xff, (value >> 16) & 0xff, (value >> 24) & 0xff])

        for i in range(4):
            self.ser.write([ba[i]])


        print('done writing %s' % name)

        print('%s of bitfile: ' % name,)
        # received = self.ser.readline()
        received = self.readSerialLine()
        print('received \'{}\''.format(received),)
        try:
            if value != int(received):
                print('%s incorrect! aborting.' % name)
                return False
        except:
            print('did not receive %s:' % name, received)
            return False

        return True

    # read configuration from flash memory
    def readConfig(self, config, selectmap=False, jtag=False):
        config.loadFile()
        self.busy = True

        # if size is None:
        # 	print('inferring size')
        # 	self.getAllSizes()
        # 	size = self.size - self.skip

        # if size == 0:
        # 	print('size zero')
        # 	return

        if selectmap:
            self.writeCommand(b'P')
        elif jtag:
            self.writeCommand(b'j')
        else:
            self.writeCommand(b'f')
        print('address', config.address, config.size)

        print("address:")
        self.writeValue(config.address, "address", quiet=False)
        # if not self.writeValue(config.address, "address", quiet=False): return
        print("size:")
        self.writeValue(config.size, "size")
        # if not self.writeValue(config.size, "size"): return

        if not selectmap and not jtag:
            # sending to flash
            self.writeValue(config.destination, "destination")
        # if not self.writeValue(config.destination, "destination"):
        # return

        self.waitSerialReady()
        # self is when it actually happens

        # wait for confirmation
        self.busy = False

        return self.waitSerialDone()

    # read configuration from flash memory
    def verify(self, config, mcuFlash):
        config.loadFile()
        # read in blocks
        BLOCK_SIZE = 256

        remaining = config.size
        if mcuFlash:
            position = config.address
        else:
            position = config.destination

        print(mcuFlash, position)

        # if size > self.size:
        # 	size = self.size

        # open bitfile to compare
        bit = open(config.filename, 'rb')

        # skip header
        bit.read(config.skip)
        remaining -= config.skip

        if SKIP is not None:
            print("Skipping many!")
            bit.read(SKIP)
            remaining -= SKIP
            position += SKIP

        count = 0

        # store errors for printing afterward
        errorLines = list()

        ret = True
        while remaining > 0:
            # if True:
            if remaining > BLOCK_SIZE:
                block = BLOCK_SIZE
            else:
                block = remaining


            # print('\tVerifying:', position, block)
            # time.sleep(1)
            if mcuFlash:
                self.writeCommand(b'V')
            else:
                self.writeCommand(b'v')
            # print('Command done')
            self.writeValue(position, "address", quiet=True)
            self.writeValue(block, "size", quiet=True)

            errorLines = list()

            self.ser.timeout = 1

            b = bit.read(block)
            count += 1

            to = self.ser.timeout
            self.ser.timeout = None
            read = self.ser.read(block)

            self.ser.timeout = to

            if (len(b) != len(read)):
                ret = False
                remaining = 0
                print("incorrect number of bytes read!", len(b), len(read))
                errorLines.append("Incorrect number of bytes read %d %d" % (len(b), len(read)))

                for i in range(len(read)):
                    r = ord(read[i])
                    br = ord(b[i])
                    if r != br:
                        print(('READBACK FAIL %d %X expected: %d received: %d %s %s' % (i, position + i, br, r, b[i], read[i])))


                continue

            for i in range(block):
                r = ord(read[i])
                br = ord(b[i])
                if r != br:
                    errorLines.append('READBACK FAIL %d %X expected: %d received: %d %s %s' % (i, position + i, br, r, b[i], read[i]))
            # else:
            # errorLines.append('READBACK FINE %d %X expected: %d received: %d %s %s' % (i, position + i, br, r, b[i], read[i]))

            # errorLines.append('READBACK FINE', i, position + i, b, read)

            if len(errorLines) > 0:
                for l in errorLines[:10]:
                    print(l,)
                    print("")
                # stop verifying once an error is found
                remaining = 0
                # remaining -= block

                print("Errors occurred!")
                ret = False
            else:
                sys.stdout.write('.')
                sys.stdout.flush()
                # print('.',)

                position += block
                remaining -= block

            self.waitSerialReady()
            self.waitSerialDone()

        bit.close()

        print('\nVerify complete...')
        return ret


    def sendConfig(self, config, selectmap=False, jtag=False, flash=False, fpgaflash=False):

        config.loadFile()

        sending = None
        bit = None

        ret = True
        try:
            sendcount = 0
            print('waiting for ack...')

            if jtag: self.writeCommand(b'J')
            elif selectmap: self.writeCommand(b'M') # also broken somehow
            elif flash:
                self.writeCommand(b'F')
                # print('[chao_debug] command F sent.')
            elif fpgaflash: self.writeCommand(b'p')
            else: print(("No idea how to send config!!"))

            # sending bitfile
            bit = open(config.filename, "rb")
            # skip the first bunch of data
            print('skipping', config.skip)
            # skip the first bunch of data
            bit.read(config.skip)

            if SKIP is not None:
                print("Skipping many!")
                bit.read(SKIP)
                config.size -= SKIP
                config.address += SKIP

            address = config.address
            if flash:
                print('sending address', config.address)
                self.writeValue(config.address, "address")
            elif fpgaflash:
                print('sending remote address', config.destination)
                self.writeValue(config.destination, "destination")
            # sending size
            # print('[chao_debug] config_size is ', config.size)
            self.writeValue(config.size, "size");

            # print('[chao_debug] wainting for flash erase finished\r\n')
            # give device time for some debug
            self.waitSerialReady(quiet=False)
            print('sending data')

            oldperc = -1;

            # self.ser.timeout = None
            blockSize = 256
            currentAddress = 0


            while currentAddress < config.size:
                # print('[chao_debug] in sending loop..\r\n')
                perc = int(float(currentAddress) / config.size * 100)

                if oldperc != perc:
                    sys.stdout.write('\r{}%'.format(perc))
                    sys.stdout.flush()
                    # print('\r{}%'.format(perc))
                    oldperc = perc

                if (config.size - currentAddress) < blockSize:
                    print("Last block!")
                    blockSize = config.size - currentAddress
                    # print(blockSize)

                elif (config.size - currentAddress) == blockSize:
                    print("Last full block!")


                # self.ser.flushInput()# might can be delete
                # self.ser.flushOutput()# might can be delete
                sending = bytearray(bit.read(blockSize))
                # sending = b'\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff'#bit.read(blockSize)
                # sending = (b'\255\255\x33\x31\x31\x31\x31\x31\x31\x31\x31\x31\x32\x33\x34\x35')
                # real_send = b'\xAa\xBb'+sending+b'\xCc\xDd'
                # time.sleep(0.01) # might can be delete
                # print('[chao_debug] a block is read out from bit file, type is',type(sending),',its size is:', len(sending))
                # print('[chao_debug] data content', sending)
                

                bytes_has_written = self.ser.write(sending)
                self.ser.flush()
                # or
                # self.serial_send_block(sending, blockSize)



                # print('[chao_debug] data block written finished. ')

                if flash or fpgaflash:
                    if not self.confirmCommand(sending[-1]):
                        print("Could not confirm data!")

                        if bit is not None:
                            bit.close()
                        ret = False
                        break
                currentAddress += blockSize
                # print('[chao_debug] data has been send to the mcu wait for ack..\r\n')
                self.waitSerialDone()

            # # confirm the last
            print('ready to proceed with uart')
            self.waitSerialDone()
        except KeyboardInterrupt:
            pass
        finally:
            if bit is not None:
                bit.close()

        return ret
    # 	self.ser.close();

    def serial_send_block(self, block_data, len):
        print("block to send:", block_data)
        cnt = 0
        while(cnt<len):
            # if ord(block_data[cnt:cnt+1])==255:
            #     self.ser.write(block_data[cnt:cnt+1])
            #     self.ser.flush()
            self.ser.write(block_data[cnt:cnt+1])
            self.ser.flush()
            self.ser.flushOutput()
            # time.sleep(0.002)
            print("cnt:", cnt ,block_data[cnt:cnt+1])
            cnt=cnt+1
    def findCurrentSensors(self):
        self.writeCommand('s')

    def controlFpga(self, newvalue):
        if newvalue:
            self.writeCommand("C");
        else:
            self.writeCommand("c");
        self.waitSerialDone()

    def controlLed(self, newvalue):
        if newvalue:
            self.writeCommand("L")
        else:
            self.writeCommand("l")


    def controlDebug(self, newvalue):
        self.debug = newvalue
        if newvalue:
            self.writeCommand("D")
        else:
            self.writeCommand("d")


    # def startMonitor(self):
    # 	# send command to start monitoring
    # 	self.writeCommand(b'M')

    # 	self.monitoring = True

    # 	threading.Thread(target=self.waitWhileMonitoring, args=[]).start()
    # 	# self.remainingMonitor = length


    # def endMonitor(self):
    # 	# send command to start monitoring
    # 	self.writeCommand(b'm')

    # 	# flush out the last results
    # 	# self.printAllMeasurements();

    # 	# print("Values:",)
    # 	# print(self.ser.readline())
    # 	# self.remainingMonitor = 0
    # 	self.monitoring = False

    def resetUL(self):
        # send a reset signal to the fpga (only resets the ul)
        self.writeCommand(b'r')
        return self.waitSerialDone()

    def resetFPGA(self):
        self.writeCommand(b'R')
        return self.waitSerialDone();

    def setFpgaFlashInterface(self, enabled):
        if enabled:
            self.writeCommand(b'[')
        else:
            self.writeCommand(b']')
        return self.waitSerialDone()

    def testTransfer(self, variable=False):
        self.writeCommand(b'T')
        if variable:
            self.writeCommand(b'V')
        else:
            self.writeCommand(b'T')

    def testTransferLatency(self):
        self.writeCommand(b'T')
        self.writeCommand(b't')

    def testSectionTransfer(self):
        self.writeCommand(b'T')
        self.writeCommand(b'v')

    def testLeds(self):
        self.writeCommand(b'T')
        self.writeCommand(b'L')
        return self.waitSerialDone()

    def testWireless(self):
        self.writeCommand(b'T')
        self.writeCommand(b'W')
        self.waitSerialDone();

    def testFlash(self):
        self.writeCommand(b'T')
        self.writeCommand(b'F')
        self.waitSerialDone();

    def learnWireless(self):
        self.writeCommand(b'T')
        self.writeCommand(b'l')
        self.waitSerialDone();

    def wirelessBlock(self):
        self.beginExperiment()


        self.writeCommand(b'T')
        self.writeCommand(b'w')

        print('waiting...')

        self.plotter.clearAverageCurrent()

        self.latency(request=False, plot=True, offloading=True)

        print('almost done')
        self.endExperiment()
        self.ser.readline()
        print('done')

    def testFIR(self):
        self.writeCommand(b'T')
        self.writeCommand(b'f')
        self.waitSerialDone()

    def testANNsmall(self):
        self.writeCommand(b'T')
        self.writeCommand(b'a')
        self.waitSerialDone()

    def testANN(self, repeat=1):
        self.suspend(False)

        self.beginExperiment()

        # perform experiments with ANN
        for i in range(repeat):
            self.writeCommand(b'T')
            self.writeCommand(b'A')

            # if self.debug:
            print('waiting...')
            self.plotter.clearAverageCurrent()

            self.latency(request=False, plot=True, offloading=False)

            print('almost done')

        # print('plot', plot)
        # if plot:
        # 	latency = self.latency(request=False, plot=True)
        # 	print('latency:', latency)

        # else:
        # 	for i in range(5):
        # 		print(self.ser.readline(),)
        # 	print('')

        self.endExperiment()
        self.ser.readline()
        print('done')
        self.suspend(True)

    def storeWeights(self, address):
        self.writeCommand(b'b')

        # ba = bytearray([address & 0xff, (address >> 8) & 0xff, (address >> 16) & 0xff])
        # for i in range(3):
        # 	self.ser.write([ba[i]])
        self.writeValue(address, "address", quiet=True)

    def loadWeights(self, address):
        self.writeCommand(b'Y')

        # ba = bytearray([address & 0xff, (address >> 8) & 0xff, (address >> 16) & 0xff])
        # for i in range(3):
        # 	self.ser.write([ba[i]])
        self.writeValue(address, "address", quiet=True)

    def readWeights(self, address):
        self.writeCommand(b'B')

        self.writeValue(address, "address", quiet=True)


        result = self.ser.read(120)
        self.ser.readline() # indicates it's done

        np.set_printoptions(precision=3)
        np.set_printoptions(suppress=True)

        # print(result)
        weights = result[:96]
        weights = np.reshape(np.frombuffer(weights, dtype=np.uint16), (3, 4, 4))
        weights = np.array(weights, dtype='float') / 1024.
        print(weights)

        bias = result[96:]
        bias = np.reshape(np.frombuffer(bias, dtype=np.uint16), (3, 4))
        bias = np.array(bias, dtype='float') / 1024.
        print(bias)

    def printhwfid(self):
        self.writeCommand(0x12)
        self.waitSerialDone()

    def hwfid(self):
        self.writeCommand(0x15)
        hwfid = ord(self.ser.read(1))
        print("HWF ID: {:X}".format(hwfid))
        return hwfid

    def mmMcu(self):
        self.writeCommand(0x13)
        return self.readMatrix(4,5)

    def mmFpga(self):
        self.writeCommand(0x14)
        return self.readMatrix(4,5)

    def readMatrix(self, columns, rows):
        self.ser.timeout = None
        result = self.ser.read(columns * rows * 4)

        print('result')
        print(result)

        matrix = np.reshape(np.frombuffer(result, dtype=np.int32), (columns, rows))
        print(matrix)
        return matrix

    def testFpgaPower(self):
        self.writeCommand(b'T')
        self.writeCommand(b'P')

    def testMatrixMultiplication(self, repeat=1):

        # perform experiments with matrix multiplication

        # matrixA = np.zeros((4, 3), dtype=np.uint32)
        matrixA = np.reshape(np.arange(1, 13, dtype=np.uint32), (4,3))
        matrixB = np.reshape(np.arange(1, 16, dtype=np.uint32), (3,5))


        local = np.dot(matrixA, matrixB)

        print('sending...')

        self.writeCommand(b'T')
        self.writeCommand(b'M')

        print('reading...')
        result = self.ser.read(20 * 4)
        result = np.reshape(np.frombuffer(result, dtype=np.uint32), local.shape)

        if (result != local).any():
            print('Result does not match local!')
            print('A:')
            print(matrixA)
            print('B:')
            print(matrixB)
            print('localResult:')
            print(local)
            print('remoteResult:')
            print(result)

        print('done')


    def testXor(self):
        self.writeCommand(b'o')
    # self.writeCommand(num)

    def printIdCode(self):
        # request jtag id code on the debug line
        self.writeCommand(b'I')
        print(self.ser.readline())

    def warmboot(self, config):
        config.loadFile()
        self.writeCommand(b'w')
        self.writeValue(config.address, "Warm Boot Address")
        return self.waitSerialDone()

    def multiboot(self, config):
        self.writeCommand(b'm')
        ba = bytearray([config.destination & 0xff, (config.destination >> 8) & 0xff, (config.destination >> 16) & 0xff])
        for i in range(3):
            self.ser.write([ba[i]])
        return self.waitSerialDone()

    def suspend(self, enableSuspend):
        if enableSuspend:
            self.writeCommand(b'S')
        else:
            self.writeCommand(b's')

    def writeSize(self):
        self.writeValue(self.size, 'size')

    def writeValue(self, value, name, quiet=False):
        ba = bytearray([value & 0xff, (value >> 8) & 0xff, (value >> 16) & 0xff, (value >> 24) & 0xff])

        # print('hex', hex(value))
        # ba = ba[::-1]
        for i in range(4):
            # time.sleep(0.5)
            # print(ba[i])
            self.ser.write([ba[i]])

        # self.waitSerialReady();

        # self.ser.flush()
        if not quiet:
            print(name, 'of bitfile: ',)
        # received = self.readSerialLine()
        receivedRaw = bytearray(self.ser.read(4))
        received = 0
        for i in range(4):
            received += (receivedRaw[i]) << (8 * i)

        if not quiet:
            print(received,)
        try:
            if value != int(received):
                print(name, 'incorrect! aborting.')
                sys.exit(0)
        except:
            # received = list(received)
            print('did not receive', name, ':', len(receivedRaw), '(', value, ')')
            print('{:X}'.format(int(received)))
            for response in receivedRaw:
                print(hex(response), chr(response))

            sys.exit(0)


    def writeCommand(self, command):
        # backup current busy state
        bk = self.busy
        self.busy = True

        ret = False
        if type(command) == int:
            command = struct.pack("B", command)

        for i in range(5):
            self.ser.timeout = 1

            self.ser.write(command)
            # self.ser.flushOutput()
            # self.ser.flush()
            if self.confirmCommand(command):
                ret = True
                break
            else:
                print('repeating command...', command)

        self.ser.timeout = None

        self.busy = bk
        # print('command successful')
        return ret

    def confirmCommand(self, command):
        # print('[chao_debug] waiting for confirm from MCU,,,')
        response = self.readSerialBlocking(1)
        # print('[chao_debug] we get response now')
        # response = self.ser.read(1)
        charResponse = response
        try:
            command = ord(command)
        except TypeError:
            pass
        try:
            response = ord(response)
        except TypeError:
            pass

        if response != command:
            if response == '':
                print('No response...')
            else:
                try:
                    hex(response)
                    print("Received incorrect ack! Command", command, "response ", charResponse, " '", response, "' (", hex(response), ")")
                except:
                    print("Received incorrect ack! Command", command, "response ", charResponse, " '", response, "' (cannot hex)")
            if response == 12:
                print('RESET DETECTED')
                self.ser.close()
                sys.exit(1)
            # time.sleep(10)
            return False
        return True
    # sys.exit(0)

    def timer(self):
        self.writeCommand(b't')
        result = self.ser.read(4)
        result = np.frombuffer(result, dtype=np.float32)

    def beginExperiment(self):
        print('beginning experiments')
        self.writeCommand(b'E')
        self.ser.readline()
    def endExperiment(self):
        print('ending experiments')
        self.writeCommand(b'e')
        self.ser.readline()

    def latency(self, request=True, plot=False, offloading=False):

        if request:
            self.writeCommand(b'a')

        # retrieve size
        size = self.ser.read(4)
        print('size:',)
        for s in size: print(ord(s))
        size = np.frombuffer(size, dtype=np.uint32)
        print('fetching', size, 'bytes')

        latencies = list()
        # read latencies one at a time
        for i in range(size / 4):
            data = self.ser.read(4)
            data = np.frombuffer(data, dtype=np.float32)

            if plot:
                if self.plotter is not None:
                    self.plotter.plotLatency(data, offloading)

            for dat in data:
                latencies.append(dat)

        print('done reading')

        print('done getting latency')
        return data

    def runTarget(self, target):
        if target == "readflashselectmap":
            selectmap = True
            jtag = False
            self.readConfig(ADDRESS, selectmap, jtag)
        elif target == "readflashjtag":
            selectmap = False
            jtag = True
            self.readConfig(ADDRESS, selectmap, jtag)
        elif target == "writeflash":
            flash = True
            selectmap = False
            jtag = False
            self.sendConfig(ADDRESS, selectmap=selectmap, jtag=jtag)
        elif target == "selectmap":
            flash = False
            selectmap = True
            jtag = False
            self.bitFile = self.bscanFile
            self.sendConfig(ADDRESS, size=self.bscansize, skip=0, selectmap=selectmap, jtag=jtag)
        elif target == "idcode":
            self.printIdCode()
        elif target == "jtag":
            flash = False
            selectmap = False
            jtag = True
            self.bitFile = self.testFile
            self.getAllSizes()
            self.sendConfig(ADDRESS, size=self.testsize, selectmap=selectmap, jtag=jtag, flash=flash)

        elif target == "writefir":
            flash = True
            selectmap = False
            fpgaflash = False
            jtag = False
            self.sendConfig(self.firConfig, selectmap=selectmap, jtag=jtag, flash=flash, fpgaflash=fpgaflash)
        elif target == "writefirflash":
            selectmap = False
            jtag = False
            self.readConfig(self.firConfig, selectmap=selectmap, jtag=jtag)
        # wait for it to finish
        # self.ser.readline();
        elif target == "loadfir":
            selectmap = True
            jtag = False
            self.readConfig(self.firConfig, selectmap=selectmap, jtag=jtag)
            # wait for it to finish
            self.ser.readline();
        elif target == "fir":
            self.testFIR()
        elif target == "bootfir":
            self.multiboot(self.firConfig)
            self.waitSerialDone()
        elif target == "verifyfir":
            self.verify(self.firConfig, mcuFlash=True)
        elif target == "verifyfirfpga":
            self.verify(self.firConfig, mcuFlash=False)

        elif target == "writemm":
            flash = True
            selectmap = False
            fpgaflash = False
            jtag = False
            self.sendConfig(self.mmConfig, selectmap=selectmap, jtag=jtag, flash=flash, fpgaflash=fpgaflash)
        elif target == "writemmflash":
            selectmap = False
            jtag = False
            self.readConfig(self.mmConfig, selectmap=selectmap, jtag=jtag)
        # wait for it to finish
        # self.ser.readline();
        elif target == "loadmm":
            selectmap = True
            jtag = False
            self.readConfig(self.firConfig, selectmap=selectmap, jtag=jtag)
            # wait for it to finish
            self.ser.readline();
        # elif target == "fir":
        # 	self.writeCommand(b'T')
        # 	self.writeCommand(b'f')
        # 	self.waitSerialDone()
        elif target == "bootmm":
            self.multiboot(self.mmConfig)
            self.waitSerialDone()

        elif target == "bootann":
            self.multiboot(self.annConfig)
            self.waitSerialDone()
        elif target == "verifyann":
            self.verify(self.annConfig, mcuFlash=True)
        elif target == "verifyannfpga":
            self.verify(self.annConfig, mcuFlash=False)
        elif target == "ann":
            self.testANNsmall();
        elif target == "learnann":
            self.writeCommand(b'T')
            self.writeCommand(b'm')
        elif target == "writeannflash":
            flash = False
            selectmap = False
            jtag = False
            fpgaflash = True
            # self.sendConfig(self.annConfig, selectmap=selectmap, jtag=jtag, flash=flash, fpgaflash=fpgaflash)
            self.readConfig(self.annConfig, selectmap=selectmap, jtag=jtag)
            # wait for it to finish
            self.waitSerialDone()
        elif target == "writeann":
            flash = True
            selectmap = False
            jtag = False
            self.sendConfig(self.annConfig, selectmap=selectmap, jtag=jtag, flash=flash)
        elif target == "loadann":
            selectmap = True
            jtag = False
            self.readConfig(self.annConfig, selectmap=selectmap, jtag=jtag)
        # wait for it to finish

        elif target == "matrix":
            self.testMatrixMultiplication()
        elif target == "verify":
            self.verify(self.bscanConfig, mcuFlash=True)

        elif target == "softreset":
            self.resetUL();
        elif target == "hardreset":
            self.resetFPGA()
        elif target == "current":
            self.printCurrent() #self.fetchCurrent());
        elif target == "fetchcurrent":
            self.printAllMeasurements();
        elif target == "xor":
            self.testXor();
        elif target == "startmonitor":
            self.startMonitor()
        elif target == "endmonitor":
            self.endMonitor()
        elif target == "wait":
            self.busy = True

            time.sleep(1)

            self.busy = False

        # elif target == "findcurrent":
        # 	self.findCurrentSensors()

        elif target == "writetest":
            flash = True
            selectmap = False
            jtag = False
            # self.bitFile = self.testFile
            # self.getAllSizes()
            self.sendConfig(self.testConfig, selectmap=selectmap, jtag=jtag, flash=flash)
        elif target == "loadtest":
            selectmap = True
            jtag = False
            # self.getAllSizes()
            self.readConfig(self.testConfig, selectmap=selectmap, jtag=jtag)
            # wait for it to finish
            self.ser.readline();


        elif target == "writebscan":
            flash = True
            selectmap = False
            jtag = False
            self.sendConfig(self.bscanConfig, selectmap=selectmap, jtag=jtag, flash=flash)
        elif target == "loadbscan":
            selectmap = True
            jtag = False
            self.readConfig(self.bscanConfig, selectmap=selectmap, jtag=jtag)
            # wait for it to finish
            self.ser.readline();
        elif target == "loadflash":
            selectmap = False
            jtag = False
            self.readConfig(self.annConfig, selectmap=selectmap, jtag=jtag, destination=0x0)
            # wait for it to finish
            self.ser.readline();

        elif target == "storeweights":
            self.storeWeights(ANN_WEIGHTS_ADDRESS)
        # self.writeCommand(b'b')
        # elif target == "storeweightsoff":
        # 	self.writeCommand(b'A')
        elif target == "readweights":
            self.readWeights(ANN_WEIGHTS_ADDRESS);
        elif target == "loadweights":
            self.loadWeights(ANN_WEIGHTS_ADDRESS);

        elif target == "ledon":
            self.controlLed(True)
        elif target == "ledoff":
            self.controlLed(False)
        elif target == "ledtest":
            self.testLeds()

        elif target == "fpgaon":
            self.controlFpga(True)
        elif target == "fpgaoff":
            self.controlFpga(False)

        elif target == "fpgapowertest":
            self.testFpgaPower();

        elif target == "debugon":
            self.controlDebug(True);
        elif target == "debugoff":
            self.controlDebug(False);

        elif target == "demo":
            self.demo();

        elif target == "multiboot":
            self.multiboot(0x60000)
            self.waitSerialDone()

        elif target == "transfertest":
            self.testTransfer()
        elif target == "variabletransfertest":
            self.testTransfer(True)
        elif target == "latencytransfertest":
            self.testTransferLatency()
        elif target == "sectiontransfertest":
            self.testSectionTransfer()

        elif target == "wirelesstest":
            self.testWireless()
        elif target == "wirelesson":
            self.writeCommand(b'W')
        elif target == "wirelessoff":
            self.writeCommand(b'w')
        elif target == "wirelesslearn":
            self.learnWireless()

        elif target == "xmemon":
            self.writeCommand(b'x')
        elif target == "xmemoff":
            self.writeCommand(b'X')

        elif target == "fpgasuspendon":
            self.suspend(1)
        elif target == "fpgasuspendoff":
            self.suspend(0)

        elif target == "fpgaflashon":
            self.writeCommand(0x10)
            self.waitSerialDone()
        elif target == "fpgaflashoff":
            self.writeCommand(0x11)
            self.waitSerialDone()

        elif target == "hwfid":
            self.hwfid()


        elif target == "timer":
            self.timer()

        elif target == "latency":
            plot = False
            print(self.latency())

        elif target == "testextspi":
            self.writeCommand(b'T')
            self.writeCommand(b'S')

            self.waitSerialDone()

        elif target == "flashtest":
            self.testFlash();

        elif target == "resetweights":
            self.writeCommand(b'y')

        else:
            print("Target", target, "not defined")


if __name__ == '__main__':


    serialTest = SerialTest(backgroundDebug=True)
    # self.startMonitor()
    # time.sleep(2)
    # self.endMonitor()
    flash = True
    selectmap = False
    ADDRESS = 0


    if len(sys.argv) > 1:

        targets = sys.argv[1:]
        print
        # probably coming from makefile
        if len(targets) == 1:
            targets = targets[0].split(',')

        for i in range(len(targets)):
            target = targets[i].lower()

            print("Starting target:", target)

            serialTest.runTarget(target)


    while serialTest.monitoring:
        try:
            time.sleep(0.1)
            serialTest.redraw()
        except KeyboardInterrupt:
            serialTest.monitoring = False
        # self.endMonitor()

    serialTest.ser.close();

