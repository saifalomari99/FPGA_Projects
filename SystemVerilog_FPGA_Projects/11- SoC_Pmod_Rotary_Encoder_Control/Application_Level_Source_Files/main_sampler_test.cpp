/*****************************************************************//**
 * @file main_sampler_test.cpp
 *
 * @brief Basic test of nexys4 ddr mmio cores
 *
 * @author p chu
 * @version v1.0: initial release
 *********************************************************************/

#define _DEBUG
#include "chu_init.h"
#include "gpio_cores.h"
#include "xadc_core.h"
#include "sseg_core.h"
#include "spi_core.h"
#include "i2c_core.h"
#include "ps2_core.h"
#include "ddfs_core.h"
#include "adsr_core.h"
#include "pmod_core.h"


//------------------------------------------------- original test functions:
void timer_check(GpoCore *led_p);
void led_check(GpoCore *led_p, int n);
void sw_check(GpoCore *led_p, GpiCore *sw_p);
void uart_check();
void adc_check(XadcCore *adc_p, GpoCore *led_p);
void pwm_3color_led_check(PwmCore *pwm_p);
void debounce_check(DebounceCore *db_p, GpoCore *led_p);
void sseg_check(SsegCore *sseg_p);
void gsensor_check(SpiCore *spi_p, GpoCore *led_p);
void adt7420_check(I2cCore *adt7420_p, GpoCore *led_p);
void ps2_check(Ps2Core *ps2_p);
void ddfs_check(DdfsCore *ddfs_p, GpoCore *led_p);
void adsr_check(AdsrCore *adsr_p, GpoCore *led_p, GpiCore *sw_p);
void show_test_id(int n, GpoCore *led_p);
//----------------------------------------------- New testing functions
void pmod_test(GpoCore *led, PmodCore *enc, SsegCore *sseg_p, PwmCore *pwm_p);
void rgb_controlled_pmod(GpoCore *led,PmodCore *enc, SsegCore *sseg, PwmCore *pwm );
void chase_LED (GpoCore *led_p, GpiCore *sw_p, int n , XadcCore *adc_p);


//------------------------------------------------- declare classes
GpoCore led(get_slot_addr(BRIDGE_BASE, S2_LED));
GpiCore sw(get_slot_addr(BRIDGE_BASE, S3_SW));
XadcCore adc(get_slot_addr(BRIDGE_BASE, S5_XDAC));
PwmCore pwm(get_slot_addr(BRIDGE_BASE, S6_PWM));
DebounceCore btn(get_slot_addr(BRIDGE_BASE, S7_BTN));
SsegCore sseg(get_slot_addr(BRIDGE_BASE, S8_SSEG));
SpiCore spi(get_slot_addr(BRIDGE_BASE, S9_SPI));
I2cCore adt7420(get_slot_addr(BRIDGE_BASE, S10_I2C));
Ps2Core ps2(get_slot_addr(BRIDGE_BASE, S11_PS2));
DdfsCore ddfs(get_slot_addr(BRIDGE_BASE, S12_DDFS));
AdsrCore adsr(get_slot_addr(BRIDGE_BASE, S13_ADSR), &ddfs);

//------------- New class
PmodCore enc(get_slot_addr(BRIDGE_BASE, S4_PMOD));                                        // initialize the class





