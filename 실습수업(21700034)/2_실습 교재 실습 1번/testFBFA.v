`timescale 1ns / 1ps
module testFBFA( );
 reg [3:0] A;
 reg [3:0] B;
 reg Cin;
 wire [3:0] S;
 wire Cout;
 
 FBfulladder F1(
 .A(A),
 .B(B),
 .Cin(Cin),
 .Cout(Cout),
 .S(S)
 );
 

always #10 A = A + 1'b1;
always #160 B = B + 1'b1;
always #2560 Cin = ~Cin;


initial begin
A = 4'h0; 
B = 4'h0; 
Cin = 0;
/*
#100
A = 1;
B = 3;
#100
A = 4;
B = 3;
#100
A = 7;
B = 7;
#100
A = 8;
B = 9;
*/
#5120 
$finish;
end
endmodule