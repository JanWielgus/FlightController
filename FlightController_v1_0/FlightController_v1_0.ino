/*	Author: Jan Wielgus
 *	Date: 10.08.2017r.
 *	
	Pierwsza wersja kontrolera lotu drona.
	Funkcje i mo¿liwoœci:
	- Sterowanie w trybie stabilizacji bez wspomagania
	- Komunikacja poprzez bibliotekê PacketSerial
	- Biblioteki obs³ugi akcelerometru, ¿yroskopu, barometru i magnetometru - Korneliusz Jarzebski
 */

#include "Komunikacja.h"
#include "Sensors.h"
#include "Motors.h"


uint32_t last_loop = 0; // czas ostatniego wykonania funkcji komunikacji




void setup()
{
	kom.init();
	sensors.init();
	motors.init();
	
	delay(500);
}


void loop()
{
	int value = map(kom.zmienna1, 10, 1023, 0, 1000);
	value = constrain(value, 0, 1000);
	//if (value > 1010) value = 1200;
	motors.setOnAllMotors(value);
	
	
	sensors.updateDeltaTime();
	
	
	uint32_t timenow = millis();
	if ((timenow - last_loop) > 50)
	{
		kom.odbierz();
		kom.updateSignal();
		
		/*
		if (kom.isSignal())
		{
			kom.zmienna1 = 150;
		}
		else kom.zmienna1 = 0;
		*/
		
		//kom.zmiennaTestowa1.value = 1723.5;
		//kom.wyslij(DRON_RAMKA_TEST_TYPE);
		
		
		last_loop = millis();
	}
	
}


////////////////////////////////////////////////////////////////////// ====== END LOOP ======


