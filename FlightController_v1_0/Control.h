// Control.h
// Author: Jan Wielgus
// Date: 07.11.2017
//

#ifndef _CONTROL_h
#define _CONTROL_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "config.h"
#include "Komunikacja.h"
#include "Sensors.h"
#include "Motors.h"
#include "PID.h"



class ControlClass
{
//  =====   FUNKCJE   =====
 public:
	void init();
	void updateCommunication();
	void stabilize();
	
	
//  =====   ZMIENNE   =====
 public:
	
	
 private:
	uint32_t last_loop = 0; // czas ostatniego wykonania funkcji komunikacji
	
};

extern ControlClass copter;

#endif

