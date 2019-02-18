`timescale 1ns / 1ps

module top(
	input clk, rst, switch,
	output dp,
	output [6:0]sseg,
	output [7:0]AN
    );

ss_drive uut0(.clk(clk), .rst(rst), .switch(switch), .sseg(sseg), .ssDP(dp), .AN(AN));
    
endmodule
