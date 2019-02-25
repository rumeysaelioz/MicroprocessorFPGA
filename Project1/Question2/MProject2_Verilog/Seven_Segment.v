module Seven_Segment( grounds, grounds1, display, clk , dip_switch);

output reg [3:0] grounds;
output reg [6:0] display;
output [3:0] grounds1;
input [3:0] dip_switch;
input clk;

reg [71:0] student_ids; // concat of student ids.
reg [3:0] data [3:0] ; //number to be printed on display
reg [1:0] count;       //which data byte to display.
reg [25:0] clk1;


//------------for dip_switch[0] --------//
reg [26:0] ticker_normal; // normal speed of clock
reg [25:0] ticker_double; // double speed of clock
reg ticker; // determines clock's speed
wire tick_click; // it processes data with given speed.
 
reg [24:0] blinker_count; // count for blink mode in dip_switch[3]
wire blinker_click; // if blinker_count is 1, it'll be 1. If it's not 1, it will be 0.
reg blinker_clock; // acts like clock for blink operation.

reg ds2_mode; // dip_switch[1] is on
reg swing_mode; // for dip_swich[1], flag for swing operation 
reg [7:0] swing_counter; // for swing counter

reg ds3_mode; // dip_switch[2] is on
reg ds4_mode; // dip_switch[3] is on



// ---- DISPLAY CLOCK ----- 
always @(posedge clk1[15]) //25 slow //19 wavy //15 perfect
begin
    grounds<={grounds[2:0],grounds[3]};
    count<=count+1;
end

always @(posedge clk)
    clk1<=clk1+1;

// ---- TICKER DOUBLE CLOCK -----
always @ (posedge clk) //always block for the ticker
begin
 if(ticker_double[25] == 1) //reset after 0.5 second
  ticker_double <= 0;
 else
  ticker_double <= ticker_double + 1;
end


// ---- TICKER NORMAL CLOCK -----
always @ (posedge clk) //always block for the ticker
begin
 if(ticker_normal[26] == 1) //reset after 1 second
  ticker_normal <= 0;
 else
  ticker_normal <= ticker_normal + 1;
end

// ---- BLINKER CLOCK -----

always @ (posedge clk) //always block for the ticker
begin
 if(blinker_count[24] == 1) //reset after 0.25 second
	 begin
	 blinker_count <= 0;
	 blinker_clock = ~blinker_clock;
	 end
 else
	 begin
	 blinker_count <= blinker_count + 1;
	 end
end


// -----------------------

// ------ DISPLAY SETTINGS ------
always @ (posedge tick_click) //always block for the ticker
begin
	//pass the numbers in data array.
	data[0]=student_ids[71:68];
	data[1]=student_ids[67:64];
	data[2]=student_ids[63:60];
	data[3]=student_ids[59:56];
	

	case(ds2_mode) // 0: rotate mode or 1: swing mode
	0:
		begin 
		
		if(ds3_mode == 0) // if switch is in rotate mode and dip_switch[2] is not open(0), continue in rotate mode and counter increases and hold its current location.
			begin 
			student_ids = {student_ids[70:0],student_ids[71]};
			student_ids = {student_ids[70:0],student_ids[71]};
			student_ids = {student_ids[70:0],student_ids[71]};
			student_ids = {student_ids[70:0],student_ids[71]};
			swing_counter = (swing_counter + 4) % 72;
			end
		else // if switch is in rotate mode and dip_switch[2] is open(1), rotate direction is changed and counter decreases and hold its current location.
			begin 
			student_ids = {student_ids[0],student_ids[71:1]};
			student_ids = {student_ids[0],student_ids[71:1]};
			student_ids = {student_ids[0],student_ids[71:1]};
			student_ids = {student_ids[0],student_ids[71:1]};
			swing_counter = (swing_counter - 4) % 72;
			end
		
		end
	1:
		begin 
	 
		if (swing_mode == 0)
				
			
					begin // if switch in swing mode, value comes the end, values are displayed as reverse 
					 
						if (  swing_counter % 56 == 0 &&  swing_counter != 0 ) // in the end of datas(18 value), swing mode will be 1, values are reversed and hold the location with counter.
						begin
							swing_mode = 1;
						
							student_ids = {student_ids[0],student_ids[71:1]};
							student_ids = {student_ids[0],student_ids[71:1]};
							student_ids = {student_ids[0],student_ids[71:1]};
							student_ids = {student_ids[0],student_ids[71:1]};
							swing_counter = (swing_counter - 4);
						end
						else // the values not in the end of datas rotate mode continue and the hold the location with counter.
						begin
							swing_counter = (swing_counter + 4);
					
							student_ids = {student_ids[70:0],student_ids[71]};
							student_ids = {student_ids[70:0],student_ids[71]};
							student_ids = {student_ids[70:0],student_ids[71]};
							student_ids = {student_ids[70:0],student_ids[71]};
						end
					
					end

				
		else if(swing_mode == 1) //  in dip_swich[1] is on and swing_mode value is 1,  continue swing mode mode till the end. then pass the rotate mode again.
				
		
					begin 
					
			
					if (swing_counter % 56 == 0 && swing_counter != 56  ) // in the end of datas(18 value), swing_mode will be 0, continue in rotate mode and hold the location with counter.
						begin
						swing_mode = 0;
						student_ids = {student_ids[70:0],student_ids[71]};
						student_ids = {student_ids[70:0],student_ids[71]};
						student_ids = {student_ids[70:0],student_ids[71]};
						student_ids = {student_ids[70:0],student_ids[71]};
						swing_counter = (swing_counter + 4);
						end
					else //  till the end of datas, displays rotate mode.
						begin
						swing_counter = (swing_counter - 4);
					
						student_ids = {student_ids[0],student_ids[71:1]};
						student_ids = {student_ids[0],student_ids[71:1]};
						student_ids = {student_ids[0],student_ids[71:1]};
						student_ids = {student_ids[0],student_ids[71:1]};
					end
					
					
					end
			
		else //as default, swing_mode is 0.
				begin
				swing_mode = 0;
				end

		end
	endcase
