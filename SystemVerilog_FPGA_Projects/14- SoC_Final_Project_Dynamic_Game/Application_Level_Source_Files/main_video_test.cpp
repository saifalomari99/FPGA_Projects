

// ============================================ Main File version v4.0 =======================================

//#define _DEBUG
#include "chu_init.h"
#include "gpio_cores.h"
#include "vga_core.h"
#include "sseg_core.h"
#include "ps2_core.h"
#include "xadc_core.h"

#include "one_player_game.h"
#include "two_player_game.h"



// -------------------------------------------- video cores test functions
void test_start(GpoCore *led_p) ;
void bar_check(GpvCore *bar_p);
void gray_check(GpvCore *gray_p);
void osd_check(OsdCore *osd_p);
void frame_check(FrameCore *frame_p);
void mouse_check(SpriteCore *mouse_p);
void ghost_check(SpriteCore *ghost_p);

int start_menu (FrameCore *frame_p , OsdCore *osd_p, Ps2Core *ps2_p);

// --------------------------------------------- external core instantiation
GpoCore led(get_slot_addr(BRIDGE_BASE, S2_LED));
GpiCore sw(get_slot_addr(BRIDGE_BASE, S3_SW));
FrameCore frame(FRAME_BASE);
GpvCore bar(get_sprite_addr(BRIDGE_BASE, V7_BAR));
GpvCore gray(get_sprite_addr(BRIDGE_BASE, V6_GRAY));
SpriteCore ghost(get_sprite_addr(BRIDGE_BASE, V3_GHOST), 1024);
SpriteCore mouse(get_sprite_addr(BRIDGE_BASE, V1_MOUSE), 1024);
SpriteCore ghost2(get_sprite_addr(BRIDGE_BASE, V4_GHOST2), 1024);         //NEW VGA CORE. Replaced USER4
OsdCore osd(get_sprite_addr(BRIDGE_BASE, V2_OSD));
SsegCore sseg(get_slot_addr(BRIDGE_BASE, S8_SSEG));
Ps2Core ps2(get_slot_addr(BRIDGE_BASE, S11_PS2));
XadcCore adc(get_slot_addr(BRIDGE_BASE, S5_XDAC));

// =========================================== Main =============================================================================
int main()
{
	// --------------------- check the ps2 device:
	   int id = ps2.init();
	   uart.disp("\n\rPS2 device (1-keyboard / 2-mouse): ");
	   uart.disp(id);
	   uart.disp("\n\r");


	// ----------------- Starting by turning off all VGA cores
	ghost.bypass(1);
	ghost2.bypass(1);
    frame.bypass(1);
    bar.bypass(1);
    gray.bypass(1);
    osd.bypass(1);
    mouse.bypass(1);
	


	bool menu = true;                                  //controls when player is in the menu
	int game_select;
	bool games_restart;

	while (1)
	{
		if (menu)
		{
			games_restart = true;
			game_select = start_menu(&frame,&osd,&ps2);
		}




		// ------------------- Go to the games
		if (game_select == 1)
		{
			led.write(1, 0);
			menu = false;                            // get off the menu screen
			one_player_game(&frame, &osd, &ghost, &ps2, &mouse , games_restart, &adc);
			game_select = 0;
		}
		else if (game_select == 2)
		{
			led.write(1, 1);
			menu = false;                            // get off the menu screen
			two_player_game(&frame, &osd, &ghost, &ghost2, &ps2, &mouse, &adc);
			game_select = 0;
			uart.disp("\n\r it exit the two player game.");
		}
		else if (game_select == 0)
		{
			menu = true;
		}


	} // end while

/*

*/
} // end main
// =============================================== end main ==========================================================






