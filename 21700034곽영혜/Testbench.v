`timescale 1ns / 1ps

module Testbench( );
//for counter & shiftReg
wire[7:0] Q1, Q2, Q3;    //output : counter
wire S1, S2, S3;      //output : shiftReg
reg[7:0] cD, sD;     //input : counter & shiftReg
reg inR, Sin, load, clk, rstn;      //input : inR(inc&ShiftR), load((shiftL)

counter1 c1 (Q1, inR, rstn, clk);    //reset>inc
counter2 c2 (Q2, cD, load, inR, rstn, clk);    //reset>inc>ld 
counter3 c3 (Q3, cD, load, inR, rstn, clk);    //reset>ld>inc

shiftReg1 s1 (S1, inR, sD, Sin, load, clk);
shiftReg2 s2 (S2, inR, sD, Sin, load, clk, rstn);
shiftReg3 s3 (S3, inR, sD, Sin, load, clk, rstn);      //load=shiftL

initial clk = 1'b0;
always #5 clk = ~ clk;

initial rstn = 1'b0;
always #40 rstn = ~ rstn; 
initial load = 1'b0;
always #20 load = ~ load; 
initial inR = 1'b0;
always #10 inR = ~ inR;

initial begin   //test for Counter(8)
cD = 254;
#1000
$finish;
end

initial begin   //test for shiftReg(8)
Sin = 1'b0;
sD = 8'b00001001;
#1000
$finish;
end

endmodule
