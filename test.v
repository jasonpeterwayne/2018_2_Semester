`timescale 1ns / 1ps
module test;
 
 // Inputs
 reg clock;
 reg reset;
 reg change;
 
 // Outputs
 wire [12:0] rnd;
 
 // Instantiate the Unit Under Test (UUT)
 lsfr uut (
  .clock(clock), 
  .reset(reset), 
  .change(change),
  .rnd(rnd)
 );
  
 initial begin
  clock = 0;
  forever
   #10 clock = ~clock;
  end
   
 initial begin
  reset = 0;
  change = 0;
  #100;
    reset = 1;
  #200;
    reset = 0;
  #500;
    change = 1;
  #100;
    change = 0;
  #300;
    change = 1;
    reset = 1;
  #200;
    reset = 0;
  #100;
    change = 0;
  #400;
    change = 1;
 end    
endmodule