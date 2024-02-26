`timescale 1ns / 1ps


module square_wave_generator_tb();

// ----- [1] Signal declration
    logic clk, reset;
    logic [3:0] m;               // Control signal for (on) intervals       
    logic [3:0] n;               // Control signal for (off) intervals

    logic sq_wave;

// ----- [2] instantiate unit under test (uut)
square_wave_generator uut0(.clk(clk), .reset(reset), .up(m), .down(n), .sq_wave(sq_wave));

// ----- [3] test vectors 
initial 
begin
    m = 1;
    n = 1;
    #500;
    
    m = 4;
    #1500
    
    m = 2;
    n = 3;
    #500;
 
end



//clock generator (10 ns clock)
always
begin
    clk = 1'b1;
    #5;
    clk = 1'b0;
    #5;
end

//initial reset
initial
begin
    reset = 1'b1;
    @(negedge clk)
    reset = 1'b0;
end

endmodule