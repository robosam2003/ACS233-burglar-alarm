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

# This is based on the facerec_from_webcam_faster.py example from the face_recognition package.

import time

import face_recognition
import cv2
import numpy as np
import serial
import asyncio
import keyboard
from tkinter import *

# This is a demo of running face recognition on live video from your webcam. It's a little more complicated than the
# other example, but it includes some basic performance tweaks to make things run a lot faster:
#   1. Process each video frame at 1/4 resolution (though still display it at full resolution)
#   2. Only detect faces in every other frame of video.

# PLEASE NOTE: This example requires OpenCV (the `cv2` library) to be installed only to read from your webcam.
# OpenCV is *not* required to use the face_recognition library. It's only required if you want to run this
# specific demo. If you have trouble installing it, try any of the other demos that don't require it instead.

# Get a reference to webcam #0 (the default one)
print("Starting up webcam...")
video_capture = cv2.VideoCapture(0)
print("Webcam started")

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

# alex_image = face_recognition.load_image_file("Alex1.jpg")
# alex_face_encoding = face_recognition.face_encodings(alex_image)[0]
#
# simon_image = face_recognition.load_image_file("simon1.jpg")
# simon_face_encoding = face_recognition.face_encodings(simon_image)[0]
#
# tara_image = face_recognition.load_image_file("tara1.jpg")
# tara_face_encoding = face_recognition.face_encodings(tara_image)[0]

# Create arrays of known face encodings and their names
known_face_encodings = [
    # obama_face_encoding,
    # biden_face_encoding,
    sam_face_encoding,
    # eurico_face_encoding,
    # alex_face_encoding,
    # simon_face_encoding,
    # tara_face_encoding
]
known_face_names = [
    # "Barack Obama",
    # "Joe Biden",
    "Sam Scott",
    # "Eurico Benedict",
    # "Alex Hicks",
    # "Simon Desir",
    # "Tara Baldacchino"
]

# Initialize some variables
face_locations = []
face_encodings = []
face_names = []
process_this_frame = True

globalFrame = 0

debug = True

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

# --------------------------------------------------------------------------------
def set_up_key_press():
    keyboard.on_press_key('1', lambda _: entered_pin.append(1))  # adds pin to a buffer
    keyboard.on_press_key('2', lambda _: entered_pin.append(2))
    keyboard.on_press_key('3', lambda _: entered_pin.append(3))
    keyboard.on_press_key('4', lambda _: entered_pin.append(4))
    keyboard.on_press_key('5', lambda _: entered_pin.append(5))
    keyboard.on_press_key('6', lambda _: entered_pin.append(6))
    keyboard.on_press_key('7', lambda _: entered_pin.append(7))
    keyboard.on_press_key('8', lambda _: entered_pin.append(8))
    keyboard.on_press_key('9', lambda _: entered_pin.append(9))
    keyboard.on_press_key('0', lambda _: entered_pin.append(0))


def AdminMenu():
    global userPin, entered_pin
    keyboard.unhook_all() #Unhooks keyboard interrupts
    answer = input("\nWelcome back admin\n Would you like to change the PIN? Y/N?")
    while answer != 'Y' and answer != 'N':
        answer = input("Please enter a valid answer (Y/N):  ")
    if answer == 'Y':
        inputPIN0 = int(input("Enter a 10 digit new user PIN:  "))
        inputPIN1 = int(input("Enter PIN again to:  "))

        if inputPIN0 == inputPIN1:
            userPin = [int(x) for x in str(inputPIN0)]
            userPin = userPin[0:10]
            print("PIN successfully changed. Have a nice day!")
        else:
            print("PINs do not match. Try again by typing in your admin pin")

    set_up_key_press()
    entered_pin = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]


ser = serial.Serial("COM6", 115200, timeout=0.05)
verification_window_open = False
face_verified = False
verified = False
Admin_verified = False
set_up_key_press()

entered_pin = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]

root = Tk()
root.title("Burglar Alarm")

