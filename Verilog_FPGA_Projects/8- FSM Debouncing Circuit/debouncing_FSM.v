`timescale 1ns / 1ps

    /////////////////////////////////////////////////////////////////
    //       Project: The Debouncing Circuit                       //
    //       Engineer: Saif Alomari                                // 
    //       Spring 2023                                           //                        
    ///////////////////////////////////////////////////////////////// 
      

//==================================== main up module ===================================
module lab_10 (clk, reset, buttonUP, buttonDOWN,        dbUP, dbDOWN, _edgeUP,  _edgeDOWN, Q);
input clk, reset, buttonUP, buttonDOWN;
output dbUP, dbDOWN;
output _edgeUP,  _edgeDOWN;
output reg [3:0] Q;

//---------------------- filter the debounsing of the buttons
db_fsm   stage0 (clk, reset, buttonUP,         dbUP);
db_fsm   stage1 (clk, reset, buttonDOWN,       dbDOWN);

//---------------------- getting only the edge change from (0 to 1) of the buttons
edge_detect_moore (clk, reset, dbUP ,          _edgeUP );
edge_detect_moore (clk, reset, dbDOWN ,        _edgeDOWN );

//---------------------- Up and Down Counter 
always @ (posedge clk)
    begin 
    if (_edgeUP)
        Q <= Q + 1;           
    else if (_edgeDOWN)
        Q <= Q - 1;  
        
    end//end always


endmodule 
//================================ end up module =========================================


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
    