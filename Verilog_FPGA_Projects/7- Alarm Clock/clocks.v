`timescale 1ns / 1ps




//================================ 1 Hz slow Clk Genertor ==============================
module slowerClkGen1Hz(clk, resetSW, outsignal);

input clk;
input resetSW;             // you connect this to a switch to manully reset the clock generator
output outsignal;          //the outsignle that is going to the 3-bit counter
 
reg [26:0] counter; 
reg outsignal;

always @ (posedge clk)
 begin
  if (resetSW)            //if you click the reset switch, it resets the counter and the outsignal
   begin
    counter=0;
    outsignal=0;
   end
  
  else                                 //if the reset switch is off:
   begin
    counter = counter +1;
    if (counter == 50_000_000)        //give an up signal every half a second
     begin
      outsignal=~outsignal;
      counter =0;
    end
  end
end
endmodule

//================================ 10 Hz Human speed Clk Genertor ==============================
module slowerClkGen10Hz(clk, resetSW, outsignal);

input clk;
input resetSW;                   // you connect this to a switch to manully reset the clock generator
output outsignal;                //the outsignle that is going to the 3-bit counter
 
reg [26:0] counter; 
reg outsignal;

always @ (posedge clk)
 begin
  if (resetSW)    //if you click the reset switch, it resets the counter and the outsignal
   begin
    counter=0;
    outsignal=0;
   end
  
  else                                 //if the reset switch is off:
   begin
    counter = counter +1;
    if (counter == 5_000_000)        //10 Hz clock
     begin
      outsignal=~outsignal;
      counter =0;
    end
  end
end
endmodule


//================================ 400 Hz fast Clk Genertor ==============================
module slowerClkGen400Hz (clk, resetSW, outsignal);

input clk;
input resetSW;                   // you connect this to a switch to manully reset the clock generator
output outsignal;                //the outsignle that is going to the 3-bit counter
 
reg [26:0] counter; 
reg outsignal;

always @ (posedge clk)
 begin
  if (resetSW)    //if you click the reset switch, it resets the counter and the outsignal
   begin
    counter=0;
    outsignal=0;
   end
  
  else                                 //if the reset switch is off:
   begin
    counter = counter +1;
    if (counter == 125_000)        //fast clock
     begin
      outsignal=~outsignal;
      counter =0;
    end
  end
end
endmodule




