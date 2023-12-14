`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 12/13/2023 07:59:40 PM
// Design Name: 
// Module Name: Fibonacci_TB
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


module Fibonacci_TB(

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
    select_mem = 40;
    reset = 1;
    #100 reset = 0;
    #40 instruction_input = 24'b000000010000000000000000;
    #40 instruction_input = 24'b000000100000000000000100;
    #40 instruction_input = 24'b000000110000000100000000;
    #40 instruction_input = 24'b000010010000000100000000;
    #40 instruction_input = 24'b000001100000011000000001;
    #40 instruction_input = 24'b000011010000001000000001;
    #40 instruction_input = 24'b000100001111110100000000;
    #40 instruction_input = 24'b000000100000001100101000;
    #40 instruction_input = 24'b000001000000001100000110;
    #40 instruction_input = 24'b000000000000000000000000;
    #2500 reset = 1;
    $finish;
    end
    
endmodule
