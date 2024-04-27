

module square_demo 
   (
    input  logic [10:0] x, y,     // treated as x-/y-axis
    input logic [1:0] side_width, 
    input logic [11:0] square_rgb,
    output logic [11:0] bar_rgb 
   );

   // declaration
   logic [3:0] up, down;
   logic [3:0] r, g, b;
   logic [7:0] width;
   localparam center_x = 320;
   localparam center_y = 240;
   logic r_back, g_back, b_back;
   
   // ------------------------------- body
   
   always_comb
   begin
    unique case (side_width) 
         2'b00: 
         begin
            width = 16;
         end   
         2'b01: 
         begin
            width = 32;      
         end   
         2'b10: 
         begin
            width = 64;
         end   
         2'b11: 
         begin
            width = 128;  
         end   
     endcase
   
    if (y > (center_y - width) && y < (center_y + width) && x > (center_x - width) && x < (center_x + width))                   // This code displays a squre with width 200 in the middle of the screen 
    begin
        r = {square_rgb[3:0]};
        g = {square_rgb[7:4]};
        b = {square_rgb[11:8]};
        //r = 4'b1111;
        //g = 4'b0000;
         //b = 4'b0000;
    end
    else
    begin
//         r_back = 4'b1111 - r;
//         g_back = 4'b1111 - g;
//         b_back = 4'b1111 - b;
        r = 4'b1111 - {square_rgb[3:0]} ;
        g = 4'b1111 - {square_rgb[7:4]};
        b = 4'b1111 - {square_rgb[11:8]} ;
    end
   
  
   end // always   
   
   // output
   assign bar_rgb = {b, g, r};
endmodule