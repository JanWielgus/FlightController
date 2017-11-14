// Author: Jan Wielgus
// Date: 07.11.2017
// 

#include "Control.h"
ControlClass copter;



void ControlClass::init()
{
	kom.init();
	sensors.init();
	motors.init();
	levelX_PID.init(kP_level, kI_level, kD_level);
	levelY_PID.init(kP_level, kI_level, kD_level);
	yaw_PD.init(kP_yaw, kD_yaw);
	
	delay(500);
}



void ControlClass::updateCommunication()
{
	if ((millis() - last_loop) > 48)
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
	sensors.readAngles();
	
	
	int value = map(kom.pilot.throttle, 10, 1023, 0, 1000);
	value = constrain(value, 0, 1000);
	
	if (value != 0) value -= levelX_PID.getPID(sensors.pitch, sensors.dt_);
	
	motors.setOnAllMotors(value);
	
	
}



