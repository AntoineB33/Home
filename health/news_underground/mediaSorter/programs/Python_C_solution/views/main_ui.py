from tkinter import Tk, Frame, Button, Entry

def get_task_controller():
    from controllers.task_controller import TaskController
    return TaskController

class MainUI:
    def __init__(self):
        TaskController = get_task_controller()
        self.controller = TaskController(self)
        self.root = Tk()
        self.root.title("Clock")
        self.root.geometry("800x300")
        
        self.setup_ui()
    
    def setup_ui(self):
        frame = Frame(self.root)
        frame.pack()

        self.start_btn = Button(frame, text="Start", fg="white", bg="green", width=15, command=self.controller.start_task)
        self.start_btn.pack(side="left")

        self.stop_btn = Button(frame, text="Stop", fg="white", bg="red", width=15, command=self.controller.stop_task)
        self.stop_btn.pack(side="left")

        self.sound_toggle_btn = Button(frame, text="Toggle Sound", fg="white", bg="black", width=15, command=self.controller.toggle_sound)
        self.sound_toggle_btn.pack(side="left")

        self.volume_btn = Button(frame, text="Adjust Volume", fg="white", bg="black", width=15, command=self.controller.adjust_volume)
        self.volume_btn.pack(side="left")

        self.root.mainloop()
