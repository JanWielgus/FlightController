// Author: Jan Wielgus
// Date: 24.10.2017
// 

#include "Komunikacja.h"
KomunikacjaClass kom;

void _protOdbierz(const uint8_t* buffer, size_t size)
{
	kom.odbierzPriv(buffer, size);
}



void KomunikacjaClass::init(SoftwareSerial *_softwareserial)
{
	pSerial.setPacketHandler(_protOdbierz);
	_softwareserial->begin(BAUD_RATE);
	pSerial.begin(_softwareserial);
}



void KomunikacjaClass::odbierz()
{
	pSerial.update();
}



void KomunikacjaClass::odbierzPriv(const uint8_t* bufferR, size_t PacketSize)
{
	if (bufferR[1] == DRON_RAMKA_TEST_TYPE && PacketSize == DRON_RAMKA_TEST_SIZE && sprawdzSumeKontr(bufferR, PacketSize))
	{
		zmiennaTestowa.bajt[0] = bufferR[2];
		zmiennaTestowa.bajt[1] = bufferR[3];
		zmiennaTestowa.bajt[2] = bufferR[4];
		zmiennaTestowa.bajt[3] = bufferR[5];
	}
	
	if (bufferR[1] == DRON_RAMKA_POZYCJA_TYPE && PacketSize == DRON_RAMKA_POZYCJA_SIZE && sprawdzSumeKontr(bufferR, PacketSize))
	{
		/*
		jakisOsiemT = bufferR[2];
		jakisSzesnT = word(bufferR[4], bufferR[3]);
		jakisSzesnT = word(bufferR[6], bufferR[5]);
		jakisOsiemT = bufferR[7];
		jakisOsiemT = bufferR[8];
		jakisOsiemT = bufferR[9];
		jakisOsiemT = bufferR[10];
		*/
	}
}



void KomunikacjaClass::wyslij(uint8_t typRamki)
{
	buforT[1] = typRamki;
	
	if (typRamki == PILOT_RAMKA_TEST_TYPE)
	{
		buforT[2] = lowByte(zmienna1);
		buforT[3] = highByte(zmienna1);
		buforT[4] = lowByte(zmienna2);
		buforT[5] = highByte(zmienna2);
		buforT[6] = ping.bajt;
		
		buforT[0] = liczSumeKontr(buforT, PILOT_RAMKA_TEST_SIZE);
		
		pSerial.send(buforT, PILOT_RAMKA_TEST_SIZE);
	}
	else if (typRamki == PILOT_RAMKA_STEROWANIE_TYPE)
	{
		//ustawianie zmiennych...
		
		buforT[0] = liczSumeKontr(buforT, PILOT_RAMKA_STEROWANIE_SIZE);
		
		pSerial.send(buforT, PILOT_RAMKA_STEROWANIE_SIZE);
	}
}



bool KomunikacjaClass::sprawdzSumeKontr(const uint8_t* buffer, size_t PacketSize)
{
	uint8_t suma_kontrolna = buffer[1];
	for(int i=2; i<PacketSize; i++)
	suma_kontrolna ^= buffer[i];	//xor'owanie kolejnych bajt?w

	if(suma_kontrolna==buffer[0]) return true;
	else return false;
}



uint8_t KomunikacjaClass::liczSumeKontr(const uint8_t* buffer, size_t PacketSize)
{
	uint8_t suma_kontrolna = buffer[1];
	for(int i=2; i<PacketSize; i++)
	suma_kontrolna ^= buffer[i];	//xor'owanie kolejnych bajt?w

	return suma_kontrolna;
}


