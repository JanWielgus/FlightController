// Author: Jan Wielgus
// Date: 07.11.2017
// 

#include "Control.h"
ControlClass copter;



void ControlClass::init()
{
	kom.init();                                       // inicjalizacja komunikacji
	sensors.init();                                   // inicjalizacja wszystkich czujników
	motors.init();                                    // inicjalizacja silników
	
	// Dopóki nie odbierze konfiguracji od pilota
	//while (!kom.recievedFirstConfigPacket)
		kom.odbierz();
	
	delay(100);
}



// Funkcje trzeba wywo³ywaæ ca³y czas. Funkcja odbiera ustawion¹ iloœæ razy na sekunde.
void ControlClass::updateCommunication()
{
	if ((millis() - last_loop) > COMMUNICATION_WAIT_TIME)
	{
		kom.odbierz();
		kom.updateSignal();
		
		/*
		if (kom.isSignal())
		{
			kom.zmienna1 = 150;
		}
		else kom.zmienna1 = 0;
		*/
		
		//kom.zmiennaTestowa1.value = 1723.5;
		//kom.wyslij(DRON_RAMKA_TEST_TYPE);
		
		
		last_loop = millis();
	}
}



void ControlClass::stabilize()
{
	// DOPISAC W PILOCIE MAP'A WARTOŒCI NA ODPOWIEDNIE ZAKRESY!!!
	// MAJA BYC WYSYLANE JUZ PRZELICZONE!!!
	// THROTTLE ju¿ przeloczane
	sensors.readAngles();
	int motor_main_power = kom.pilot.throttle;
	
	if (motor_main_power < 3) motors.armMotors(false);
	else motors.armMotors(true);
	
	float pidX = levelX_PID.getPID(sensors.angle.pitch, 0);
	float pidY = levelY_PID.getPID(sensors.angle.roll, 0);
	
	motors.setOnTL(motor_main_power + pidX - pidY);
	motors.setOnTR(motor_main_power + pidX + pidY);
	motors.setOnBR(motor_main_power - pidX + pidY);
	motors.setOnBL(motor_main_power - pidX - pidY);
}



void ControlClass::configureESC()
{
	motors.armMotors(true);
	int motor_main_power = kom.pilot.throttle;
	
	motors.setOnTL(motor_main_power);
	motors.setOnTR(motor_main_power);
	motors.setOnBR(motor_main_power);
	motors.setOnBL(motor_main_power);
}