//========================================================== main ================================================================
int main()
{
	// -------------------- initialize the switches for selecting the functions
	int sw13 = sw.read(13);
	int sw14 = sw.read(14);
	int sw15 = sw.read(15);

   timer_check(&led);
   while (1)
   {
	   if (sw13 == 1)
	   {
		   pmod_test(&led, &enc, &sseg, &pwm);
	   }
	   else if (sw14 == 1)
	   {
		   chase_LED (&led, &sw,16, &adc);
	   }
	   else if (sw15 == 1)
	   {
		   rgb_controlled_pmod (&led, &enc, &sseg, &pwm);
	   }
	   else
	   {
		   pmod_test(&led, &enc, &sseg, &pwm);
	   }

	// ------------- new test functions
	  //rgb_controlled_pmod (&led, &enc, &sseg, &pwm);
	  //pmod_test(&led, &enc, &sseg, &pwm);
	  //chase_LED (&led, &sw,16, &adc);

	// ------------ origanal test functions
      //show_test_id(1, &led);
      //led_check(&led, 16);
      //sw_check(&led, &sw);
      //show_test_id(3, &led);
      //uart_check();
      //debug("main - switch value / up time : ", sw.read(), now_ms());
      //show_test_id(5, &led);
      //adc_check(&adc, &led);
      //show_test_id(6, &led);
      //pwm_3color_led_check(&pwm);
      //show_test_id(7, &led);
      //debounce_check(&btn, &led);
      //show_test_id(8, &led);
      //sseg_check(&sseg);
      //show_test_id(9, &led);
      //gsensor_check(&spi, &led);
      //show_test_id(10, &led);
      //adt7420_check(&adt7420, &led);
      //show_test_id(11, &led);
      //ps2_check(&ps2);
      //show_test_id(12, &led);
      //ddfs_check(&ddfs, &led);
      //show_test_id(13, &led);
      //adsr_check(&adsr, &led, &sw);
   } //while
} //main
//========================================================== end main ================================================================



// --------------------------------- function to test the pmod core
void pmod_test(GpoCore *led_p,PmodCore *enc, SsegCore *sseg_p, PwmCore *pwm_p)
{

			// ------------- Initialize the pins:
			int A = enc->read(0);
			int B = enc->read(1);
			int btn = enc->read(2);
			int sw = enc->read(3);


			// ------------------------- LEDs indicators
			if (A == 1)
			{
				led_p->write(1,0);
			}
			if (B == 1)
			{
				led_p->write(1,1);
			}
			if(sw == 1)
			{
				led_p->write(1,14);
			}
			if (btn == 1)
			{
				led_p->write(1,15);
			}

			//-------------------------- counter
			static double counter = 0;
			if (A == 1)                                    // moving the pmod right
			{
				A = 0;
				counter = counter + 1;
				sleep_ms(20);
				A = 0;
			}
			if (B == 1)                                    // moving the pmod left
			{
				B = 0;
				counter = counter - 1;
				sleep_ms(20);
				B = 0;
			}
			if (btn == 1)                                   // btn = reset the counter to 0
			{
				counter = 0;
			}

			// -------- handle the limits of the counter
			if (counter < 0)
			{
				counter = 0; // Or handle wrap-around as required
			}
			else if (counter > 100)
			{
				counter = 100;
			}

		int a, b , c;
		// Calculate digits for ones, tens, and hundreds places
		a = int(counter / 100.0) % 10;  // Hundreds
		b = int(counter / 10.0) % 10;   // Tens
		c = int(counter) % 10 ;          // Ones

		// Assuming sseg_p->h2s() converts an integer to the appropriate segment pattern
		sseg_p->set_dp(0);  // Assuming this sets a decimal point correctly if needed
		sseg_p->write_1ptn(sseg_p->h2s(c), 0);  // Display hundreds
		sseg_p->write_1ptn(sseg_p->h2s(b), 1);  // Display tens
		sseg_p->write_1ptn(sseg_p->h2s(a), 2);  // Display units


		sleep_ms(40);
		led_p->write(0,0);
		led_p->write(0,1);
		led_p->write(0,14);
		led_p->write(0,15);


}// end function




