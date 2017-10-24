// Komunikacja.h
// Author: Jan Wielgus
// Date: 24.10.2017

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


#define BAUD_RATE 9600
#define MAX_SEND_SIZE 10

#define RAMKA_STER_SIZE 10
#define RAMKA_STER_TYPE 0x00

#define RAMKA_DANE_SIZE 4
#define RAMKA_DANE_TYPE 0x01

#define RAMKA_DRON_SIZE 4 //odebrane
// ---
#define tx_pin 2
#define rx_pin 3

class KomunikacjaClass
{
 public:
	PacketSerial pSerial;
	uint8_t buforT[MAX_SEND_SIZE];
	
 // === FUNKCJE ===
 //public:
	void init();
	void odbierz();
	void odbierzPriv(const uint8_t* bufferR, size_t PacketSize);
	void wyslij();
	bool sprawdzSumeKontr(const uint8_t* buffer, size_t PacketSize);
	uint8_t liczSumeKontr(const uint8_t* buffer, size_t PacketSize);
	void setRxBitByte(); // przypisywanie zmiennych bitbyte do odpowiednich zmiennych
	void setTxBitByte(); // przypisywanie odpowiednich zmiennych do zmiennych bitbyte
	
	// sprawdzenia
	bool isSignal();
	
 // === ZMIENNE ===
 public:
	
	// Odebrane
	uint16_t throttle = 0; // Prêdkoœæ na silniki
	int rotation = 0; // Obrot drona
	int forw_back = 0; // Przód, tyl
	int left_right = 0; // Prawo, lewo
	uint8_t flight_mode = 1; // Tryby lotu: 1 - Stabilize, 2 - Alt Hold
	bitByte dodatkoweRx; // Odebrane dane uzupelniajace: bit 7 - sygna³ "ping" pilota, bit 6 -

	// Wys³ane
	bitByte dodatkoweTx; // Wysylane dane uzupelniajace: bit 7 - sygnal "pong" drona, bit 6 -
	uint8_t battery_level = 0; // Stan baterii (0-200)
	uint8_t drone_height = 0; // Wysokosc drona
	
	// Inne
	bool last_pilot_ping_state; // ostatni odebrany stan ping wys³any przez pilota
	bool pilot_ping_state; // obecny odebrany stan ping
};

extern KomunikacjaClass komun;

#endif


/*
Opis protokolu Modernity Master-Manipulator Serial:

1. Na konkretnych pozycjach pakietu kodowanego COBS znajduje sie kolejne dane.
   Bajty wystepuja w naturalnej kolejnosci dla AVR i PC - Little Endian - najpierw idzie nizszy bajt

2. Pakiet kierunku pilot--->dron moze wystepowac w kilku typach:

	Typ ramki - gownie STERowanie (bajt 0: 0x00):
	0. XOR wszystkich nastepnych bajtow (suma kontrolna) (unsigned 8b)
	1. Typ ramki - 0x00 sterowanie (unsigned 8b)
	2. Przepustnica (unsigned 16b)
	3. cd^
	4. Obrot (signed 16b)
	5. cd^
	6. Przod/ty³ (signed 16b)
	7. cd^
	8. Prawo/lewo (signed 16b)
	9. cd^

	Typ ramki - przelaczniki i dodatkowe DANE (bajt 0: 0x01):
	0. XOR wszystkich nastepnych bajtow (suma kontrolna) (unsigned 8b)
	1. Typ ramki - 0x01 dane (unsigned 8b)
	2. Tryb lotu (unsigned 8b): 1 - Stabilize, 2 - Alt Hold
	3. 8 bitow bool: bit 7 - sygna³ "ping" pilota, bit 6 - 
   

3. Pakiet kierunku dron--->pilot :wystepuje tylko w jednym typie:
	0. XOR wszystkich nastepnych bajtow (suma kontrolna) (unsigned 8b)
	1. 8 bitow bool: bit 7 - sygnal "pong" drona, bit 6 - 
	2. Stan baterii (unsigned 8b)
	3. Wysokoœæ wzglêdna (unsigned 8b)
*/

