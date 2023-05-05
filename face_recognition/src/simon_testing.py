import time

import keyboard

entered_pin = []
userPin = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
AdminPin = [0, 0, 0, 0, 0, 0, 0, 0, 0, 1]




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

set_up_key_press()
def AdminMenu():
    global userPin, entered_pin
    KeyCode == ()
    keyboard.unhook_all() #Unhooks keyboard interrupts
    answer = input("Welcome back commander\n Would you like to change the PIN? Y/N?") # TODO: Make this proper english
    if answer == 'Y':
        inputPIN0 = int(input("Enter new user PIN"))
        inputPIN1 = int(input("Enter PIN again to confirm"))
        if inputPIN0 == inputPIN1:
            UserPin = [int(x) for x in str(inputPIN0)]
            print("PIN successfully changed. Have a nice day!")
        # else:
        #     # ...
    set_up_key_press()
    KeyCode = []

face_verified = True
def verify_pin():
    lastFour = entered_pin[-4:]
    UserFullPin = entered_pin[-10:]

    print(lastFour)
    print (UserFullPin)

    if (face_verified == True) and (userPin[0:4] == lastFour) :
        #print("Now you're really in baby;)")
        print("You used facial rec")


    if (face_verified == False) and (UserFullPin == userPin)   :
        print("You did not use facial rec")

    if (UserFullPin == AdminPin):
        AdminMenu()
        print("User Pin = ", userPin)
