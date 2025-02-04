from datetime import datetime, timedelta
import pickle
import os

TIME_RECORDS_FILE = os.path.join("..", "data", "timeRecords.txt")
CLOCK_TXT_PATH = os.path.join("..", "data", "clock.txt")

class TimeManager:
    def __init__(self):
        self.now = datetime.now()
        self.yeux = self.now + timedelta(minutes=20)
        self.quart = self.now + timedelta(hours=2)
        self.mat = self.quart
        self.li = [['mat de ' + self.now.strftime('%H:%M:%S'), timedelta(hours=2)]]
        self.po = 0

    def save_state(self):
        with open(CLOCK_TXT_PATH, 'wb') as f:
            pickle.dump((self.now, self.yeux, self.quart, self.mat, self.li, self.po), f)

    def load_state(self):
        if os.path.exists(CLOCK_TXT_PATH):
            with open(CLOCK_TXT_PATH, 'rb') as f:
                self.now, self.yeux, self.quart, self.mat, self.li, self.po = pickle.load(f)

    def update_time(self):
        self.now = datetime.now()

    def should_rest(self):
        return self.yeux < self.now, self.quart < self.now
