`timescale 1ns / 1ps

//=======================================================//
// Engineer: Saif Alomari                                //
// Topic: Digital Clock on the 7 segments displays       //
// Date: March 8, 2023                                   //
//=======================================================//


//================================== main module ========================================
module digitalClock(clk, reset1HzclkSW, reset400HzclkSW, resetCounterSW,         clk1HzOutsignal, clk400HzOutsignal, seconds ,minutes      ,AN,CX);
input clk;
input reset1HzclkSW;                    //1Hz clk reset button          
input reset400HzclkSW;                  //400Hz clk reset button
input resetCounterSW;                   //counter reset switch

output clk1HzOutsignal;                 //1Hz clk:   slow
output clk400HzOutsignal;               //400Hz clk: fast
wire [1:0]Q;                            //counter outputs = selecters
output [5:0] seconds, minutes;
output [7:0]AN;
output [7:0]CX;
wire [3:0] upperdigitS, lowerdigitS, upperdigitM, lowerdigitM;


slowerClkGen1Hz subClkGen (clk, reset1HzclkSW, clk1HzOutsignal ); 
slowerClkGen400Hz sub400clk (clk, reset400HzclkSW, clk400HzOutsignal);

counter2bit subcounter (resetCounterSW, clk400HzOutsignal, 1'b1, Q);     //2-bit counter to run through all 4 7-segemnts displays really fast using the 400Hz clk Selecters
counters        stage0 (clk1HzOutsignal, seconds, minutes);
digitsSeparator stage1 (seconds, minutes,  upperdigitS, lowerdigitS, upperdigitM, lowerdigitM);
mux4to1         stage2 (Q,  upperdigitS, lowerdigitS, upperdigitM, lowerdigitM,  AN, CX);

endmodule
//================================== end main ============================================


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

//================================= seconds & minutes Counters =======================================
module counters(slow_clock, seconds, minutes);
input slow_clock;
output reg [5:0] seconds, minutes;

always@ (posedge slow_clock)
begin
    if (seconds != 6'b111011)                  //if the seconds != 59
        begin 
        seconds <= seconds + 1;
        end
    else                                      //if the seconds = 59
        if (minutes != 6'b111011)             //if the minutes != 59 
            begin
            minutes <= minutes + 1;
            seconds = 0;
            end
        else                                  //if the minutes = 59 ==> the end, start from the zero again
            begin
            minutes <= 0;
            seconds <= 0;
            end     
end
endmodule                                     // remember that         59 = 111 011
 

//================================ digits separator ==================================================
module digitsSeparator (seconds, minutes,        upperdigitS, lowerdigitS, upperdigitM, lowerdigitM);
input [5:0] seconds;
input [5:0] minutes;
output reg [3:0] upperdigitS;
output reg [3:0] lowerdigitS;
output reg [3:0] upperdigitM;
output reg [3:0] lowerdigitM;

always @*
begin
    upperdigitS = seconds/10;
    lowerdigitS = seconds%10;  
    upperdigitM = minutes/10;
    lowerdigitM = minutes%10;       
end

endmodule 

//=================================== Counter (selectors) ===========================================
module counter2bit (Resetn, Clock, E, Q);
input Resetn, Clock, E;
output reg [1:0] Q;                     

always @(negedge Resetn, posedge Clock)
if (!Resetn)
Q <= 0;
else if (E)
Q <= Q + 1;

endmodule

//==================================== 4 to 1 multiplexer module ======================================
module mux4to1 (select, upperdigitS, lowerdigitS, upperdigitM, lowerdigitM,        AN, CX);
input [1:0]select;                     //the two selectors
input [3:0] upperdigitS, lowerdigitS, upperdigitM, lowerdigitM;   //the inputs

output reg [7:0]AN;                    //the different 8 7-segment-displays
output reg [7:0]CX;                    //the 7-segments

always @ (select,upperdigitS, lowerdigitS, upperdigitM, lowerdigitM )
begin
    case(select)                       //case statement to run through 4 digits and display them all together really fast
    2'b00:  
        begin                                                                     
            AN = 8'b11110111;    
            case (upperdigitM)         //display the minutes upper digit                       
                0: CX = 8'b00000011;        
                1: CX = 8'b10011111;        
                2: CX = 8'b00100101;
                3: CX = 8'b00001101;
                4: CX = 8'b10011001;
                5: CX = 8'b01001001;
                6: CX = 8'b01000001;
                7: CX = 8'b00011111;
                8: CX = 8'b00000001;
                9: CX = 8'b00001001; 
            endcase
        end                                           
    2'b01:
        begin                                
            AN = 8'b11111011;    
            case (lowerdigitM)         //display the minutes lower digit       
                0: CX = 8'b00000010;        
                1: CX = 8'b10011110;        
                2: CX = 8'b00100100;
                3: CX = 8'b00001100;
                4: CX = 8'b10011000;
                5: CX = 8'b01001000;
                6: CX = 8'b01000000;
                7: CX = 8'b00011110;
                8: CX = 8'b00000000;
                9: CX = 8'b00001000; 
            endcase
        end        
    2'b10:
        begin                                
            AN = 8'b11111101;    
            case (upperdigitS)         //display the seconds upper digit       
                0: CX = 8'b00000011;        
                1: CX = 8'b10011111;        
                2: CX = 8'b00100101;
                3: CX = 8'b00001101;
                4: CX = 8'b10011001;
                5: CX = 8'b01001001;
                6: CX = 8'b01000001;
                7: CX = 8'b00011111;
                8: CX = 8'b00000001;
                9: CX = 8'b00001001;   
            endcase
        end             
    2'b11:
        begin                                
            AN = 8'b11111110;    
            case (lowerdigitS)        //display the seconds lower digit       
                0: CX = 8'b00000011;        
                1: CX = 8'b10011111;        
                2: CX = 8'b00100101;
                3: CX = 8'b00001101;
                4: CX = 8'b10011001;
                5: CX = 8'b01001001;
                6: CX = 8'b01000001;
                7: CX = 8'b00011111;
                8: CX = 8'b00000001; 
                9: CX = 8'b00001001;  
             endcase
        end                      
    endcase
end
endmodule 

