`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
//////////////////////////////////////////////////////////////////////////////////


module mod_m_counter
    (
        input logic clk, reset,
        input logic [3:0] M,
        //output logic [3:0] q,
        output logic max_tick
    );

    // signal declaration
    logic [10:0] r_next, r_reg;                                // 11 bits can represent 2^11 different values (0 to 2047)

    // body
    // [1] Register segment
    always_ff @(posedge clk, posedge reset)
    begin
        if(reset)
            r_reg <= 0;
        else
            r_reg <= r_next;
    end

    // [2] next-state logic segment
    assign r_next = (r_reg == ((M*10) - 1))? 0: r_reg + 1;     // if m = 4, (if r_reg == 40ns) r_next = 0, if not, keep counting.

    // [3] output logic segment
    //assign q = r_reg;

    assign max_tick = (r_reg == (M*10) - 1) ? 1'b1: 1'b0;

endmodule

