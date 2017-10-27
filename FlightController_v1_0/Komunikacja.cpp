// Author: Jan Wielgus
// Date: 24.10.2017
// 

#include "Komunikacja.h"
KomunikacjaClass kom;

void _protezaOdbierz(const uint8_t* buffer, size_t size)
{
	kom.odbierzPriv(buffer, size);
}



void KomunikacjaClass::init()
{
	pSerial.setPacketHandler(_protezaOdbierz);
	pSerial.begin(BAUD_RATE, serialPort);
}



void KomunikacjaClass::odbierz()
{
	pSerial.update();
}



void KomunikacjaClass::odbierzPriv(const uint8_t* bufferR, size_t PacketSize)
{
	if (PacketSize == RAMKA_DRON_SIZE && sprawdzSumeKontr(bufferR, PacketSize))
	{
		zmienna1 = word(bufferR[2], bufferR[1]);
		zmienna2 = word(bufferR[4], bufferR[3]);
		pong.bajt = bufferR[5];
	}
}



void KomunikacjaClass::wyslij(uint8_t typRamki)
{
	buforT[1] = typRamki;
	
	if (typRamki == RAMKA_STER_TYPE)
	{
		buforT[2] = ping.bajt;
		
		
		buforT[0] = liczSumeKontr(buforT, RAMKA_DRON_SIZE);
		
		pSerial.send(buforT, RAMKA_DRON_SIZE);
	}
	
	else if (typRamki == RAMKA_DANE_TYPE)
	{
		/*
		buforT[2] = jakisOsiemTtx;
		buforT[3] = jakisBitByteTx;
		*/
	}
}



void KomunikacjaClass::updateSignal()
{
	if (pong.b0 != ostatni_pong)
	{
		ostatni_pong = pong.b0;
		zgubione_ramki = 0;
	}
	else
	{
		zgubione_ramki++;
	}
}



bool KomunikacjaClass::isSignal()
{
	if (zgubione_ramki > MAX_ILOSC_ZGUBIONYCH_RAMEK)
		return false;
	return true;
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



