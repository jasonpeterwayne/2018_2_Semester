module adder(A, B, C, Sum, Cout);
input A; //���� �߿�! (���ῡ ��������) 
input B;
input C;
output Sum;
output Cout;

assign Sum=A^B^C;
assign Cout=(A&B)|(B&C)|(C&A);

endmodule
