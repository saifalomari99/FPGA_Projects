




#ifndef _VGA_H_INCLUDED
#define _VGA_H_INCLUDED

#include "chu_init.h"
#include <stdlib.h>


// ============================================= General-purpose video core =====================================
class GpvCore
{
public:
	// register map
    enum
    {
       BYPASS_REG = 0x2000  /**< bypass control register */
    };


    /* ------------------- methods --------------------- */
    // ----------------- [1] constructor
    GpvCore(uint32_t core_base_addr);
    ~GpvCore();                              // not used

    // ----------------- [2] write a 32-bit word to memory module/registers of a video core
    // addr offset address within core
    // color data to be written
    void wr_mem(int addr, uint32_t color);

    // ----------------- [3] enable/disable core bypass
    // by 1: bypass current core; 0: not bypass
    void bypass(int by);

private:
   uint32_t base_addr;
};




// ========================================================== Sprite Core ==========================================================
// video subsystem HDL parameter:
//        - CHROMA_KEY (KEY_COLOR) = 0
class SpriteCore
{
public:

	// register map
   enum {
      BYPASS_REG = 0x2000,     /**< bypass control register */
      X_REG = 0x2001,          /**< x-axis of sprite origin */
      Y_REG = 0x2002,          /**< y-axis of sprite origin */
      SPRITE_CTRL_REG = 0x2003 /**< sprite control register */
   };
   // symbolic constants
   enum {
      KEY_COLOR = 0,  /**< chroma-key color */
   };

   /* -------------------------------------------- methods -------------------------------------------- */
   // ----------------------------- [1]
   SpriteCore(uint32_t core_base_addr, int size);
   ~SpriteCore();                  // not used


   // ----------------------------- [2] write a 32-bit word to memory module/registers of a video core
   // addr offset address within core
   // color data to be written
   void wr_mem(int addr, uint32_t color);


   // ----------------------------- [3] move sprite to a location
   // x x-coordinate of sprite origin
   // y y-coordinate of sprite origin
   // origin is the top-left corner of sprite
   void move_xy(int x, int y);


   // ----------------------------- [4] write sprite control command
   // cmd control command
   void wr_ctrl(int32_t cmd);


   // ----------------------------- [5] enable/disable core bypass
   // by 1: bypass current core; 0: not bypass
   // type of command depends on each individual sprite core
   void bypass(int by);

private:
   uint32_t base_addr;
   int size;         // sprite memory size
};






/**********************************************************************
 * OSD Core
 *********************************************************************/
/**
 * osd (on-screen display) video core driver
 *
 * video subsystem HDL parameter:
 *  - CHROMA_KEY (CHROMA_KEY_COLOR) = 0
 *
 */
// ====================================================== OSD Core =============================================================
class OsdCore
{
public:

	// register map
    enum
	{
      BYPASS_REG = 0x2000,  /**< bypass control register */
      FG_CLR_REG = 0x2001,  /**< foreground color register */
      BG_CLR_REG = 0x2002   /**< background color register */
    };

    // symbolic constants
    enum {
      CHROMA_KEY_COLOR = 0,   // chroma key
      NULL_CHAR = 0x00,       // signature for transparent char tile
      CHAR_X_MAX = 80,        // 80 char per row
      CHAR_Y_MAX = 30         // 30 char per column
    };

    /* ---------------------------- methods ------------------------------- */
    // ----------------------------- [1]
    OsdCore(uint32_t core_base_addr);
   ~OsdCore();
   // not used


   // ----------------------------- [2] set foreground/background text display colors
   // fg_color foreground text display color
   // bg_color background text display color
   void set_color(uint32_t fg_color, uint32_t bg_color);


   // ----------------------------- [3] write a char to tile RAM
   // x x-coordinate of the tile (between 0 and CHAR_X_MAX)
   // y y-coordinate of the tile (between 0 and CHAR_Y_MAX)
   // reverse 0: normal display; 1: reversed display
   void wr_char(uint8_t x, uint8_t y, char ch, int reverse = 0);


   // ----------------------------- [4] clear tile RAM (by writing NULL_CHAR to all tiles)
   void clr_screen();


   // ----------------------------- [5] enable/disable core bypass
   // by 1: bypass current core; 0: not bypass
   void bypass(int by);

private:
   uint32_t base_addr;
};







/**********************************************************************
 * Frame Core
 *********************************************************************/
/**
 * frame buffer core driver
 *
 */
class FrameCore {
public:
   /**
    * Register map
    *
    */
   enum {
      BYPASS_REG = 0xfffff  /**< bypass control register */
   };
   /**
    * Symbolic constants for frame buffer size
    *
    */
   enum {
    HMAX = 640,  /**< 640 pixels per row */
    VMAX = 480   /**< 480 pixels per row */
   };
   /* methods */
   FrameCore(uint32_t frame_base_addr);
   ~FrameCore();                  // not used

   /**
    * write a pixel to frame buffer
    * @param x x-coordinate of the pixel (between 0 and HMAX)
    * @param y y-coordinate of the pixel (between 0 and VMAX)
    * @param color pixel color
    *
    */
   void wr_pix(int x, int y, int color);

   /**
    * clear frame buffer (fill the frame with a specific color)
    * @param color color to fill the frame
    *
    */
   void clr_screen(int color);


   /**
    * generate pixels for a line in frame buffer (plot a line)
    * @param x1 x-coordinate of starting point
    * @param y1 y-coordinate of starting point
    * @param x2 x-coordinate of ending point
    * @param y2 y-coordinate of ending point
    * @param color line color
    *
    */
   void plot_line(int x1, int y1, int x2, int y2, int color);

   /**
    * enable/disable core bypass
    * @param by 1: bypass current core; 0: not bypass
    *
    */
   void bypass(int by);


private:
   uint32_t base_addr;
   void swap(int &a, int &b);
};

#endif  // _VGA_H_INCLUDED
