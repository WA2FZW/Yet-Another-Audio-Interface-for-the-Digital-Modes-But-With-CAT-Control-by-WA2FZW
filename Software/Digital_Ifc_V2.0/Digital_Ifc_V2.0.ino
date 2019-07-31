/*
 *	Digital_Ifc_V01.2
 *
 *	This is a very dumb CAT control program designed to run on an Arduino Nano for
 *	my homebrew audio interface board for using digital modes on my old Swan-250C
 *	transceiver. I use it in conjunction with the DxCommander program which I use for
 *	the digital modes, however is should work with any CAT controller capable of using
 *	the Yaesu FT-891 command set.
 *
 *	I designed it for the Yaesu FT-891 command language as that is my primary radio and
 *	the message handling code was lifted from another project related to CAT control
 *	for that radio.
 *
 *	I call it a "dumb" CAT control program in that although it will respond correctly
 *	to any command or status request sent by DxCommander (or some other program), it
 *	doesn't really do anything with any of those messages other than the "TX" message,
 *	which will key (or un-key) the transmitter.
 *
 *	Commands from the controller program simply set values in memory for the various
 *	parameters and status requests for those values are replied to based on what is in
 *	the program's memory.
 */

#include <FT891_CAT.h>			// From: https://github.com/WA2FZW/An-FT-891-CAT-Control-Emulator-Library-by-WA2FZW

#define	BIT_RATE	115200		// Baud setting for the USB port


/*
 *	The only pins used are D4, which keys the transmitter PTT line via a solid
 *	state relay and D13 which turns the LED on the Arduino board on or off.	
 */

#define	PTT_PIN	 4				// Push-to-talk line
#define	LED_PIN	13				// The Arduino's built in LED


/*
 *	These definitions are for the on/off state of the LED
 */

#define	LED_ON	HIGH			// HIGH turns the LED on
#define	LED_OFF	LOW				// So LOW must turn it off!


	FT891_CAT	CAT;			// Make CAT control object


/*
 *	"setup" initializes everything the rest of the program needs to function properly
 */

void setup ()
{

/*
 *	Set up the LED:
 */

	pinMode ( LED_PIN, OUTPUT );				// Turns the Arduino's built-in LED on or off
	digitalWrite ( LED_PIN, LED_OFF );			// Turn the LED off


/*
 *	Start the serial monitor
 */

	Serial.begin ( BIT_RATE );					// Serial monitor runs at high speed

	CAT.begin ( PTT_PIN );						// Initialize CAT control stuff
}


/*
 *	The loop processing is pretty trivial. We see if DxCommander has anything to say,
 *	and if so, we read it and process it.
 */

void loop ()
{
	if ( CAT.CheckCAT() )							// Any new commands?
	{
		if ( CAT.GetTX() )							// Transmitter on?
			digitalWrite ( LED_PIN, LED_ON );		// Light the LED

		else										// Receiving
			digitalWrite ( LED_PIN, LED_OFF );		// So turn the LED off
	}

	Delay ( 25 );								// Only check every 25mS
}


/*
 *	"Delay" is a non-blocking delay function. The standard Arduino "delay" function
 *	turns off interrupts, which prevents the Serial I/O functions from working. That
 *	sometimes causes characters to be missed and junk commands to show up.
 */

void Delay ( unsigned long interval )
{
unsigned long currentTime = 0UL;

unsigned long startTime = millis();

	while ( startTime + interval > currentTime )
    	currentTime = millis();
}
