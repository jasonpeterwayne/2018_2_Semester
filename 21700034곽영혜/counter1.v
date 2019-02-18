module counter1(Q, inc, rstn, clk);
output [7:0] Q ;
input inc, rstn, clk;
reg [7:0] Q;

always @(posedge clk)
  if(!rstn) Q = 8'd0;
  else if (inc) Q = Q + 1;

endmodule
