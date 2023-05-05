from tkinter import *

root = Tk()
root.title("Burglar Alarm")

entered_pin = []  # array to store entered pin
verified = False
Admin_verified = False

Armed = [1]
Change = Armed

Disarmed = [2]
Cancel = Disarmed

Home = [3]

userPin = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
adminPin = [2, 2, 2, 2, 2, 2, 2, 2, 2, 2]

entry_box = Entry(root, width=35, borderwidth=5)  # making entry box (where numbers go) with specifications
entry_box.grid(row=0, column=0, columnspan=3, padx=10, pady=30)

text_box = Text(root, width=35, height=5)  # creating dialogue box to prompt user of what the system is doing
text_box.grid(row=1, column=5)


# text_box.insert(1.0, "Please wait for instructions")


def button_click(number):  # function for clicking any number
    text_box.delete(1.0, END)  # when you begin typing a number, the dialogue box is emptied
    current = entry_box.get()  # gets current number
    entered_pin.append(number)  # adds number to list
    entry_box.delete(0, END)  # deletes current
    entry_box.insert(0, str(current) + str(number))  # reprints full number


def button_clear():  # clears entry box
    entry_box.delete(0, END)
    entered_pin.clear()  # clears list of numbers entered so far
    text_box.delete(1.0, END)


def modes():
    text_box.insert(2.0, "\nPress one to arm system (1),\nPress two to disarm (2),\nPress three for at home mode (3) ")


def admin_settings():
    text_box.insert(2.0, "\nPress one to change pin (1),\nPress two to cancel (2)")


def button_enter():
    global verified, Admin_verified
    # print(entered_pin)
    # if entered_pin == userPin:  # if long pin is correct:
    #     entry_box.delete(0, END)  # delete entry box
    #     entered_pin.clear()  # delete numebrs entered so far
    #     text_box.insert(1.0, "Correct User Long Pin")
    #     # e.insert(0,"Correct User Long Pin")
    #     modes()
    #     verified = True  # user is verified

    elif entered_pin == userPin[0:len(entered_pin)] and verified == True:
        entry_box.delete(0, END)
        entered_pin.clear()
        text_box.insert(1.0, "Correct User Short Pin")  # if short pin is correct

    elif entered_pin == adminPin:
        entry_box.delete(0, END)
        entered_pin.clear()
        text_box.insert(1.0, "Correct Admin Pin")
        admin_settings()
        selection_number = [5]

    # elif verified:
    #     if entered_pin == Disarmed:  # if long pin was correct and user selects mode 1
    #         entry_box.delete(0, END)  # delete entry box
    #         entered_pin.clear()  # delete numebrs entered so far
    #         text_box.insert(1.0, "DISARMED")
    #     elif entered_pin == Armed:  # if long pin was correct and user selects mode 1
    #         entry_box.delete(0, END)  # delete entry box
    #         entered_pin.clear()  # delete numebrs entered so far
    #         text_box.insert(1.0, "ARMED")
    #         User_verified = False  # user is no longer verified
    #     elif entered_pin == Home:  # if long pin was correct and user selects mode 1
    #         entry_box.delete(0, END)  # delete entry box
    #         entered_pin.clear()  # delete numebrs entered so far
    #         text_box.insert(1.0, "AT HOME MODE")

    elif Admin_verified:
        if entered_pin == Change:
            entry_box.delete(0, END)  # delete entry box
            entered_pin.clear()  # delete numebrs entered so far
            text_box.insert(1.0, "Pin Changed")
            verified = False  # user is no longer verified
        elif entered_pin == Disarmed:  # if long pin was correct and user selects mode 1
            entry_box.delete(0, END)  # delete entry box
            entered_pin.clear()  # delete numebrs entered so far
            text_box.insert(1.0, "CANCELLED")
            verified = False  # user is no longer verified

    # else:
    #     entry_box.delete(0, END)
    #     entered_pin.clear()
    #     text_box.insert(1.0, "Incorrect Pin")


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

button_enter = Button(root, text="ENTER", padx=40, pady=25, command=button_enter)
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
button_enter.grid(row=3, column=3)

root.mainloop()
