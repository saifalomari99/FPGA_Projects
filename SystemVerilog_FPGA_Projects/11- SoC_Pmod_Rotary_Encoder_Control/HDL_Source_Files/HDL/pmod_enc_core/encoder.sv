`timescale 1ns / 1ps






//====================================== encoder module ===================================
module encoder(
    input logic clk,
    input logic A,
    input logic B,
    input logic BTN,
    
    output logic [4:0] EncOut,
    output logic [1:0] LED,
    output logic encoded_A,
    output logic encoded_B
    ); 
    
   	 
    logic [31:0] curState = "idle";
	logic [31:0] nextState;
	 
	 // ===========================================================================
	 // 										Implementation
	 // ===========================================================================


	 // *******************************************
	 //  					     Clock
	 // *******************************************
	 always@(posedge clk or posedge BTN)
	 begin
			 if (BTN) begin
				 curState <= "idle";
				 EncOut <= 0;
				 encoded_A <= 0;  
				 encoded_B <= 0;
			 end
			 // detect if the shaft is rotated to right or left
			 // right: add 1 to the position at each click
			 // left: subtract 1 from the position at each click
			 else 
			 begin
				 if(curState != nextState) 
				 begin
						if(curState == "add")                            // ----------------- add state = for sure it is full right
						begin
						        encoded_A <= 1; 
								if(EncOut < 19) begin
									EncOut <= EncOut + 1;
								end
								else begin
									EncOut <= 0;
								end
						end
						else if(curState == "sub")                       // ----------------- sub state = for sure it is full left 
						begin
						        encoded_B <= 1;
								if(EncOut > 0) begin
									EncOut <= EncOut - 1;
								end
								else begin
									EncOut <= 5'd19;
								end
						end
						else 
						begin
						        encoded_A <= 0; 
						        encoded_B <= 0;  
								EncOut <= EncOut;
						end
				 end
				 else 
				 begin
				        //encoded_A <= 0; 
						//encoded_B <= 0;
						EncOut <= EncOut;
				 end

                 curState <= nextState;
			 end
	 end


	 // *******************************************
	 //  					  Next State
	 // *******************************************
	 always@(curState or A or B)
	 begin
				 case (curState)
					  //detent position
					  "idle" : begin
							 LED <= 2'b00;

							 if (B == 1'b0) begin
								 nextState <= "R1";
							 end
							 else if (A == 1'b0) begin
								 nextState <= "L1";
							 end
							 else begin
								 nextState <= "idle";
							 end
					  end
				     // start of right cycle
				     // R1
					  "R1" : begin
							 LED <= 2'b01;

							 if (B == 1'b1) begin
								 nextState <= "idle";
							 end
							 else if (A == 1'b0) begin
								 nextState <= "R2";
							 end
							 else begin
								 nextState <= "R1";
							 end
					  end
					  // R2
					  "R2" : begin
							 LED <= 2'b01;

							 if (A == 1'b1) begin
								 nextState <= "R1";
							 end
							 else if (B == 1'b1) begin
								 nextState <= "R3";
							 end
							 else begin
								 nextState <= "R2";
							 end
					  end
					  // R3
					  "R3" : begin
							 LED <= 2'b01;

							 if (B == 1'b0) begin
								 nextState <= "R2";
							 end
							 else if (A == 1'b1) begin
								 nextState <= "add";
							 end
							 else begin
								 nextState <= "R3";
							 end
					  end
					  // R3
					  "R3" : begin
							 LED <= 2'b01;

							 if (B == 1'b0) begin
								 nextState <= "R2";
							 end
							 else if (A == 1'b1) begin
								 nextState <= "add";
							 end
							 else begin
								 nextState <= "R3";
							 end
					  end
					  // Add
					  "add" : begin
							 LED <= 2'b01;
							 nextState <= "idle";
					  end
   				  // Start of left cycle
                 // L1
					  "L1" : begin
							 LED <= 2'b10;

							 if (A == 1'b1) begin
								 nextState <= "idle";
							 end
							 else if (B == 1'b0) begin
								 nextState <= "L2";
							 end
							 else begin
								 nextState <= "L1";
							 end
					  end
                 // L2
					  "L2" : begin
							 LED <= 2'b10;

							 if (B == 1'b1) begin
								 nextState <= "L1";
							 end
							 else if (A == 1'b1) begin
								 nextState <= "L3";
							 end
							 else begin
								 nextState <= "L2";
							 end
					  end
                 // L3
					  "L3" : begin
							 LED <= 2'b10;

							 if (A == 1'b0) begin
								 nextState <= "L2";
							 end
							 else if (B == 1'b1) begin
								 nextState <= "sub";
							 end
							 else begin
								 nextState <= "L3";
							 end
					  end
                 // Sub
					  "sub" : begin
							 LED <= 2'b10;
							 nextState <= "idle";
					  end
					  //  Default
					  default : begin
							 LED <= 2'b11;
							 nextState <= "idle";
					  end
				 endcase

	 end

endmodule