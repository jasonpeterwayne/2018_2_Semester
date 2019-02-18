module adder(A, B, C, Sum, Cout);
input A; //순서 중요! (연결에 관련있음) 
input B;
input C;
output Sum;
output Cout;

assign Sum=A^B^C;
assign Cout=(A&B)|(B&C)|(C&A);

endmodule
