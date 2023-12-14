# XYZ23 Microprocessor
## Members
Leo Chen, lchen26@bu.edu <br />
Thomas Poimenidis, tpoimen@bu.edu <br />
Michael Waetzman, mwae@bu.edu <br />
Zhiheng Xu, dreamx@bu.edu <br />

# Project Information
## Project Overview
Demo Video: [link]

## How to Run
Note: This project was intended for use on a Xilinx A7 FPGA Board. It may run on other boards, but changes may be needed. <br />

After downloading all files and creating a Verilog project (in Vivado), the user should write a program in XYZ-lang (see _instruction-set.pdf_ for information on proper formatting). This can be compiled using _XYZassembler.cpp_. Compile _XYZassembler.cpp_ with the XYZ-lang file as a command-line argument. A binary file (.BIN) of the same name will be saved on your device. Ideally, the user would input this via the Kermit protocol (using a terminal emulator such as Tera Term or mobaXterm), but this was unable to be implemented. **ADD INFO ON HOW TO PUSH CODE**  

## Code Structure
There are three main units within the code: the assembler/decoder, the UART modules, and the top 
