



#include "one_player_game.h"





// ======================================== 1 Player Game ========================================================
void one_player_game(FrameCore *frame_p, OsdCore *osd_p, SpriteCore *player1, Ps2Core *ps2_p, SpriteCore *mouse_p , bool games_restart, XadcCore *adc_p)
{
	char ch;
	osd_p->clr_screen();                         //clear screen of menu text
	frame_p->clr_screen(0x00F);                 //make background frame blue
	int ghost_X = 0;
	bool hit = false;
	int win = false;

	bool clicked;

	bool restart = false;                     //checks if game was restarted. Used to restart static values
	if (games_restart)
	{
		restart = true;
	}


	while (1)
	{
		// [1] ---------------------------------- run through the game functions
		initialization_game1 (frame_p, osd_p);
		ghost_X = player_move (player1, ps2_p , restart);
		Coordinates barrel_coord = barrel_throw(mouse_p , restart, adc_p);
		hit = collision_check(ghost_X, barrel_coord);
		win = update_life_and_score (hit , osd_p, barrel_coord , restart);

		// [2] --------------------------------- turn off the restart value after you reset all static values to their default.
		restart = false;

		// [3] --------------------------------- Check the winning or losing status
		if (win == 2)   // -------------------------------------------- if win
		{
			while (1)
			{
				winning_screen (frame_p, osd_p);
				if (ps2_p->get_kb_ch(&ch))
				{
					if (ch == 's')
					{
						clicked = true;
						break;
					}
				}
			} // end while loop
			// ------------- check user input
			if (clicked)
			{
				break;                                                  // exit the game back to the start menu
			}
		}// end if
		else if (win == 1)   // ---------------------------------------- if lose
		{
			while (1)
			{
				losing_screen (frame_p, osd_p);
				if (ps2_p->get_kb_ch(&ch))
				{
					if (ch == 's')
					{
						clicked = true;
						break;
					}
				}
			} // end while loop
			// ------------- check user input
			if (clicked)
			{
				break;                                                  // exit the while loop exit the game back to the start menu
			}
		}// end else if


	} // end while loop


} // end fucntion


// ---------------------------------------------------- losing screen function
void losing_screen (FrameCore *frame_p, OsdCore *osd_p)
{
	osd_p->wr_char(36 , 42, 89);      // Y                      // YOU LOST
	osd_p->wr_char(37 , 42, 79);      // O
	osd_p->wr_char(38 , 42, 85);      // U

	osd_p->wr_char(40 , 42, 76);      // L
	osd_p->wr_char(41 , 42, 79);      // O
	osd_p->wr_char(42 , 42, 83);      // S
	osd_p->wr_char(43 , 42, 84);      // T

	osd_p->wr_char(28 , 43, 67);      // C                      // CLICK S TO GO TO START MENU
	osd_p->wr_char(29 , 43, 76);      // L
	osd_p->wr_char(30 , 43, 73);      // I
	osd_p->wr_char(31 , 43, 67);      // C
	osd_p->wr_char(32 , 43, 75);      // K

	osd_p->wr_char(34 , 43, 83);      // S

	osd_p->wr_char(36 , 43, 84);      // T
	osd_p->wr_char(37 , 43, 79);      // O

	osd_p->wr_char(39 , 43, 71);      // G
	osd_p->wr_char(40 , 43, 79);      // O   ..

	osd_p->wr_char(42 , 43, 84);      // T
	osd_p->wr_char(43 , 43, 79);      // O

	osd_p->wr_char(45 , 43, 83);      // S
	osd_p->wr_char(46 , 43, 84);      // T
	osd_p->wr_char(47 , 43, 65);      // A
	osd_p->wr_char(48 , 43, 82);      // R
	osd_p->wr_char(49 , 43, 84);      // T

	osd_p->wr_char(51 , 43, 77);      // M
	osd_p->wr_char(52 , 43, 69);      // E
	osd_p->wr_char(53 , 43, 78);      // N
	osd_p->wr_char(54 , 43, 85);      // U

	osd_p->wr_char(40 , 15, 83);      // S

	// ---------------------- small square
	frame_p->plot_line(310, 230, 335, 230, 300);    // top                 x = 310 - 335  y = 230
	frame_p->plot_line(310, 260, 335, 260, 300);    // bottom              x = 310 - 335  y = 260
	frame_p->plot_line(310, 230, 310, 260, 300);    // right               x = 310        y = 230 - 260
	frame_p->plot_line(335, 230, 335, 260, 300);    // left                x = 335        y = 230 - 260


} // end function



