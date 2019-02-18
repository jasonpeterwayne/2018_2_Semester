`timescale 1ns / 1ps

module testb( );
    reg x;
    reg y;
    
    wire S;
    wire C;
    
    fulladder u0 (x, y, 0, S, C);
    
    initial begin
    x=0;
    y=0;
    #10
    x=1;
    #10
    x=0;
    #10
    y=1;
    #10
    y=0;
    #10
    x=1;
    y=1;
    end
    
endmodule
