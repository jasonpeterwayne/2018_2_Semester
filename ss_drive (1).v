`timescale 1ns / 1ps

module ss_drive(
    input [2:0] data3,
    input [2:0] data2,
    input [2:0] data1,
    input [2:0] data0,
    input [1:0] sel,
    output ssA,
    output ssB,
    output ssC,
    output ssD,
    output ssE,
    output ssF,
    output ssG,
    output ssDP,
    output reg[7:0] AN
    );

	reg [3:0] data;

	always @(data3, data2, data1, data0, sel)
		case(sel)
			2'b00 : data = data0;
			2'b01 : data = data1;
			2'b10 : data = data2;
            default : data = data3; //2'b11
		endcase

	always @(sel)
	   begin 
           AN = 8'b1111_1111;
        case(sel)
            2'b00 : AN[0]=0;
            2'b01 : AN[1]=0;
            2'b10 : AN[2]=0;
            default : AN[3]=0; //2'b11
        endcase
       end
	
	ss_decoder data_decode (
    .Din(data), 
    .a(ssA), 
    .b(ssB), 
    .c(ssC), 
    .d(ssD), 
    .e(ssE), 
    .f(ssF), 
    .g(ssG), 
    .dp(ssDP)
    );

endmodule