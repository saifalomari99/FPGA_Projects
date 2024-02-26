`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
//////////////////////////////////////////////////////////////////////////////////


module reverser #(parameter N = 3) 
(
    input logic [2**N-1:0] org,
    output logic [2**N-1:0] rev
);

    genvar i;
    generate
        for (i = 0; i < 2**N; i++)
            begin 
            assign rev[i] = org[2**N-1-i]; //goes in order putting msb to lsb until i> 2^N
            end
    endgenerate

endmodule
