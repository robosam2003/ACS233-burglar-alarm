from tkinter import*

root = Tk()
root.title("Burglar Alarm")

entered_pin = []
userPin = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
adminPin = [2, 2, 2, 2, 2, 2, 2, 2, 2, 2]

e = Entry(root, width = 35, borderwidth=5)
e.grid(row = 0,column =0, columnspan=3,padx=10,pady =30)

text_box = Text(root,width=35,height =5)
text_box.grid(row = 1,column =5)

#text_box.insert(1.0, "Please wait for instructions")

def button_click(number):

    current = e.get()
    entered_pin.append(number)
    e.delete(0,END)
    e.insert(0,str(current) + str(number))

def button_clear():
    e.delete(0,END)
    entered_pin.clear()
    
    

def button_enter():

    #print(entered_pin)
    
    if (entered_pin == userPin):
        e.delete(0,END)
        text_box.insert(1.0, "Correct User Long Pin")
        #e.insert(0,"Correct User Long Pin")

    elif (entered_pin == userPin[0:4]):
        e.delete(0,END)
        text_box.insert(1.0, "Correct User Short Pin")
        #e.insert(0,"Correct User Short Pin")    
        
    elif (entered_pin == adminPin):
        e.delete(0,END)
        text_box.insert(1.0, "Correct Admin Pin")
        #e.insert(0,"Correct Admin Pin")
        
    else:
        e.delete(0,END)
        text_box.insert(1.0, "Incorrect Pin")
        #e.insert(0,"Incorrect Pin")
            
 

def button_delete():
    global entered_pin
    e.delete(len(e.get())-1,END)
    entered_pin= entered_pin[: -1]


number_1 = Button(root,text = "1",padx= 40,pady = 20, command=lambda: button_click(1))
number_2 = Button(root,text = "2",padx= 40,pady = 20, command=lambda: button_click(2))
number_3 = Button(root,text = "3",padx= 40,pady = 20, command=lambda: button_click(3))
number_4 = Button(root,text = "4",padx= 40,pady = 20, command=lambda: button_click(4))
number_5 = Button(root,text = "5",padx= 40,pady = 20, command=lambda: button_click(5))
number_6 = Button(root,text = "6",padx= 40,pady = 20, command=lambda: button_click(6))
number_7 = Button(root,text = "7",padx= 40,pady = 20, command=lambda: button_click(7))
number_8 = Button(root,text = "8",padx= 40,pady = 20, command=lambda: button_click(8))
number_9 = Button(root,text = "9",padx= 40,pady = 20, command=lambda: button_click(9))
number_0 = Button(root,text = "0",padx= 40,pady = 20, command=lambda: button_click(0))

button_enter = Button(root,text = "ENTER",padx= 40,pady = 20, command=button_enter)
button_delete = Button(root,text = "DELETE",padx= 37,pady = 20, command=button_delete)
button_clear = Button(root,text = "CLEAR",padx= 40,pady = 20, command=button_clear)


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
