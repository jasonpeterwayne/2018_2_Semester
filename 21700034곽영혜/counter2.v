module counter2(Q, D, ld, inc, rstn, clk);
output [7:0] Q ;
input [7:0] D;
input ld, inc, rstn, clk;
reg [7:0] Q;

always @(posedge clk)
  if(!rstn) Q <= 8'd0;
  else if (inc) Q <= Q + 1;
  else if (ld) Q <= D;

endmodule
