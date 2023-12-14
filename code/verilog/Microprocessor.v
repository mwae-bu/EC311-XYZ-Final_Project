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
    input wire reset,
    input wire [23:0] instruction_input,
    input wire [6:0] select_mem,
    output reg [23:0] output_mem_cell,
    output reg program_done_flag
);

reg [7:0] pc = 0;
reg [23:0] instruction;
reg cmp_result = 0;
reg halt_detection = 0;

// Registers
reg [23:0] registers [7:0];

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

always @ (negedge clk) begin
    if (halt_detection && !program_done_flag) begin
        instruction <= memory[pc];
    end
end

// Execute stage
always @(posedge clk || reset || instruction_input) begin
    
    //Write Block
    if (!halt_detection && !program_done_flag) begin    
        if (instruction_input == 24'b0) begin
            halt_detection <= 1'b1;
            memory[pc] <= instruction_input;
            pc <= 8'h0;
        end
        else begin
            memory[pc] <= instruction_input;
            pc <= pc + 1;
        end
    end
    
    // Reset Block
    if (reset) begin
        pc <= 8'h0;
        for (i=0; i<128; i = i+1) begin
            memory[i] <= 24'b0;
        end
        for (i=0; i<8; i = i+1) begin
            registers[i] <= 24'b0;
        end
        cmp_result <= 1'b0;
        halt_detection <= 1'b0;
        program_done_flag <= 1'b0;
    end
    
    if (halt_detection && !program_done_flag) begin
        pc <= pc + 1;
    end
    
    // Execute Block
    if (halt_detection) begin
        
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
                pc <= instruction[15:8];
            end
            JE: begin
                if (cmp_result) begin
                    pc <= pc - (~(instruction[15:8]) + 1);
                end
            end
            JNE: begin
                if (!cmp_result) begin
                    pc <= pc - (~(instruction[15:8]) + 1);
                end
            end
            HALT: begin
                program_done_flag <= 1'b1;
            end
        endcase
    end
    
    output_mem_cell <= memory[select_mem];
end

endmodule
