import os
import argparse
import re

def pretty_format(name):
    words = name.replace("_", " ").title()
    return words

def calculate_spacing(line, total_length=77):
    current_length = len(line)
    if current_length < total_length:
        return " " * (total_length - current_length)
    return ""

def replace_placeholders(content, name, pretty_name):
    content = content.replace("###name###", name)
    content = content.replace("###proper_name###", pretty_name)
    
    def spacing_replacer(match):
        line = match.group(1)
        return line + calculate_spacing(line)

    # Replace ###spacing### with calculated spaces
    content = re.sub(r"(.*)###spacing###", spacing_replacer, content)
    return content


def copy_and_replace_file(src_path, dest_path, name, pretty_name):
    with open(src_path, 'r') as src_file:
        content = src_file.read()

    # Replace placeholders
    content = replace_placeholders(content, name, pretty_name)

    with open(dest_path, 'w') as dest_file:
        dest_file.write(content)


def main():
    parser = argparse.ArgumentParser(description="Create a .h and optionally a matching .cpp file.")
    parser.add_argument("name", help="The name for the new file(s).")
    parser.add_argument("--cpp", action="store_true", help="Generate the matching .cpp file.")
    args = parser.parse_args()

    stub_dir = "stub"
    include_dir = "include"
    source_dir = "source"

    name = args.name
    pretty_name = pretty_format(name)

    # Ensure the destination directories exist
    os.makedirs(include_dir, exist_ok=True)
    if args.cpp:
        os.makedirs(source_dir, exist_ok=True)

    # Handle .h file
    h_stub_path = os.path.join(stub_dir, "new_file_template.h")
    h_dest_path = os.path.join(include_dir, f"{name}.h")
    if os.path.exists(h_stub_path):
        copy_and_replace_file(h_stub_path, h_dest_path, name, pretty_name)
        print(f"Copied and updated: {h_dest_path}")
    else:
        print(f"Error: {h_stub_path} not found.")

    # Handle .cpp file if specified
    if args.cpp:
        cpp_stub_path = os.path.join(stub_dir, "new_file_template.cpp")
        cpp_dest_path = os.path.join(source_dir, f"{name}.cpp")
        if os.path.exists(cpp_stub_path):
            copy_and_replace_file(cpp_stub_path, cpp_dest_path, name, pretty_name)
            print(f"Copied and updated: {cpp_dest_path}")
        else:
            print(f"Error: {cpp_stub_path} not found.")


if __name__ == "__main__":
    main()
