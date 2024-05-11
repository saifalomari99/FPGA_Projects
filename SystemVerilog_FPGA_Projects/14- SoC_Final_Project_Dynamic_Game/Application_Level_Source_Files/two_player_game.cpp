

#include "two_player_game.h"



// =========================================================== 2 players Game ====================================================
void two_player_game (FrameCore *frame_p, OsdCore *osd_p, SpriteCore *player1,SpriteCore *player2, Ps2Core *ps2_p, SpriteCore *mouse_p, XadcCore *adc)
{
	bool gameStart = false;                            //checks if the game is about to start
	int playerHit;                                     //gives a 1 or 2 for which player was hit. 0 if not players are hit
	int winner=0;                                      //1 or 2 depending on who won. 0 if no one has won yet
	bool restart;                                      //checks if game was restarted. Used to restart static values
	char ch;                                           //for keyboard input
	bool exit_clicked = false;

	while (1)
   	{
			if(gameStart == false)
			{
				osd_p->clr_screen();                      //clear screen of menu text
				osd_p->set_color(0x0f0,0x001);            //make new text green and background black
				frame_p->clr_screen(0x00F);               //make background frame blue
				gameStart = true;
				restart = true;                        //sets all static values to initial values (in the case of a restart)
		  	}
			playerPos x_cords = playerMovement(player1,player2,ps2_p,restart);          //playerMovement
			barrelPos bar_cor = barrelThrow(mouse_p,restart,adc);                     //barrel movement
		   	playerHit = printHit(x_cords, bar_cor);                                   //hit detection
		   	winner = playerScore(osd_p,playerHit,restart);                             //keeps score and returns the winner 1 or 2
		   	restart = false;

		   	while(winner != 0)
		   	{
				 winGame(osd_p,winner);                                                 //displays who the winner is
			  	 if(ps2_p->get_kb_ch(&ch))
			  	 {
				 	if(ch=='s' || ch == 'S')                                           //prepares program for restart
				 	{
						winner=0;
						//menu =true;
					   	gameStart = false;
					   	osd_p->clr_screen();
					   	frame_p->clr_screen(0x0008);
					   	player1->bypass(1);
					   	player2->bypass(1);
					   	mouse_p->bypass(1);

					   	exit_clicked = true;
				   	}
			   	}//end of if(ps2.get_kb_ch(&ch))

		   	}//end of while(!winner==0)
		   	if (exit_clicked)
		   	{
		   		break;
		   	}

   } // while

} // end function
//NOTE: Using UART delays EVERYTHING. DO NOT USE UART OF SLEEP_MS()!!!










//controls both players movement. A&D buttons for P1. L&J for player2
playerPos playerMovement(SpriteCore *player1,SpriteCore *player2, Ps2Core *ps2, bool restart )
{
	int y = 460;                                           //x=620 and y = 460 is bottom right of screen
	char ch;
	static int x1 = 10;                                    //starting position for player1
	static int x2 = 30;                                    //starting position for player2
	playerPos x_cords;                                     //initalize struct that saves players x cords
	x_cords.p1_x = x1;                                     //save x value for p1 in struct
	x_cords.p2_x = x2;                                     //save x value for p2 in struct

	player1->bypass(0);                                    //turns on the player sprite
    player2->bypass(0);
	player1->wr_ctrl(0x1c);                                //animation; blue ghost

	if(restart)
	{
		x1=10;
		x2=30;
	}

	if(ps2->get_kb_ch(&ch))
	{
		if(ch == 'd'){
			x1=x1+6;
		}

		else if(ch=='a'){
			x1=x1-6;
		}

		else if (ch =='l'){
			x2=x2+10;
		}

		else if (ch == 'j'){
			x2=x2-10;
		}
	}

////Keep Players in Bounds
	 if(x1<10){
		 x1=10;
	 }

	 if(x1>620){
		 x1=620;
	 }

	 if(x2<10){
		 x2=10;
	 }

	 if(x2>620){
		 x2=620;
	 }
////

////Updated Player Position
	 player1->move_xy(x1, y);
	 player2->move_xy(x2, y);
////

	 return x_cords;

} // end function


