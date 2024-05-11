

#include <cstdlib>                 // for random numbers
#include "chu_init.h"
#include "gpio_cores.h"
#include "vga_core.h"
#include "sseg_core.h"
#include "ps2_core.h"
#include "xadc_core.h"




// ------------------------------- Game Functions --------------------------
//holds player1 and player2 x-cords
struct playerPos{
	int p1_x;
	int p2_x;
};
//struct holds barrels x and y cords
struct barrelPos{
	int x;
	int y;
};


// ---------------------------- 2 Player Game Functions ----------------------------------
void two_player_game (FrameCore *frame_p, OsdCore *osd_p, SpriteCore *player1,SpriteCore *player2, Ps2Core *ps2_p, SpriteCore *mouse_p, XadcCore *adc);
playerPos playerMovement(SpriteCore *player1,SpriteCore *player2, Ps2Core *ps2, bool restart );
barrelPos barrelThrow(SpriteCore *barrel, bool restart, XadcCore *adc);
int printHit(playerPos playerPos,barrelPos barrelPos);
int playerScore(OsdCore *osd,int hit, bool restart);
void winGame(OsdCore *osd, int winner);
