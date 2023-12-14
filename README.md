# XYZ23 Microprocessor
## Members
Leo Chen, lchen26@bu.edu <br />
Thomas Poimenidis, tpoimen@bu.edu <br />
Michael Waetzman, mwae@bu.edu <br />
Zhiheng Xu, dreamx@bu.edu <br />

# Project Information
## Project Overview
This is a single cycle microprocessor with a custom instruction set, assembly language, and assembler. Using the instruction set and assembler, users can create their own programs that get sent to the microprocessor on the FPGA via UART protocol. The user can then read values in memory by reading from a memory dump, which can be organized into a table using the provided memory dump decoder.
<br />
Demo Video: [https://drive.google.com/file/d/1NjAvVf56B6VZb_COWC1eKtaYYqETxyMS/view?usp=sharing]

## How to Run
Note: This project was intended for use on a Xilinx A7 FPGA Board. It may run on other boards, but changes may be needed. <br />

After downloading all files and creating a Verilog project (in Vivado), the user should write a program in XYZ-lang (see _instruction-set.pdf_ for information on proper formatting). This can be compiled using _XYZassembler.cpp_. Compile _XYZassembler.cpp_ with the XYZ-lang file as a command-line argument. A binary file (.BIN) of the same name will be saved on your device. Ideally, the user would input this via the Kermit protocol (using a terminal emulator such as Tera Term or mobaXterm), but this was unable to be implemented. Instead, files can be run via the testbench (see fibonacci, as it was displayed in the video).

## Using the Assembler and Memory Dump Decoder
To use the provided _XYZassembler.cpp_ and _DumpDecoder.cpp_, it is recommended to compile using C++17 standard. Use the following to compile:

g++ -std=c++17 XYZassembler.cpp -o XYZassembler <br />
g++ -std=c++17 DumpDecoder.cpp -o DumpDecoder <br />

This will result in executable programs for each file.

To use the XYZassembler, run the executable with a .xyz file as shown below:

./XYZassembler example.xyz <br />

Similarly, for the DumpDecoder, provide a .BIN file as shown below:

./DumpDecoder example.BIN <br />

## Code Structure
There are three main units within the code: the assembler/decoder, the UART modules, and the MPU (microprocessor). Within the MPU exists the controller, ALU, registers, and memory. <br />

The assembler converts user-generated XYZ code into binary. The UART modules (in theory) take the binary and send it to the MPU 8 bits at a time. Given each full instruction is 24 bits (8 bits per parameter, 3 parameters per instruction), the MPU must combine 3 UART “pulses”  to create one full instruction. <br />

Once in the MPU, instructions are stored in the 128×24 bit memory file. Once data stops being sent to the MPU, it begins executing the code. Using a program counter, the MPU executes instructions by sequentially progressing through memory via the program counter’s value. Once the MPU has received a “halt” signal, the program has concluded and a “program done” flag is raised high to alert the user that their program has finished executing. After this point, all results are stored in memory and the user can access each memory cell using the mem_select variable in the Microprocessor.v file. <br/>
