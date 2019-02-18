module FBfulladder(
 input [3:0] A,
 input [3:0] B,
 input Cin,
 output Cout,
 output [3:0] S
 );
 
 wire c1,c2,c3,c4;
 wire s1,s2,s3,s4;
 
 fulladder
f1(.A(A[0]), .B(B[0]), .Cin(Cin), .Cout(c1),
.S(S[0]));

 fulladder
f2(.A(A[1]), .B(B[1]), .Cin(c1), .Cout(c2), .
S(S[1]));

 fulladder
f3(.A(A[2]), .B(B[2]), .Cin(c2), .Cout(c3), .
S(S[2]));

 fulladder
f4(.A(A[3]), .B(B[3]), .Cin(c3), .Cout(Cout),
.S(S[3]));

endmodule