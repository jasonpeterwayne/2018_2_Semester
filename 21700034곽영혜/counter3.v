module counter3(Q, D, ld, inc, rstn, clk);
output [7:0] Q ;
input [7:0] D;
input ld, inc, rstn, clk;
reg [7:0] Q;

always @(posedge clk or negedge rstn)
  if(!rstn) Q <= 8'd0;
  else if (ld) Q <= D;
  else if (inc) Q <= Q + 1;

endmodule
