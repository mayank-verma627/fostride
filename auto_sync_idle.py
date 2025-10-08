import os
import time
import subprocess

# Path to your Git repository
REPO_PATH = r"C:\Users\White_devil\Desktop\fostride"  # <-- change this
CHECK_INTERVAL = 600  # 10 minutes in seconds

def git_auto_update():
    """Commit and push changes to GitHub if there are any."""
    os.chdir(REPO_PATH)
    
    # Check for any changes
    diff = subprocess.run(["git", "status", "--porcelain"], capture_output=True, text=True)
    if diff.stdout.strip():  # if output is not empty, there are changes
        timestamp = time.strftime("%Y-%m-%d %H:%M:%S")
        msg = f"Auto-update at {timestamp}"
        print(f"[{timestamp}] ⏫ Changes detected. Pushing to GitHub...")
        subprocess.run(["git", "add", "."], check=False)
        subprocess.run(["git", "commit", "-m", msg], check=False)
        subprocess.run(["git", "push"], check=False)
        subprocess.run(["git", "pull"], check=False)
        print(f"[{timestamp}] ✅ Commit & push done!\n")
    else:
        timestamp = time.strftime("%Y-%m-%d %H:%M:%S")
        print(f"[{timestamp}] 💤 No changes detected.")

if __name__ == "__main__":
    print(f"📡 Watching '{REPO_PATH}' for changes every {CHECK_INTERVAL//60} minutes...")
    while True:
        git_auto_update()
        time.sleep(CHECK_INTERVAL)