# entered_pin = []  # array to store entered pin

Armed = 1
Change = Armed

Disarmed = 2
Cancel = Disarmed

Home = 3

userPin = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
adminPin = [2, 2, 2, 2, 2, 2, 2, 2, 2, 2]

entry_box = Entry(root, width=35, borderwidth=5)  # making entry box (where numbers go) with specifications
entry_box.grid(row=0, column=0, columnspan=3, padx=10, pady=30)

text_box = Text(root, width=60, height=5)  # creating dialogue box to prompt user of what the system is doing
text_box.grid(row=1, column=5)


def button_click(number):  # function for clicking any number
    # text_box.delete(1.0, END)  # when you begin typing a number, the dialogue box is emptied
    current = entry_box.get()  # gets current number
    entered_pin.append(number)  # adds number to list
    entry_box.delete(0, END)  # deletes current
    entry_box.insert(0, str(current) + str(number))  # reprints full number



def button_clear():  # clears entry box
    entry_box.delete(0, END)
    # entered_pin.clear()  # clears list of numbers entered so far
    text_box.delete(1.0, END)


def modes():
    text_box.delete(1.0, END)
    text_box.insert(2.0, "\nPress one to arm system (1),\nPress two to disarm (2),\nPress three for at home mode (3) ")
    root.update()


def admin_settings():
    text_box.insert(2.0, "\nPress one to change pin (1),\nPress two to cancel (2)")
    root.update()


# def button_enter():
#     global verified, Admin_verified
#     # print(entered_pin)
#     if (entered_pin == userPin):  # if long pin is correct:
#         entry_box.delete(0, END)  # delete entry box
#         entered_pin = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]  # delete numebrs entered so far
#         text_box.insert(1.0, "Correct User Long Pin")
#         # e.insert(0,"Correct User Long Pin")
#         modes()
#         verified = True  # user is verified
#
#     elif entered_pin == Armed and verified == True:  # if long pin was correct and user selects mode 1
#         entry_box.delete(0, END)  # delete entry box
#         entered_pin = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]  # delete numebrs entered so far
#         text_box.insert(1.0, "ARMED")
#         verified = False  # user is no longer verified
#
#     elif entered_pin == userPin[0:len(entered_pin)]:
#         entry_box.delete(0, END)
#         entered_pin = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
#         text_box.insert(1.0, "Correct User Short Pin")  # if short pin is correct
#
#     elif entered_pin == adminPin:
#         entry_box.delete(0, END)
#         entered_pin = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
#         text_box.insert(1.0, "Correct Admin Pin")
#         admin_settings()
#         selection_number = [5]
#
#     elif verified:
#         if entered_pin == Disarmed:  # if long pin was correct and user selects mode 1
#             entry_box.delete(0, END)  # delete entry box
#             entered_pin = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]  # delete numebrs entered so far
#             text_box.insert(1.0, "DISARMED")
#
#         elif entered_pin == Home:  # if long pin was correct and user selects mode 1
#             entry_box.delete(0, END)  # delete entry box
#             entered_pin = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]  # delete numebrs entered so far
#             text_box.insert(1.0, "AT HOME MODE")
#
#     elif Admin_verified:
#         if entered_pin == Change:
#             entry_box.delete(0, END)  # delete entry box
#             entered_pin = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]  # delete numebrs entered so far
#             text_box.insert(1.0, "Pin Changed")
#             verified = False  # user is no longer verified
#         elif entered_pin == Disarmed:  # if long pin was correct and user selects mode 1
#             entry_box.delete(0, END)  # delete entry box
#             entered_pin = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]  # delete numebrs entered so far
#             text_box.insert(1.0, "CANCELLED")
#             verified = False  # user is no longer verified
#
#     else:
#         entry_box.delete(0, END)
#         entered_pin = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
#         text_box.insert(1.0, "Incorrect Pin")


