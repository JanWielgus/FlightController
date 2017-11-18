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
	copter.updateCommunication();
	copter.stabilize();
}


////////////////////////////////////////////////////////////////////// ====== END LOOP ======


