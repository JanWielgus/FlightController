// Kod pilota drona do wersji 1.0
//
// Author: Jan Wielgus
// Date: 26.10.2017r.
//

#include <TimerOne.h>
#include "Komunikacja.h"


void setup()
{
	Serial.begin(9600);
	komun.init();
	
	Timer1.initialize(100000); // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
	Timer1.attachInterrupt( timerIsr ); // attach the service routine here
}

void loop()
{
	
}



// -- Funkcja uruchamiana co 0.1s (10Hz) --
void timerIsr()
{
	Serial.println("dziala");
	komun.odbierz();
	//komun.updateSignalState(); // sprawdü czy jest sygna≥
	
		
	komun.wyslij(RAMKA_STER_TYPE);
	komun.wyslij(RAMKA_DANE_TYPE);
}
