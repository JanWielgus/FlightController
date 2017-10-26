/*	Author: Jan Wielgus
 *	Date: 10.08.2017r.
 *	
	Pierwsza wersja kontrolera lotu drona.
	Funkcje i mo¿liwoœci:
	- Sterowanie w trybie stabilizacji bez wspomagania
	- Komunikacja poprzez bibliotekê PacketSerial
	- Biblioteki obs³ugi akcelerometru, ¿yroskopu, barometru i magnetometru - Korneliusz Jarzebski
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
		Serial.println("Jest sygna³!!!");
		sensors.updateDeltaTime();
		sensors.readAngles();
		
	}
	else
	{
		Serial.println("Nie ma sygna³u :(");
	}
}


////////////////////////////////////////////////////////////////////// ====== END LOOP ======



// -- Funkcja uruchamiana co 0.1s (10Hz) --
void timerIsr()
{
	Serial.println("Pêtla kom.");
	
	komun.odbierz();
	komun.updateSignalState(); // sprawdŸ czy jest sygna³
	
	komun.dodatkoweTx.b7 = !komun.dodatkoweTx.b7;// Zmiana stanu ping na przeciwny
	//battery_level = ;
	//wysokosc = ;
	
	komun.wyslij();
}


