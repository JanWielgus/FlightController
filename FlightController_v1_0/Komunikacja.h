// Komunikacja.h
// Author: Jan Wielgus
// Date: 24.10.2017
//

#ifndef _KOMUNIKACJA_h
#define _KOMUNIKACJA_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include <PacketSerial.h>
#include "typyBitowe.h"
#include "config.h"



class KomunikacjaClass
{
// ==== FUNKCJE ====
 private:
	PacketSerial pSerial;
	uint8_t buforT[MAX_SEND_SIZE];
 public:
	void init();                     // inicjalizacja
	void odbierz();                  // wywo³ywanie odbierania
	void odbierzPriv(const uint8_t* buffer, size_t size);
	void wyslij(uint8_t typRamki);   // wysy³a odpowiedni typ ramki
	void updateSignal();             // aktualizuje stan syna³u. Wywo³ywane po odbieraniu
	bool isSignal();                 // true - jest sygan³; false - nie ma sygna³u
 private:
	bool sprawdzSumeKontr(const uint8_t* buffer, size_t manipPacketSize); //xor'owanie
	uint8_t liczSumeKontr(const uint8_t* buffer, size_t manipPacketSize);
	
// ==== ZMIENNE ====
 public:
	bitByte pong;           // 8 boolean'ów odebranych
	struct steering         // zmienne sterowania
	{
		int16_t throttle;  //                       |    0 - +1000
		int8_t tilt_TB;     // <0 is backward        | -120 - +120
		int8_t tilt_LR;     // <0 is left            | -120 - +120
		int8_t rotate;      // >0 - right; <0 - left | -120 - +120
		}pilot;
	floatByte zmiennaTestowa1; // do testów wysy³ania
	
	bitByte ping;           // 8 boolean'ów wysy³anych
	
 private:
	boolean ostatni_pong = false; // stan ostatniego odebranego pongu
	int zgubione_ramki = 0;       // liczba nieodebranych ramek
};

extern KomunikacjaClass kom;

#endif

