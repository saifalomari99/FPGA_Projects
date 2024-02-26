`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
//////////////////////////////////////////////////////////////////////////////////




//========================================== multi barrel shifter (N-bit) =================================================
module multi_barrel_shifter_mux
    #(parameter N = 3)
    (
    input logic [(2**N)-1:0] a,
    input logic [N-1:0] amt,
    input logic lr, 
    output logic [(2**N)-1:0] y
    );

    logic [(2**N)-1:0] y_right, y_left; //holds left and right shift values

    
    param_right_shifter #(.N(N)) rotate_right ( //rotate right
        .a(a),
        .amt(amt),
        .y(y_right)
    );

    
    param_left_shifter #(.N(N)) rotate_left ( //rotate left
        .a(a),
        .amt(amt),
        .y(y_left)
    );

    
    always_comb begin //mux for left or right shift
        y = lr ? y_left : y_right;
    end

endmodule