`timescale 1ns / 1ps
module testfulladder( );
 reg A, B, Cin;
 wire Cout, S;
 
 fulladder f1(
 .A(A),
 .B(B),
 .Cin(Cin),
 .Cout(Cout),
 .S(S)
 );
 
 initial begin
 A = 0; B = 0; Cin = 0;
 #100 A = 0; B = 1;
 #100 A = 1; B = 0;
 #100 A = 1; B = 1;
 #100 A = 0; B = 0; Cin = 1;
 #100 A = 0; B = 1;
 #100 A = 1; B = 0;
 #100 A = 1; B = 1; #100
$finish;
 end
endmodule