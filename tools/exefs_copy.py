import json
import shutil
import os

def copy_files_to_directories(json_file, source_folder, filenames):
    # Read the JSON file to get the list of directories
    try:
        with open(json_file, 'r') as file:
            directories = json.load(file)
    except FileNotFoundError:
        # If the file is not found, create a default JSON file
        print(f"JSON file not found. Creating a default JSON file: {json_file}")
        directories = []  # Default directory list
        with open(json_file, 'w') as file:
            json.dump(directories, file)
    except Exception as e:
        print(f"Error reading the JSON file: {e}")
        return

    # Copy each file to each directory
    for directory in directories:
        for filename in filenames:
            source_file = os.path.join(source_folder, filename)
            destination_file = os.path.join(directory, filename)
            try:
                shutil.copy2(source_file, destination_file)
                print(f"Copied {filename} to {directory}")
            except Exception as e:
                print(f"Error copying {filename} to {directory}: {e}")

# Define the source folder and filenames
source_folder = 'build'
filenames = ['main.npdm', 'subsdk9']

# Call the function with the JSON file and source details
copy_files_to_directories('exefs_directories.json', source_folder, filenames)
