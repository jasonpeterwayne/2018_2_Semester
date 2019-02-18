module shiftReg3(Sout,shiftR, D, Sin, shiftL, clk, rstn); //shiftL(=load)
output Sout;
input [7:0] D ;
input shiftR,Sin, shiftL, clk, rstn;
reg [7:0] Q; // temporal register 

always @(posedge clk or negedge rstn)
  if(!rstn) Q <= 8'b0;
  else if (shiftR) Q <= {Sin, Q[7:1]}; 
  else if (shiftL) Q <= {Q[7:1], Sin}; 
  else Q <= D;
assign Sout = Q[0]; // serial out

endmodule
