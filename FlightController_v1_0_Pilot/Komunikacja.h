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
#include <SoftwareSerial.h>
#include "typyBitowe.h"
#include "config.h"


class KomunikacjaClass
{
 public:
	PacketSerial pSerial;
	uint8_t buforT[MAX_SEND_SIZE];
	
// ===== FUNKCJE =====
 public:
	void init(SoftwareSerial *_softwareserial);
	void odbierz();
	void odbierzPriv(const uint8_t* bufferR, size_t PacketSize);
	void wyslij(uint8_t typRamki);
	bool sprawdzSumeKontr(const uint8_t* buffer, size_t PacketSize);
	uint8_t liczSumeKontr(const uint8_t* buffer, size_t PacketSize);
	
// ===== ZMIENNE =====
 public:
	//wysy³ane
	int zmienna1, zmienna2;
	bitByte ping;
	floatByte zmiennaTestowa;
	
	//odebrane
	bitByte pong;
};

extern KomunikacjaClass kom;

#endif

