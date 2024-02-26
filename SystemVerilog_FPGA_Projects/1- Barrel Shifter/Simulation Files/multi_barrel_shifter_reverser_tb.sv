`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
//////////////////////////////////////////////////////////////////////////////////


module multi_barrel_shifter_reverser_tb();
localparam N =3;

logic [(2**N)-1:0] a;
logic [N-1:0] amt;
logic [(2**N)-1:0] y;
logic lr;

multi_barrel_shifter_reverser #(.N(N)) uut0(.*);

initial
begin
#120 $finish;
end

initial
begin
    a=8'b01010110;
    amt=3'b011;
    lr = 0;
    #10;
    
    a=8'b11110000;
    amt=3'b001;
    lr = 0;
    #10;
    
    a=8'b11111000;
    amt=3'b010;
    lr = 0;
    #10;
    
    a=8'b01110101;
    amt=3'b100;
    lr = 0;
    #10;
    
    /////////////////////////////////////////
    
    a=8'b01010110;
    amt=3'b011;
    lr = 1;
    #10;
    
    a=8'b11110000;
    amt=3'b001;
    lr = 1;
    #10;
    
    a=8'b11111000;
    amt=3'b010;
    lr = 1;
    #10;
    
    a=8'b01110101;
    amt=3'b100;
    lr = 1;
    #10;
    
end

endmodule