// ----------------------------------------------------------------- start menu screen
int start_menu (FrameCore *frame_p , OsdCore *osd_p, Ps2Core *ps2_p  )
{
	ghost.bypass(1);
	mouse.bypass(1);

	osd.clr_screen();                                  //clear screen from any text
	frame.bypass(0);                                   //turn on background
	frame.clr_screen(0x0008);                          //dark green (for main menu)
	osd.bypass(0);                                     //turn on text
	char ch;

	while (1)
	{
		frame_p->plot_line(220, 140, 420, 140, 300);         // Top line   x = 220 - 420  y = 140
		frame_p->plot_line(220, 340, 420, 340, 300);         // bottom line  x = 220 - 420 y = 340
		frame_p->plot_line(220, 140, 220, 340, 300);         // right line x = 220  y = 140 - 340
		frame_p->plot_line(420, 140, 420, 340, 300);         // left line x = 420 y = 140 - 340

		// ------------------------ display welcome message
		osd_p->set_color(0xf00, 0); //Text Color, Background Color

		// osd_p->wr_char(x , y, ch);
		osd_p->wr_char(35 , 10, 83);    // S       // START MENU
		osd_p->wr_char(36 , 10, 84);    // T
		osd_p->wr_char(37 , 10, 65);    // A
		osd_p->wr_char(38 , 10, 82);    // R
		osd_p->wr_char(39 , 10, 84);    // T

		osd_p->wr_char(41 , 10, 77);    // M
		osd_p->wr_char(42 , 10, 69);    // E
		osd_p->wr_char(43 , 10, 78);    // N
		osd_p->wr_char(44 , 10, 85);    // U

		//----------------------------------

		osd_p->wr_char(29 , 12, 67);    // C        CHOOSE A GAME TO START
		osd_p->wr_char(30 , 12, 72);    // H
		osd_p->wr_char(31 , 12, 79);    // O
		osd_p->wr_char(32 , 12, 79);    // O
		osd_p->wr_char(33 , 12, 83);    // S
		osd_p->wr_char(34 , 12, 69);    // E

		osd_p->wr_char(36 , 12, 65);    // A

		osd_p->wr_char(38 , 12, 71);    // G
		osd_p->wr_char(39 , 12, 65);    // A
		osd_p->wr_char(40 , 12, 77);    // M
		osd_p->wr_char(41 , 12, 69);    // E

		osd_p->wr_char(43 , 12, 84);    // T
		osd_p->wr_char(44 , 12, 79);    // O

		osd_p->wr_char(46 , 12, 83);    // S
		osd_p->wr_char(47 , 12, 84);    // T
		osd_p->wr_char(48 , 12, 65);    // A
		osd_p->wr_char(49 , 12, 82);    // R
		osd_p->wr_char(50 , 12, 84);    // T



		//osd_p->wr_char(40 , 15, 83);    // S

		// ---------------------- small square 1
		frame_p->plot_line(250, 230, 390, 230, 300);    // top                 x = 250 - 390  y = 230
		frame_p->plot_line(250, 260, 390, 260, 300);    // bottom              x = 250 - 390  y = 260
		frame_p->plot_line(250, 230, 250, 260, 300);    // right               x = 250        y = 230 - 260
		frame_p->plot_line(390, 230, 390, 260, 300);    // left                x = 390        y = 230 - 260

		osd_p->wr_char(33 , 15, 79);    // O            ONE PLAYER GAME
		osd_p->wr_char(34 , 15, 78);	// N
		osd_p->wr_char(35 , 15, 69);	// E

		osd_p->wr_char(37 , 15, 80);    // P
		osd_p->wr_char(38 , 15, 76);    // L
		osd_p->wr_char(39 , 15, 65);    // A
		osd_p->wr_char(40 , 15, 89);    // Y
		osd_p->wr_char(41 , 15, 69);    // E
		osd_p->wr_char(42 , 15, 82);    // R

		osd_p->wr_char(44 , 15, 71);    // G
		osd_p->wr_char(45 , 15, 65);    // A
		osd_p->wr_char(46 , 15, 77);    // M
		osd_p->wr_char(47 , 15, 69);    // E






		// ---------------------- small square 2
		frame_p->plot_line(250, 280, 390, 280, 300);    // top                 x = 250 - 390  y = 280
		frame_p->plot_line(250, 310, 390, 310, 300);    // bottom              x = 250 - 390  y = 310
		frame_p->plot_line(250, 280, 250, 310, 300);    // right               x = 250        y = 280 - 310
		frame_p->plot_line(390, 280, 390, 310, 300);    // left                x = 390        y = 280 - 310

		osd_p->wr_char(33 , 18, 84);    // T            TWO PLAYER GAME
		osd_p->wr_char(34 , 18, 87);	// W
		osd_p->wr_char(35 , 18, 79);	// O

		osd_p->wr_char(37 , 18, 80);    // P
		osd_p->wr_char(38 , 18, 76);    // L
		osd_p->wr_char(39 , 18, 65);    // A
		osd_p->wr_char(40 , 18, 89);    // Y
		osd_p->wr_char(41 , 18, 69);    // E
		osd_p->wr_char(42 , 18, 82);    // R

		osd_p->wr_char(44 , 18, 71);    // G
		osd_p->wr_char(45 , 18, 65);    // A
		osd_p->wr_char(46 , 18, 77);    // M
		osd_p->wr_char(47 , 18, 69);    // E






		if (ps2_p->get_kb_ch(&ch))
		{
			   //uart.disp(ch);
			   //uart.disp("\n\r");
			if(ch=='1')
			{
				return 1;
			}

			else if (ch=='2')
			{
				return 2;
			}
		}

	}


		return 0;
} // end function









