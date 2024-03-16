`timescale 1ns / 1ps

module adder_sim;

reg [3:0] X, Y;
reg carryin;

wire [3:0] S;
wire carryout;

adder4 uut (carryin, X, Y, S, carryout);
initial begin
 X=0; Y=0; carryin =0;
 #1 X=1;
 #1 X=2;
 #1 Y=1;
end

endmodule
