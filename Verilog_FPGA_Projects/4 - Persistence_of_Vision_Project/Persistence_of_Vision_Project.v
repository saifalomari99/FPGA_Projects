
// --------------------------------------
//
//          Persistence of Vision Project
//          Saif Alomari  
//          Spring 2023
//
// --------------------------------------


`timescale 1ns / 1ps

//=============================================== main module ===============================================
module mainProject(clk, reset1HzclkSW, reset400HzclkSW, resetCounterSW, clk1HzOutsignal, clk400HzOutsignal, Q, Qpattern   ,AN, CX);
input clk;
input reset1HzclkSW;                //1Hz clk reset button          
input reset400HzclkSW;              //400Hz clk reset button
input resetCounterSW;               //counter reset switch

output clk1HzOutsignal;             //1Hz clk:   slow
output clk400HzOutsignal;           //400Hz clk: fast
output [1:0]Q;                      //counter outputs
output [1:0]Qpattern;               //pattern counter outputs
output [7:0]AN;
output [7:0]CX;

slowerClkGen1Hz subClkGen (clk, reset1HzclkSW, clk1HzOutsignal ); 
slowerClkGen400Hz sub400clk (clk, reset400HzclkSW, clk400HzOutsignal);

counter2bit subcounter (resetCounterSW, clk400HzOutsignal, 1'b1, Q);     //2-bit counter to run through all 4 7-segemnts displays really fast using the 400Hz clk
patternGenerator subGen (clk1HzOutsignal,1'b1 , Qpattern);               //2-bit counter to run through the 4 different patterns with the 1Hz clk
mux4to1 subMux (Q, Qpattern, AN, CX);                                    //Mux to contol the patterns and displaying them to the 7-segemnts displays

endmodule
//================================== end ==================================================





//================================ 1 Hz slow Clk Genertor ==============================
module slowerClkGen1Hz(clk, resetSW, outsignal);

input clk;
input resetSW;             // you connect this to a switch to manully reset the clock generator
output outsignal;          //the outsignle that is going to the 3-bit counter
 
reg [26:0] counter; 
reg outsignal;

always @ (posedge clk)
 begin
  if (resetSW)            //if you click the reset switch, it resets the counter and the outsignal
   begin
    counter=0;
    outsignal=0;
   end
  
  else                                 //if the reset switch is off:
   begin
    counter = counter +1;
    if (counter == 50_000_000)        //give an up signal every half a second
     begin
      outsignal=~outsignal;
      counter =0;
    end
  end
end
endmodule

//================================ 400 Hz fast Clk Genertor ==============================
module slowerClkGen400Hz(clk, resetSW, outsignal);

input clk;
input resetSW;                   // you connect this to a switch to manully reset the clock generator
output outsignal;                //the outsignle that is going to the 3-bit counter
 
reg [26:0] counter; 
reg outsignal;

always @ (posedge clk)
 begin
  if (resetSW)    //if you click the reset switch, it resets the counter and the outsignal
   begin
    counter=0;
    outsignal=0;
   end
  
  else                                 //if the reset switch is off:
   begin
    counter = counter +1;
    if (counter == 125_000)        //fast clock
     begin
      outsignal=~outsignal;
      counter =0;
    end
  end
end
endmodule


//=================================== Counter (selectors) ===========================================
module counter2bit (Resetn, Clock, E, Q);
input Resetn, Clock, E;
output reg [1:0] Q;                     // connect those to 3 lights

always @(negedge Resetn, posedge Clock)
if (!Resetn)
Q <= 0;
else if (E)
Q <= Q + 1;

endmodule

//===================================== pattern counter ===============================
module patternGenerator (clk,E, Qpattern);
input clk;
input E;
output reg [1:0] Qpattern;

always @(posedge clk)
if (E)
  Qpattern <= Qpattern + 1;
  


endmodule 

//================================ 4 to 1 multiplexer module =============================
module mux4to1 (select, Qpattern, AN, CX);
input [1:0]select;                    //the two selectors
input [1:0]Qpattern;                  //to choose the pattern

output reg [7:0]AN;                   //the different 8 7-segment-displays
output reg [7:0]CX;                   //the 7-segments

always @ (select, Qpattern)
begin
    case(Qpattern)                       //case statement to run through 4 possible values using the slow clk
    2'b00:                               //first pattern = SAIF
        if (select==0)                   //if statement to turn all 4 LEDs at once with help of the counter 
        begin                            //that counts so fast and keeps looping through the 4 possible values
        AN = 8'b11110111;    
        CX = 8'b01001001;    //S
        end
        else if (select == 1)
        begin
        AN = 8'b11111011;
        CX = 8'b00010001;    //A
        end
        else if (select == 2)
        begin
        AN = 8'b11111101;
        CX = 8'b10011111;    //I
        end
        else if (select == 3)
        begin
        AN = 8'b11111110;
        CX = 8'b01110001;    //F
        end
 
    2'b01:                                //second pattern = 1999
        if (select==0)
        begin
        AN = 8'b11110111;
        CX = 8'b10011111;     //1
        end
        else if (select == 1)
        begin
        AN = 8'b11111011;
        CX = 8'b00001001;    //9
        end
        else if (select == 2)
        begin
        AN = 8'b11111101;
        CX = 8'b00001001;    //9
        end
        else if (select == 3)
        begin
        AN = 8'b11111110;
        CX = 8'b00001001;    //9
        end
     
    2'b10:                             //pattern 3 = JAZZ
        if (select==0)
        begin
        AN = 8'b11110111;
        CX = 8'b10001111;     //J
        end
        else if (select == 1)
        begin
        AN = 8'b11111011;
        CX = 8'b00010001;    //A
        end
        else if (select == 2)
        begin
        AN = 8'b11111101;
        CX = 8'b00100101;    //z
        end
        else if (select == 3)
        begin
        AN = 8'b11111110;
        CX = 8'b00100101;    //z
        end
    
    2'b11:                          //pattern 4 = 2000
        if (select==0)
        begin
        AN = 8'b11110111;
        CX = 8'b00100101;     //2
        end
        else if (select == 1)
        begin
        AN = 8'b11111011;
        CX = 8'b00000011;    //0
        end
        else if (select == 2)
        begin
        AN = 8'b11111101;
        CX = 8'b00000011;    //0
        end
        else if (select == 3)
        begin
        AN = 8'b11111110;
        CX = 8'b00000011;    //0
        end
    endcase



end

endmodule 






