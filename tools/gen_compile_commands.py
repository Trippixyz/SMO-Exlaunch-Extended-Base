import json
import os

def replace_base_dir(json_data, base_dir):
    """Replace all occurrences of @BASE_DIR@ with the base_dir in the JSON data."""
    json_string = json.dumps(json_data)
    return json.loads(json_string.replace("@BASE_DIR@", base_dir))

def main():
    # Determine the parent directory of this script
    parent_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

    # Original JSON data with @BASE_DIR@
    original_json = [
        {
            "arguments": [
                "clang++",
                "-Wall",
                "-O3",
                "-D__thread=^-^",
                "-DEXPORT_SYM",
                "-I@BASE_DIR@/include",
                "-I@BASE_DIR@/include/symlinks",
                "-I@BASE_DIR@/include/symlinks/al",
                "-I@BASE_DIR@/build",
                "-I@BASE_DIR@/src",
                "-I@BASE_DIR@/src/program",
                "-I@BASE_DIR@/src/lib",
                "-I@BASE_DIR@/src/helpers",
                "-I@BASE_DIR@/lib/NintendoSDK/include",
                "-I@BASE_DIR@/lib/NintendoSDK/include/nvn",
                "-I@BASE_DIR@/lib/sead/include",
                "-I@BASE_DIR@/lib/imgui",
                "-I@BASE_DIR@/lib/OdysseyDecomp/lib",
                "-I@BASE_DIR@/lib/OdysseyDecomp/src",
                "-I@BASE_DIR@/lib/OdysseyDecomp/lib/agl/include",
                "-D__SWITCH__",
                "-DNNSDK",
                "-Dprivate=public",
                "-D__RTLD_6XX__",
                "-DEXL_LOAD_KIND=Module",
                "-DEXL_LOAD_KIND_ENUM=2",
                "-DEXL_PROGRAM_ID=0x0100000000010000",
                "-DGAME_VERSION=100",
                "-std=gnu++20",
                "@BASE_DIR@/src/program/main.cpp"
            ],
            "directory": "@BASE_DIR@/build",
            "file": "@BASE_DIR@/src/program/main.cpp"
        }
    ]

    # Replace @BASE_DIR@ with the actual parent directory
    modified_json = replace_base_dir(original_json, parent_dir)

    # Write the modified JSON to a file
    with open(os.path.join(parent_dir, "compile_commands.json"), "w") as file:
        json.dump(modified_json, file, indent=4)

if __name__ == "__main__":
    main()
