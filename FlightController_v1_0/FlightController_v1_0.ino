/*	Author: Jan Wielgus
 *	Date: 10.08.2017r.
 *	
	Pierwsza wersja kontrolera lotu drona.
	Funkcje i mo¿liwoœci:
	- Sterowanie w trybie stabilizacji bez wspomagania
	- Komunikacja poprzez bibliotekê PacketSerial
	- Biblioteki obs³ugi akcelerometru, ¿yroskopu, barometru i magnetometru - Korneliusz Jarzebski
 */

#include "config.h"
#include "Komunikacja.h"
#include "Sensors.h"
#include "Motors.h"
#include "PID.h"


//  =====   ZMIENNE   =====

uint32_t last_loop = 0; // czas ostatniego wykonania funkcji komunikacji

float headingToHold = 0; // kat do utrzymania przez drona



//  =====   FUNKCJE   =====

inline void updateCommunication();   // aktualizacja komunikacji w odpowiednim czasie
inline void stabilize();             // stabilizacja drona i utrzymywanie kierunku
void configureESC();          // Tryb konfiguracji ESC
void updatePIDParams();       // Aktualizacja parametrów PID odebranymi danymi


PID levelX_PID;
PID levelY_PID;
PID heading_PID;





void setup()
{
	kom.init();                                       // inicjalizacja komunikacji
	sensors.init();                                   // inicjalizacja wszystkich czujników
	motors.init();                                    // inicjalizacja silników
	
	// Dopóki nie odbierze parametrów lotu od pilota
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
	sensors.readCompass();
	Serial.print(sensors.angle.pitch);
	Serial.print("\t");
	Serial.print(sensors.angle.roll);
	Serial.print("\t");
	Serial.println(sensors.headnigGyroMagn);
	*/
}


////////////////////////////////////////////////////////////////////// ====== END LOOP ======


// Funkcje trzeba wywo³ywaæ ca³y czas. Funkcja odbiera ustawion¹ iloœæ razy na sekunde.
inline void updateCommunication()
{
	if ((millis() - last_loop) > COMMUNICATION_WAIT_TIME)
	{
		kom.odbierz();
		kom.updateSignal();
		
		// Jeœli odebrano ramkê z parametrami -> zaktualizuj parametry
		if (kom.recievedConfigPacket) updatePIDParams();
		
		if (kom.isSignal())
		{
			//kom.zmienna1 = 150;
			if (kom.pilot.throttle < 5)
			{
				motors.armMotors(false);
				headingToHold = sensors.headnigGyroMagn;
			}
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



inline void stabilize()
{
	// DOPISAC W PILOCIE MAP'A WARTOŒCI NA ODPOWIEDNIE ZAKRESY!!!
	// MAJA BYC WYSYLANE JUZ PRZELICZONE!!!
	// THROTTLE ju¿ przeloczane
	sensors.readAngles();
	sensors.readCompass();
	
	// Zaktualizuj dt dla calej klasy PID
	PID::updateDeltaTime();
	
	// headingToHold += odebrany obrot; // do zrobienia !!!
	//PID::getDeltaTime(); // zwraca dt
	
	// PIDy poziomowania
	int32_t pidX = levelX_PID.get_pid(sensors.angle.pitch);
	int32_t pidY = levelY_PID.get_pid(sensors.angle.roll);
	
	// PID yaw
	static float headingError;
	headingError = headingToHold-sensors.headnigGyroMagn;
	if (headingError >= 180) headingError -= 360;
	else if (headingError <= -180) headingError += 360;
	int32_t pidHead = heading_PID.get_pid(headingError);
	
	motors.setOnTL(kom.pilot.throttle + pidX - pidY - pidHead);
	motors.setOnTR(kom.pilot.throttle + pidX + pidY + pidHead);
	motors.setOnBR(kom.pilot.throttle - pidX + pidY - pidHead);
	motors.setOnBL(kom.pilot.throttle - pidX - pidY + pidHead);
}



void configureESC() // u¿ywane do kalibracji ESC gdy trzeba wszystkie ustawiæ na max/min
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
	heading_PID.kP(3); // 3
	heading_PID.kI(0); // 1
	heading_PID.kD(0);
	heading_PID.imax(100);
	kom.recievedConfigPacket = false; // ¿eby po¿niej wykorzystaæ jako zmienn¹ oznajmiaj¹c¹, ¿e przysz³a ramka z parametrami
}