end


assign grounds1=grounds;

// ------ DISPLAY SETTINGS - DATA ------
always @(*)
begin 
	case(ds4_mode) // If the dip_swich[3] is on, blink mode starts.
	0:
		begin 
			 case(data[count])
			  0:display=7'b0111111; //starts with a, ends with g
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
		end
	1:	
		begin // anytime blinker_clock is 0, display the data.
		 if(!blinker_clock)
			begin 
				 case(data[count])
				  0:display=7'b0111111; //starts with a, ends with g
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
			end
		 
			else //  if the blinker_lock is 1 show nothing on display.
				begin 
				display=7'b0000000;
				end
		end
	endcase
	
end


// ------ Ticker for rotate mode( any direction) or swing mode ------ //

assign tick_click = ((ticker == 1)?1'b1:1'b0);

// ------ Blink for to check 1/4 of the second  ------ //

assign blinker_click = ((blinker_count[24] == 1)?1'b1:1'b0);


// ------ DIP SWITCH SETTINGS ------

// ------ DIP SWITCH 1 ------

always @(dip_switch[0])
    case(dip_switch[0])
        1: // if dip_swich[0] is off (returns 1), every second ticker will be 1.
			begin 
			ticker = ((ticker_normal[26] == 1)?1'b1:1'b0);
			end
        0:
			begin //if dip_swich[0] is on, every half second ticker will be 1.
			ticker = ((ticker_double[25] == 1)?1'b1:1'b0);
			end
		  default: ticker = ((ticker_normal[26] == 1)?1'b1:1'b0); // our default is normal speed mode.
    
    endcase

// ------ DIP SWITCH 2 ------

always @(dip_switch[1])
begin
    case(dip_switch[1])
		1:
			begin  //if dip_swich[1] is off, ds2_mode will be 0 its mean rotate mode. 
			ds2_mode = 1'b0;
			end
      0:
			begin  //if dip_swich[1] is on, ds2_mode will be 1 its mean swing mode.
			ds2_mode = 1'b1;
			end
			
		default:  // our default is rotate mode
			begin
			ds2_mode = 1'b0;
			end
			
    endcase
end

// ------ DIP SWITCH 3 ------

always @(dip_switch[2])
    case(dip_switch[2])
		1:
			begin  //if dip_swich[2] is off, ds3_mode will be 0 its mean continue the datas sliding in rotate mode.
			ds3_mode =  1'b0;
			end
      0:
			begin //if dip_swich[1] is on, ds3_mode will be 1 its mean rotate will be change direction if dip_swich[2] is off.
			ds3_mode =  1'b1;
			end
			
		default: // our default is rotate mode.
			begin
			ds2_mode = 1'b0;
			end
			
    endcase

// ------ DIP SWITCH 4 ------

always @(dip_switch[3])
    case(dip_switch[3])
		1:
			begin //if dip_swich[3] is off, ds4_mode will be 0 its mean datas doesn't blink when scrolling.
			ds4_mode =  1'b0;
			end
      0:
			begin //if dip_swich[3] is on, ds4_mode will be 1 its mean datas blinks  when scrolling.
			ds4_mode =  1'b1;
			end
			
		default: // our default, datas doesn't blink when scrolling.
			begin
			ds4_mode = 1'b0;
			end
			
    endcase


initial begin
    count = 2'b0;
    grounds= 4'b1110;
    student_ids = 72'b000101010000000100010101000000001001000101010000000100010100000000010110;
	 //student_ids = 72'b000100100011010001010110011110001001101010111100110111101111001101011100;
    clk1=0;
	 ticker_normal = 0;
    ticker_double = 0;
	 data[0]=student_ids[71:68];
    data[1]=student_ids[67:64];
    data[2]=student_ids[63:60];
    data[3]=student_ids[59:56];
    swing_mode = 0;
	 swing_counter = 4'b0000;
    blinker_clock = 0;
	 blinker_count = 0;
end

endmodule
