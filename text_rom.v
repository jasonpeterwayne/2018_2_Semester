module text_rom
(
	input wire clk,
	input wire [9:0] pix_x, pix_y,
	input wire [11:0] font_background,
	output wire [1:0] text_on,
	output reg [11:0] text_rgb
);

localparam COLOR_WHITE = 12'b1111_1111_1111;
localparam COLOR_RED = 12'b0000_0000_1111;
localparam COLOR_BLACK = 12'b0000_0000_0000;
localparam COLOR_BLUE = 12'b1111_0000_0000;

localparam SCORE_COLOR = COLOR_RED;
localparam LOG_COLOR = COLOR_BLUE;

wire [10:0] rom_addr;
wire [7:0] font_word;

reg [6:0] char_addr;
reg [3:0] row_addr;
reg [2:0] bit_addr;

reg [6:0] char_addr_s, char_addr_l;
wire [3:0] row_addr_s, row_addr_l;
wire [2:0] bit_addr_s, bit_addr_l;

wire font_bit, score_on, logo_on;

font_rom font_unit(
	.clk(clk), .addr(rom_addr), .data(font_word)
);

// s c o r e r e g i o n
// - display two-digit score , ball on top left
// - scale to 16-by-32 font
// - line1 , 16 chars : "Score:DD Bal1:D"

assign score_on = (pix_y[9:6] ==0) && (pix_x[9:4] < 32);
// y_size = 16 * 2^1 = 32  >> 5bit
// x_size = 8 * 2^1 = 16 >> 4bit
// y = 32 * 0 = 0 	|		0 <= y < 32 * 1
// 				  	|		0 <= x < 16 * 16 = 256 
// it mean 16 character display in first line //위치?
assign row_addr_s = pix_y [5:2]; // [4:1] >> 1bit scaling //세로영역 
assign bit_addr_s = pix_x [3:1] ; // [3:1] >> 1bit scaling //가로영역

always @* begin
	case (pix_x [7:4]) //시작 지점 
//		4'h0: char_addr_s = 7'h00; // S
		4'h5: char_addr_s = 7'h53; // c
		4'h6: char_addr_s = 7'h4e; // o
		4'h7: char_addr_s = 7'h41; // r
		4'h8: char_addr_s = 7'h4b; // e
		4'h9: char_addr_s = 7'h45; // :
		4'ha: char_addr_s = 7'h00;
		4'hb: char_addr_s = 7'h00;
		4'hc: char_addr_s = 7'h47; //
		4'hd: char_addr_s = 7'h41; //
		4'he: char_addr_s = 7'h4d; // B
		4'hf: char_addr_s = 7'h45; // a
		default : char_addr_s = 7'h00;
//		4'hc: char_addr_s = 7'h00; // 1
//		4'hd: char_addr_s = 7'h00; // 1
//		4'he: char_addr_s = 7'h00; // :
	endcase
end

// logo r e g i o n :
// - display logo at t o p c e n t e r
// - used as background
// - scale to 64-by-128 font
assign logo_on = (pix_y[9:7] == 2) && (2 <= pix_x[9:6] ) && (pix_x[9:6] <= 8);
// y_size = 16 * 2^3 = 128  >> 7bit
// x_size = 8 * 2^3 = 64 >> 6bit
// y = 128 * 2 == 256 		|	256 <= y < 128 * (2 + 1) = 384
// 							|	64*3 = 192 <= x < 64 * 7 = 448
// it mean 16 character display in first line
assign row_addr_l = pix_y [6:3] ; // [6:3] >> 3bit scaling
assign bit_addr_l = pix_x [5:3] ; // [5:3] >> 3bit scaling

always @* begin
	case (pix_x [8:6])
		3'h3: char_addr_l = 7'h53; // P
		3'h4: char_addr_l = 7'h74; // 0
		3'h5: char_addr_l = 7'h61; // N
		3'h6 : char_addr_l = 7'h72; // G
		3'h7 : char_addr_l = 7'h74;
		default : char_addr_l = 7'h00;
	endcase
end

always @* begin
	char_addr = 7'h00;
	row_addr = 4'h0;
	bit_addr = 3'h0;
	text_rgb = font_background;
	if(score_on) begin
		char_addr = char_addr_s;
		row_addr = row_addr_s;
		bit_addr = bit_addr_s;
		if(font_bit)
			text_rgb = SCORE_COLOR;
	end
	else if (logo_on) begin
		char_addr = char_addr_l;
		row_addr = row_addr_l;
		bit_addr = bit_addr_l;
		if(font_bit)
			text_rgb = LOG_COLOR;
	end
end

assign text_on = {score_on, logo_on};
assign rom_addr = {char_addr, row_addr};
assign font_bit = font_word[7-bit_addr]; // MSB display first

endmodule
