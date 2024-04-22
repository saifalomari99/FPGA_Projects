



#ifndef _PS2_H_INCLUDED
#define _PS2_H_INCLUDED

#include "chu_init.h"

/**
 * ps2 core driver
 *  - transmit/receive raw byte stream to/from MMIO timer core.
 *  - initialize ps2 mouse
 *  - get mouse movement/button activities
 *  - get keyboard char
 *
 */

//=============================================== Ps2 Core Class ========================================
class Ps2Core {
public:
   // ----------- Register map
   enum {
      RD_DATA_REG = 0,                 /**< read data/status register */
      PS2_WR_DATA_REG = 1,             /**< 8-bit write data register */
      RM_RD_DATA_REG = 2               // remove read data
   };

   // ------------ field masks
   enum {
      TX_IDLE_FIELD = 0x00000200,          /**< bit 9 of rd_data_reg; full bit  */
      RX_EMPT_FIELD = 0x00000100,          /**< bit 10 of rd_data_reg; empty bit */
      RX_DATA_FIELD = 0x000000ff           /**< bits of 7..0 rd_data_reg; read data */
   };


   // --------------------------------- methods ------------------------------------
   // [1] ----------------------------------------------------------- 
   // constructor
   // set default baud rate to 9600
   Ps2Core(uint32_t core_base_addr);
   ~Ps2Core();     // not used

   // [2] -----------------------------------------------------------
   // check whether the ps2 receiver fifo is empty
   // 1: if empty; 0: otherwise
   int rx_fifo_empty();

   // [3] -----------------------------------------------------------
   // check whether the ps2 transmitter is idle
   // 1: if idle; 0: otherwise
   int tx_idle();

   // [4] -----------------------------------------------------------
   // send an 8-bit command to ps2
   void tx_byte(uint8_t cmd);

   // [5] -----------------------------------------------------------
   // check ps2 fifo and, if not empty, read data and then remove it
   // -1 if fifo is empty; fifo data otherwise
   int rx_byte();

   // [6] -----------------------------------------------------------
   // reset and identify the type of ps2 device (mouse or keyboard).
   // return device id or error code as follows:
   // 1: keyboard;
   // 2: mouse (set to stream mode);
   // -1: no response;
   // -2: unknown device;
   // -3: failure to set mouse to stream mode;
   // keyboard does not require initialization; init() checks device id
   int init();

   // [7] -----------------------------------------------------------
   // get mouse activity
   // 0: no new data; 1: with new data
   // lbtn return 1 when left mouse button pressed;
   // rbtn return 1 when right mouse button pressed;
   // xmov return x-axis movement;
   // ymov return y-axis movement;
   int get_mouse_activity(int *lbtn, int *rbtn, int *xmov, int *ymov);

   // [8] -----------------------------------------------------------
   // get keyboard activity
   // 0: no new data; 1: with new data
   // ch return ASCII code of the pressed key
   // special codes returned for non-ASCII keys (F1, ESC etc.)
   int get_kb_ch(char *ch);



private:
   /* variable to keep track of current status */
   uint32_t base_addr;
};

#endif  // _PS2_H_INCLUDED
