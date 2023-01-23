# A burglar alarm system (GROUP 4) 23 Jan 2023
# Authors: Samuel Scott, Eurico Benedict, Simon Desir, Alex Hicks, Yu-ta Chou
#
# This program is the computer end of a two piece inter-working software that runs
# the burglar alarm system. It functions as follows, it monitors for verification requests,
# mode change requests and verifies user using a facial recognition algorithm and
# password input. It also outputs user verification status and mode status to the
# arduino system.
#
# Input devices are listed as follows;
# Webcam - used in the facial recognition algorithm and takes pictures when wrong password is inputted
# Keyboard - used for password input and selecting mode
# Arduino serial stream - used for receiving authorisation request
#
# Output devices are listed as follows;
# Arduino serial stream - used for sending verification to arduino and system mode status
# Device storage - used to store images taken during failed logins
# Monitor - used for showing user prompts like password input and mode changes
#
import time

import face_recognition
import cv2
import numpy as np
import serial
import asyncio
import keyboard


# This is a demo of running face recognition on live video from your webcam. It's a little more complicated than the
# other example, but it includes some basic performance tweaks to make things run a lot faster:
#   1. Process each video frame at 1/4 resolution (though still display it at full resolution)
#   2. Only detect faces in every other frame of video.

# PLEASE NOTE: This example requires OpenCV (the `cv2` library) to be installed only to read from your webcam.
# OpenCV is *not* required to use the face_recognition library. It's only required if you want to run this
# specific demo. If you have trouble installing it, try any of the other demos that don't require it instead.

# Get a reference to webcam #0 (the default one)
video_capture = cv2.VideoCapture(0)

# # Load a sample picture and learn how to recognize it.
# obama_image = face_recognition.load_image_file("obama.jpg")
# obama_face_encoding = face_recognition.face_encodings(obama_image)[0]
#
# # Load a second sample picture and learn how to recognize it.
# biden_image = face_recognition.load_image_file("biden.jpg")
# biden_face_encoding = face_recognition.face_encodings(biden_image)[0]

sam_image = face_recognition.load_image_file("sam1.jpg")
sam_face_encoding = face_recognition.face_encodings(sam_image)[0]

# eurico_image = face_recognition.load_image_file("eurico1.jpg")
# eurico_face_encoding = face_recognition.face_encodings(eurico_image)[0]

alex_image = face_recognition.load_image_file("Alex1.jpg")
alex_face_encoding = face_recognition.face_encodings(alex_image)[0]

simon_image = face_recognition.load_image_file("simon1.jpg")
simon_face_encoding = face_recognition.face_encodings(simon_image)[0]

tara_image = face_recognition.load_image_file("tara1.jpg")
tara_face_encoding = face_recognition.face_encodings(tara_image)[0]

# Create arrays of known face encodings and their names
known_face_encodings = [
    # obama_face_encoding,
    # biden_face_encoding,
    sam_face_encoding,
    # eurico_face_encoding,
    alex_face_encoding,
    simon_face_encoding,
    tara_face_encoding
]
known_face_names = [
    # "Barack Obama",
    # "Joe Biden",
    "Sam Scott",
    # "Eurico Benedict",
    "Alex Hicks",
    "Simon Desir",
    "Tara Baldacchino"
]

# Initialize some variables
face_locations = []
face_encodings = []
face_names = []
process_this_frame = True


globalFrame = 0

def verify_face():
    # Grab a single frame of video
    ret, frame = video_capture.read()
    global globalFrame
    globalFrame = frame
    isThereAVerifiedFaceThere = False
    # Only process every other frame of video to save time
    # Resize frame of video to 1/4 size for faster face recognition processing
    small_frame = cv2.resize(frame, (0, 0), fx=0.25, fy=0.25)

    # Convert the image from BGR color (which OpenCV uses) to RGB color (which face_recognition uses)
    rgb_small_frame = small_frame[:, :, ::-1]

    # Find all the faces and face encodings in the current frame of video
    face_locations = face_recognition.face_locations(rgb_small_frame)
    face_encodings = face_recognition.face_encodings(rgb_small_frame, face_locations)

    face_names = []
    for face_encoding in face_encodings:
        # See if the face is a match for the known face(s)
        matches = face_recognition.compare_faces(known_face_encodings, face_encoding)
        name = "Unknown"

        # # If a match was found in known_face_encodings, just use the first one.
        if True in matches:
            isThereAVerifiedFaceThere = True
            first_match_index = matches.index(True)
            name = known_face_names[first_match_index]

        # Or instead, use the known face with the smallest distance to the new face
        # face_distances = face_recognition.face_distance(known_face_encodings, face_encoding)
        # best_match_index = np.argmin(face_distances)
        # if matches[best_match_index]:
        #     name = known_face_names[best_match_index]

        face_names.append(name)

    # process_this_frame = not process_this_frame


    # Display the results
    for (top, right, bottom, left), name in zip(face_locations, face_names):
        # Scale back up face locations since the frame we detected in was scaled to 1/4 size
        top *= 4
        right *= 4
        bottom *= 4
        left *= 4

        # Draw a box around the face
        cv2.rectangle(frame, (left, top), (right, bottom), (0, 0, 255), 2)

        # Draw a label with a name below the face
        cv2.rectangle(frame, (left, bottom - 35), (right, bottom), (0, 0, 255), cv2.FILLED)
        font = cv2.FONT_HERSHEY_DUPLEX
        cv2.putText(frame, name, (left + 6, bottom - 6), font, 1.0, (255, 255, 255), 1)

    # Display the resulting image
    cv2.imshow('Video', frame)

    # Hit 'q' on the keyboard to quit!
    if cv2.waitKey(1) & 0xFF == ord('q'):
        pass
    return isThereAVerifiedFaceThere