def button_delete():  # deletes just last number
    global entered_pin
    entry_box.delete(len(entry_box.get()) - 1, END)
    entered_pin = entered_pin[: -1]


number_1 = Button(root, text="1", padx=40, pady=25, command=lambda: button_click(1))
# creation of buttons and giving them the function of button click (see above)
number_2 = Button(root, text="2", padx=40, pady=25, command=lambda: button_click(2))
number_3 = Button(root, text="3", padx=40, pady=25, command=lambda: button_click(3))
number_4 = Button(root, text="4", padx=40, pady=25, command=lambda: button_click(4))
number_5 = Button(root, text="5", padx=40, pady=25, command=lambda: button_click(5))
number_6 = Button(root, text="6", padx=40, pady=25, command=lambda: button_click(6))
number_7 = Button(root, text="7", padx=40, pady=25, command=lambda: button_click(7))
number_8 = Button(root, text="8", padx=40, pady=25, command=lambda: button_click(8))
number_9 = Button(root, text="9", padx=40, pady=25, command=lambda: button_click(9))
number_0 = Button(root, text="0", padx=40, pady=25, command=lambda: button_click(0))

# button_enter = Button(root, text="ENTER", padx=40, pady=25, command=button_enter)
button_delete = Button(root, text="DELETE", padx=37, pady=25, command=button_delete)
button_clear = Button(root, text="CLEAR", padx=40, pady=25, command=button_clear)

# ORGANISING THE GUI
number_1.grid(row=1, column=0)
number_2.grid(row=1, column=1)
number_3.grid(row=1, column=2)
number_4.grid(row=2, column=0)
number_5.grid(row=2, column=1)
number_6.grid(row=2, column=2)
number_7.grid(row=3, column=0)
number_8.grid(row=3, column=1)
number_9.grid(row=3, column=2)
number_0.grid(row=4, column=1)
button_delete.grid(row=1, column=3)
button_clear.grid(row=2, column=3)
# button_enter.grid(row=3, column=3)

# root.mainloop()
root.update()


def verify_pin(face_verified):
    global entered_pin

    lastFour = entered_pin[-4:]
    userFullPin = entered_pin[-10:]

    if (len(entered_pin) >= 40): # Limits the user to 3 attempts at getting the pin right
        print("You've entered an incorrect pin too many times") # TODO: Change to GUI
        if debug: print("(Python) CLOSING VERIFICATION WINDOW")
        verification_window_open = False
        verified = False
        entered_pin = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]

        window_closed_string = "VERIFICATION WINDOW CLOSED\n"  # need the \n for Serial reading
        window_closed_bytes = window_closed_string.encode()
        ser.write(window_closed_bytes)
        if debug: print("(Python) wrote window closed bytes")

        time.sleep(15*60*2)

    if (face_verified == True) and (userPin[0:4] == lastFour):
        #print("Now you're really in baby;)")
        entry_box.delete(0, END)
        entered_pin = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]  # delete numebrs entered so far
        text_box.insert(1.0, "Correct User Short Pin")  # if short pin is correct
        root.update()
        print("You used facial rec")
        return True

    if (face_verified == False) and (userFullPin == userPin):
        entry_box.delete(0, END)  # delete entry box
        entered_pin = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]  # delete numebrs entered so far
        text_box.insert(1.0, "Correct User Long Pin")
        root.update()
        # e.insert(0,"Correct User Long Pin")
        modes()
        return True

    if (userFullPin == adminPin):
        AdminMenu()
        print("User Pin now set to = ", userPin)
        print("Please enter your pin. ")

    return False

