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
 private:
	PacketSerial pSerial;
	uint8_t buforT[MAX_SEND_SIZE];
 public:
	void init();
	void odbierz();
	void odbierzPriv(const uint8_t* buffer, size_t size);
	void wyslij(uint8_t typRamki);
	bool isSignal(); // true - jest sygan³; false - nie ma sygna³u
	void updateSignal(); // aktualizuje stan syna³u
 private:
	bool sprawdzSumeKontr(const uint8_t* buffer, size_t manipPacketSize);	//xor'owanie
	uint8_t liczSumeKontr(const uint8_t* buffer, size_t manipPacketSize);
	
// ==== ZMIENNE ====
 public:
	bitByte pong;
	int zmienna1, zmienna2;
	floatByte zmiennaTestowa1;
	
	bitByte ping;
	
 private:
	boolean ostatni_pong = false; //stan ostatniego odebranego pongu
	boolean stan_sygnalu = false;
	int zgubione_ramki = 0; // liczba nieodebranych ramek
};

extern KomunikacjaClass kom;

#endif

