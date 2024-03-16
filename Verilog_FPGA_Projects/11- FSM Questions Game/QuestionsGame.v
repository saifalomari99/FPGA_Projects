`timescale 1ns / 1ps

//////////////////////////////////////////////////////////////////////////////////
//                                                                              //
//                   Project Name: Questions Game                               //
//                   Engineer: SAIF ALOMARI                                     //
//                                                                              // 
//////////////////////////////////////////////////////////////////////////////////


//=================================== Main up Module ======================================
module QuestionsGame(clk, reset, readyBtn, answer,resetSound,             done , AN, CX, audioOut, aud_sd);
input clk;
wire slow_clock;
wire fast_clock;

input reset;

wire [3:0] state_reg;
output done;

input readyBtn;
wire readyBtndb;

input [3:0] answer;                                                   //the input answer by the player

//------------------ clocks generators
slowerClkGen1Hz   (clk,     slow_clock);
slowerClkGen400Hz (clk,     fast_clock);

//------------------- click of a button modules using FSM
db_fsm            (clk, 1'b0,    readyBtn ,     readyBtndb);
edge_detect_moore (clk, 1'b0,    readyBtndb,    readyfinal); 


//------------------- The game levels control system
statesFSM (clk, reset, readyfinal, answer,           state_reg, done  );


//-------------------- 7 segements displays control system
wire [2:0] Q;
output [7:0]AN;
output [7:0]CX;
counter3bit (1'b1, fast_clock,1'b1    ,Q);
mux8to1     (Q, state_reg,            AN, CX);

//------------------- Sound Generators
reg playSoundGood;
//reg playSoundBad;
input resetSound;
output audioOut, aud_sd;
SongPlayerGood( clk, resetSound, playSoundGood,     audioOut, aud_sd);
//SongPlayerBad ( clk, resetSound, playSoundBad,      audioOut, aud_sd);

always @*
    begin
        if (state_reg == 4'b1000)
            playSoundGood <= 1;
        else
            playSoundGood  <= 0;       

    end//end always

endmodule
//============================= end up module ==========================================








//================================ Update Levels =======================================
module statesFSM (clock, reset, ready, answer,           state_reg, done  );

input clock, reset, ready;
input [3:0] answer;                    //4-bit input answer

output reg [3:0] state_reg;
output reg done;



//---- states declartion
reg [3:0] state_next;
localparam [3:0] s1= 4'b0001, 
                 s2= 4'b0010, 
                 s3= 4'b0011,
                 s4= 4'b0100,
                 s5= 4'b0101,
                 s6= 4'b0110,
                 s7= 4'b0111,
                 s8= 4'b1000;
                 

//---- registers declarations
//reg [2:0] bitCount_next;
//reg [3:0] registerA, registerA_next;
reg done_next;



//------------------------ first always block: update the regesters
always @(posedge clock)
       if (reset)
          begin
             state_reg <= s1;
             done      <= 0;
          end
       else
          begin
             state_reg <= state_next;
             done      <= done_next;
          end
          
          
//------------------------ second always block: the states          
always @*
begin
    state_next = state_reg; 
    done_next=done;   
    
     
    //-------------- states ---------------------------------
    case (state_reg)
        s1:                         //<------------------------------------- s1 (Start)
            begin         
            done_next <= 0;            
            if (ready == 1)                       //enter
                begin
                state_next=s2;               
                end  
            end 
        s2:                         //<------------------------------------- s2 (Question 1)
            begin        
            if (ready == 1)
                begin
                if (answer == 6)
                    state_next=s3;  
                else
                    state_next = s7;    //game over    
                end 
            end          
        s3:                       //<------------------------------------- s3 (Question 2)
            begin
            //done_next=1;                         
            if (ready == 1)
                begin
                if (answer == 10) //4+6
                    state_next=s4;  
                else
                    state_next = s7;   //game over    
                end
            end//end state s3
        s4:                       //<------------------------------------- s3 (Question 3)
            begin
            //done_next=1;                         
            if (ready == 1)
                begin
                if (answer == 14) //5+9
                    state_next= s5;  
                else
                    state_next = s7;   //game over    
                end
            end//end state s4           
        s5:                       //<------------------------------------- s3 (Question 4)
            begin
            //done_next=1;                         
            if (ready == 1)
                begin
                if (answer == 15) //8+7
                    state_next= s6;  
                else
                    state_next = s7;   //game over    
                end
            end//end state s5 
        s6:                       //<------------------------------------- s3 (Question 5)
            begin
            //done_next=1;                         
            if (ready == 1)
                begin
                if (answer == 8) //2+6
                    state_next= s8;    //Winning
                else
                    state_next = s7;   //game over    
                end
            end//end state s6
        
        s7:                       //<------------------------------------- s3 (Game Over)
            begin
            //done_next=1;                         
            if (ready == 1)                       //enter
                state_next= s1;    //start menu       
            end    
             
        s8:                       //<------------------------------------- s3 (Winning)
            begin
            done_next = 1;                         
            if (ready == 1)                       //enter
                state_next=s1;    //go back to menu       
            end   
   
        default:
            begin
            state_next = s1;      //start menu   
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

//================================ 400 Hz fast Clk Genertor ==============================
module slowerClkGen400Hz(clk,     outsignal);

input clk;
output reg outsignal;                //the outsignle that is going to the 3-bit counter
reg [26:0] counter; 

always @ (posedge clk)
 begin
    counter = counter +1;
    if (counter == 125_000)        //fast clock
      begin
      outsignal=~outsignal;
      counter =0;
      end
 end
endmodule








//=============================== filter debouncing Module ================================
module db_fsm ( input wire clk, reset, input wire sw,         output reg db );

    // symbolic state declaration 
    localparam [2:0] zero    = 3'b000, 
                     wait1_1 = 3'b001, 
                     waitl_2 = 3'b010, 
                     waitl_3 = 3'b011, 
                     one     = 3'b100, 
                     wait0_1 = 3'b101,                       
                     wait0_2 = 3'b110,
                     wait0_3 = 3'b111;
                                        
    // number of counter bits (2"N * 2Ons = lOms tick) 
    localparam N = 19; 
    
    // signal declaration 
    reg  [N-1 : 0] q_reg; 
    wire [N-1 : 0] q_next ; 
    wire m_tick; 
    reg [2:0] state_reg , state_next ; 
    
    // body 
    
    
    //............................................... 
    // counter to generate 10 ms tick 
    //............................................... 
    always @ (posedge clk) 
        q_reg <= q_next;
        
        
            
    //next-state logic 
    assign q_next = q_reg + 1; 
    // output tick 
    assign m_tick = (q_reg == 0) ? 1'b1 : 1'b0; 
        
    
        
    //............................................... 
    // debouncing FSM 
    //............................................... 
    // state register 
    always @ ( posedge clk , posedge reset) 
        if (reset) 
            state_reg <= zero; 
        else 
            state_reg <= state_next ; 
    
    
    
        
    // next-state logic and output logic 
    always @* 
        begin 
        state_next = state_reg;         // default state: the same 
        db = 1'b0;                      // default output: 0 
        
        case (state_reg) 
            zero: 
                //begin 
                //db = 1'b0;            
                if (sw) 
                    state_next = wait1_1 ; 
                //end
            wait1_1: 
                if (~sw) 
                    state_next = zero; 
                else 
                    if (m_tick) 
                        state_next = waitl_2 ; 
            waitl_2: 
                if (~sw) 
                    state_next = zero; 
                else 
                    if (m_tick) 
                        state_next = waitl_3; 
            waitl_3: 
                if (~sw) 
                    state_next = zero; 
                else 
                    if (m_tick) 
                        state_next = one; 
            one: 
                begin 
                db = 1'b1; 
                if (~sw) 
                    state_next = wait0_1; 
                end 
            wait0_1: 
                begin 
                db = 1'b1; 
                if (sw) 
                    state_next = one; 
                else 
                    if (m_tick) 
                        state_next = wait0_2; 
                end 
            wait0_2:
                begin 
                db = 1'b1; 
                if (sw) 
                    state_next = one; 
                else 
                    if (m_tick) 
                        state_next = wait0_3; 
                end 
            wait0_3: 
                begin 
                db = 1'b1; 
                if (sw) 
                    state_next = one; 
                else 
                    if (m_tick) 
                        state_next = zero; 
                end 
            default : 
                state_next = zero; 
        endcase 
        
    end//end always 
    
    
endmodule


//============================= moore detect =============================================
module edge_detect_moore (input wire clk, reset, level, output reg tick ); 
    
    localparam [1:0] zero=2'b00, edg=2'b01, one=2'b10;
    reg [1:0] state_reg, state_next;
    
    //slowerClkGen1Hz    clk1Hz   (clk, reset1HzclkSW, clk1HzOutsignal );      //slow
    
    always @(posedge clk, posedge reset)
        if (reset)
            state_reg<=zero;
        else
            state_reg<=state_next;
 
    always@*
        begin
        state_next=state_reg;
        tick=1'b0; //default output
        case (state_reg)
            zero:
                begin
                tick=1'b0;
                if (level)
                    state_next=edg;
                end
            edg:
                begin
                tick=1'b1;
                if (level)
                    state_next=one;
                else
                    state_next=zero;
                end
            one: 
                if (~level)
                    state_next=zero;
            default: state_next=zero;
        endcase
        
        end//end always
        
endmodule



//=================================== Counter (selectors) ===========================================
module counter3bit (Resetn, Clock, E, Q);
input Resetn, Clock, E;
output reg [2:0] Q;                     

always @(negedge Resetn, posedge Clock)
if (!Resetn)
Q <= 0;
else if (E)
Q <= Q + 1;

endmodule




//==================================== 8 to 1 multiplexer module ======================================
module mux8to1 (select,state_reg,            AN, CX);
input [2:0] select;                                         //the two selectors
input [3:0] state_reg;


output reg [7:0]AN;                                       //the different 8 7-segment-displays
output reg [7:0]CX;                                       //the 7-segments

always @ (select, state_reg )
begin
    case(select)                       //case statement to run through 4 digits and display them all together really fast
    3'b000:  
        begin                                                                     
            AN = 8'b01111111;    
            case (state_reg)                               
                1: CX = 8'b01001001;        //S        start              0001
                2: CX = 8'b11100011;        //L        L1  3+3=
                3: CX = 8'b11100011;        //L        L2  4+6=
                4: CX = 8'b11100011;        //L        L3  5+9=
                5: CX = 8'b11100011;        //L        L4  8+7=
                6: CX = 8'b11100011;        //L        L5  2+6=
                7: CX = 8'b01000011;        //G        GAME OVER
                8: CX = 8'b01110001;        //F        FInISH       
                //9: CX = 8'b00000001;
                //9: CX = 8'b00001001; 
            endcase
        end                                           
    3'b001:
        begin                                
            AN = 8'b10111111;    
            case (state_reg)               
                1: CX = 8'b11100001;        //t        start   
                2: CX = 8'b10011111;        //1        L1  3+3=
                3: CX = 8'b00100101;        //2        L2  4+6=
                4: CX = 8'b00001101;        //3        L3  5+9=
                5: CX = 8'b10011001;        //4        L4  8+7=
                6: CX = 8'b01001001;        //5        L5  2+6=
                7: CX = 8'b00010001;        //A        GAME OVER
                8: CX = 8'b10011111;        //I        FInISH 
            endcase
        end        
    3'b010:
        begin                                
            AN = 8'b11011111;    
            case (state_reg)           
                1: CX = 8'b00010001;        //A        start  
                2: CX = 8'b11111111;        //         L1  3+3=
                3: CX = 8'b11111111;        //         L2  4+6=
                4: CX = 8'b11111111;        //         L3  5+9=
                5: CX = 8'b11111111;        //         L4  8+7=
                6: CX = 8'b11111111;        //         L5  2+6=
                7: CX = 8'b11010101;        //n        GAME OVER
                8: CX = 8'b11010101;        //n        FInISH 
            endcase
        end             
    3'b011:
        begin                                
            AN = 8'b11101111;    
            case (state_reg)              
                1: CX = 8'b11110101;        //r          start
                2: CX = 8'b11111111;        //           L1  3+3=
                3: CX = 8'b11111111;        //           L2  4+6=
                4: CX = 8'b11111111;        //           L3  5+9=
                5: CX = 8'b11111111;        //           L4  8+7=
                6: CX = 8'b11111111;        //           L5  2+6=
                7: CX = 8'b01100001;        //E          GAME OVER
                8: CX = 8'b10011111;        //I          FInISH  
             endcase
        end
     3'b100:
        begin                                
            AN = 8'b11110111;    
            case (state_reg)                 
                1: CX = 8'b11100001;        //t          start
                2: CX = 8'b00001101;        //3          L1  3+3=
                3: CX = 8'b10011001;        //4          L2  4+6=
                4: CX = 8'b01001001;        //5          L3  5+9=
                5: CX = 8'b00000001;        //8          L4  8+7=
                6: CX = 8'b00100101;        //2          L5  2+6=
                7: CX = 8'b00000011;        //O          GAME OVER
                8: CX = 8'b01001001;        //S          FInISH   
             endcase
        end 
     3'b101:
        begin                                
            AN = 8'b11111011;    
            case (state_reg)              
                1: CX = 8'b11111111;        //           start
                2: CX = 8'b11000111;        //u +        L1  3+3=
                3: CX = 8'b11000111;        //u          L2  4+6=
                4: CX = 8'b11000111;        //u          L3  5+9=
                5: CX = 8'b11000111;        //u          L4  8+7=
                6: CX = 8'b11000111;        //u          L5  2+6=
                7: CX = 8'b10000011;        //V          GAME OVER
                8: CX = 8'b10010001;        //H          FInISH  
             endcase
        end 
      3'b110:
        begin                                
            AN = 8'b11111101;    
            case (state_reg)              
                1: CX = 8'b11111111;        //            start
                2: CX = 8'b00001101;        //3           L1  3+3=
                3: CX = 8'b01000001;        //6           L2  4+6=
                4: CX = 8'b00001001;        //9           L3  5+9=
                5: CX = 8'b00011111;        //7           L4  8+7=
                6: CX = 8'b01000001;        //6           L5  2+6=
                7: CX = 8'b01100001;        //E           GAME OVER
                8: CX = 8'b11111111;        //            FInISH  
             endcase
        end 
      3'b111:
        begin                                
            AN = 8'b11111110;    
            case (state_reg)                       
                1: CX = 8'b11111111;        //              start      
                2: CX = 8'b11101101;        //=             L1  3+3=
                3: CX = 8'b11101101;        //=             L2  4+6=
                4: CX = 8'b11101101;        //=             L3  5+9=
                5: CX = 8'b11101101;        //=             L4  8+7=
                6: CX = 8'b11101101;        //=             L5  2+6=
                7: CX = 8'b11110101;        //r             GAME OVER
                8: CX = 8'b11111111;        //              FInISH    
             endcase
        end                                                                                                           
    endcase//end the big case of the selectors
end
endmodule 









//================================================== Song Player Module ==================================================
module SongPlayerGood( input clock, input reset, input playSound,     output reg audioOut, output wire aud_sd);

    reg [19:0] counter;                               //Counter: counts the number of clock periods in one musical note.
    reg [31:0] time1;                                 //Counts the total number of clock periods that have passed.
    reg [31:0] noteTime;                              //Stores the number of FPGA clock periods in one note.
    reg [9:0] msec;                                   //millisecond counter, and sequence number of musical note.
    reg [9:0] number;                                 //Stores the sequence number of the current note being played.
    
    wire [4:0] note;                            
    wire [4:0] duration;                        
    wire [19:0] notePeriod;                           //Stores the number of clock periods in one note.
    parameter clockFrequency = 100_000_000;           //Set the clock frequency to 100 MHz.
    
    assign aud_sd = 1'b1;                             //Set to 1, and is used to synchronize with an external device.
    
    MusicSheetGood mysong(number, notePeriod, duration ); 
    
    always @ (posedge clock) 
        begin
        if(reset | ~playSound)                        //if reset switch is 
            begin 
            counter  <=0; 
            time1    <=0; 
            number   <=0; 
            audioOut <=1;
            end
        else                                          //start the project
            begin
            counter <= counter + 1;                   
            time1<= time1+1;                          
            if( counter >= notePeriod)               
                begin
                counter <=0;
                audioOut <= ~audioOut;                //toggle audio output 
                end                                   
            if( time1 >= noteTime)                                                    
                begin
                time1 <=0;
                number <= number + 1;                 //play next note
                end               
            if(number == 48) number <=0;              //Make the number reset at the end of the song
            end
        end
 
    always @(duration) 
        noteTime = duration * clockFrequency / 8;     //Number of FPGA clock periods in one note.
        
endmodule 
//======================================= end =====================================================

//=================================== MusicSheet Module ===========================================
module MusicSheetGood( input [9:0] number,   output reg [19:0] note, output reg [4:0] duration);

    //what is the max frequency
    parameter QUARTER = 5'b00010;                        // 2
    parameter HALF = 5'b00100;                           // 4
    parameter ONE = 2* HALF;                             // 8
    parameter TWO = 2* ONE;                              //16
    parameter FOUR = 2* TWO;                             //32
    
    parameter A4= 22727, B4= 20242, C5= 19111, D5= 17026, E5= 15168, F5= 14318, G5= 12755, SP = 1; 
                                                         //
    always @ (number) 
        begin
        case(number)                                     //Hype song
        0:  begin note = G5; duration = HALF; end    //dun
        1:  begin note = G5; duration = HALF; end    //dun
        2:  begin note = G5; duration = HALF; end    //dun
        3:  begin note = G5; duration = HALF; end    //dun
        4:  begin note = G5; duration = HALF; end    //dun
        5:  begin note = F5; duration = HALF; end    //dun
        6:  begin note = G5; duration = HALF; end    //dun
        7:  begin note = G5; duration = HALF; end    //dun
        8:  begin note = A4; duration = ONE; end     //hooray!
        9:  begin note = SP; duration = HALF; end    //-----
        10: begin note = G5; duration = HALF; end    //go
        11: begin note = G5; duration = HALF; end    //go
        12: begin note = G5; duration = HALF; end    //go
        13: begin note = G5; duration = HALF; end    //go
        14: begin note = F5; duration = HALF; end    //go
        15: begin note = G5; duration = HALF; end    //go
        16: begin note = G5; duration = HALF; end    //go
        17: begin note = A4; duration = ONE; end     //woo-hoo!
        18: begin note = SP; duration = HALF; end    //-----
        19: begin note = G5; duration = HALF; end    //yeah
        20: begin note = G5; duration = HALF; end    //yeah
        21: begin note = G5; duration = HALF; end    //yeah
        22: begin note = G5; duration = HALF; end    //yeah
        23: begin note = F5; duration = HALF; end    //yeah
        24: begin note = G5; duration = HALF; end    //yeah
        25: begin note = G5; duration = HALF; end    //yeah
        26: begin note = A4; duration = ONE; end     //let's celebrate!
        default: begin note = C5; duration = FOUR; end
        endcase
    end
endmodule







//================================================== Song Player Module ==================================================
//module SongPlayerBad( input clock, input reset, input playSound,     output reg audioOut, output wire aud_sd);

//    reg [19:0] counter;                               //Counter: counts the number of clock periods in one musical note.
//    reg [31:0] time1;                                 //Counts the total number of clock periods that have passed.
//    reg [31:0] noteTime;                              //Stores the number of FPGA clock periods in one note.
//    reg [9:0] msec;                                   //millisecond counter, and sequence number of musical note.
//    reg [9:0] number;                                 //Stores the sequence number of the current note being played.
    
//    wire [4:0] note;                            
//    wire [4:0] duration;                        
//    wire [19:0] notePeriod;                           //Stores the number of clock periods in one note.
//    parameter clockFrequency = 100_000_000;           //Set the clock frequency to 100 MHz.
    
//    assign aud_sd = 1'b1;                             //Set to 1, and is used to synchronize with an external device.
    
//    MusicSheetBad mysong(number, notePeriod, duration ); 
    
//    always @ (posedge clock) 
//        begin
//        if(reset | ~playSound)                        //if reset switch is 
//            begin 
//            counter  <=0; 
//            time1    <=0; 
//            number   <=0; 
//            audioOut <=1;
//            end
//        else                                          //start the project
//            begin
//            counter <= counter + 1;                   
//            time1<= time1+1;                          
//            if( counter >= notePeriod)               
//                begin
//                counter <=0;
//                audioOut <= ~audioOut;                //toggle audio output 
//                end                                   
//            if( time1 >= noteTime)                                                    
//                begin
//                time1 <=0;
//                number <= number + 1;                 //play next note
//                end               
//            if(number == 48) number <=0;              //Make the number reset at the end of the song
//            end
//        end
 
//    always @(duration) 
//        noteTime = duration * clockFrequency / 8;     //Number of FPGA clock periods in one note.
        
//endmodule 
////======================================= end =====================================================

////=================================== MusicSheet Module ===========================================
//module MusicSheetBad( input [9:0] number,   output reg [19:0] note, output reg [4:0] duration);

//    //what is the max frequency
//    parameter QUARTER = 5'b00010;                        // 2
//    parameter HALF = 5'b00100;                           // 4
//    parameter ONE = 2* HALF;                             // 8
//    parameter TWO = 2* ONE;                              //16
//    parameter FOUR = 2* TWO;                             //32
    
//    parameter A4= 22727, B4= 20242, C5= 19111, D5= 17026, E5= 15168, F5= 14318, G5= 12755, SP = 1; 
//                                                         //
//    always @ (number) 
//        begin
//        case(number)                                     //Sad Melody
//            0:  begin note = C5; duration = HALF; end    //I
//            1:  begin note = C5; duration = HALF; end    //lost
//            2:  begin note = D5; duration = HALF; end    //the
//            3:  begin note = D5; duration = HALF; end    //game
//            4:  begin note = E5; duration = HALF; end    //it's
//            5:  begin note = E5; duration = HALF; end    //not
//            6:  begin note = F5; duration = HALF; end    //fair
//            7:  begin note = SP; duration = HALF; end    //-----
//            8:  begin note = G5; duration = HALF; end    //I
//            9:  begin note = G5; duration = HALF; end    //tried
//            10: begin note = A4; duration = HALF; end    //so
//            11: begin note = A4; duration = HALF; end    //hard
//            12: begin note = B4; duration = HALF; end    //but
//            13: begin note = B4; duration = HALF; end    //failed
//            14: begin note = C5; duration = ONE; end     //anyway
//            15: begin note = SP; duration = HALF; end    //-----
//            16: begin note = F5; duration = HALF; end    //I'll
//            17: begin note = F5; duration = HALF; end    //try
//            18: begin note = E5; duration = HALF; end    //again
//            19: begin note = E5; duration = HALF; end    //someday
//            20: begin note = D5; duration = ONE; end     //maybe
//            default: begin note = C5; duration = FOUR; end
//        endcase
//    end
//endmodule






