import logging
import os
import sys
import time
import subprocess
from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler

# Configure logging
logging.basicConfig(level=logging.INFO, format="%(asctime)s - %(levelname)s - %(message)s")

class ReloadHandler(FileSystemEventHandler):
    """Reload the application when a file changes."""
    def __init__(self, script):
        self.script = script
        self.process = None
        self.start_process()

    def start_process(self):
        """Start or restart the process."""
        if self.process:
            logging.info("🔄 Restarting process...")
            self.process.terminate()
            self.process.wait()  # Ensure the old process is fully terminated
        self.process = subprocess.Popen([sys.executable, self.script], stdout=sys.stdout, stderr=sys.stderr)

    def on_modified(self, event):
        """Reload if a Python file is modified."""
        if event.src_path.endswith('.py'):
            logging.info(f"🔄 File changed: {event.src_path}, reloading app...")
            self.start_process()

if __name__ == "__main__":
    script_to_run = "main.py"  # Change this to your entry file
    event_handler = ReloadHandler(script_to_run)
    
    observer = Observer()
    observer.schedule(event_handler, ".", recursive=True)
    observer.start()

    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        logging.info("🛑 Stopping observer...")
        observer.stop()
    observer.join()
