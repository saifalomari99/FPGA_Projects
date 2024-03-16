`timescale 1ns / 1ps

// --------------------------------------
//
//          7 Segments Display
//          Saif Alomari 
//          Spring 2023
//
// --------------------------------------




//======================== main module ===================
module Lab4(X, AN, CX);

input [2:0] X;
output [7:0] AN;
output [7:0] CX;
wire [7:0] y;

decoder number1(X, y);
seven_segment_control number1cont(y, AN, CX);

endmodule
//============================ end =========================


//------------------------ 3to8 decoder module
module decoder(input [2:0] data, output reg [7:0] Y);
always @(data)
 begin
       Y=0;
       Y[data]=1;
end
endmodule


//-------------------------- seven_segment control module 
module seven_segment_control(input [7:0] y, output [7:0] AN, output reg [7:0] CX);

//turn on only the first 7 segment display
assign AN = 8'b11111110;

//start of behavioral always function:
always @*
case(y)
    8'b00000001:
         CX = 8'b00000010;
    8'b00000010:
         CX = 8'b10011110;
    8'b00000100:
         CX = 8'b00100100;
    8'b00001000:
         CX = 8'b00001100;
    8'b00010000:
         CX = 8'b10011000;
    8'b00100000:
         CX = 8'b01001000;
    8'b01000000:
         CX = 8'b01000000;
    8'b10000000:
         CX = 8'b00011110;
    default:
         CX = 8'b11111110;
endcase

endmodule