

#include <cstdlib>                 // for random numbers
#include "chu_init.h"
#include "gpio_cores.h"
#include "vga_core.h"
#include "sseg_core.h"
#include "ps2_core.h"
#include "xadc_core.h"


// ---------------------------- 1 Player Game Functions ----------------------------------
// ----------- construct to hold the coordinates of x and y
struct Coordinates
{
    int x;
    int y;
};

void one_player_game(FrameCore *frame_p, OsdCore *osd_p, SpriteCore *player1, Ps2Core *ps2_p, SpriteCore *mouse_p , bool games_restart, XadcCore *adc_p);

void initialization_game1 (FrameCore *frame_p, OsdCore *osd_p);

int player_move (SpriteCore *player1, Ps2Core *ps2_p,  bool restart);

Coordinates barrel_throw(SpriteCore *mouse_p, bool restart, XadcCore *adc_p);

bool collision_check (int ghostX, Coordinates barrelCoord);

int update_life_and_score (bool hit , OsdCore *osd_p, Coordinates barrelCoord,  bool restart);

void winning_screen (FrameCore *frame_p, OsdCore *osd_p);

void losing_screen (FrameCore *frame_p, OsdCore *osd_p);


