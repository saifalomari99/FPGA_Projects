`timescale 1ns / 1ps
///////////////////////////////////////////////////////////////////////////////
//
//             Topic: Counting how many 1's in a binary number
//                        Engineer: SAIF ALOMARI
// 
//////////////////////////////////////////////////////////////////////////////

//================================== Bit Count Module =======================================
module bit_Count(clk, reset, command, dataA,            bitCount, state_reg, done);
input clk, reset, command;
input [3:0] dataA;                   //4-bit input data

output reg [2:0] bitCount;
output reg [1:0] state_reg;
output reg done;



//---- states declartion
reg [1:0] state_next;
localparam [1:0] s1=2'b01, s2=2'b10, s3=2'b11;

//---- registers declarations
reg [2:0] bitCount_next;
reg [3:0] registerA, registerA_next;
reg done_next;

//---- slow clock 
wire clk1Hzoutsignal;
slowerClkGen1Hz (clk, clk1Hzoutsignal); 



//------------------------ first always block: update the regesters
always @(posedge clk1Hzoutsignal)
       if (reset)
          begin
             state_reg <= s1;
             registerA <= 0;
             bitCount  <= 0;
             done      <= 0;
          end
       else
          begin
             state_reg <= state_next;
             registerA <= registerA_next;
             bitCount  <= bitCount_next;
             done      <= done_next;
          end
          
          
//------------------------ second always block: the states          
always @*
begin
    state_next = state_reg; 
    registerA_next=registerA;
    done_next=done;
    
    //-------------- states ---------------------------------
    case (state_reg)
        s1:              //<------------------------------ s1
            begin
            bitCount_next <= 0;
            done_next <= 0;
            if (command==1)
                begin
                state_next=s2;               
                end
            else
                begin
                registerA_next = dataA;
                end
            end 
        s2:              //<------------------------------ s2
            begin
            if (registerA == 0)
                state_next=s3;
            else
                begin
                if (registerA_next[0]==1)
                    begin
                    bitCount_next <= bitCount + 1;         //add 1 to the count
                    end
                else 
                    begin
                    bitCount_next <= bitCount;             //dont count zeros
                    end
                end
                registerA_next = registerA >> 1;           //shift right
            end
            
        s3:              //<------------------------------ s3
            begin
            done_next=1;
            bitCount_next = bitCount;              
            if (command==0)
                state_next=s1;
            end
    
        default:
            begin
            state_next = s1;        
            done_next=0;
            end
    endcase
    
end//end always

endmodule



//================================ 1 Hz slow Clk Genertor ==============================
module slowerClkGen1Hz(clk,  outsignal);

input clk;
output reg outsignal;                            //the outsignle that is going to the 3-bit counter
reg [26:0] counter; 

always @ (posedge clk)
    begin
    counter = counter +1;
    if (counter == 100_000_000)                   //give an up signal every half a second
      begin
      outsignal=~outsignal;
      counter =0;
      end
    end
endmodule


