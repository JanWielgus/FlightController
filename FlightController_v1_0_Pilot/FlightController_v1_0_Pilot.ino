// Kod pilota drona do wersji 1.0
//
// Author: Jan Wielgus
// Date: 26.10.2017r.
//

//#include <TimerOne.h>
//#include "Komunikacja.h"
#include <PacketSerial.h>
#include <SoftwareSerial.h>
#include "typyBitowe.h"


//-------Pami?ta? o zmianie wielko?ci ramek po zmianie protoko?u!!!-------
#define BAUD_RATE 9600
#define MAX_SEND_SIZE 12

//pierwsza ramka
#define RAMKA_PREDK_SIZE 3
#define RAMKA_PREDK_TYPE 0x00

//druga ramka
#define RAMKA_POZ_SIZE 3
#define RAMKA_POZ_TYPE 0x01

#define RAMKA_WYSYLKA_SIZE 6

#define tx_pin 2
#define rx_pin 3

PacketSerial pSerial;
uint8_t buforT[MAX_SEND_SIZE];
SoftwareSerial software_serial(tx_pin, rx_pin); // HC-12 TX Pin, HC-12 RX Pin

//wysy?ane
int zmienna1, zmienna2;
bitByte ping;

//odebrane
bitByte pong;

long czas_ostatniego_pong = 0;
boolean ostatni_pong = false; //stan ostatniego odebranego pongu
boolean stan_sygnalu = false;





void setup()
{
	Serial.begin(9600);
	//komun.init();
	_init();
	
	//Timer1.initialize(100000); // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
	//Timer1.attachInterrupt( timerIsr ); // attach the service routine here
}

void loop()
{
	odbierz();
	
	if (pong.b0 != ostatni_pong)
	{
		ostatni_pong = pong.b0;
		czas_ostatniego_pong = millis();
	}
	
	if ((millis() - czas_ostatniego_pong) > 1000) stan_sygnalu = false;
	else stan_sygnalu = true;
	
	ping.b0 = !ping.b0;
	
	zmienna1 = analogRead(A0);
	
	wyslij();

	if (stan_sygnalu == true) digitalWrite(LED_BUILTIN, HIGH);
	else digitalWrite(LED_BUILTIN, LOW);
	
	delay(100);
}


/*
// -- Funkcja uruchamiana co 0.1s (10Hz) --
void timerIsr()
{
	Serial.println("dziala");
	komun.odbierz();
	//komun.updateSignalState(); // sprawdü czy jest sygna≥
	
		
	komun.wyslij(RAMKA_STER_TYPE);
	komun.wyslij(RAMKA_DANE_TYPE);
}
*/





void _protOdbierz(const uint8_t* buffer, size_t size)
{
	odbierzPriv(buffer, size);
}


void _init()
{
	pSerial.setPacketHandler(_protOdbierz);
	software_serial.begin(BAUD_RATE);
	pSerial.begin(&software_serial);
}


void odbierz()
{
	pSerial.update();
}


void odbierzPriv(const uint8_t* bufferR, size_t PacketSize)
{
	if (bufferR[1] == RAMKA_PREDK_TYPE && PacketSize == RAMKA_PREDK_SIZE && sprawdzSumeKontr(bufferR, PacketSize))
	{
		pong.bajt = bufferR[2];
	}
	
	if (bufferR[1] == RAMKA_POZ_TYPE && PacketSize == RAMKA_POZ_SIZE && sprawdzSumeKontr(bufferR, PacketSize))
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


void wyslij()
{
	buforT[1] = lowByte(zmienna1);
	buforT[2] = highByte(zmienna1);
	buforT[3] = lowByte(zmienna2);
	buforT[4] = highByte(zmienna2);
	buforT[5] = ping.bajt;
	
	buforT[0] = liczSumeKontr(buforT, RAMKA_WYSYLKA_SIZE);
	
	pSerial.send(buforT, RAMKA_WYSYLKA_SIZE);
}


bool sprawdzSumeKontr(const uint8_t* buffer, size_t PacketSize)
{
	uint8_t suma_kontrolna = buffer[1];
	for(int i=2; i<PacketSize; i++)
	suma_kontrolna ^= buffer[i];	//xor'owanie kolejnych bajt?w

	if(suma_kontrolna==buffer[0]) return true;
	else return false;
}

uint8_t liczSumeKontr(const uint8_t* buffer, size_t PacketSize)
{
	uint8_t suma_kontrolna = buffer[1];
	for(int i=2; i<PacketSize; i++)
	suma_kontrolna ^= buffer[i];	//xor'owanie kolejnych bajt?w

	return suma_kontrolna;
}

