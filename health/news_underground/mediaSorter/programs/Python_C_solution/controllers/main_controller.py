from datetime import datetime
from models.time_manager import TIME_RECORDS_FILE
import os

def on_key_press(event):
    """Handles keyboard shortcuts."""
    match event.char:
        case "s":
            with open(TIME_RECORDS_FILE, 'a') as f:
                f.write(f"\nstart : {datetime.now()}\n")
        case "e":
            with open(TIME_RECORDS_FILE, 'r+') as f:
                lines = f.readlines()
                for i in range(len(lines) -1, 0, -1):
                    try:
                        start = datetime.strptime(lines[i].split(" : ")[1], "%Y-%m-%d %H:%M:%S.%df")
                        lines[i] = f"duration : {datetime.now() - start}"
                        f.seek(0)
                        f.writelines(lines)
                        f.truncate()
                        break
                    except:
                        print("no start time")