//moves the barrel down
barrelPos barrelThrow(SpriteCore *barrel, bool restart, XadcCore *adc)
{
	static int y=0;                                                //x=620 and y = 460 is bottom right of screen
	static double counter = 0;
	static int random_num =256;
	barrelPos barrel_pos;
	double barSpeed=adc->read_adc_in(0);                           //read adc
	barSpeed=(barSpeed*1000)+100;
	if(restart){
		y=0;
		counter = 0;
		random_num = 256;
	}

	barrel->bypass(0); //turns on the barrel sprite

	counter++;
	if(counter>barSpeed){ //increase counter value for slower speed // 2000 med speed
		y=y+6;
		counter=0;
	}

	if(y>480){
		y=0;
		random_num = 10 + rand() % (621-10);
	}

	barrel_pos.x=random_num;
	barrel_pos.y=y;
	barrel->move_xy(random_num, y);
	return barrel_pos;



} // end function

//Checks if players hit a barrel
int printHit(playerPos playerPos,barrelPos barrelPos)
{
	if(barrelPos.x >= playerPos.p1_x -20 && barrelPos.x <= playerPos.p1_x +20){
		if(barrelPos.y >=460){
			return 1;
		}
	}

	if(barrelPos.x >= playerPos.p2_x -20 && barrelPos.x <= playerPos.p2_x +20){
		if(barrelPos.y >=460){
			return 2;
		}
	}

	return 0;
} // end function



int playerScore(OsdCore *osd, int hit, bool restart)
{
	static int p1_health =3;
	static int p2_health =3;
	static int p1Counter =0;
	static int p2Counter =0;

	if (restart)
	{
		p1_health =3;
		p2_health=3;
		p1Counter=0;
		p2Counter=0;
	}

    ////// ------------------------------ PLAYER 1 SECTION
	osd->wr_char(1,1,80);//P
	osd->wr_char(2,1,49);//1
	osd->wr_char(3,1,32);//space

	osd->wr_char(4,1,76);//L
	osd->wr_char(5,1,73);//I
	osd->wr_char(6,1,86);//V
	osd->wr_char(7,1,69);//E
	osd->wr_char(8,1,83);//S
	osd->wr_char(9,1,58);//:

	//While player is inside the barrel, subtract health and start counting p1Counter up
	//When the player is no longer inside the barrel bounds (hit ==0), and reset counter
	if(hit==1)                                   //player 1 is hit
	{
		if(p1Counter==0)
		{
			p1_health--;
		}
		p1Counter++;

	}
	else if(hit==0)
	{
		if (p1Counter > 0)
		{
			p1Counter=0;
		}
	}


	if(p1_health == 3)
	{
		osd->wr_char(10,1,51);//3
	}
	else if(p1_health == 2)
	{
		osd->wr_char(10,1,50);//2
	}
	else if(p1_health ==1)
	{
		osd->wr_char(10,1,49);//1
	}
	else if(p1_health == 0)
	{
		osd->wr_char(10,1,48);//0
	}
	else
	{
		return 2;
	}
    ////////////END OF PLAYER 1 SECTION


    ////// -------------------------------- PLAYER 2 SECTION
	osd->wr_char(69,1,80);//P
	osd->wr_char(70,1,50);//2
	osd->wr_char(71,1,32);//space

	osd->wr_char(72,1,76);//L
	osd->wr_char(73,1,73);//I
	osd->wr_char(74,1,86);//V
	osd->wr_char(75,1,69);//E
	osd->wr_char(76,1,83);//S
	osd->wr_char(77,1,58);//:

	//While player is inside the barrel, subtract health and start counting p1Counter up
	//When the player is no longer inside the barrel bounds (hit ==0), and reset counter
	if(hit==2){//player 2 is hit
		if(p2Counter==0){
			p2_health--;
		}
		p2Counter++;
	}

	else if(hit==0){
		if (p2Counter > 0){
			p2Counter=0;
		}
	}

	if(p2_health == 3){
		osd->wr_char(78,1,51);//3
	}


	else if(p2_health == 2){
		osd->wr_char(78,1,50);//2
	}

	else if(p2_health ==1){
		osd->wr_char(78,1,49);//1
	}

	else if(p2_health == 0){
		osd->wr_char(78,1,48);//0
	}

	else{
		return 1;
	}
	////////////END OF PLAYER 2 SECTION

	return 0;
} // end function


