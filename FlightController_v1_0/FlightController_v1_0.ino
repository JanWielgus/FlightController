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



void setup()
{
	Serial.begin(9600);
	
	komun.init();
	sensors.init();
	motors.init();
	
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
	komun.updateSignalState(); // sprawd� czy jest sygna�
	
	komun.dodatkoweTx.b7 = !komun.dodatkoweTx.b7;// Zmiana stanu ping na przeciwny
	//battery_level = ;
	//wysokosc = ;
	
	komun.wyslij();
}