// -----------------------------------------------------------------------------------------------------------------------------
void test_start(GpoCore *led_p)
{
   int i;

   for (i = 0; i < 20; i++) {
      led_p->write(0xff00);
      sleep_ms(50);
      led_p->write(0x0000);
      sleep_ms(50);
   }
}

/**
 * check bar generator core
 * @param bar_p pointer to Gpv instance
 */
void bar_check(GpvCore *bar_p)
{
   bar_p->bypass(0);
   sleep_ms(3000);
}

/**
 * check color-to-grayscale core
 * @param gray_p pointer to Gpv instance
 */
void gray_check(GpvCore *gray_p)
{
   gray_p->bypass(0);
   sleep_ms(3000);
   gray_p->bypass(1);
}

/**
 * check osd core
 * @param osd_p pointer to osd instance
 */
void osd_check(OsdCore *osd_p)
{
   osd_p->set_color(0x0f0, 0x001); // dark gray/green
   osd_p->bypass(0);
   osd_p->clr_screen();
   for (int i = 0; i < 64; i++) {
      osd_p->wr_char(8 + i, 0, i);
      osd_p->wr_char(8 + i, 1, 64 + i, 1);
      sleep_ms(100);
   }
   sleep_ms(3000);
}

/**
 * test frame buffer core
 * @param frame_p pointer to frame buffer instance
 */
void frame_check(FrameCore *frame_p)
{
   int x, y, color;

   frame_p->bypass(0);
   for (int i = 0; i < 10; i++) {
      frame_p->clr_screen(0x008);  // dark green
      for (int j = 0; j < 20; j++) {
         x = rand() % 640;
         y = rand() % 480;
         color = rand() % 512;
         frame_p->plot_line(400, 200, x, y, color);
      }
      sleep_ms(300);
   }
   sleep_ms(3000);
}

/**
 * test ghost sprite
 * @param ghost_p pointer to mouse sprite instance
 */
void mouse_check(SpriteCore *mouse_p)
{
   int x, y;

   mouse_p->bypass(0);
   // clear top and bottom lines
   for (int i = 0; i < 32; i++) {
      mouse_p->wr_mem(i, 0);
      mouse_p->wr_mem(31 * 32 + i, 0);
   }

   // slowly move mouse pointer
   x = 0;
   y = 0;
   for (int i = 0; i < 80; i++) {
      mouse_p->move_xy(x, y);
      sleep_ms(50);
      x = x + 4;
      y = y + 3;
   }
   sleep_ms(3000);
   // load top and bottom rows
   for (int i = 0; i < 32; i++) {
      sleep_ms(20);
      mouse_p->wr_mem(i, 0x00f);
      mouse_p->wr_mem(31 * 32 + i, 0xf00);
   }
   sleep_ms(3000);
}

/**
 * test ghost sprite
 * @param ghost_p pointer to ghost sprite instance
 */
void ghost_check(SpriteCore *ghost_p)
{
   int x, y;

   // slowly move mouse pointer
   ghost_p->bypass(0);
   ghost_p->wr_ctrl(0x1c);  //animation; blue ghost
   x = 0;
   y = 100;
   for (int i = 0; i < 156; i++) {
      ghost_p->move_xy(x, y);
      sleep_ms(100);
      x = x + 4;
      if (i == 80) {
         // change to red ghost half way
         ghost_p->wr_ctrl(0x04);
      }
   }
   sleep_ms(3000);
}



















