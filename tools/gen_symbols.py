import sys

def main():
    # Check if the correct number of arguments are provided
    if len(sys.argv) != 4:
        print("Usage: python script.py <path_to_input_file> <path_to_output_file> <mode>")
        sys.exit(1)

    # Retrieve the file paths and mode from the command-line arguments
    input_file_path = sys.argv[1]
    output_file_path = sys.argv[2]
    mode = sys.argv[3]

    # Determine the suffix based on the mode
    suffix = "__sdk_start;" if mode == "sdk" else "__main_start;" if mode == "main" else None

    # Check if the mode is valid
    if suffix is None:
        print("Invalid mode. Use 'sdk' or 'main'.")
        sys.exit(1)

    # Process the file and write the output to the specified file
    try:
        with open(input_file_path, 'r') as input_file, open(output_file_path, 'w') as output_file:
            for line in input_file:
                components = line.split()
                if len(components) >= 8:
                    if (components[1] != "0000000000000000"):
                        output_file.write(f"{components[7]} = 0x{components[1]} + {suffix}\n")
    except FileNotFoundError:
        print(f"File not found: {input_file_path}")

if __name__ == "__main__":
    main()
