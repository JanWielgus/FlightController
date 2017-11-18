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
	copter.updateCommunication();
	copter.stabilize();
}


////////////////////////////////////////////////////////////////////// ====== END LOOP ======


