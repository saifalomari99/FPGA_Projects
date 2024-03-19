

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


// ------------------------ instantiate classes switch, led
GpoCore led(get_slot_addr(BRIDGE_BASE, S2_LED));                            //Global Variables
GpiCore sw(get_slot_addr(BRIDGE_BASE, S3_SW));



//------------------------- Different Test Functions 
// new function
void chase_LED (GpoCore *led_p, GpiCore *sw_p, int n);

// original test functions
void timer_check(GpoCore *led_p);
void led_check(GpoCore *led_p, int n);
void sw_check(GpoCore *led_p, GpiCore *sw_p);
void uart_check();


//================================================= Main ================================================================
int main() {

   while (1) 
   {
	   //chase_LED(&led, &sw, 16);
	   led_chase1(&led,16, &sw);








      //---------------------- uncomment the following lines if you want to test different functions
      //timer_check(&led);
      ///led_check(&led, 16);
      //sw_check(&led, &sw);
      //uart_check();
      //debug("main - switch value / up time : ", sw.read(), now_ms());

    } //while
} //main
//============================================== end main ===============================================================





//---------------------------------- chasing LED Function
void chase_LED (GpoCore *led_p, GpiCore *sw_p, int n)
{
	static int direction = 0;
	int sw0;                                                                 // initialize the process
	int switchs_all;                                                         // Control the speed
	static int speed;
	static int prevSpeed;                                                    // to hold the value of the previous speed for displaying to the UART


	if (direction == 0)//---------------------------------------------------- right direction
	{
		int i;
		for (i = 0; i < n; i++)
		{
			led_p->write(0, 0);                                              // to initialize bit 0 to 0 every time, because of sw0 function
			sw0 = sw_p->read(0);                                             // read the value of sw1
			switchs_all = sw_p->read();                                      // read the values of all switches

			prevSpeed = speed;
	        speed = ((switchs_all & 62) - 70) * - 5;                         // (switchs_all & 62): performs a bitwise AND operation with the number 62 (which is 00111110 in binary).
	        											                     //      The result of this operation ensures that only bits 1-5 of switchs_all are retained.
	        											                     // (switchs_all & 63) - 70: After isolating the lower 6 bits, the expression subtracts 70 from this value.
	        											                     //      The use of 70 as a subtracted constant may be specific to the desired speed range or a particular application's logic.
                                                                             // ((switchs_all & 63) - 70) * -5: Finally, the result of the subtraction is multiplied by -5.
                                                                             //      multiplying by 5 scales the magnitude of the effect, If the value after subtraction is negative, multiplying by -5 makes the speed positive
	        if(prevSpeed!=speed)
	        {
	        	debug("current speed: ", speed, speed );
	        }


			//--------------------------
			if (sw0 == 1)                                                    // check switch 0
			{
				led_p->write(1, 0);
				direction = 0;
				i = 0;
			}
			else
			{
				led_p->write(1, i);
			    sleep_ms(speed);
			    led_p->write(0, i);
			    sleep_ms(speed);
			}
			//--------------------------
		}//---- end loop

		direction = 1;                                                       // change the direction when done
	}
	else//-------------------------------------------------------------------- Left direction
	{
		int i;
		for (i = n-2; i > 0; i--)                                            //from 14 to 1 for smooth transition
		{
			led_p->write(0, 0);                                              // to initialize bit 0 to 0 every time, because of sw0 function
			sw0 = sw_p->read(0);                                             // read the value of sw1
			switchs_all = sw_p->read();                                      // read all switches

			prevSpeed = speed;
			speed = ((switchs_all &63) - 70) * - 5;

			if(prevSpeed!=speed)
			{
				debug("current speed: " ,speed , speed );
			}


			//--------------------------
			if (sw0 == 1)                                                    // check switch 0
			{
				led_p->write(1, 0);
				direction = 0;
				i = 0;
			}
			else
			{
				led_p->write(1, i);
			    sleep_ms(speed);
			    led_p->write(0, i);
			    sleep_ms(speed);
			}
			//--------------------------
		}//---- end loop
		direction = 0;                                  // change the direction when done
	}



}//---- end function




//---------------------------------- Timer Check Function (blink once per second for 5 times.)
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

//---------------------------------- LED Check Function
void led_check(GpoCore *led_p, int n) {
   int i;

   for (i = 0; i < n; i++) {
      led_p->write(1, i);
      sleep_ms(200);
      led_p->write(0, i);
      sleep_ms(200);
   }
}


//---------------------------------- Switch input Check (leds flash according to switch positions.)
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

//---------------------------------- UART check (uart transmits test line.)
void uart_check() {
   static int loop = 0;

   uart.disp("uart test #");
   uart.disp(loop);
   uart.disp("\n\r");
   loop++;
}