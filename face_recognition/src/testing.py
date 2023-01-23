import time

import serial

ser = serial.Serial("COM6", 9600)

while True:
    a = "HELLO WORLD!"
    b = a.encode()
    ser.write(b)
    time.sleep(1)

    line = ser.readline()
    line = line.decode()  # decode the bytes into a string
    print(line, end="")
