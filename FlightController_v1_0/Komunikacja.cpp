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
	if (PacketSize == RAMKA_STER_SIZE && sprawdzSumeKontr(bufferR, RAMKA_STER_SIZE))
	{
		dodatkoweRx.bajt = bufferR[1];
		throttle = bufferR[2];
	}
	
	/*
	//if (bufferR[1] == RAMKA_STER_TYPE && PacketSize == RAMKA_STER_SIZE && sprawdzSumeKontr(bufferR, RAMKA_STER_SIZE))
	if (bufferR[1] == RAMKA_STER_TYPE && PacketSize == RAMKA_STER_SIZE)
	{
		throttle = bufferR[2];
		rotation = bufferR[3];
		forw_back = bufferR[4];
		left_right = bufferR[5];
	}
	
	//if (bufferR[1] == RAMKA_DANE_TYPE && PacketSize == RAMKA_DANE_SIZE && sprawdzSumeKontr(bufferR, RAMKA_DANE_SIZE))
	if (bufferR[1] == RAMKA_DANE_TYPE && PacketSize == RAMKA_DANE_SIZE)
	{
		flight_mode = bufferR[2];
		dodatkoweRx.bajt = bufferR[3];
		
		setRxBitByte(); // Przepisanie typów bitowych do zmiennych
	}
	*/
}



void KomunikacjaClass::wyslij()
{
	setTxBitByte(); // przypisz do zmiennych bitbyte, odpowienne zmienne
	
	buforT[1] = dodatkoweTx.bajt;
	buforT[2] = battery_level;
	buforT[3] = drone_height;
	
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

void KomunikacjaClass::updateSignalState()
{
	if (pilot_ping_state == last_pilot_ping_state)
	{
		stan_sygnalu = false;
	}
	else
	{
		last_pilot_ping_state = pilot_ping_state;
		stan_sygnalu = true;
	}
}


bool KomunikacjaClass::isSignal()
{
	return stan_sygnalu;
}



void KomunikacjaClass::setRxBitByte()
{
	pilot_ping_state = dodatkoweRx.b7;
	//reszta...
	Serial.println("dziala");
}


void KomunikacjaClass::setTxBitByte()
{
	//reszta...
}

