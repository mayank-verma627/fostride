import os
import time
import subprocess
from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler

REPO_PATH = r"C:\Users\White_devil\Desktop\fostride"  # <-- CHANGE THIS to your cloned repo path
IDLE_TIME = 600  # 10 minutes

class ChangeHandler(FileSystemEventHandler):
    def __init__(self):
        self.last_modified = time.time()
    def on_any_event(self, event):
        self.last_modified = time.time()

def git_auto_update():
    os.chdir(REPO_PATH)
    timestamp = time.strftime("%Y-%m-%d %H:%M:%S")
    msg = f"Auto-update at {timestamp}"
    print(f"\n[{timestamp}] ⏫ Pushing updates...")

    subprocess.run(["git", "add", "."], check=False)
    diff = subprocess.run(["git", "diff", "--cached", "--quiet"])
    if diff.returncode != 0:
        subprocess.run(["git", "commit", "-m", msg], check=False)
        subprocess.run(["git", "push"], check=False)
        subprocess.run(["git", "pull"], check=False)
        print(f"[{timestamp}] ✅ Commit & push done!\n")
    else:
        print(f"[{timestamp}] 💤 No changes to commit.\n")

if __name__ == "__main__":
    os.chdir(REPO_PATH)
    handler = ChangeHandler()
    observer = Observer()
    observer.schedule(handler, REPO_PATH, recursive=True)
    observer.start()

    print(f"📡 Watching '{REPO_PATH}' for changes...")
    last_update = time.time()

    try:
        while True:
            time.sleep(5)
            now = time.time()
            if now - handler.last_modified > IDLE_TIME and now - last_update > IDLE_TIME:
                git_auto_update()
                last_update = now
    except KeyboardInterrupt:
        observer.stop()
    observer.join()
