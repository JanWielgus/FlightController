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
	if (PacketSize == RAMKA_DRON_SIZE && sprawdzSumeKontr(bufferR, RAMKA_DRON_SIZE))
	{
		dodatkoweRx.bajt = bufferR[1];
		battery_level = bufferR[2];
		drone_height = bufferR[3];
	}
}



void KomunikacjaClass::wyslij(uint8_t typRamki)
{
	dodatkoweTx.b7 = !dodatkoweTx.b7;
	
	buforT[1] = dodatkoweTx.bajt;
	buforT[2] = throttle;
	
	buforT[0] = liczSumeKontr(buforT, RAMKA_STER_SIZE);
	pSerial.send(buforT, RAMKA_STER_SIZE);
	/*
	buforT[1] = typRamki;
	
	if (typRamki == RAMKA_STER_TYPE)
	{
		buforT[2] = throttle;
		buforT[3] = rotation;
		buforT[4] = forw_back;
		buforT[5] = left_right;
		
		buforT[0] = liczSumeKontr(buforT, RAMKA_STER_SIZE);
		pSerial.send(buforT, RAMKA_STER_SIZE);
	}
	
	if (typRamki == RAMKA_DANE_TYPE)
	{
		//setTxBitByte(); // przypisz do zmiennych bitbyte, odpowienne zmienne
		dodatkoweTx.b7 = !dodatkoweTx.b7;
		
		buforT[2] = flight_mode;
		buforT[3] = dodatkoweTx.bajt;
		
		buforT[0] = liczSumeKontr(buforT, RAMKA_DANE_SIZE);
		pSerial.send(buforT, RAMKA_DANE_SIZE);
	}
	*/
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
		stan_sygnalu = false;
	last_pilot_ping_state = pilot_ping_state;
	stan_sygnalu = true;
}


bool KomunikacjaClass::isSignal()
{
	return stan_sygnalu;
}



void KomunikacjaClass::setRxBitByte()
{
	pilot_ping_state = dodatkoweRx.b7;
	//reszta...
}


void KomunikacjaClass::setTxBitByte()
{
	dodatkoweTx.b7 = !dodatkoweTx.b7;
}