// ------------------------------------ winning screen function
void winning_screen (FrameCore *frame_p, OsdCore *osd_p)
{


	osd_p->wr_char(32 , 41, 83);       // S                     // SCORE REACHED 100!
	osd_p->wr_char(33 , 41, 67);       // C
	osd_p->wr_char(34 , 41, 79);       // O
	osd_p->wr_char(35 , 41, 82);       // R
	osd_p->wr_char(36 , 41, 69);       // E

	osd_p->wr_char(38 , 41, 82);       // R
	osd_p->wr_char(39 , 41, 69);       // E
	osd_p->wr_char(40 , 41, 65);       // A   ....
	osd_p->wr_char(41 , 41, 67);       // C
	osd_p->wr_char(42 , 41, 72);       // H
	osd_p->wr_char(43 , 41, 69);       // E
	osd_p->wr_char(44 , 41, 68);       // D

	osd_p->wr_char(46 , 41, 49);       // 1
	osd_p->wr_char(47 , 41, 48);       // 0
	osd_p->wr_char(48 , 41, 48);       // 0
	osd_p->wr_char(49 , 41, 19);       // !



	osd_p->wr_char(37 , 42, 89);      // Y                      // YOU WON
	osd_p->wr_char(38 , 42, 79);      // O
	osd_p->wr_char(39 , 42, 85);      // U

	osd_p->wr_char(41 , 42, 87);      // W
	osd_p->wr_char(42 , 42, 79);      // O
	osd_p->wr_char(43 , 42, 78);      // N

	osd_p->wr_char(28 , 43, 67);      // C                      // CLICK S TO GO TO START MENU
	osd_p->wr_char(29 , 43, 76);      // L
	osd_p->wr_char(30 , 43, 73);      // I
	osd_p->wr_char(31 , 43, 67);      // C
	osd_p->wr_char(32 , 43, 75);      // K

	osd_p->wr_char(34 , 43, 83);      // S

	osd_p->wr_char(36 , 43, 84);      // T
	osd_p->wr_char(37 , 43, 79);      // O

	osd_p->wr_char(39 , 43, 71);      // G
	osd_p->wr_char(40 , 43, 79);      // O   ..

	osd_p->wr_char(42 , 43, 84);      // T
	osd_p->wr_char(43 , 43, 79);      // O

	osd_p->wr_char(45 , 43, 83);      // S
	osd_p->wr_char(46 , 43, 84);      // T
	osd_p->wr_char(47 , 43, 65);      // A
	osd_p->wr_char(48 , 43, 82);      // R
	osd_p->wr_char(49 , 43, 84);      // T

	osd_p->wr_char(51 , 43, 77);      // M
	osd_p->wr_char(52 , 43, 69);      // E
	osd_p->wr_char(53 , 43, 78);      // N
	osd_p->wr_char(54 , 43, 85);      // U

	osd_p->wr_char(40 , 15, 83);      // S


	// ---------------------- small square
	frame_p->plot_line(310, 230, 335, 230, 300);    // top                 x = 310 - 335  y = 230
	frame_p->plot_line(310, 260, 335, 260, 300);    // bottom              x = 310 - 335  y = 260
	frame_p->plot_line(310, 230, 310, 260, 300);    // right               x = 310        y = 230 - 260
	frame_p->plot_line(335, 230, 335, 260, 300);    // left                x = 335        y = 230 - 260

} // end function







