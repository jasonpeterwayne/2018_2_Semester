`timescale 1ns / 1ps

module testadder( );
 reg A, B;
 wire Cout, S;
 
 halfadder h1(
 .A(A),
 .B(B),
 .Cout(Cout),
 .S(S)
 );
 
 initial begin
 A = 0; B = 0;
 #100 A = 0; B = 1;
 #100 A = 1; B = 0;
 #100 A = 1; B = 1;
 #100 A = 0; B = 0;
 #100 $finish;
 end
endmodule