`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 12/13/2023 05:12:05 PM
// Design Name: 
// Module Name: Multiplication_TB
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


module Multiplication_TB(

    );
    
    reg clk;
    reg reset;
    reg [23:0] instruction_input;
    reg [6:0] select_mem;
    wire [23:0] output_mem_cell;
    wire program_done_flag;
    
    Microprocessor microtest1(clk, reset, instruction_input, select_mem, output_mem_cell, program_done_flag);
    
    always begin
    #20 clk = ~clk;
    end
    
    initial begin
    clk = 0;
    select_mem = 30;
    reset = 1;
    #100 reset = 0;
    #40 instruction_input = 24'b000000010000000000000000;
    #40 instruction_input = 24'b000000100000000000000100;
    #40 instruction_input = 24'b000000100000000100000101;
    #40 instruction_input = 24'b000001100000001000000000;
    #40 instruction_input = 24'b000010010000000100000000;
    #40 instruction_input = 24'b000011010000001100000001;
    #40 instruction_input = 24'b000100001111110100000000;
    #40 instruction_input = 24'b000000100000001100011110;
    #40 instruction_input = 24'b000001000000001100000010;
    #40 instruction_input = 24'b000000000000000000000000;
    #2500 reset = 1;
    $finish;
    end
    
endmodule
