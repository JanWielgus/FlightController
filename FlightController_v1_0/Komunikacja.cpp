// Author: Jan Wielgus
// Date: 24.10.2017
// 

#include "Komunikacja.h"
KomunikacjaClass komun;

void _protOdbierz(const uint8_t* buffer, size_t size)
{
	//odbierzPriv(buffer, size);
	komun.odbierzPriv(buffer, size);
}



void KomunikacjaClass::init()
{
	SoftwareSerial software_serial(tx_pin, rx_pin); // HC-12 TX Pin, HC-12 RX Pin
	pSerial.setPacketHandler(_protOdbierz);
	software_serial.begin(BAUD_RATE);
	pSerial.begin(&software_serial);
}



void KomunikacjaClass::odbierz()
{
	pSerial.update();
}



void KomunikacjaClass::odbierzPriv(const uint8_t* bufferR, size_t PacketSize)
{
	if (bufferR[1] == RAMKA_STER_TYPE && PacketSize == RAMKA_STER_SIZE && sprawdzSumeKontr(bufferR, PacketSize))
	{
		throttle = word(bufferR[3], bufferR[2]);
		rotation = word(bufferR[5], bufferR[4]);
		forw_back = word(bufferR[7], bufferR[6]);
		left_right = word(bufferR[9], bufferR[8]);
	}
	
	if (bufferR[1] == RAMKA_DANE_TYPE && PacketSize == RAMKA_DANE_SIZE && sprawdzSumeKontr(bufferR, PacketSize))
	{
		flight_mode = bufferR[2];
		dodatkoweRx.bajt = bufferR[3];
		
		setRxBitByte(); // Przepisanie typów bitowych do zmiennych
	}
}



void KomunikacjaClass::wyslij()
{
	setTxBitByte(); // przypisz do zmiennych bitbyte, odpowienne zmienne
	
	buforT[1] = dodatkoweTx.bajt;
	buforT[2] = battery_level;
	buforT[3] = wysokosc;
	
	buforT[0] = liczSumeKontr(buforT, RAMKA_DRON_SIZE);
	
	pSerial.send(buforT, RAMKA_DRON_SIZE);
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



// ====== SPRAWDZENIA ======

bool KomunikacjaClass::isSignal()
{
	if (pilot_ping_state == last_pilot_ping_state)
		return false;
	last_pilot_ping_state = pilot_ping_state;
	return true;
}



void KomunikacjaClass::setRxBitByte()
{
	pilot_ping_state = dodatkoweRx.b7;
	//reszta...
}


void KomunikacjaClass::setTxBitByte()
{
	//reszta...
}

