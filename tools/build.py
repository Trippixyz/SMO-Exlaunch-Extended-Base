import subprocess
import sys

def run_make_and_execute_script():
    try:
        # Run the 'make' command
        print("Starting 'make' process...")
        result = subprocess.run(["make"], check=True)

        # Check if 'make' was successful
        if result.returncode == 0:
            print("'make' completed successfully. Running 'exefs_copy.py'...")
            # Run the 'exefs_copy.py' script
            subprocess.run(["python", "tools/exefs_copy.py"], check=True)
        else:
            print("Error in 'make' process.")
            sys.exit(1)

    except subprocess.CalledProcessError as e:
        print(f"An error occurred: {e}")
        sys.exit(1)

if __name__ == "__main__":
    run_make_and_execute_script()
