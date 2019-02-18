`timescale 1ns / 1ps

module ss_decoder(
	input [3:0] D,
	output reg [6:0] sseg, // sseg[0] = ssA, sseg[6] = ssG
	output dp
);

	assign dp = 1'b1;
    
	always @(D)
	begin
    	case(D)
	       4'd0: sseg = 7'b100_0000;
	       4'd1: sseg = 7'b111_1001;
	       4'd2: sseg = 7'b010_0100;
	       4'd3: sseg = 7'b011_0000;
	       4'd4: sseg = 7'b001_1001;
	       4'd5: sseg = 7'b001_0010;
	       4'd6: sseg = 7'b000_0010;
	       4'd7: sseg = 7'b101_1000;
	       4'd8: sseg = 7'b000_0000;
	       4'd9: sseg = 7'b001_0000;
           default: sseg = 7'b100_0000;
	endcase
	end
endmodule