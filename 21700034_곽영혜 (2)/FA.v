module FA(A, B, C, Sum);
input [3:0]A;
input [3:0]B;
input C;
wire C1, C2, C3, C4;
output [3:0]Sum;

adder u0 (A[0], B[0], 0, Sum[0], C1);
adder u1 (A[1], B[1], C1, Sum[1], C2);
adder u2 (A[2], B[2], C2, Sum[2], C3);
adder u3 (A[3], B[3], C3, Sum[3], C4);

endmodule
