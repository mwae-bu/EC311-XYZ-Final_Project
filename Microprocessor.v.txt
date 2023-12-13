`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company:
// Engineer:
//
// Create Date: 12/10/2023 06:31:49 PM
// Design Name:
// Module Name: Microprocessor
// Project Name:
// Target Devices:
// Tool Versions:
// Description:
//
// Dependencies:
//
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
//
//////////////////////////////////////////////////////////////////////////////////


module Microprocessor (
	input wire clk,
	input wire [23:0] instruction_input,
	output reg [23:0] registers_out [0:7],
	output reg [23:0] memory_out [127:0]
    
);

reg [23:0] pc;
reg [23:0] instruction;
reg cmp_result;
reg halt_detection;

// Registers
reg [23:0] registers [0:7];

// Memory
reg [23:0] memory [127:0];

// Opcodes
localparam HALT = 8'b00000000;
localparam RESETREGS = 8'b00000001;
localparam MOVNUMREG = 8'b00000010;
localparam MOVREGREG = 8'b00000011;
localparam MOVMEMREG = 8'b00000100;
localparam MOVREGMEM = 8'b00000101;
localparam ADD = 8'b00000110;
localparam SUB = 8'b00000111;
localparam INC = 8'b00001000;
localparam DEC = 8'b00001001;
localparam AND = 8'b00001010;
localparam OR = 8'b00001011;
localparam XOR = 8'b00001100;
localparam CMP = 8'b00001101;
localparam JMP = 8'b00001110;
localparam JE = 8'b00001111;
localparam JNE = 8'b00010000;

//i for loop
integer i;

//Initialization
initial pc <= 24'h0;
initial for (i=0; i<128; i = i+1) begin
        	memory[i] <= 24'b0;
    	end
initial for (i=0; i<8; i = i+1) begin
        	registers[i] <= 24'b0;
    	end
initial cmp_result <= 1'b0;
initial halt_detection <= 1'b0;

// Write instruction to memory
always @(instruction_input) begin
	if (instruction_input != 24'b0) begin
    	memory[pc] <= instruction_input;
    	pc <= pc + 1;
	end
	else if (instruction_input[23:16] == HALT) begin
    	halt_detection <= 1'b1;
    	memory[pc] <= instruction_input;
    	pc <= 24'h0;
	end
end

// Fetch stage
always @ (posedge clk) begin
	if (halt_detection) begin
    	instruction <= memory[pc];
    	pc <= pc + 1;
	end
end

// Execute stage
always @(instruction) begin
	case(instruction[23:16]) // Fetching opcode from bits 23 to 16
    	RESETREGS: begin
        	for (i=0; i<8; i=i+1) begin
            	registers[i] <= 24'b0;
        	end
    	end
    	MOVNUMREG: begin
        	registers[instruction[15:8]] <= instruction[7:0];   
    	end
    	MOVREGREG: begin
        	registers[instruction[15:8]] <= registers[instruction[7:0]];
    	end
    	MOVMEMREG: begin
        	memory[registers[instruction[15:8]]] <= registers[instruction[7:0]];
    	end
    	MOVREGMEM: begin
        	registers[instruction[15:8]] <= memory[registers[instruction[7:0]]];
    	end
    	ADD: begin
        	registers[instruction[15:8]] <= registers[instruction[15:8]] + registers[instruction[7:0]];
    	end
    	SUB: begin
        	registers[instruction[15:8]] <= registers[instruction[15:8]] - registers[instruction[7:0]];
    	end
    	INC: begin
        	registers[instruction[15:8]] <= registers[instruction[15:8]] + 1;
    	end
    	DEC: begin
        	registers[instruction[15:8]] <= registers[instruction[15:8]] - 1;
    	end
    	AND: begin
        	registers[instruction[15:8]] <= registers[instruction[15:8]] & registers[instruction[7:0]];
    	end
    	OR: begin
        	registers[instruction[15:8]] <= registers[instruction[15:8]] | registers[instruction[7:0]];
    	end
    	XOR: begin
        	registers[instruction[15:8]] <= registers[instruction[15:8]] ^ registers[instruction[7:0]];
    	end
    	CMP: begin
        	if (registers[instruction[15:8]] == registers[instruction[7:0]]) begin
            	cmp_result <= 1'b1;
        	end else begin
            	cmp_result <= 1'b0;
        	end
    	end
    	JMP: begin
        	pc <= instruction[15:0];
    	end
    	JE: begin
        	if (cmp_result) begin
            	pc <= instruction[15:0];
        	end
    	end
    	JNE: begin
        	if (!cmp_result) begin
            	pc <= instruction[15:0];
        	end
    	end
    	HALT: begin
        	for (i=0; i<8; i=i+1) begin
            	registers_out[i] <= registers[i];
        	end
        	for (i=0; i<128; i=i+1) begin
            	memory_out[i] <= memory[i];
        	end
    	end
	endcase
end

endmodule
