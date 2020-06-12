#!/usr/bin/env python

import serial
import sys

print("python echo script started\n")
HEX = False

ser = serial.Serial(sys.argv[1], sys.argv[2])

try:
	while True:
		sys.stdout.write(ser.read(1))
		sys.stdout.flush()
except KeyboardInterrupt:
	print("key exc")
finally:
	print('done')
	ser.close()
