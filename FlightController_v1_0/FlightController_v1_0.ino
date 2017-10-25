/*	Author: Jan Wielgus
 *	Date: 10.08.2017r.
 *	
	Pierwsza wersja kontrolera lotu drona.
	Funkcje i mo�liwo�ci:
	- Sterowanie w trybie stabilizacji bez wspomagania
	- Komunikacja poprzez bibliotek� PacketSerial
	- Biblioteki obs�ugi akcelerometru, �yroskopu, barometru i magnetometru - Korneliusz Jarzebski
 */


#include <TimerOne.h>
#include "Komunikacja.h"
#include "Sensors.h"
#include "Motors.h"

#define TLmotorPin 5
#define TRmotorPin 6
#define BLmotorPin 9
#define BRmotorPin 10

// obiekty silnik�w
MotorsClass motorTL; // Top left
MotorsClass motorTR; // Top right
MotorsClass motorBL; // Back left
MotorsClass motorBR; // Back right

// obiekty serw (jako silniki)
Servo mTL;
Servo mTR;
Servo mBL;
Servo mBR;


void setup()
{
	Serial.begin(9600);
	
	komun.init();
	sensors.init();
	
	// przekazanie wska�nik�w na obiekty serw i piny silnik�w
	motorTL.init(&mTL, TLmotorPin);
	motorTR.init(&mTR, TRmotorPin);
	motorBL.init(&mBL, BLmotorPin);
	motorBR.init(&mBR, BRmotorPin);
	
	Timer1.initialize(100000); // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
	Timer1.attachInterrupt( timerIsr ); // attach the service routine here
	
	delay(1000);
}

void loop()
{
	
	if (komun.isSignal())
	{
		Serial.println("Jest sygna�!!!");
		sensors.updateDeltaTime();
		sensors.readAngles();
		
	}
	else
	{
		Serial.println("Nie ma sygna�u :(");
	}
}


////////////////////////////////////////////////////////////////////// ====== END LOOP ======



// -- Funkcja uruchamiana co 0.1s (10Hz) --
void timerIsr()
{
	Serial.println("P�tla kom.");
	
	komun.odbierz();
	komun.updateSignalState();
	
	komun.dodatkoweTx.b7 = !komun.dodatkoweTx.b7;// Zmiana stanu ping na przeciwny
	//battery_level = ;
	//wysokosc = ;
	
	komun.wyslij();
}