def set_up_key_press():
    keyboard.on_press_key('1', lambda _: keyCode.append(1))  # adds pin to a buffer
    keyboard.on_press_key('2', lambda _: keyCode.append(2))
    keyboard.on_press_key('3', lambda _: keyCode.append(3))
    keyboard.on_press_key('4', lambda _: keyCode.append(4))
    keyboard.on_press_key('5', lambda _: keyCode.append(5))
    keyboard.on_press_key('6', lambda _: keyCode.append(6))
    keyboard.on_press_key('7', lambda _: keyCode.append(7))
    keyboard.on_press_key('8', lambda _: keyCode.append(8))
    keyboard.on_press_key('9', lambda _: keyCode.append(9))
    keyboard.on_press_key('0', lambda _: keyCode.append(0))

def AdminMenu():
    global userPin, keyCode
    keyboard.unhook_all() #Unhooks keyboard interrupts
    answer = input("Welcome back commander\n Would you like to change the PIN? Y/N?") # TODO: Make this proper english
    if answer == 'Y':
        inputPIN0 = int(input("Enter new user PIN"))
        inputPIN1 = int(input("Enter PIN again to confirm"))
        if inputPIN0 == inputPIN1:
            userPin = [int(x) for x in str(inputPIN0)]
            print("PIN successfully changed. Have a nice day!")
        # else:
        #     # ...
    set_up_key_press()
    keyCode = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]


ser = serial.Serial("COM6", 9600, timeout=0.1)
verification_window_open = False
verified = False
set_up_key_press()

keyCode = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
userPin = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
adminPin = [2, 2, 2, 2, 2, 2, 2, 2, 2, 2]

def verify_pin(face_verified):
    global keyCode
    lastFour = keyCode[-4:]
    userFullPin = keyCode[-10:]

    print(lastFour[-1])
    print(userFullPin)

    if (len (keyCode) >= 40):
        print("You've entered an incorrect pin too many times")
        print("(Python) CLOSING VERIFICATION WINDOW")
        verification_window_open = False
        verified = False
        keyCode = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]

        window_closed_string = "VERIFICATION WINDOW CLOSED\n"  # need the \n for Serial reading
        window_closed_bytes = window_closed_string.encode()
        ser.write(window_closed_bytes)
        print("(Python) wrote window closed bytes")

    if (face_verified == True) and (userPin[0:4] == lastFour):
        #print("Now you're really in baby;)")
        print("You used facial rec")
        return True






    if (face_verified == False) and (userFullPin == userPin):
        print("You did not use facial rec")
        return True

    if (userFullPin == adminPin):
        AdminMenu()
        print("User Pin = ", userPin)

    return False

# TODO: Add limit to number of pin tries in a verification window
while True:
    try:
        verify_face()
        # read line from serial
        line = ser.readline()
        line = line.decode()  # decode the bytes into a string
        print(line, end="")
        if line.startswith("VERIFICATION WINDOW OPEN"):
            print("(Python) OPENING VERIFICATION WINDOW")
            verification_window_open = True

        if line.startswith("VERIFICATION WINDOW CLOSED"):
            print("(Python) CLOSING VERIFICATION WINDOW")
            verification_window_open = False
            verified = False
            keyCode = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]

        if line.startswith("VERIFICATION REQUEST"):
            print("(Python) THERE WAS A VERIFICATION REQUEST")

            # Do the verification
            if (verification_window_open == True) and (verified == False):
                face_verified = verify_face()
                verified = verify_pin(face_verified)
            if verified:
                verified_string = "VERIFIED\n"  # need the \n for Serial reading
                verified_bytes = verified_string.encode()
                ser.write(verified_bytes)
                print("(Python) Wrote verified bytes to serial")

                # Changing mode activity
                mode = -1
                while mode not in [1, 2, 3]:
                    print("Please select a new system mode:\n 1: ARMED MODE\n 2: AT-HOME MODE\n 3: DISARMED MODE\n ")
                    mode = int(input())
                    if mode not in [1, 2, 3]:
                        print("Invalid mode selected. Please try again.")

                s = ""
                if mode == 1:
                    s = "ARMED"
                elif mode == 2:
                    s = "AT-HOME"
                elif mode == 3:
                    s = "DISARMED"
                mode_string = "MODE: " + s + "\n"  # need the \n for Serial reading
                mode_bytes = mode_string.encode()
                ser.write(mode_bytes)
                print("(Python) Wrote mode change bytes to serial")
            else:
                # TODO: Add deleting of photos after 48 days - Req 8.11
                not_verified_string = "NOT VERIFIED\n"  # need the \n for Serial reading
                not_verified_bytes = not_verified_string.encode()
                ser.write(not_verified_bytes)
                print("(Python) wrote not verified bytes")
                currentTime= int(time.time_ns()/1000000)
                filename = './unauth_photos/unauthIntruder%s.png' % (currentTime)
                cv2.imwrite(filename, globalFrame)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
    except KeyboardInterrupt:
        break



ser.close()

# Release handle to the webcam
video_capture.release()
cv2.destroyAllWindows()
