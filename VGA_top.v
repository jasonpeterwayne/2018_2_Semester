`timescale 1ns / 1ps

module VGA_top(
    input CLK100MHZ, reset,
    output VGA_HS, VGA_VS,
    output [11:0] vga
    );
    
    localparam COLOR_WHITE  = 12'b1111_1111_1111;
    localparam COLOR_RED = 12'b0000_0000_1111;
    localparam COLOR_GREEN = 12'b0000_1111_0000;
    localparam COLOR_BLUE = 12'b1111_0000_0000;
    localparam COLOR_YELLOW = 12'b0000_1111_1111;
    localparam COLOR_BLACK  = 12'b0000_0000_0000;
    
    localparam BLOCKS_WIDTH  = 50;
    localparam BLOCKS_HEIGHT = 50;
    
    integer BLOCK_R_X    = 50;
    integer BLOCK_R_Y    = 50;
    
    localparam BLOCKB1_WIDTH  = 30;
    localparam BLOCKB1_HEIGHT = 300;
    
    localparam BLOCK_B1_X    = 200;
    localparam BLOCK_B1_Y    = 0;
    
    localparam BLOCKB2_WIDTH  = 30;
    localparam BLOCKB2_HEIGHT = 380;

    localparam BLOCK_B2_X    = 400;
    localparam BLOCK_B2_Y    = 100;
    
    localparam BLOCK_Y_X    = 500;
    localparam BLOCK_Y_Y    = 300;
    
    
    wire video_on, block_onR, block_onB1, block_onB2, block_onY;
    wire pixel_clk;
    reg [9:0] blink;

    wire [9:0] pixel_x, pixel_y;

    reg [11:0] vga_next, vga_reg;
    
    VGA_controller VGA_controller_1(
        .clk(CLK100MHZ), .reset(reset),
        .hsync(VGA_HS), .vsync(VGA_VS),
        .video_on(video_on), .pixel_clk(pixel_clk),
        .pixel_x(pixel_x), .pixel_y(pixel_y)
    );

    always @(posedge CLK100MHZ, posedge reset)
    begin
        if(reset)
            vga_reg <= 12'd0;
        else
            if(pixel_clk)
                vga_reg <= vga_next;
    end
    
    always @(posedge CLK100MHZ, posedge reset) begin
        if(reset)
            blink <= 10'd0;
        else if(blink == 10000)
            blink <= 10'd0;
        else  blink <= blink + 1'b1;
    end
    
    always@(posedge blink) begin
        if(BLOCK_R_X == 641) 
            BLOCK_R_X = 0;
        else 
            BLOCK_R_X = BLOCK_R_X + 10;
     end

    always @(*)
    begin
        vga_next = vga_reg;
        if(~video_on)
            vga_next = COLOR_BLACK;
        else if(blink)
        begin //ps2:scancode=... move, //보고서(표지 필수!!!)는 내용보단 형식을 중점으로!!! 교수님은 화려하신 걸 좋아하심!!!
            if(block_onR) //enable 신호 를 추가하는 방식으로 프레임 속도 조절 //scancode로 키보드 아스키 값을 인식할 수 있다.
                vga_next = COLOR_RED;
            else if(block_onY)
                    vga_next = COLOR_YELLOW; 
            else
                vga_next =  COLOR_BLACK;
        end
        else vga_next =  COLOR_BLACK;
    end
    
    assign block_onR = (BLOCK_R_X <= pixel_x ) && (pixel_x < BLOCK_R_X + BLOCKS_WIDTH) && (BLOCK_R_Y <= pixel_y) && (pixel_y < BLOCK_R_Y + BLOCKS_HEIGHT);        
    assign block_onB1 = (BLOCK_B1_X <= pixel_x ) && (pixel_x < BLOCK_B1_X + BLOCKB1_WIDTH) && (BLOCK_B1_Y <= pixel_y) && (pixel_y < BLOCK_B1_Y + BLOCKB1_HEIGHT);
    assign block_onB2 = (BLOCK_B2_X <= pixel_x ) && (pixel_x < BLOCK_B2_X + BLOCKB2_WIDTH) && (BLOCK_B2_Y <= pixel_y) && (pixel_y < BLOCK_B2_Y + BLOCKB2_HEIGHT);
    assign block_onY = (BLOCK_Y_X <= pixel_x ) && (pixel_x < BLOCK_Y_X + BLOCKS_WIDTH) && (BLOCK_Y_Y <= pixel_y) && (pixel_y < BLOCK_Y_Y + BLOCKS_HEIGHT);

    assign vga = vga_reg;

endmodule
