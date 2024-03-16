`timescale 1ns / 1ps

// --------------------------------------
//
//          Counter using 7-Segments Display
//          Saif Alomari 
//          Spring 2023
//
// --------------------------------------




//================================ main up module ======================================
module mainProject ( resetSW, resetSWCo, clk,    outsignal, Q , AN, CX);

input clk;                         //device clk input
input resetSW;                     //clk reset button
input resetSWCo;                   //counter reset button
output outsignal;                  //clk out signal

output   [2:0]Q;                   //3 bit output
 
wire [7:0] y;                      //wire between decoder and 7 segment

output [7:0] AN;                  //8 7-seg-display
output [7:0] CX;                  //7 segements displays



slowerClkGen subClkGen (clk, resetSW, outsignal );     
upcounter subCounter (resetSWCo, outsignal, 1'b1, Q);
decoder subDecoder (Q, y);
seven_segment_control sub7Seg (y, AN , CX);

endmodule 
//=================================== end =======================================







//================================ slower Clk Genertor ==============================
module slowerClkGen(clk, resetSW, outsignal);

 input clk;
 input resetSW;             // you connect this to a switch to manully reset the clock generator
 output outsignal;          //the outsignle that is going to the 3-bit counter
 
reg [26:0] counter; 
reg outsignal;

 always @ (posedge clk)
 begin
 
if (resetSW)    //if you click the reset switch, it resets the counter and the outsignal
 begin
counter=0;
outsignal=0;
 end
 
else
 begin
 counter = counter +1;
 if (counter == 50_000_000)      //give an up signal every half a second
begin
outsignal=~outsignal;
counter =0;
end

end

 end
endmodule


//=================================== Counter ===========================================
module upcounter (Resetn, Clock, E, Q);
input Resetn, Clock, E;
output reg [2:0] Q;                     // connect those to 3 lights

always @(negedge Resetn, posedge Clock)
if (!Resetn)
Q <= 0;
else if (E)
Q <= Q + 1;

endmodule


//=========================== 3to8 decoder module ==================================
module decoder(input [2:0] data, output reg [7:0] Y);
always @(data)
 begin
       Y=0;
       Y[data]=1;
end
endmodule


//============================ seven_segment control module ===========================
module seven_segment_control(input [7:0] y, output [7:0] AN, output reg [7:0] CX);

//turn on only the first 7 segment display
assign AN = 8'b11111110;

//start of behavioral always function:
always @*
case(y)
    8'b00000001:
         CX = 8'b00000011;
    8'b00000010:
         CX = 8'b10011111;
    8'b00000100:
         CX = 8'b00100101;
    8'b00001000:
         CX = 8'b00001101;
    8'b00010000:
         CX = 8'b10011001;
    8'b00100000:
         CX = 8'b01001001;
    8'b01000000:
         CX = 8'b01000001;
    8'b10000000:
         CX = 8'b00011111;
    default:
         CX = 8'b11111111;
endcase

endmodule










