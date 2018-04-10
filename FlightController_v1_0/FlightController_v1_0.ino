/*	Author: Jan Wielgus
 *	Date: 10.08.2017r.
 *	
	Pierwsza wersja kontrolera lotu drona.
	Funkcje i mo�liwo�ci:
	- Sterowanie w trybie stabilizacji bez wspomagania
	- Komunikacja poprzez bibliotek� PacketSerial
	- Biblioteki obs�ugi akcelerometru, �yroskopu, barometru i magnetometru - Korneliusz Jarzebski
 */

#include "config.h"
#include "Komunikacja.h"
#include "Sensors.h"
#include "Motors.h"
#include "PID.h"


//  =====   ZMIENNE   =====

uint32_t last_loop = 0; // czas ostatniego wykonania funkcji komunikacji



//  =====   FUNKCJE   =====

void updateCommunication();   // aktualizacja komunikacji w odpowiednim czasie
void stabilize();             // stabilizacja drona i utrzymywanie kierunku
void configureESC();          // Tryb konfiguracji ESC
void updatePIDParams();       // Aktualizacja parametr�w PID odebranymi danymi


PID levelX_PID;
PID levelY_PID;
// pid do osi Z (yaw)





void setup()
{
	kom.init();                                       // inicjalizacja komunikacji
	sensors.init();                                   // inicjalizacja wszystkich czujnik�w
	motors.init();                                    // inicjalizacja silnik�w
	
	// Dop�ki nie odbierze parametr�w lotu od pilota
	while (!kom.recievedConfigPacket)
		kom.odbierz();

	updatePIDParams();
	
	delay(100);
}


void loop()
{
	updateCommunication();
	stabilize();
	/*
	sensors.readAngles();
	Serial.print(sensors.angle.pitch);
	Serial.print("\t");
	Serial.println(sensors.angle.roll);*/
}


////////////////////////////////////////////////////////////////////// ====== END LOOP ======


// Funkcje trzeba wywo�ywa� ca�y czas. Funkcja odbiera ustawion� ilo�� razy na sekunde.
void updateCommunication()
{
	if ((millis() - last_loop) > COMMUNICATION_WAIT_TIME)
	{
		kom.odbierz();
		kom.updateSignal();
		
		// Je�li odebrano ramk� z parametrami -> zaktualizuj parametry
		if (kom.recievedConfigPacket) updatePIDParams();
		
		if (kom.isSignal())
		{
			//kom.zmienna1 = 150;
			if (kom.pilot.throttle < 5) motors.armMotors(false);
			else motors.armMotors(true);
		}
		else
		{
			//kom.zmienna1 = 0;
			motors.armMotors(false);
		}
		
		//kom.zmiennaTestowa1.value = 1723.5;
		//kom.wyslij(DRON_RAMKA_TEST_TYPE);
		
		
		last_loop = millis();
	}
}



void stabilize()
{
	// DOPISAC W PILOCIE MAP'A WARTO�CI NA ODPOWIEDNIE ZAKRESY!!!
	// MAJA BYC WYSYLANE JUZ PRZELICZONE!!!
	// THROTTLE ju� przeloczane
	sensors.readAngles();
	sensors.readCompass();
	
	int32_t pidX = levelX_PID.get_pid((int32_t)sensors.angle.pitch, 1);
	int32_t pidY = levelY_PID.get_pid((int32_t)sensors.angle.roll, 1);
	
	motors.setOnTL(kom.pilot.throttle + pidX - pidY);
	motors.setOnTR(kom.pilot.throttle + pidX + pidY);
	motors.setOnBR(kom.pilot.throttle - pidX + pidY);
	motors.setOnBL(kom.pilot.throttle - pidX - pidY);
}



void configureESC() // u�ywane do kalibracji ESC gdy trzeba wszystkie ustawi� na max/min
{
	motors.armMotors(true);
	
	motors.setOnTL(kom.pilot.throttle);
	motors.setOnTR(kom.pilot.throttle);
	motors.setOnBR(kom.pilot.throttle);
	motors.setOnBL(kom.pilot.throttle);
}



void updatePIDParams()
{
	levelX_PID.kP(kom.conf.kP_level.value);
	levelX_PID.kI(kom.conf.kI_level.value);
	levelX_PID.kD(kom.conf.kD_level.value);
	levelX_PID.imax(kom.conf.I_level_limiter);
	levelY_PID.kP(kom.conf.kP_level.value);
	levelY_PID.kI(kom.conf.kI_level.value);
	levelY_PID.kD(kom.conf.kD_level.value);
	levelY_PID.imax(kom.conf.I_level_limiter);
	kom.recievedConfigPacket = false; // �eby po�niej wykorzysta� jako zmienn� oznajmiaj�c�, �e przysz�a ramka z parametrami
}


