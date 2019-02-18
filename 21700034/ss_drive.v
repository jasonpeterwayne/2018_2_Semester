`timescale 1ns / 1ps

module ss_drive(
	input clk,
	input rst,
	input switch,
	output [6:0]sseg,
	output ssDP,
	output reg [7:0]AN
    );
    
	reg [1:0] sel;
	reg [3:0] sec0, sec1, min0, min1;
    wire up;
	integer cnt;
	
	always @(posedge clk)
		if(rst)
		begin
			cnt <= 0;
			sel <= 2'd00;
		end
		else if (cnt == 1000000) //100MEGHz / 20000 = 5KHz = 1/ (0,2ms). 0.2ms * 8 = 1.6ms
		begin
			cnt <= 0;
			sel <= sel+1;
		end
		else if(switch)
		begin
			cnt <= cnt + 1;
			sel <= sel;
		end

    assign up = (cnt == 1000000)?1'b1:1'b0;
    
    always @ (posedge clk or posedge rst)
        begin
        if (rst) begin
            sec0 <= 0;
            sec1 <= 0;
            min0 <= 0;
            min1 <= 0;
        end
   
        else if (up) 
        begin
        if(sec0 == 9) begin  
            sec0 <= 0;
            if (sec1 == 9) begin  
                sec1 <= 0;
                if (min0 == 9) begin 
                    min0 <= 0;
                    if(min1 == 5) 
                        min1 <= 0;
                    else
                        min1 <= min1 + 1;
                end
                else //else_3
                    min0 <= min0 + 1;
            end
            else 
                sec1 <= sec1 + 1;
        end
        else 
            sec0 <= sec0 + 1;
        end
        end
    
    reg [3:0] data;
   
	always @(*)
		case(sel)
		2'b00 : begin
		          data = sec0;
		          AN = 8'b1111_1110;
		end
		2'b01 : begin
				  data = sec1;
		          AN = 8'b1111_1101;
		end
		2'b10 : begin
		          data = min0;
		          AN = 8'b1111_1011;
		end                				
	    2'b11 : begin
	              data = min1;
                  AN = 8'b1111_0111;
        end             
		endcase
	
	ss_decoder data_decode (
                    .D(data), 
                    .sseg(sseg),
                    .dp(ssDP)
                    );
endmodule
