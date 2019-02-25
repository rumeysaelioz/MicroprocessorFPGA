module Frog_CPU(input wire clk, output wire [15:0] r0_ledoutput, input wire button);

reg [1:0] state;
reg [15:0] memory[0:255];
wire[15:0] aluinr, aluinl; // two inputs will be processed in alu
wire[3:0] aluop; // which alu operation
reg [9:0] pc; // program counter
reg [15:0] ir; // instruction register
reg [15:0] aluout; // alu result
reg [15:0] regbank[0:3];  /// 4 bit register

//--STATES--//
localparam  FETCH = 2'b00; 
localparam  LDI   = 2'b01;
localparam  ALU   = 2'b10;

    always @(posedge button)
        case(state)
 
            FETCH:
            begin
            	state<=memory[pc][15:14];  //opcode
           	ir<=memory[pc][11:0];  // 12 bit instruction register
           	pc<=pc+1;
            end

            LDI:
            begin
                state<=FETCH;                      // ir[1:0] -> destination register 
                regbank[ ir[1:0] ] <= memory[pc]; // read from memory and write in dest reg.
                pc<=pc+1;
            end

            ALU:
            begin
                state<=FETCH;
                regbank[ ir[1:0] ] <= aluout;    // write alu result in dest reg.                   
            end

        endcase
 
assign aluinl=regbank[ir[7:6]]; // source reg 1
assign aluinr=regbank[ir[4:3]]; // source reg 2
assign aluop=ir[11:8];  // which alu operation
assign r0_ledoutput = regbank[0]; // we display regbank[0]

//-----ALU OPERATIONS--
always @*
    case( aluop )
        4'h0:  aluout = aluinl + aluinr;
        4'h1:  aluout = aluinl - aluinr;
        4'h2:  aluout = aluinl & aluinr;
        4'h3:  aluout = aluinl | aluinr;
        4'h4:  aluout = aluinl ^ aluinr;
        4'h5:  aluout = ~aluinr;
        4'h6:  aluout =  aluinr;
        4'h7:  aluout = aluinr + 16'h0001;
        4'h8:  aluout = aluinr - 16'h0001;
        default: aluout = 0;
    endcase

               
initial begin
     $readmemh("prog.txt", memory);  //must be exactly 512 locations
     state = FETCH; //initial state always be fetch.
     pc = 0;
end

endmodule
 
