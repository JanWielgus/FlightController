/*	Author: Jan Wielgus
 *	Date: 10.08.2017r.
 *	
	Pierwsza wersja kontrolera lotu drona.
	Funkcje i mo�liwo�ci:
	- Sterowanie w trybie stabilizacji bez wspomagania
	- Komunikacja poprzez bibliotek� PacketSerial
	- Biblioteki obs�ugi akcelerometru, �yroskopu, barometru i magnetometru - Korneliusz Jarzebski
 */

#include "Control.h"
// reszra includ'�w w piliku powy�ej



void setup()
{
	copter.init(); // ca�a konfiguracja wst�pna
}


void loop()
{
	uint32_t loop_start_time = micros();
	
	copter.updateCommunication();
	copter.stabilize();
	
	// Sta�y czas wykonywania p�tli
	int16_t temporary = DELTA_TIME-(micros()-loop_start_time);
	delayMicroseconds(temporary>=0?temporary:0);  // Wype�niaj pozosta�y czas
}


////////////////////////////////////////////////////////////////////// ====== END LOOP ======