// ============================================================= RGB controlled pmod function
void rgb_controlled_pmod(GpoCore *led_p, PmodCore *enc, SsegCore *sseg_p, PwmCore *pwm_p)
{
	// -------------------------- Initialize the pins:
	int A = enc->read(0);
	int B = enc->read(1);
	int btn = enc->read(2);
	int sw = enc->read(3);
	sseg_p->set_dp(0);  // Assuming this sets a decimal point correctly if needed

	// -------------------------- LEDs indicators for pmod
				if (A == 1)
				{
					led_p->write(1,0);
				}
				if (B == 1)
				{
					led_p->write(1,1);
				}
				if(sw == 1)
				{
					led_p->write(1,14);
				}
				if (btn == 1)
				{
					led_p->write(1,15);
				}

				// Turn off the leds
				sleep_ms(35);
				led_p->write(0,0);
				led_p->write(0,1);
				led_p->write(0,14);
				led_p->write(0,15);


	//-------------------------------------selected color--------------------------------------------------------------
			//------------------------ RGB 1
			//pwm_p->set_duty(0, 0);                 // Blue
			//pwm_p->set_duty(0.54, 1);              // Green
			//pwm_p->set_duty(0, 2);                 // Red

			// ----------------------- RGB 2
			//pwm_p->set_duty(0, 3);                 // Blue
			//pwm_p->set_duty(1.0, 4);               // Green
			//pwm_p->set_duty(0, 5);                 // Red


			static int selected_color = 0;

			if (btn == 1)                                       // when you press the btn, you change to other color to be modified
			{
				selected_color = selected_color + 1;
				sleep_ms(200);
				if (selected_color > 2)
				{
					selected_color = 0;
				}
			}

			if (selected_color == 0)            // --------------------------------------- Blue
			{
				pwm_p->set_duty(1.0, 0);
				pwm_p->set_duty(0, 1);
				pwm_p->set_duty(0, 2);

			}
			else if (selected_color == 1)        // ------------------------------------ Green
			{
				pwm_p->set_duty(0, 0);
				pwm_p->set_duty(1.0, 1);
				pwm_p->set_duty(0, 2);

			}
			else if (selected_color == 2)        // ------------------------------------ Red
			{
				pwm_p->set_duty(0, 0);
				pwm_p->set_duty(0, 1);
				pwm_p->set_duty(1.0, 2);

			}

	 //---------------------------------- Changing Color -------------------------------------------------------------
			int changing_color = selected_color + 3;


			static double counter_blue = 0;
			static double counter_green = 0;
			static double counter_red = 0;


			if (changing_color == 3)     // -------------------------------------------- Blue
			{
				if (A == 1)
				{
						A = 0;
						counter_blue = counter_blue + 1;
						sleep_ms(20);
						A = 0;
				}
				if (B == 1)
				{
						B = 0;
						counter_blue = counter_blue - 1;
						sleep_ms(20);
						B = 0;
				}
				// --------- control the limits of the counter
				if (counter_blue < 0)
				{
					counter_blue = 0; // Or handle wrap-around as required
				}
				else if (counter_blue > 100)
				{
					counter_blue = 100;
				}



						// ------------------------- set the 7 segments
						int a_b, b_b , c_b;
						// Calculate digits for ones, tens, and hundreds places
						a_b = int(counter_blue / 100.0) % 10;  // Hundreds
						b_b = int(counter_blue / 10.0) % 10;   // Tens
						c_b = int(counter_blue) % 10 ;          // Ones

						// Assuming sseg_p->h2s() converts an integer to the appropriate segment pattern
						sseg_p->set_dp(0);  // Assuming this sets a decimal point correctly if needed
						sseg_p->write_1ptn(sseg_p->h2s(c_b), 0);  // Display hundreds
						sseg_p->write_1ptn(sseg_p->h2s(b_b), 1);  // Display tens
						sseg_p->write_1ptn(sseg_p->h2s(a_b), 2);  // Display units

			}
			if (changing_color == 4)       // ------------------------------------------- green
			{
				if (A == 1)
				{
						A = 0;
						counter_green = counter_green + 1;
						sleep_ms(20);
						A = 0;
				}
				if (B == 1)
				{
						B = 0;
						counter_green = counter_green - 1;
						sleep_ms(20);
						B = 0;
				}


						// ------------------------- set the 7 segments
						int a_g, b_g , c_g;
						// Calculate digits for ones, tens, and hundreds places
						a_g = int(counter_green / 100.0) % 10;  // Hundreds
						b_g = int(counter_green / 10.0) % 10;   // Tens
						c_g = int(counter_green) % 10 ;          // Ones

						// Assuming sseg_p->h2s() converts an integer to the appropriate segment pattern
						sseg_p->set_dp(0);  // Assuming this sets a decimal point correctly if needed
						sseg_p->write_1ptn(sseg_p->h2s(c_g), 0);  // Display hundreds
						sseg_p->write_1ptn(sseg_p->h2s(b_g), 1);  // Display tens
						sseg_p->write_1ptn(sseg_p->h2s(a_g), 2);  // Display units
			}
			if (changing_color == 5)      // --------------------------------------------- red
			{
				if (A == 1)
				{
						A = 0;
						counter_red = counter_red + 1;
						sleep_ms(20);
						A = 0;
				}
				if (B == 1)
				{
						B = 0;
						counter_red = counter_red - 1;
						sleep_ms(20);
						B = 0;
				}


						// ------------------------- set the 7 segments
						int a_r, b_r , c_r;
						// Calculate digits for ones, tens, and hundreds places
						a_r = int(counter_red / 100.0) % 10;  // Hundreds
						b_r = int(counter_red / 10.0) % 10;   // Tens
						c_r = int(counter_red) % 10 ;          // Ones

						// Assuming sseg_p->h2s() converts an integer to the appropriate segment pattern
						sseg_p->set_dp(0);  // Assuming this sets a decimal point correctly if needed
						sseg_p->write_1ptn(sseg_p->h2s(c_r), 0);  // Display hundreds
						sseg_p->write_1ptn(sseg_p->h2s(b_r), 1);  // Display tens
						sseg_p->write_1ptn(sseg_p->h2s(a_r), 2);  // Display units
			}

			pwm_p->set_duty(counter_blue / 100.0, 3);
			pwm_p->set_duty(counter_green / 100.0, 4);
			pwm_p->set_duty(counter_red / 100.0, 5);

} // end function







