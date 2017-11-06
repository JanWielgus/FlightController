/*	Author: Jan Wielgus
 *	Date: 10.08.2017r.
 *	
	Pierwsza wersja kontrolera lotu drona.
	Funkcje i mo�liwo�ci:
	- Sterowanie w trybie stabilizacji bez wspomagania
	- Komunikacja poprzez bibliotek� PacketSerial
	- Biblioteki obs�ugi akcelerometru, �yroskopu, barometru i magnetometru - Korneliusz Jarzebski
 */

#include "Komunikacja.h"
#include "Sensors.h"
#include "Motors.h"
#include "PID.h"


uint32_t last_loop = 0; // czas ostatniego wykonania funkcji komunikacji




void setup()
{
	kom.init();
	sensors.init();
	motors.init();
	levelX_PID.init(kP_level, kI_level, kD_level);
	levelY_PID.init(kP_level, kI_level, kD_level);
	
	delay(500);
}


void loop()
{
	sensors.readAngles();
	
	int value = map(kom.zmienna1, 10, 1023, 0, 1000);
	value = constrain(value, 0, 1000);
	//if (value > 1010) value = 1200;
	
	if (value != 0) value += levelX_PID.getPID(sensors.pitch, sensors.dt_);
	//value += sensors.pitch*2;
	
	motors.setOnAllMotors(value);
	

	
	
	uint32_t timenow = millis();
	if ((timenow - last_loop) > 48)
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


