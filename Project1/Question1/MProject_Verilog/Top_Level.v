module Top_Level(grounds, display, clk,button); 

output wire [3:0] grounds;
output wire [6:0] display;
input wire button; //push button
wire [3:0] grounds1;
input wire clk;
wire[15:0] r0_ledoutput; // register[0]

Segment7 seg7( r0_ledoutput, grounds, grounds1, display, clk); // we connected the Frog_CPU module's R0_ledoutput in  
Frog_CPU frogcpu( clk , r0_ledoutput, button);                // Segment7 module's din and gave the same clk.

endmodule
