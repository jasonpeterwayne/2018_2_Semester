`timescale 1ns / 1ps

module lsfr (   // Output of the counter
  clk,  // clock input
  reset,              // reset input
  out
  );
  output [13:0] out;
  input  clk, reset;
  reg [13:0] out;
  reg [4:0] saver;
  wire        linear_feedback;
  
  //-------------Code Starts Here-------
  assign linear_feedback =  ! (out[13] ^ out[3]);
 
  always @(posedge clk)
  if (reset) begin // active high reset
    out <= 8'b0 ;
  end else begin
    out <= {out[13],out[12],out[11],out[10],out[9],out[8],out[7],out[6],out[5],
            out[4],out[3],
            out[2],out[1],
            out[0], linear_feedback};
  end 
  
  endmodule // End Of Module counter
