module dec(input [3:0] sum, output reg d11, d7, d6);
always @(*)
    case(sum)
        4'b1011 : d11=1'b1;
        4'b0111 : d7=1'b1; 
        4'b0110 : d6=1'b1;
        default : begin d11=1'b0; d7=1'b0; d6=1'b0; end
    endcase
endmodule
