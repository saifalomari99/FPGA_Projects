`timescale 1ns / 1ps

// --------------------------------------
//
//          Sound Generator
//          Saif Alomari 
//          Spring 2023
//
// --------------------------------------



//================================================== main up module ==================================================
module SongPlayer( input clock, input reset, input playSound,     output reg audioOut, output wire aud_sd);

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
    
    MusicSheet mysong(number, notePeriod, duration ); 
    
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
module MusicSheet( input [9:0] number,       output reg [19:0] note, output reg [4:0] duration);

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
        case(number)                                     //Twinkle Twinkle Little Star
            0:  begin note = C5; duration = HALF; end    //twin
            1:  begin note = C5; duration = HALF; end    //kle
            2:  begin note = G5; duration = HALF; end    //twin
            3:  begin note = G5; duration = HALF; end    //kle
            4:  begin note = A4; duration = HALF; end    //lit
            5:  begin note = A4; duration = HALF; end    //tle
            6:  begin note = G5; duration = ONE; end     //star
            7:  begin note = SP; duration = HALF; end    //-----
            8:  begin note = F5; duration = HALF; end    //how
            9:  begin note = F5; duration = HALF; end    //I
            10: begin note = E5; duration = HALF; end    //won
            11: begin note = E5; duration = HALF; end    //der
            12: begin note = D5; duration = HALF; end    //what
            13: begin note = D5; duration = HALF; end    //you
            14: begin note = C5; duration = ONE; end     //are
            15: begin note = SP; duration = HALF; end    //-----
            16: begin note = G5; duration = HALF; end    //up
            17: begin note = G5; duration = HALF; end    //a
            18: begin note = F5; duration = HALF; end    //bove
            19: begin note = F5; duration = HALF; end    //the
            20: begin note = E5; duration = HALF; end    //world
            21: begin note = E5; duration = HALF; end    //so
            22: begin note = D5; duration = ONE; end     //high
            default: begin note = C5; duration = FOUR; end
        endcase
    end
endmodule



