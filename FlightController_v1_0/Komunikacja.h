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


//-------Pami?ta? o zmianie wielko?ci ramek po zmianie protoko?u!!!-------
#define serialNaPakiety Serial
#define serialPort 0	//Serial: 0, Serial1: 1, Serial2: 2, Serial3: 3
#define BAUD_RATE 9600
#define MAX_SEND_SIZE 12

#define RAMKA_STER_SIZE 3
#define RAMKA_STER_TYPE 0x00

#define RAMKA_DANE_SIZE 4
#define RAMKA_DANE_TYPE 0x01

#define RAMKA_DRON_SIZE 6 //odebrane

#define MAX_ILOSC_ZGUBIONYCH_RAMEK 3



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
	
	bitByte ping;
	
 private:
	//long czas_ostatniego_pong = 1000;
	boolean ostatni_pong = false; //stan ostatniego odebranego pongu
	boolean stan_sygnalu = false;
	int zgubione_ramki = 0; // liczba nieodebranych ramek
};

extern KomunikacjaClass kom;

#endif

