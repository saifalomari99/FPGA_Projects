`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
//                                                                              //
//              Topic: Data Transmission with parity bit check                  //
//              Engineer: SAIF ALOMARI                                          //
//                                                                              //
//////////////////////////////////////////////////////////////////////////////////

//================================== main up module ===========================================
module Data_Transmission(clock, Load, Parallelinput,      Q, p, select ,   serial_out);
input clock;
input Load;
input  [7:0] Parallelinput;
wire  w;
output [2:0] Q;
output reg select;
output p;
wire mux_out;
output serial_out;

wire clearCounter;
assign clearCounter = ~Load;


//----------------------------
slowerClkGen1Hz (clock,  clk1Hzoutsignal);
shiftRegister (Parallelinput, Load , 1'b0 , clk1Hzoutsignal , w);
counter3bit (clearCounter, clk1Hzoutsignal, 1'b1 , Q);

odd_parity_bit_generator(clk1Hzoutsignal, ~Load, w,       p);

mux2to1 ( w, p, select,     mux_out);

Dfilpflop (clk1Hzoutsignal, mux_out, serial_out);



//-----------------------------
always @ *
    begin
    if (Q == 3'b111)
        select = 1;
    else
        select = 0;
    end


endmodule








//================================ 1 Hz slow Clk Genertor ==============================
module slowerClkGen1Hz(clk,  outsignal);

input clk;
output reg outsignal;                            //the outsignle that is going to the 3-bit counter
reg [26:0] counter; 

always @ (posedge clk)
    begin
    counter = counter +1;
    if (counter == 100_000_000)                   //give an up signal every half a second
      begin
      outsignal=~outsignal;
      counter =0;
      end
    end
endmodule



//============================= Shift Register ==================================
module shiftRegister (inputR, L, ser, Clock, w);
  input [7:0] inputR;                            //parallel inputs
  input L;                                       //load
  input ser;                                       //serial input
  input Clock;
  output reg [7:0] w;                            //serial outputs


  always @(posedge Clock)
     if (L)                                      //if Load = 1, load the 4-bit valuse of the parallel inputs to the 4-bit parallel outputs
         w <= inputR; 
    else
         begin                                   //if Load = 0, start shifting w to the right, 
            w[0] <= w[1]; 
            w[1] <= w[2];
            w[2] <= w[3];
            w[3] <= w[4]; 
            w[4] <= w[5];
            w[5] <= w[6];
            w[6] <= w[7];
            w[7] <= ser;
         end 

endmodule


//=================================== 3-bit Counter ===========================================
module counter3bit (Resetn, Clock, E, Q);
input Resetn, Clock, E;
output reg [2:0] Q;                     

always @(negedge Resetn, posedge Clock)
    if (!Resetn)
        Q <= 0;
    else if (E)
        Q <= Q + 1;
        
        

endmodule



//============================ Odd parity bit generator ===================================
module odd_parity_bit_generator(clock, reset, w,       p);
input clock, reset, w;
output reg p;

localparam even = 1'b0, odd = 1'b1;
reg [1:0] current_state, next_state;

always @(posedge clock, negedge reset)
    if(~reset)
    current_state <= even;
    else
    current_state <= next_state;


always@*
    begin
    next_state = current_state;
    p = 0;                     
    case (current_state)
        even: begin
            p = 1;                  //Parity Bit = 1
            if (w)
            next_state = odd;
            end
        odd: begin
            p = 0;                  //Parity Bit = 0
            if (w)
            next_state = even;
            end
        default:      
            next_state = even;
    endcase
end//end always
endmodule


//========================== 2 to 1 MUX =================================================
module mux2to1 ( w, p, select,     mux_out);
input w, p, select;
output reg mux_out;

always @*
    begin
    if (select)
        mux_out <= p;
    else 
        mux_out <= w;
        
    end//end always


endmodule




//==================================== D flip flop ========================================
module Dfilpflop (clk, D, Q);
input clk, D;
output reg Q;

always @ (posedge clk)
    Q = D;

endmodule