// ------------------------ displays the winner and restart option
void winGame(OsdCore *osd, int winner)
{
	if(winner==1)
	{
		osd->wr_char(36,1,80);//P
		osd->wr_char(37,1,49);//1
		osd->wr_char(38,1,32);//space

		osd->wr_char(39,1,87);//W
		osd->wr_char(40,1,73);//I
		osd->wr_char(41,1,78);//N
		osd->wr_char(42,1,83);//S
		osd->wr_char(43,1,33);//!

	}
	else    // winner == 2
	{
		osd->wr_char(36,1,80);//P
		osd->wr_char(37,1,50);//2
		osd->wr_char(38,1,32);//space

		osd->wr_char(39,1,87);//W
		osd->wr_char(40,1,73);//I
		osd->wr_char(41,1,78);//N
		osd->wr_char(42,1,83);//S
		osd->wr_char(43,1,33);//!
	}

	osd->wr_char(28 , 43, 67);      // C                      // CLICK S TO GO TO START MENU
	osd->wr_char(29 , 43, 76);      // L
	osd->wr_char(30 , 43, 73);      // I
	osd->wr_char(31 , 43, 67);      // C
	osd->wr_char(32 , 43, 75);      // K

	osd->wr_char(34 , 43, 83);      // S

	osd->wr_char(36 , 43, 84);      // T
	osd->wr_char(37 , 43, 79);      // O

	osd->wr_char(39 , 43, 71);      // G
	osd->wr_char(40 , 43, 79);      // O   ..

	osd->wr_char(42 , 43, 84);      // T
	osd->wr_char(43 , 43, 79);      // O

	osd->wr_char(45 , 43, 83);      // S
	osd->wr_char(46 , 43, 84);      // T
	osd->wr_char(47 , 43, 65);      // A
	osd->wr_char(48 , 43, 82);      // R
	osd->wr_char(49 , 43, 84);      // T

	osd->wr_char(51 , 43, 77);      // M
	osd->wr_char(52 , 43, 69);      // E
	osd->wr_char(53 , 43, 78);      // N
	osd->wr_char(54 , 43, 85);      // U

	osd->wr_char(40 , 15, 83);      // S

	// ---------------------- small square
	//frame_p->plot_line(310, 230, 335, 230, 300);    // top                 x = 310 - 335  y = 230
	//frame_p->plot_line(310, 260, 335, 260, 300);    // bottom              x = 310 - 335  y = 260
	//frame_p->plot_line(310, 230, 310, 260, 300);    // right               x = 310        y = 230 - 260
	//frame_p->plot_line(335, 230, 335, 260, 300);    // left                x = 335        y = 230 - 260











//	osd->wr_char(31,3,80);//P
//	osd->wr_char(32,3,82);//R
//	osd->wr_char(33,3,69);//E
//	osd->wr_char(34,3,83);//S
//	osd->wr_char(35,3,83);//S
//	osd->wr_char(36,3,32);//space
//	osd->wr_char(37,3,82);//R
//	osd->wr_char(38,3,32);//space
//	osd->wr_char(39,3,84);//T
//	osd->wr_char(40,3,79);//O
//	osd->wr_char(41,3,32);//space
//	osd->wr_char(42,3,82);//R
//	osd->wr_char(43,3,69);//E
//	osd->wr_char(44,3,83);//S
//	osd->wr_char(45,3,84);//T
//	osd->wr_char(46,3,65);//A
//	osd->wr_char(47,3,82);//R
//	osd->wr_char(48,3,84);//T

} // end function


