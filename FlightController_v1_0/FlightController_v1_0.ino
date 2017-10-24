/*	Author: Jan Wielgus
 *	Date: 10.08.2017r.
 *	
	Pierwsza wersja kontrolera lotu drona.
	Funkcje i mo¿liwoœci:
	- Sterowanie w trybie stabilizacji bez wspomagania
	- Komunikacja poprzez bibliotekê PacketSerial
	- Biblioteki obs³ugi akcelerometru, ¿yroskopu, barometru i magnetometru - Korneliusz Jarzebski
 */


#include <TimerOne.h>
#include <Wire.h>
#include <MPU6050.h>
#include <MS5611.h>
#include "Komunikacja.h"


MPU6050 mpu;


//---- Zmienne programu ----
bool stan_sygnalu = true; // false - nie ma polaczenia z pilotem, true - jest polaczenie
uint32_t timer = 0;
uint32_t timerPrev = 0;
double dt_;

float pitch = 0;
float roll = 0;
float yaw = 0;
float pitchAcc = 0;
float rollAcc = 0;



void setup()
{
	komun.init();
	
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
	read_RPY();
	stabilize();
	
}
else
{
	
}
}


////////////////////////////////////////////////////////////////////// ====== END LOOP ======


void read_RPY() // Read roll, pitch, yaw
{
	timerPrev = timer;
	timer = micros();
	dt_ = float(timer - timerPrev) / 1000000.0;
	
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
	
	//-- Filtr komplementarny --
	pitch = 0.98*pitch + 0.02*pitchAcc;
	roll = 0.98*roll + 0.02*rollAcc;
}


void stabilize()
{
	
}


// -- Funkcja uruchamiana co 0.1s (10Hz) --
void timerIsr()
{
	komun.odbierz();
	
	komun.dodatkoweTx.b7 = !komun.dodatkoweTx.b7;// Zmiana stanu ping na przeciwny
	//battery_level = ;
	//wysokosc = ;
	
	komun.wyslij();
}


