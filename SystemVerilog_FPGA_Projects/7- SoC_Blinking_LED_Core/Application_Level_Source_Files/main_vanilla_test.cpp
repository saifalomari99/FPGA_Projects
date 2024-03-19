
	//**********************************************************************//
    //                                                                      //
	// 			       Main Vanilla cpp Application File                    //
	//                      Engineer: Saif Alomari                          //
	//                           Spring 2024                                //
	//                                              				        //
	//**********************************************************************//


#define _DEBUG
#include "chu_init.h"
#include "gpio_cores.h"

// ------------------------------------------- instantiate classes switch, led
GpoCore led(get_slot_addr(BRIDGE_BASE, S2_LED));
GpiCore sw(get_slot_addr(BRIDGE_BASE, S3_SW));
BlinkingCore led1(get_slot_addr(BRIDGE_BASE, S4_Blinking));                      //declare the class for blinking LED


//-------------------------------------------- Functions
// original test functions
void timer_check(GpoCore *led_p);
void led_check(GpoCore *led_p, int n);
void sw_check(GpoCore *led_p, GpiCore *sw_p);
void uart_check();
void led_chase1 (GpoCore *led_p, int n, GpiCore *sw_p);

// test function for the blinking LED core
void blinking_led (BlinkingCore *led_p)
{
	led_p->write_reg(1000, 0);                // 1 second  blinking rate interval, 0.5 seconds (on), 0.5 seconds (off)
	led_p->write_reg(2000, 1);                // 2 seconds blinking rate interval, 1 seconds (on), 1 seconds (off)
	led_p->write_reg(4000, 2);                // 4 seconds blinking rate interval, 2 seconds (on), 2 seconds (off)
	led_p->write_reg(8000, 3);                // 8 seconds blinking rate interval, 4 seconds (on), 4 seconds (off)
}



//==================================================== Main ====================================================
int main() {

   while (1)
   {     	   
       blinking_led(&led1);



       //---------------------- uncomment the following lines if you want to test different functions
       //timer_check(&led);
       ///led_check(&led, 16);
       //sw_check(&led, &sw);
       //uart_check();
       //debug("main - switch value / up time : ", sw.read(), now_ms());


    } //while
} //main
//==================================================== End ====================================================




// --------- blink once per second for 5 times. and provide a sanity check for timer (based on SYS_CLK_FREQ)
void timer_check(GpoCore *led_p) {
   int i;

   for (i = 0; i < 5; i++) {
      led_p->write(0xffff);
      sleep_ms(500);
      led_p->write(0x0000);
      sleep_ms(500);
      debug("timer check - (loop #)/now: ", i, now_ms());
   }
}

// ------- check individual led
void led_check(GpoCore *led_p, int n) {
   int i;

   for (i = 0; i < n; i++) {
      led_p->write(1, i);
      sleep_ms(200);
      led_p->write(0, i);
      sleep_ms(200);
   }
}

// ------- leds flash according to switch positions.
void sw_check(GpoCore *led_p, GpiCore *sw_p) {
   int i, s;

   s = sw_p->read();
   for (i = 0; i < 30; i++) {
      led_p->write(s);
      sleep_ms(50);
      led_p->write(0);
      sleep_ms(50);
   }
}

// ------- uart transmits test line. (uart instance is declared as global variable in chu_io_basic.h)
void uart_check() {
   static int loop = 0;

   uart.disp("uart test #");
   uart.disp(loop);
   uart.disp("\n\r");
   loop++;
}

// -------- chasing LED function 
void led_chase1 (GpoCore *led_p, int n, GpiCore *sw_p){
    int i, j;
    int enable = 0;
    int sw1_5 = sw_p->read();
    int sw0 = sw_p->read(0);
    static int speed; // 5 is MAX SPEED
    static int prevSpeed;

    for (i = 0; i < n; i++) {
        sw0 = sw_p->read(0);
        sw1_5 = sw_p->read();

        if (sw0 == 1) {
            led_p->write(1, 0);
            break;
        }

        prevSpeed = speed;
        //speed = ((sw1_5 - 70) * -5) & 63;
        speed = ((sw1_5 &63) - 70) * -5; //&63 is for preventing other switches as inputs


        if (speed<0){
            speed = 50;
        }

        if(prevSpeed!=speed){
            debug("Test - now/speed: ",now_ms(),speed);
        }


//        debug("Test - now/speed: ",now_ms(),speed);
        led_p->write(1, i);
        sleep_ms(speed);
        led_p->write(0, i);
        sleep_ms(speed);
        if (i == 15) {
            enable = 1;
        }
    }

    if (enable == 1) {
        for (j = 15; j > -1; j--) {
            sw0 = sw_p->read(0);
            sw1_5 = sw_p->read();

            if (sw0 == 1) {
                led_p->write(1, 0);
                break;
            }

            prevSpeed=speed;
            speed = ((sw1_5 &63) - 70) * -5;
            if (speed<0){
                speed = 50;
            }

            if(prevSpeed!=speed){
                debug("Test - now/speed: ",now_ms(),speed);
            }


//            debug("Test - now/speed: ",now_ms(),speed);
            led_p->write(1, j);
            sleep_ms(speed);
            led_p->write(0, j);
            sleep_ms(speed);
            if (j == 0) {
                enable = 0;
            }
        }
    }
}