//---------------------------------- chasing LED Function
void chase_LED (GpoCore *led_p, GpiCore *sw_p, int n, XadcCore *adc_p)
{
	static int direction = 0;
	int sw0;                                                                 // initialize the process
	//int switchs_all;                                                         // Control the speed
	static int speed;
	static int prevSpeed;                                                    // to hold the value of the previous speed for displaying to the UART
	double raw;

	if (direction == 0)//---------------------------------------------------- right direction
	{
		int i;
		for (i = 0; i < n; i++)
		{
			led_p->write(0, 0);                                              // to initialize bit 0 to 0 every time, because of sw0 function
			sw0 = sw_p->read(0);                                             // read the value of sw1

			prevSpeed = speed;
	      

			// Use ADC reading for speed control
	      raw = adc_p->read_adc_in(0);       // Reading from ADC channel 0         value from 0 and 1 must be float
	      //debug("raw value: ", raw, raw );

	      speed = (int)(raw * 1000); // Example formula to convert ADC reading to speed

	      if (speed < 50)
	      {
            speed = 50;
	      }
	      if (speed > 500)
	      {
	         speed = 450;
	      }


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

			prevSpeed = speed;

			// Use ADC reading for speed control
			raw = adc_p->read_adc_in(0);       // Reading from ADC channel 0
			speed = (int)(raw * 1000); // Example formula to convert ADC reading to speed

			if (speed < 50)
			{
				speed = 50;
			}
			if (speed > 500)
			{
				speed = 450;
			}



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










// ----------------------------------- blink once per second for 5 times (provide a sanity check for timer (based on SYS_CLK_FREQ))
void timer_check(GpoCore *led_p) {
   int i;

   for (i = 0; i < 5; i++) {
      led_p->write(0xffff);
      sleep_ms(200);
      led_p->write(0x0000);
      sleep_ms(200);
      debug("timer check - (loop #)/now: ", i, now_ms());
   }
}


// ----------------------------------- check individual led
void led_check(GpoCore *led_p, int n) {
   int i;

   for (i = 0; i < n; i++) {
      led_p->write(1, i);
      sleep_ms(100);
      led_p->write(0, i);
      sleep_ms(100);
   }
}



// ----------------------------------- leds flash according to switch positions.
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



// ----------------------------------- uart transmits test line.
// uart instance is declared as global variable in chu_io_basic.h
void uart_check()
{
   static int loop = 0;

   uart.disp("uart test #");
   uart.disp(loop);
   uart.disp("\n\r");
   loop++;
}



// ----------------------------------- read FPGA internal voltage temperature
void adc_check(XadcCore *adc_p, GpoCore *led_p) {
   double reading;
   int n, i;
   uint16_t raw;

   for (i = 0; i < 5; i++)
   {
      // display 12-bit channel 0 reading in LED
      raw = adc_p->read_raw(0);                                           // read the data from register 0 = ADC_0_REG
      raw = raw >> 4;
      led_p->write(raw);

      // display on-chip sensor and 4 channels in console
      uart.disp("FPGA vcc/temp: ");
      reading = adc_p->read_fpga_vcc();
      uart.disp(reading, 3);
      uart.disp(" / ");
      reading = adc_p->read_fpga_temp();
      uart.disp(reading, 3);
      uart.disp("\n\r");

      for (n = 0; n < 4; n++)                                     // read all 4 auxiliary channels
      {
         uart.disp("analog channel/voltage: ");
         uart.disp(n);
         uart.disp(" / ");
         reading = adc_p->read_adc_in(n);
         uart.disp(reading, 3);
         uart.disp("\n\r");
      } // end for

      sleep_ms(2000);                                            // wait for 2000 = 2 seconds

   }// --------------- end upper for loop
}


// ----------------------------------- tri-color led dims gradually
void pwm_3color_led_check(PwmCore *pwm_p) {
   int i, n;
   double bright, duty;
   const double P20 = 1.2589;  // P20=100^(1/20); i.e., P20^20=100

   pwm_p->set_freq(50);
   for (n = 0; n < 3; n++) {
      bright = 1.0;
      for (i = 0; i < 20; i++) {
         bright = bright * P20;
         duty = bright / 100.0;
         pwm_p->set_duty(duty, n);
         pwm_p->set_duty(duty, n + 3);
         sleep_ms(100);
      }
      sleep_ms(300);
      pwm_p->set_duty(0.0, n);
      pwm_p->set_duty(0.0, n + 3);
   }
}


// ----------------------------------- Test debounced buttons (count transitions of normal and debounced button)
void debounce_check(DebounceCore *db_p, GpoCore *led_p) {
   long start_time;
   int btn_old, db_old, btn_new, db_new;
   int b = 0;
   int d = 0;
   uint32_t ptn;

   start_time = now_ms();
   btn_old = db_p->read();
   db_old = db_p->read_db();
   do {
      btn_new = db_p->read();
      db_new = db_p->read_db();
      if (btn_old != btn_new) {
         b = b + 1;
         btn_old = btn_new;
      }
      if (db_old != db_new) {
         d = d + 1;
         db_old = db_new;
      }
      ptn = d & 0x0000000f;
      ptn = ptn | (b & 0x0000000f) << 4;
      led_p->write(ptn);
   } while ((now_ms() - start_time) < 5000);
}


// ----------------------------------- Test pattern in 7-segment LEDs
void sseg_check(SsegCore *sseg_p) {
   int i, n;
   uint8_t dp;

   //turn off led
   for (i = 0; i < 8; i++) {
      sseg_p->write_1ptn(0xff, i);
   }
   //turn off all decimal points
   sseg_p->set_dp(0x00);

   // display 0x0 to 0xf in 4 epochs
   // upper 4  digits mirror the lower 4
   for (n = 0; n < 4; n++) {
      for (i = 0; i < 4; i++) {
         sseg_p->write_1ptn(sseg_p->h2s(i + n * 4), 3 - i);
         sseg_p->write_1ptn(sseg_p->h2s(i + n * 4), 7 - i);
         sleep_ms(300);
      } // for i
   }  // for n
      // shift a decimal point 4 times
   for (i = 0; i < 4; i++) {
      bit_set(dp, 3 - i);
      sseg_p->set_dp(1 << (3 - i));
      sleep_ms(300);
   }
   //turn off led
   for (i = 0; i < 8; i++) {
      sseg_p->write_1ptn(0xff, i);
   }
   //turn off all decimal points
   sseg_p->set_dp(0x00);

}



// ----------------------------------- Test adxl362 accelerometer using SPI
void gsensor_check(SpiCore *spi_p, GpoCore *led_p) {
   const uint8_t RD_CMD = 0x0b;
   const uint8_t PART_ID_REG = 0x02;
   const uint8_t DATA_REG = 0x08;
   const float raw_max = 127.0 / 2.0;  //128 max 8-bit reading for +/-2g

   int8_t xraw, yraw, zraw;
   float x, y, z;
   int id;

   spi_p->set_freq(400000);
   spi_p->set_mode(0, 0);
   // check part id
   spi_p->assert_ss(0);    // activate
   spi_p->transfer(RD_CMD);  // for read operation
   spi_p->transfer(PART_ID_REG);  // part id address
   id = (int) spi_p->transfer(0x00);
   spi_p->deassert_ss(0);
   uart.disp("read ADXL362 id (should be 0xf2): ");
   uart.disp(id, 16);
   uart.disp("\n\r");
   // read 8-bit x/y/z g values once
   spi_p->assert_ss(0);    // activate
   spi_p->transfer(RD_CMD);  // for read operation
   spi_p->transfer(DATA_REG);  //
   xraw = spi_p->transfer(0x00);
   yraw = spi_p->transfer(0x00);
   zraw = spi_p->transfer(0x00);
   spi_p->deassert_ss(0);
   x = (float) xraw / raw_max;
   y = (float) yraw / raw_max;
   z = (float) zraw / raw_max;
   uart.disp("x/y/z axis g values: ");
   uart.disp(x, 3);
   uart.disp(" / ");
   uart.disp(y, 3);
   uart.disp(" / ");
   uart.disp(z, 3);
   uart.disp("\n\r");
}


// ----------------------------------- read temperature from adt7420 (adt7420_p pointer to adt7420 instance)
void adt7420_check(I2cCore *adt7420_p, GpoCore *led_p) {
   const uint8_t DEV_ADDR = 0x4b;
   uint8_t wbytes[2], bytes[2];
   //int ack;
   uint16_t tmp;
   float tmpC;

   // read adt7420 id register to verify device existence
   // ack = adt7420_p->read_dev_reg_byte(DEV_ADDR, 0x0b, &id);

   wbytes[0] = 0x0b;
   adt7420_p->write_transaction(DEV_ADDR, wbytes, 1, 1);
   adt7420_p->read_transaction(DEV_ADDR, bytes, 1, 0);
   uart.disp("read ADT7420 id (should be 0xcb): ");
   uart.disp(bytes[0], 16);
   uart.disp("\n\r");
   //debug("ADT check ack/id: ", ack, bytes[0]);
   // read 2 bytes
   //ack = adt7420_p->read_dev_reg_bytes(DEV_ADDR, 0x0, bytes, 2);
   wbytes[0] = 0x00;
   adt7420_p->write_transaction(DEV_ADDR, wbytes, 1, 1);
   adt7420_p->read_transaction(DEV_ADDR, bytes, 2, 0);

   // conversion
   tmp = (uint16_t) bytes[0];
   tmp = (tmp << 8) + (uint16_t) bytes[1];
   if (tmp & 0x8000) {
      tmp = tmp >> 3;
      tmpC = (float) ((int) tmp - 8192) / 16;
   } else {
      tmp = tmp >> 3;
      tmpC = (float) tmp / 16;
   }
   uart.disp("temperature (C): ");
   uart.disp(tmpC);
   uart.disp("\n\r");
   led_p->write(tmp);
   sleep_ms(1000);
   led_p->write(0);
}





// -----------------------------------
void ps2_check(Ps2Core *ps2_p) {
   int id;
   int lbtn, rbtn, xmov, ymov;
   char ch;
   unsigned long last;

   uart.disp("\n\rPS2 device (1-keyboard / 2-mouse): ");
   id = ps2_p->init();
   uart.disp(id);
   uart.disp("\n\r");
   last = now_ms();
   do {
      if (id == 2) {  // mouse
         if (ps2_p->get_mouse_activity(&lbtn, &rbtn, &xmov, &ymov)) {
            uart.disp("[");
            uart.disp(lbtn);
            uart.disp(", ");
            uart.disp(rbtn);
            uart.disp(", ");
            uart.disp(xmov);
            uart.disp(", ");
            uart.disp(ymov);
            uart.disp("] \r\n");
            last = now_ms();

         }   // end get_mouse_activitiy()
      } else {
         if (ps2_p->get_kb_ch(&ch)) {
            uart.disp(ch);
            uart.disp(" ");
            last = now_ms();
         } // end get_kb_ch()
      }  // end id==2
   } while (now_ms() - last < 5000);
   uart.disp("\n\rExit PS2 test \n\r");

}



// ----------------------------------- play primary notes with ddfs
// ddfs_p pointer to ddfs core, music tempo is defined as beats of quarter-note per minute. 60 bpm is 1 sec per quarter note
// "click" sound due to abrupt stop of a note
void ddfs_check(DdfsCore *ddfs_p, GpoCore *led_p) {
   int i, j;
   float env;

   //vol = (float)sw.read_pin()/(float)(1<<16),
   ddfs_p->set_env_source(0);  // select envelop source
   ddfs_p->set_env(0.0);   // set volume
   sleep_ms(500);
   ddfs_p->set_env(1.0);   // set volume
   ddfs_p->set_carrier_freq(262);
   sleep_ms(2000);
   ddfs_p->set_env(0.0);   // set volume
   sleep_ms(2000);
   // volume control (attenuation)
   ddfs_p->set_env(0.0);   // set volume
   env = 1.0;
   for (i = 0; i < 1000; i++) {
      ddfs_p->set_env(env);
      sleep_ms(10);
      env = env / 1.0109; //1.0109**1024=2**16
   }
   // frequency modulation 635-912 800 - 2000 siren sound
   ddfs_p->set_env(1.0);   // set volume
   ddfs_p->set_carrier_freq(635);
   for (i = 0; i < 5; i++) {               // 10 cycles
      for (j = 0; j < 30; j++) {           // sweep 30 steps
         ddfs_p->set_offset_freq(j * 10);  // 10 Hz increment
         sleep_ms(25);
      } // end j loop
   } // end i loop
   ddfs_p->set_offset_freq(0);
   ddfs_p->set_env(0.0);   // set volume
   sleep_ms(1000);
}



// ----------------------------------- play primary notes with ddfs
void adsr_check(AdsrCore *adsr_p, GpoCore *led_p, GpiCore *sw_p) {
   const int melody[] = { 0, 2, 4, 5, 7, 9, 11 };
   int i, oct;

   adsr_p->init();
   // no adsr envelop and  play one octave
   adsr_p->bypass();
   for (i = 0; i < 7; i++) {
      led_p->write(bit(i));
      adsr_p->play_note(melody[i], 3, 500);
      sleep_ms(500);
   }
   adsr_p->abort();
   sleep_ms(1000);
   // set and enable adsr envelop
   // play 4 octaves
   adsr_p->select_env(sw_p->read());
   for (oct = 3; oct < 6; oct++) {
      for (i = 0; i < 7; i++) {
         led_p->write(bit(i));
         adsr_p->play_note(melody[i], oct, 500);
         sleep_ms(500);
      }
   }
   led_p->write(0);
   // test duration
   sleep_ms(1000);
   for (i = 0; i < 4; i++) {
      adsr_p->play_note(0, 4, 500 * i);
      sleep_ms(500 * i + 1000);
   }
}



// ----------------------------------- core test
void show_test_id(int n, GpoCore *led_p) {
   int i, ptn;

   ptn = n; //1 << n;
   for (i = 0; i < 20; i++) {
      led_p->write(ptn);
      sleep_ms(30);
      led_p->write(0);
      sleep_ms(30);
   }
}





