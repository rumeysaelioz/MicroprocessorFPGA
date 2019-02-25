module Segment7( din, grounds, grounds1, display, clk);

input wire [15:0] din;
output reg [3:0] grounds; // clock divider
output reg [6:0] display;
output [3:0] grounds1;
input clk;

reg [3:0] data [3:0] ; //number to be printed on display
reg [1:0] count;       //which data byte to display.
reg [25:0] clk1;


always @(posedge clk1[15]) //25 slow //19 wavy //15 perfect
begin
    grounds<={grounds[2:0],grounds[3]};
    count<=count+1;
end

always @(posedge clk)
    clk1<=clk1+1;

assign grounds1=grounds;

//----DISPLAY SETTING-------//
always @(*)
    case(data[count])
        0:display=7'b0111111; //starts with 0, ends with f
        1:display=7'b0000110;
        2:display=7'b1011011;
        3:display=7'b1001111;
        4:display=7'b1100110;
        5:display=7'b1101101;
        6:display=7'b1111101;
        7:display=7'b0000111;
        8:display=7'b1111111;
        9:display=7'b1101111;
        'ha:display=7'b1110111;
        'hb:display=7'b1111100;
        'hc:display=7'b0111001;
        'hd:display=7'b1011110;
        'he:display=7'b1111001;
        'hf:display=7'b1110001;
        default display=7'b1111111;
     endcase

always @(*)
    begin  // as hexadecial display
    data[0]=din[15:12];
    data[1]=din[11:8];
    data[2]=din[7:4];
    data[3]=din[3:0];
    end

 
initial  
    begin
    count = 2'b0;
    grounds=4'b1110;
    clk1=0;
    end

endmodule

