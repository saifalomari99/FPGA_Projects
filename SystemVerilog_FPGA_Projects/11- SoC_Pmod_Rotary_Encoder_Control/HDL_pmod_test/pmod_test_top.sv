`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 04/14/2024 03:34:37 PM
// Design Name: 
// Module Name: pmod_test_top
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module pmod_test_top(
    input logic clk_100MHz,       // 100MHz from Basys 3
    input logic reset,            // btnC
    input logic [4:1] JA,         // PMOD JB
    
    output logic [1:0] LED,       // LED[1], LED[0]
    output logic enc_A,
    output logic enc_B,
    output logic [3:0] count
    
    );
	
	// Internal wires 
	logic [4:0] w_enc;
	logic db_A, db_B;
	logic A_tick, B_tick;
	//logic [4:0] prev_w_enc;  // Register to store the previous value of w_enc
	 
	// Instantiate Modules
	//------------------------------- debouncing
	debounce db
	(
	   .clk(clk_100MHz), 
	   .Ain(JA[1]), 
	   .Bin(JA[2]), 
	   
	   .Aout(db_A), 
	   .Bout(db_B)
	);
 	
 	//------------------------------ encoder
 	encoder enc 
 	(
 	  .clk(clk_100MHz), 
 	  .A(db_A), 
 	  .B(db_B), 
 	  .BTN(JA[3]), 
 	  
 	  .EncOut(w_enc),
 	  .LED(LED),
 	  .encoded_A(enc_A),
      .encoded_B(enc_B)
 	);
 	
 	
 	//---------------------------- edge detect 
 	edge_detect_moore edge_of_A
    (
        .clk(clk_100MHz),
        .reset(reset), 
        .level(enc_A), 
        
        .tick(A_tick) 
    );
     
    edge_detect_moore edge_of_B
    (
        .clk(clk_100MHz),
        .reset(reset), 
        .level(enc_B), 
        
        .tick(B_tick) 
    ); 
 	
 	
 	
 	// Logic to detect direction based on EncOut value
//    always @(posedge clk_100MHz) 
//    begin
//            // Check if EncOut has increased
//            if (w_enc > prev_w_enc) begin
//                enc_A <= 1;
//                count <= count + 1;  // Assuming you want to mirror this in count
//            end else if (w_enc < prev_w_enc) begin
//                enc_B <= 1;
//                count <= count - 1;  // Assuming you want to mirror this in count
//            end else begin
//                enc_A <= 0;
//                enc_B <= 0;
//            end

//            // Update previous value of EncOut
//            prev_w_enc <= w_enc;
        
//    end
 	
 	
 	
 	
 	
 	//---------------------- Up and Down Counter 
    always @ (posedge clk_100MHz)
    begin 
    if (A_tick)
        count <= count + 1;           
    else if (B_tick)
        count <= count - 1;        
    end//end always
 	
 	
 	
 	
 	
 	
 	//logic enc_A_flag, enc_B_flag;
// 	slowerClkGen10Hz slow_clk_gen (.clk(clk_100MHz), .outsignal(slow_clk));
 	
// 	always @(slow_clk) 
// 	begin
// 	      if (A_tick) 
// 	      begin
//                count <= count + 1;
//                enc_A_flag <= 1;  // Set the flag high when A_tick is detected
//          end else if (B_tick) 
//          begin
//                count <= count - 1;
//                enc_B_flag <= 1;  // Set the flag high when B_tick is detected
//          end

//            // Reset flags after setting them in the previous cycle
//            if (enc_A_flag)
//                enc_A_flag <= 0;
//            if (enc_B_flag)
//                enc_B_flag <= 0;
//    end

endmodule








//================================ 10 Hz Human speed Clk Genertor ==============================
module slowerClkGen10Hz(input logic clk, output logic outsignal);
 
logic [26:0] counter; 
//logic outsignal;

always @ (posedge clk)                               //if the reset switch is off:
begin
    counter = counter +1;
    if (counter == 30_000_000)        //10 Hz clock
    begin
        outsignal=~outsignal;
        counter =0;
    end
end
endmodule




//============================= moore detect =============================================
module edge_detect_moore 
(
    input logic clk, 
    input logic reset, 
    input logic level, 
    output logic tick 
); 
    
    localparam [1:0] zero=2'b00, edg=2'b01, one=2'b10;
    logic [1:0] state_reg, state_next;
    
    //slowerClkGen1Hz    clk1Hz   (clk, reset1HzclkSW, clk1HzOutsignal );      //slow
    
    always @(posedge clk, posedge reset)
        if (reset)
            state_reg<=zero;
        else
            state_reg<=state_next;
 
    always_comb
        begin
        state_next=state_reg;
        tick=1'b0; //default output
        case (state_reg)
            zero:
                begin
                tick=1'b0;
                if (level)
                    state_next=edg;
                end
            edg:
                begin
                tick=1'b1;
                if (level)
                    state_next=one;
                else
                    state_next=zero;
                end
            one: 
                if (~level)
                    state_next=zero;
            default: state_next=zero;
        endcase
        
        end//end always
        
endmodule