def change_mode():
    root.update()
    global verified, entered_pin
    mode = -1  # TODO: This could be a function (repeated)
    while mode not in [1, 2, 3]:
        modes()
        mode = entered_pin[-1]

    s = ""
    if mode == Armed:  # if long pin was correct and user selects mode 1
        entry_box.delete(0, END)  # delete entry box
        entered_pin = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]  # delete numebrs entered so far
        text_box.insert(1.0, "ARMED")
        root.update()
        verified = False  # user is no longer verified
        s = "ARMED"
    if mode == Home:  # if long pin was correct and user selects mode 1
        entry_box.delete(0, END)  # delete entry box
        entered_pin = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]  # delete numebrs entered so far
        text_box.insert(1.0, "AT HOME MODE")
        root.update()
        s = "AT-HOME"
    if mode == Disarmed:  # if long pin was correct and user selects mode 1
        entry_box.delete(0, END)  # delete entry box
        entered_pin = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]  # delete numebrs entered so far
        text_box.insert(1.0, "DISARMED")
        root.update()
        s = "DISARMED"

    mode_string = "MODE: " + s + "\n"  # need the \n for Serial reading
    mode_bytes = mode_string.encode()
    ser.write(mode_bytes)
    if debug: print("(Python) Wrote mode change bytes to serial")

    verified = False  # user is no longer verified
    root.update()
    print("Please input your pin:  ")


print("Please input your pin:  ")

while True:
    try:
        root.update()

        if verification_window_open == False: # For if the user wants to change mode from inside, with door locked
            face_verified = verify_face()
            if face_verified:
                text_box.delete(1.0, END)
                text_box.insert(1.0, "Face Verified - You can use the short pin")
                root.update()

            else:
                text_box.delete(1.0, END)
                text_box.insert(1.0, "Face Not Verified - Please use the long pin")
                root.update()
            verified = verify_pin(face_verified)
            root.update()
            if verified:
                change_mode()
                verified = False
                entered_pin = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]

        # verify_face()
        # read line from serial
        line = ser.readline()
        line = line.decode()  # decode the bytes into a string
        if (len(line) > 0 and debug): print("(Arduino) " + line, end="")
        if line.startswith("VERIFICATION WINDOW OPEN"):
            if debug: print("(Python) OPENING VERIFICATION WINDOW")
            if debug: print("Alarm Sounding! Please input your pin:  ")
            text_box.delete(1.0, END)
            text_box.insert(1.0, "Alarm Sounding! Please input your pin:  ")
            root.update()
            verification_window_open = True

        elif line.startswith("VERIFICATION WINDOW CLOSED"):
            if debug: print("(Python) CLOSING VERIFICATION WINDOW")
            verification_window_open = False
            verified = False
            entered_pin = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]

        elif line.startswith("VERIFICATION REQUEST"):
            if debug: print("(Python) THERE WAS A VERIFICATION REQUEST")

            # Do the verification
            if (verification_window_open == True) and (verified == False):
                face_verified = verify_face()
                verified = verify_pin(face_verified)
            if verified:
                verified_string = "VERIFIED\n"  # need the \n for Serial reading
                verified_bytes = verified_string.encode()
                ser.write(verified_bytes)
                if debug: print("(Python) Wrote verified bytes to serial")
                # Changing mode activity
                change_mode()
            else:
                # TODO: Add deleting of photos after 48 days - Req 8.11
                not_verified_string = "NOT VERIFIED\n"  # need the \n for Serial reading
                not_verified_bytes = not_verified_string.encode()
                ser.write(not_verified_bytes)
                if debug: print("(Python) wrote not verified bytes")
                currentTime= int(time.time_ns()/1000000)
                filename = './unauth_photos/unauthIntruder%s.png' % (currentTime)
                cv2.imwrite(filename, globalFrame)
                if debug: print("You are currently unrecognized. A photo has been taken of you for security purposes. Enter your pin to verify your identity.") # Req 8.1.2
                text_box.delete(1.0, END)
                text_box.insert(2.0, "Alarm Sounding! Please input your pin:  \n")
                text_box.insert(2.0, "You are currently unrecognized. \nPhotos are being taken of you for security purposes.") # Req 8.1.2
                root.update()

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
    except KeyboardInterrupt:
        break



ser.close()

# Release handle to the webcam
video_capture.release()
cv2.destroyAllWindows()
