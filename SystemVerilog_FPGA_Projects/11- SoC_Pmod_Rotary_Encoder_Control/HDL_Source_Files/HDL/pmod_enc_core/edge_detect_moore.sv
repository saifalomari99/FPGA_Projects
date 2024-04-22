`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 04/15/2024 06:01:04 AM
// Design Name: 
// Module Name: edge_detect_moore
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
