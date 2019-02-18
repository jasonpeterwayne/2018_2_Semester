module dice(
    input clk, rst, roll,
    output reg win, lose,
    output ssA, ssB, ssC, ssD, ssE, ssF, ssG, ssDP,
    output reg [7:0] AN
    );
    
    wire [3:0] Dice0, Dice1;
    wire D11, D6, D7, EQ, tc, trash;
    wire sync_roll;
    
    reg inc, load;
    wire [3:0] sum, point;
    reg [3:0] Data;
    
    parameter S0 = 3'b000;
    parameter S1 = 3'b001;
    parameter S2 = 3'b010;
    parameter S3 = 3'b011;
    parameter S4 = 3'b100;
    parameter S5 = 3'b101;
    
    reg[3:0] curr_st, next_st;
    
    bcd_6 b1 (clk, rst, inc, Dice0, tc);
    bcd_6 b2 (clk, rst, tc, Dice1, trash);
    
    FA f1 (Dice0, Dice1, 0, sum);
    
    dec s1 (sum, D11, D7, D6);
    
    assign EQ = (sum == point) ? 1'b1 : 1'b0;
    
    assign point = (load == 1) ? sum : 0;
    
    always @(Dice0, Dice1, inc, tc) begin
        if(inc) Data = Dice0;
        if(tc) Data = Dice1;
    end
            
    always @(roll, inc, tc) begin
        AN = 8'b1111_1111;
           if(roll)
           begin 
            if(inc) AN[0] = 0;
            if(tc) AN[1] = 0;
           end
    end
    
    drive d1 (Data, ssA, ssB, ssC, ssD, ssE, ssF, ssG, ssDP); 
    
    always @(posedge clk)
        if(rst)
            curr_st <= S0;
        else
            curr_st <= next_st;
            
    always @*
        begin 
        load = 0;
        win = 0;
        lose = 0;
        case(curr_st)
        S0 : if(roll == 0) next_st = S0;
             else next_st = S1;
        S1 : if(roll) next_st = S1;
             else if(D6) next_st = S3;
             else if(D7 || D11) next_st = S2;
             else begin
                next_st = S4;
                load = 1;
             end
        S2 : if(rst) next_st = S0;
             else begin 
                next_st = S2;
                win = 1;
             end
        S3 : if(rst) next_st = S0;
             else begin
                next_st = S3;
                lose = 1;
             end
        S4 : if(roll == 0) next_st = S4;
             else next_st = S5;
        S5 : if(roll) next_st = S5;
             else if(EQ) next_st = S2;
             else if(D7 || D6) next_st = S3;
             else next_st = S4;
        default : next_st = S0;
        endcase
        end
        
        always @*
            begin
            inc = 1'b0;
            if((curr_st == S1 && roll) || (curr_st == S5 && roll))
                inc = 1'b1;
            end
        
endmodule
