`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
//////////////////////////////////////////////////////////////////////////////////


module param_right_shifter #(parameter N = 4) (
    input logic [(2**N)-1:0] a,
    input logic [N-1:0] amt,
    output logic [(2**N)-1:0] y
);

    integer i;
    always_comb begin
        y = a; 
        for (i = 0; i < amt; i = i + 1) begin
            y = {y[0], y[(2**N)-1:1]}; //gets lsb and puts it in the front. gets the rest of the bits and puts them in the back. repeat until i > amt
        end
    end
endmodule




