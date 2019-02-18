module shiftReg1(Sout,shiftR, D, Sin, load, clk);
output Sout;
input [7:0] D ;
input shiftR, Sin, load, clk;
reg [7:0] Q; // temporal register 

always @(posedge clk)
  if (load) Q <= D; // parallel in
  else if (shiftR) Q <= {Sin, Q[7:1]}; 
assign Sout = Q[0]; // serial out

endmodule
