Below is an academic-style README file that explains the process of creating, compiling, and running the assembly program to count names from a file. It’s structured formally, as might be expected in an academic or technical documentation context.

---

# README: Assembly Name Counter Program

## Overview
This project implements an x86 assembly language program (`count_names.asm`) designed to read a text file (`names.txt`) containing a list of names, count the number of names (assuming each name is separated by a newline), and output the total count to the console. The program leverages Linux system calls for file operations and is accompanied by a Bash script (`count_names.sh`) to automate compilation and execution. This README provides a detailed explanation of the process, from setup to execution, for academic and technical understanding.

## Prerequisites
- **Operating System**: Linux (tested on x86 architecture)
- **Tools**:
  - NASM (Netwide Assembler): For assembling the `.asm` file into an object file.
  - LD (GNU Linker): For linking the object file into an executable.
  - Bash: For running the automation script.
- **Files**:
  - `count_names.asm`: The assembly source code.
  - `names.txt`: A text file containing 30–100 random names, one per line.
  - `count_names.sh`: A Bash script to compile and run the program.

## Program Design
The assembly program (`count_names.asm`) performs the following operations:
1. **File Opening**: Uses the `sys_open` system call (EAX = 5) to open `names.txt` in read-only mode.
2. **File Reading**: Reads the file in chunks of 1024 bytes using `sys_read` (EAX = 4), storing data in a buffer.
3. **Name Counting**: Iterates through the buffer, incrementing a counter each time a newline character (ASCII 10) is encountered, assuming each newline denotes a new name.
4. **File Closing**: Closes the file using `sys_close` (EAX = 6).
5. **Output Conversion**: Converts the integer count to an ASCII string using division by 10 and remainder manipulation.
6. **Console Output**: Writes the count to stdout using `sys_write` (EAX = 4).
7. **Program Termination**: Exits cleanly with `sys_exit` (EAX = 1).

Error handling is implemented to display a message to stderr if the file cannot be opened.

## Setup Instructions
1. **Prepare the Names File**:
   - Create a file named `names.txt` in the working directory.
   - Populate it with 30–100 random names, one per line. Example:
     ```
     Alice
     Bob
     Charlotte
     ...
     ```
   - Ensure the file ends with a newline to accurately count the final name.

2. **Create the Assembly Source**:
   - Save the provided assembly code as `count_names.asm`. The code is written in NASM syntax for 32-bit x86 Linux.

3. **Create the Bash Script**:
   - Save the following as `count_names.sh`:
     ```bash
     #!/bin/bash
     nasm -f elf count_names.asm
     ld -m elf_i386 count_names.o -o count_names
     ./count_names
     ```
   - Make it executable:
     ```bash
     chmod +x count_names.sh
     ```

4. **Install Required Tools**:
   - Install NASM: `sudo apt-get install nasm` (on Debian/Ubuntu).
   - Ensure LD is available (typically included with `binutils`).

## Compilation and Execution Process
The Bash script automates the following steps:
1. **Assembly**:
   - Command: `nasm -f elf count_names.asm`
   - Converts the assembly code into an ELF object file (`count_names.o`).
   - The `-f elf` flag specifies the 32-bit ELF format.

2. **Linking**:
   - Command: `ld -m elf_i386 count_names.o -o count_names`
   - Links the object file into an executable binary (`count_names`).
   - The `-m elf_i386` flag ensures compatibility with 32-bit x86 architecture.

3. **Execution**:
   - Command: `./count_names`
   - Runs the program, which reads `names.txt`, counts the names, and prints the total to the console.

To execute manually, run each command individually, or use the script:
```bash
./count_names.sh
```

## Expected Output
For a `names.txt` file with 30 names (one per line), the output will be:
```
30
```
The program appends a newline to the output for readability.

## Assumptions and Limitations
- **File Format**: Names must be separated by newlines. Multiple names on a single line are treated as one.
- **Buffer Size**: Fixed at 1024 bytes per read. Files larger than 1024 bytes are handled in chunks, but extremely large files may require adjustment.
- **Error Handling**: Limited to file-open failures; other errors (e.g., read failures) are not explicitly handled.
- **Architecture**: Designed for 32-bit x86 Linux. Modifications are needed for 64-bit or other OSes.

## Academic Context
This program demonstrates:
- Low-level system programming using x86 assembly.
- Interaction with the Linux kernel via system calls.
- Basic file I/O and string manipulation in a resource-constrained environment.
- Automation of compilation and execution workflows using Bash scripting.

## Troubleshooting
- **Permission Denied**: Ensure `count_names.sh` is executable (`chmod +x count_names.sh`).
- **File Not Found**: Verify `names.txt` exists in the working directory.
- **Segmentation Fault**: Check that `names.txt` is properly formatted and readable.

## Conclusion
This project provides a functional example of assembly programming for file processing, suitable for educational purposes in computer architecture or systems programming courses. The accompanying script simplifies the build process, making it accessible for testing and demonstration.

