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
	void odbierz();                  // wywo�ywanie odbierania
	void odbierzPriv(const uint8_t* buffer, size_t size);
	void wyslij(uint8_t typRamki);   // wysy�a odpowiedni typ ramki
	void updateSignal();             // aktualizuje stan syna�u. Wywo�ywane po odbieraniu
	bool isSignal();                 // true - jest sygan�; false - nie ma sygna�u
 private:
	bool sprawdzSumeKontr(const uint8_t* buffer, size_t manipPacketSize); //xor'owanie
	uint8_t liczSumeKontr(const uint8_t* buffer, size_t manipPacketSize);
	
// ==== ZMIENNE ====
 public:
	bitByte pong;           // 8 boolean'�w odebranych
	struct steering         // zmienne sterowania
	{
		int16_t throttle;  //                       |    0 - +1000
		int8_t tilt_TB;     // <0 is backward        | -120 - +120
		int8_t tilt_LR;     // <0 is left            | -120 - +120
		int8_t rotate;      // >0 - right; <0 - left | -120 - +120
		}pilot;
	floatByte zmiennaTestowa1; // do test�w wysy�ania
	
	bitByte ping;           // 8 boolean'�w wysy�anych
	
 private:
	boolean ostatni_pong = false; // stan ostatniego odebranego pongu
	int zgubione_ramki = 0;       // liczba nieodebranych ramek
};

extern KomunikacjaClass kom;

#endif

