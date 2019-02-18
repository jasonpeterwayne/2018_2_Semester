module fulladder(
 input A,
 input B,
 input Cin,
 output Cout,
 output S
 );
 
 assign { Cout, S } = A + B + Cin;
 
//wire w1,w2,w3;
 
 /*halfadder h1(
 .A(A),
 .B(B),
 .Cout(w2),
 .S(w1)
 );
 
 halfadder h2(
 .A(w1),
 .B(Cin),
 .Cout(w3),
 .S(S)
 );
 
 or o1(Cout,w2,w3);
 */
endmodule