int update_life_and_score (bool hit , OsdCore *osd_p, Coordinates barrelCoord,  bool restart)
{
	static int hit_counter = 0;
	static int score_counter = 0;
	int win_flag;

	if (restart)                                               // in case of game restarting, reset the static values
	{
		hit_counter = 0;
		score_counter = 0;
	}




	if (hit)                                                    // if hit, take off 1 life
	{
		hit_counter = hit_counter + 1;
		//uart.disp(hit_counter);
		//uart.disp("\n\r");
	}
	else
	{
		if (barrelCoord.y >= 400)                              // if not hit and barrel hit the ground, add 1 to the score
		{
			score_counter = score_counter+ 1;
			//uart.disp(score_counter);
			//uart.disp("\n\r");
		}
	}



	// --------------------- update the lives on the screen:
	if (hit_counter == 0)
	{
		osd_p->wr_char(27 , 3, 3);    // <3
		osd_p->wr_char(29 , 3, 3);    // <3
		osd_p->wr_char(31 , 3, 3);    // <3
	}
	if (hit_counter > 0 && hit_counter <= 100)
	{
		osd_p->wr_char(27 , 3, 3);    // <3
		osd_p->wr_char(29 , 3, 3);    // <3
		osd_p->wr_char(31 , 3, 0);    //
	}
	if (hit_counter > 100 && hit_counter <= 200)
	{
		osd_p->wr_char(27 , 3, 3);    // <3
		osd_p->wr_char(29 , 3, 0);    //
		osd_p->wr_char(31 , 3, 0);    //
	}
	if (hit_counter > 200 && hit_counter <= 300)
	{
		osd_p->wr_char(27 , 3, 0);    //
		osd_p->wr_char(29 , 3, 0);    //
		osd_p->wr_char(31 , 3, 0);    //
		//osd_p->wr_char(31 , 5, 65);   // B     ------------- Game Over

		//return false;                          // lost
		win_flag = 1;
	}



	// --------------------- update the score on the screen:
	if (score_counter == 0)
	{
		osd_p->wr_char(56 , 3, 48);    // 0
		osd_p->wr_char(57 , 3, 48);    // 0
		osd_p->wr_char(58 , 3, 48);    // 0
	}
	if (score_counter > 0 && score_counter <= 100)
	{
		osd_p->wr_char(56 , 3, 48);    // 0
		osd_p->wr_char(57 , 3, 49);    // 1
		osd_p->wr_char(58 , 3, 48);    // 0
	}
	if (score_counter > 100 && score_counter <= 200)
	{
		osd_p->wr_char(56 , 3, 48);    // 0
		osd_p->wr_char(57 , 3, 50);    // 2
		osd_p->wr_char(58 , 3, 48);    // 0
	}
	if (score_counter > 200 && score_counter <= 300)
	{
		osd_p->wr_char(56 , 3, 48);    // 0
		osd_p->wr_char(57 , 3, 51);    // 3
		osd_p->wr_char(58 , 3, 48);    // 0
	}
	if (score_counter > 300 && score_counter <= 400)
	{
		osd_p->wr_char(56 , 3, 48);    // 0
		osd_p->wr_char(57 , 3, 52);    // 4
		osd_p->wr_char(58 , 3, 48);    // 0
	}
	if (score_counter > 400 && score_counter <= 500)
	{
		osd_p->wr_char(56 , 3, 48);    // 0
		osd_p->wr_char(57 , 3, 53);    // 5
		osd_p->wr_char(58 , 3, 48);    // 0
	}
	if (score_counter > 500 && score_counter <= 600)
	{
		osd_p->wr_char(56 , 3, 48);    // 0
		osd_p->wr_char(57 , 3, 54);    // 6
		osd_p->wr_char(58 , 3, 48);    // 0
	}
	if (score_counter > 600 && score_counter <= 700)
	{
		osd_p->wr_char(56 , 3, 48);    // 0
		osd_p->wr_char(57 , 3, 55);    // 7
		osd_p->wr_char(58 , 3, 48);    // 0
	}
	if (score_counter > 700 && score_counter <= 800)
	{
		osd_p->wr_char(56 , 3, 48);    // 0
		osd_p->wr_char(57 , 3, 56);    // 8
		osd_p->wr_char(58 , 3, 48);    // 0
	}
	if (score_counter > 800 && score_counter <= 900)
	{
		osd_p->wr_char(56 , 3, 48);    // 0
		osd_p->wr_char(57 , 3, 57);    // 9
		osd_p->wr_char(58 , 3, 48);    // 0
	}
	if (score_counter > 900 && score_counter <= 1000)
	{
		osd_p->wr_char(56 , 3, 49);    // 1
		osd_p->wr_char(57 , 3, 48);    // 0
		osd_p->wr_char(58 , 3, 48);    // 0
		//osd_p->wr_char(58 , 5, 87);    // ------------------ W

		//return true;
		win_flag = 2;
	}


	return win_flag;

}//end function







// ----------------------------------------- function to check if there is a collision
bool collision_check (int ghostX, Coordinates barrelCoord)
{
	if ( ((ghostX + 8) >= (barrelCoord.x + 10) &&  (ghostX+8) <= (barrelCoord.x + 23))
			||  ((ghostX + 8) > (barrelCoord.x + 23 ) &&  (ghostX) < (barrelCoord.x + 23 ))
			||  ((ghostX + 8) < (barrelCoord.x + 10 ) &&  (ghostX + 15) > (barrelCoord.x + 10 )) )        //ghostX + 8 because the ghost is 16 pixels wide and 8 is the center
	{																			                          // barrelCoord.x + 10 where the barrel start
		if (barrelCoord.y >= 400)                                                           // barrelCoord.x + 23 where the barrel end      // note: barrel 32 wide
		{
			return true;
		}
	}
	else
	{
		return false;
	}

	return false;

}// end function




Coordinates barrel_throw(SpriteCore *mouse_p, bool restart, XadcCore *adc_p)
{
    static int y = 65;
    static int random_num = 256;
    //double raw;
    double barSpeed = 40;

    //raw = adc_p->read_adc_in(0);                           //read adc
    //barSpeed = (raw*1000) + 20;

    static double counter = 0;

	if (restart)                                            // in case of game restarting, reset the static values
	{
		y = 65;
		random_num = 256;
		counter = 0;
	}


    mouse_p->bypass(0); // turns on the barrel sprite

    counter++;
    if (counter > barSpeed)
    {
        y += 6;
        counter = 0;
    }

    if (y > 415)
    {
        y = 65;
        random_num = 160 + rand() % (465 - 160);
    }

    mouse_p->move_xy(random_num, y);

    Coordinates result;
    result.x = random_num;
    result.y = y;
    return result;
}

