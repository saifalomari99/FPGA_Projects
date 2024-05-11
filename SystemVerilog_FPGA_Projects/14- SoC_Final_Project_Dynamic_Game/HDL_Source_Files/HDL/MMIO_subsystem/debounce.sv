`timescale 1ns / 1ps



//=============================================== debouncing module ==================================================
module debounce
    (
    input logic clk,
    input logic Ain,
    input logic Bin,
    
    output logic Aout,
    output logic Bout
    );

	 // ===========================================================================
	 // 							  Parameters, Regsiters, and Wires
	 // ===========================================================================
	 logic r_Aout = 0;
	 logic r_Bout = 0;

	 logic [6:0] sclk = 0;
	 logic sampledA = 0;
	 logic sampledB = 0;
	 
	 // ===========================================================================
	 // 										Implementation
	 // ===========================================================================
	 always @ (posedge clk) 
	 begin
			sampledA <= Ain;
			sampledB <= Bin;
			// clock is divided to 1MHz
			// samples every 1uS to check if the input is the same as the sample
			// if the signal is stable, the debouncer should output the signal
			
			if(sclk == 100) begin
					if(sampledA == Ain) begin
							r_Aout <= Ain;
					end
					
					if(sampledB == Bin) begin
							r_Bout <= Bin;
					end
					
					sclk <= 0;
			end
			else begin
					sclk <= sclk + 1;
			end
	 end

    assign Aout = r_Aout;
    assign Bout = r_Bout;

endmodule


