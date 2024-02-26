`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 

//////////////////////////////////////////////////////////////////////////////////


module param_left_shifter_tb();
localparam N =2;

logic [(2**N)-1:0] a;
logic [N-1:0] amt;
logic [(2**N)-1:0] y;

param_left_shifter #(.N(N)) uut0(.*);

initial
begin
#40 $finish;
end

initial
begin  
    a=4'b1001;
    amt = 2'b01;
    #10;
    
    a=4'b1100;
    amt = 2'b01;
    #10;
    
    
    a=4'b0011;
    amt = 2'b10;
    #10
    
    a=4'b0001;
    amt = 2'b11;
    #10;
    
    
end

endmodule