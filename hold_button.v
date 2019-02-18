`timescale 1ns / 1ps

module hold_button(
    input clk,
    input [3:0] BTN,
    output reg [3:0] btn
    );
    
    always @(posedge clk) begin
        if(BTN[0] == 1'b1)  begin
             btn[0] =1;
             btn[1] =0; btn[2] =0; btn[3] =0;
         end
        else if(BTN[1] == 1'b1) begin
             btn[1] =1'b1;
             btn[0] =0; btn[2] =0; btn[3] =0;
         end
        else if(BTN[2] == 1'b1) begin
             btn[2] =1'b1;
            btn[0] =0; btn[1] =0; btn[3] =0;
        end
        else if(BTN[3] == 1'b1) begin
             btn[3] =1'b1;
             btn[0] =0; btn[1] =0; btn[2] =0;
         end
    end
endmodule