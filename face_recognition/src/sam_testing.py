import face_recognition
import cv2
import numpy as np
import serial
import asyncio
import keyboard

ser = serial.Serial("COM6", 9600)
verification_window_open = False
verified = False

while True:
    try:
        # read line from serial
        line = ser.readline()
        line = line.decode()  # decode the bytes into a string
        print(line)
        if line.startswith("VERIFICATION WINDOW OPEN"):
            print("OPENING VERIFICATION WINDOW")
            verification_window_open = True

        if line.startswith("VERIFICATION WINDOW CLOSED"):
            print("CLOSING VERIFICATION WINDOW")
            verification_window_open = False

        if line.startswith("VERIFICATION REQUEST"):
            print("THERE WAS A VERIFICATION REQUEST")
            # Do the verification
            if verification_window_open == True and (verified == False):
                # verify_face
                # verify pin
                pass
            if verified:
                verified_string = "VERIFIED\n"  # need the \n for Serial reading
                verified_bytes = verified_string.encode()
                ser.write(verified_bytes)
                print("Wrote verified bytes")
            else:
                not_verified_string = "NOT VERIFIED\n"  # need the \n for Serial reading
                not_verified_bytes = not_verified_string.encode()
                ser.write(not_verified_bytes)
                print("Wrote not verified bytes")

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
    except KeyboardInterrupt:
        break

ser.close()