/*
// -------------------------------------------------------
void barrel_throw (SpriteCore *mouse_p)
{
	static int y = 65;
	//static int x = 320;
	static int random_num = 256;

	//double barSpeed = adc->read_adc_in(0);                           //read adc
	double barSpeed = 50;
	static double counter = 0;

	mouse_p->bypass(0);                                   //turns on the barrel sprite


	// ------------------ control the speed of the barrel
	counter ++;
	if(counter == barSpeed)
	{
		y = y + 6;
		counter=0;
	}


	// ------------------ control the limit
	if(y > 410)
	{
		y = 65;
		random_num = 160 + rand() % (465-160);
	}

	mouse_p->move_xy(random_num, y);
	//mouseX = random_num;
	//mouseY = y;

} // end function
*/




// ------------------------------------ function to move the player right and left
int player_move (SpriteCore *player1, Ps2Core *ps2_p, bool restart)
{
	int static x = 320;
	int y = 421;
	char ch;

	if (restart)                                            // in case of game restarting, reset the static values
	{
		x = 320;
	}


	player1->bypass(0);                                    //turns on the player sprite
	player1->wr_ctrl(0x1c);                                //animation; blue ghost
	player1->move_xy(x, y);

	if (ps2_p->get_kb_ch(&ch))
	{
		if(ch=='d')
		{
			x = x + 7;
		}

		else if (ch=='a')
		{
			x = x - 7;
		}
	}

	// ------------- control the limits of the character:
	if (x < 161)
	{
		x = 161;
	}
	else if (x > 463)
	{
		x = 463;
	}

	return x;


} // end function



// ------------------------------------------------------------------ function initialize the game1
void initialization_game1 (FrameCore *frame_p, OsdCore *osd_p)
{
	// ------------------------------ Game Title
	osd_p->wr_char(33 , 1, 79);    // O
	osd_p->wr_char(34 , 1, 78);    // N
	osd_p->wr_char(35 , 1, 69);    // E

	osd_p->wr_char(37 , 1, 80);    // P
	osd_p->wr_char(38 , 1, 76);    // L
	osd_p->wr_char(39 , 1, 65);    // A
	osd_p->wr_char(40 , 1, 89);    // Y
	osd_p->wr_char(41 , 1, 69);    // E
	osd_p->wr_char(42 , 1, 82);    // R

	osd_p->wr_char(44 , 1, 71);    // G
	osd_p->wr_char(45 , 1, 65);    // A
	osd_p->wr_char(46 , 1, 77);    // M
	osd_p->wr_char(47 , 1, 69);    // E

	// ------------------------------- Game border
	frame_p->plot_line(160, 40,  480, 40,  300);         // Top line     x = 160 - 480 y = 40
	frame_p->plot_line(160, 440, 480, 440, 300);         // bottom line  x = 160 - 480 y = 440
	frame_p->plot_line(160, 40,  160, 440, 300);         // right line   x = 160       y = 40 - 440
	frame_p->plot_line(480, 40,  480, 440, 300);         // left line    x = 480       y = 40 - 440

	//frame_p->plot_line(160, 65,  480, 65,  300);         // Top line     x = 160 - 480 y = 65

	// ------------------------------- Life
	osd_p->wr_char(21 , 3, 76);    // L
	osd_p->wr_char(22 , 3, 73);    // I
	osd_p->wr_char(23 , 3, 70);    // F
	osd_p->wr_char(24 , 3, 69);    // E
	osd_p->wr_char(25 , 3, 58);    // :

	//osd_p->wr_char(27 , 3, 48);    // 0
	//osd_p->wr_char(28 , 3, 48);    // 0
	//osd_p->wr_char(29 , 3, 48);    // 0

	// ------------------------------- SCORE: 000
	osd_p->wr_char(49 , 3, 83);    // S
	osd_p->wr_char(50 , 3, 67);    // C
	osd_p->wr_char(51 , 3, 79);    // O
	osd_p->wr_char(52 , 3, 82);    // R
	osd_p->wr_char(53 , 3, 69);    // E
	osd_p->wr_char(54 , 3, 58);    // :

	//osd_p->wr_char(56 , 3, 48);    // 0
	//osd_p->wr_char(57 , 3, 48);    // 0
	//osd_p->wr_char(58 , 3, 48);    // 0



} // end function
