module halfadder(
 input A,
 input B,
 output Cout,
 output S
 );
 
 and a1(Cout, A, B);
 xor x1(S, A, B);
endmodule
