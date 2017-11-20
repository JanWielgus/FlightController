// PID.h
// Author: Jan Wielgus
// Date: 06.11.2017
//

#ifndef _PID_h
#define _PID_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "config.h"



class PIDClass
{
//  =====   FUNKCJE   =====
 public:
	void setPID_gains(float _kP, float _kI, float _kD);             // ustawianie wzmocnieñ
	void setPD_gains(float _kP, float _kD);                         // ustawianie wzmocnieñ
	float getPID(float _current, float _set, double _deltaT);       // zwraca obliczon¹ wartoœæ P+I+D
	float getPD(float current, float set, double deltaT);           // zwraca obliczon¹ wartoœæ P+D
	
 private:
	
	
//  =====   ZMIENNE   =====
 public:
	
 private:
	float kP, kI, kD;    // Wzmocznienia poszczególnych cz³onów kontrolera PID
	double val_I = 0;    // wartoœæ ca³ki
	float last_error;    // ostatnia wartoœæ b³êdu (do oblicznania D)
};

extern PIDClass levelX_PID;
extern PIDClass levelY_PID;
extern PIDClass yaw_PD;

#endif

