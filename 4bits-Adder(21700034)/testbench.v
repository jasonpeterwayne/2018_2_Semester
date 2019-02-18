`timescale 1ns / 1ps

module testbench( );
reg [3:0]x;
reg [3:0]y;

wire [3:0]S;
wire C1, C2, C3, C4;


adder u0 (x[0], y[0], 0, S[0], C1);
adder u1 (x[1], y[1], C1, S[1], C2);
adder u2 (x[2], y[2], C2, S[2], C3);
adder u3 (x[3], y[3], C3, S[3], C4);

initial begin
x=0;
y=0;
#10
x=1;
y=5;
/*
x[0]=1;
y[0]=1;
#10
x[1]=0;
y[1]=0;
#10
x[2]=0;
y[2]=1;
#10
x[3]=0;
y[3]=0;
*/
#20
$finish;
end
endmodule
