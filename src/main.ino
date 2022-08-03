#include <U8x8lib.h>

uint8_t pin_scl = 2;
uint8_t pin_sda = 0;
uint8_t pin_echo = 4;
uint8_t pin_trigger = 1;

/* create a instance of the OLED display */
U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(pin_scl, pin_sda);

void
setup()
{
	/* input pin from the sensor */
	pinMode(pin_echo, INPUT);

	/* output pin to trigger measuring */
	pinMode(pin_trigger, OUTPUT);
	digitalWrite(pin_trigger, LOW);

	/* initialize the display */
	u8x8.begin();
	u8x8.setFont(u8x8_font_chroma48medium8_r);

	/* tell the program is starting */
	u8x8.home();
	u8x8.clearLine(0);
	u8x8.print("Starting");
	delay(500);
}

void
loop()
{
	static double distance_cm = 0;
	static unsigned long time_microsecond = 0;

	/* wait 50 ms which is the minimum time between measuring. see the
	 * datasheet.
	 */
	delay(50);

	/* trigger the sensor by outputing 10ms pulse */
	digitalWrite(pin_trigger, HIGH);
	delayMicroseconds(10);
	digitalWrite(pin_trigger, LOW);

	/* read a pulse from pin_echo. the length of the pulse is linear to the
	 * distance.
	 */
	time_microsecond = pulseIn(pin_echo, HIGH);

	if (time_microsecond > 0) {

		/*
		 * time_microsecond is a round trip of the ultrasonic sound in
		 * microseconds.
		 *
		 * T: temperature
		 * speed of sound: 331.3 + (0.606 × T) m/sec
		 *
		 * when T = 25, 346.45 m/sec. or 0.034645 cm/microseconds.
		 */
		distance_cm = time_microsecond * 0.034645 / 2;
	} else {

		/* time out. no pulse in one second */
		return;
	}

	u8x8.home();
	u8x8.clearLine(0);
	u8x8.print(distance_cm);
	delay(1000);
}
