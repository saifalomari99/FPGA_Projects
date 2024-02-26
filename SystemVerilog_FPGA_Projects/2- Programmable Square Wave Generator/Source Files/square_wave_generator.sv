`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
//
// Company: 
// Engineer: 
// 
//////////////////////////////////////////////////////////////////////////////////


module square_wave_generator(
 input logic clk,reset,
    input logic [3:0] up,
    input logic [3:0] down,
    output logic sq_wave
    );

    logic [3:0] ud_sel;
    logic r_next, r_reg;

    //counter
     mod_m_counter counter(
        .clk(clk),
        .reset(reset),
        .M(ud_sel),
        .max_tick(r_next)
     );

   //T FF
    always_ff@(posedge clk, posedge reset)
    begin
        if(reset)
            r_reg<=0;
        else
            if(r_next)
                r_reg <= ~r_reg;
            else
                r_reg <= r_reg;
    end

    //mux
    always_comb
    begin
        if(r_reg)
            ud_sel=up;
        else
            ud_sel=down;
    end
    
    
    assign sq_wave = r_reg;

endmodule


