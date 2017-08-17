/*	Author: Jan Wielgus
 *	Date: 10.08.2017r.
 *	
	Pierwsza wersja kontrolera lotu drona.
	Funkcje i mo¿liwoœci:
	- Sterowanie w trybie stabilizacji bez wspomagania
	- Komunikacja poprzez bibliotekê PacketSerial
	- Biblioteki obs³ugi akcelerometru, ¿yroskopu, barometru i magnetometru - Korneliusz Jarzebski
 */

#include <PacketSerial.h>
#include <SoftwareSerial.h>
#include <TimerOne.h>
#include <Wire.h>
#include <MPU6050.h>
#include <MS5611.h>
#include "typyBitowe.h"


MPU6050 mpu;

//---- Komunikacja ----
#define BAUD_RATE 9600
#define MAX_SEND_SIZE 10

#define RAMKA_STER_SIZE 10
#define RAMKA_STER_TYPE 0x00

#define RAMKA_DANE_SIZE 4
#define RAMKA_DANE_TYPE 0x01

#define RAMKA_DRON_SIZE 4 //odebrane

#define tx_pin 2
#define rx_pin 3

PacketSerial pSerial;
uint8_t buforT[MAX_SEND_SIZE];
SoftwareSerial software_serial(tx_pin, rx_pin); // HC-12 TX Pin, HC-12 RX Pin


//---- Zmienne danych odebranych ----
uint16_t throttle = 0;
int rotation = 0; // Obrot drona
int forw_back = 0; // Przód, tyl
int left_right = 0; // Prawo, lewo
uint8_t flight_mode = 1; // Tryby lotu: 1 - Stabilize, 2 - Alt Hold
bitByte dodatkoweRx; // Odebrane dane uzupelniajace: bit 7 - sygna³ "ping" pilota, bit 6 - 

//---- Zmienne danych wysylanych ----
bitByte dodatkoweTx; // Wysylane dane uzupelniajace: bit 7 - sygnal "pong" drona, bit 6 - 
uint8_t battery_level = 0; // Stan baterii (0-200)
uint8_t wysokosc = 0; // Wysokosc drona

//---- Zmienne programu ----
bool stan_sygnalu = true; // false - nie ma polaczenia z pilotem, true - jest polaczenie
uint32_t timer = 0;
double dt_;

float pitch = 0;
float roll = 0;
float yaw = 0;
float pitchAcc = 0;
float rollAcc = 0;


void _protOdbierz(const uint8_t* buffer, size_t size)
{
	odbierzPriv(buffer, size);
}


void setup()
{
	pSerial.setPacketHandler(_protOdbierz);
	software_serial.begin(BAUD_RATE);
	pSerial.begin(&software_serial);
	
	while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
	{
		//Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
		delay(500);
	}
	mpu.setThreshold(3);
	
	Timer1.initialize(100000); // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
	Timer1.attachInterrupt( timerIsr ); // attach the service routine here
	
	delay(1000);
}

void loop()
{
if (stan_sygnalu == true)
{
	read_angles();
	stabilize();
	
}
else
{
	
}
}


////////////////////////////////////////////////////////////////////// ====== END LOOP ======


void read_angles()
{
	// Read normalized values
	Vector norm = mpu.readNormalizeGyro();
	Vector normAccel = mpu.readNormalizeAccel();
	
	// Calculate Pitch, Roll and Yaw (Gyro)
	pitch = pitch + norm.YAxis * dt_;
	roll = roll + norm.XAxis * dt_;
	yaw = yaw + norm.ZAxis * dt_;

	// Calculate Pitch and Roll (Acc)
	pitchAcc = -(atan2(normAccel.XAxis, sqrt(normAccel.YAxis*normAccel.YAxis + normAccel.ZAxis*normAccel.ZAxis))*180.0)/M_PI;
	rollAcc = (atan2(normAccel.YAxis, normAccel.ZAxis)*180.0)/M_PI;
	
	dt_ = ((double)(micros()-timer)/1000000);
	
	//-- Filtr komplementarny --
	pitch = 0.98*pitch + 0.02*pitchAcc;
	roll = 0.98*roll + 0.02*rollAcc;
	
	timer = micros();
}


void stabilize()
{
	
}


// -- Funkcja uruchamiana co 0.1s (10Hz) --
void timerIsr()
{
	odbierz();
	
	dodatkoweTx.b7 = !dodatkoweTx.b7; // Zmiana stanu ping na przeciwny
	//battery_level = ;
	//wysokosc = ;
	
	wyslij();
}


void odbierz()
{
	pSerial.update();
}


void odbierzPriv(const uint8_t* bufferR, size_t PacketSize)
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
	}
}


void wyslij()
{
	buforT[1] = dodatkoweTx.bajt;
	buforT[2] = battery_level;
	buforT[3] = wysokosc;
	
	buforT[0] = liczSumeKontr(buforT, RAMKA_DRON_SIZE);
	
	pSerial.send(buforT, RAMKA_DRON_SIZE);
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
