module bcd_6(
    input clk, rst, inc,
    output reg [3:0] D,
    output tc
    );
    
    always@(posedge clk, posedge rst) 
        if(rst) 
            D <= 4'b0000;
        else if(inc) begin
            if(D == 4'd5)
                D <= 4'b0000;
            else D <= D + 1;
        end
    
    assign tc = (D==4'd5) && (inc);
endmodule
