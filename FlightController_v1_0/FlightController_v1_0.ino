/*	Author: Jan Wielgus
 *	Date: 10.08.2017r.
 *	
	Pierwsza wersja kontrolera lotu drona.
	Funkcje i mo¿liwoœci:
	- Sterowanie w trybie stabilizacji bez wspomagania
	- Komunikacja poprzez bibliotekê PacketSerial
	- Biblioteki obs³ugi akcelerometru, ¿yroskopu, barometru i magnetometru - Korneliusz Jarzebski
 */

#include "Control.h"
// reszra includ'ów w piliku powy¿ej



void setup()
{
	copter.init(); // ca³a konfiguracja wstêpna
}


void loop()
{
	uint32_t loop_start_time = micros();
	
	copter.updateCommunication();
	copter.stabilize();
	
	// Sta³y czas wykonywania pêtli
	int16_t temporary = DELTA_TIME-(micros()-loop_start_time);
	delayMicroseconds(temporary>=0?temporary:0);  // Wype³niaj pozosta³y czas
}


////////////////////////////////////////////////////////////////////// ====== END LOOP ======


