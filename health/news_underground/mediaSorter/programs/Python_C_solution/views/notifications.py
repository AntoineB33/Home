from win10toast import ToastNotifier

class Notification:
    def __init__(self):
        self.notifier = ToastNotifier()

    def show_notification(self, title, message):
        self.notifier.show_toast(title, message, duration=5)
