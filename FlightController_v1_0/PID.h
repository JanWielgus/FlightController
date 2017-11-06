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
	void init(float _kP, float _kI, float _kD);                     // inicjalizacja
	float getPID(float _current, float _set, double _deltaT);       // zwraca obliczon� warto�� P+I+D
	float getPID(float _error, double _deltaT);                     // zwraca obliczon� wario�� P+I+D (funkcja przeci��ona)
	float getPD(float current, float set, double deltaT);           // zwraca obliczon� warto�� P+D
	
 private:
	
	
//  =====   ZMIENNE   =====
 public:
	
 private:
	float kP, kI, kD;    // Wzmocznienia poszczeg�lnych cz�on�w kontrolera PID
	double val_I = 0;    // warto�� ca�ki
	float last_error;  // ostatnia warto�� b��du (do oblicznania D)
};

extern PIDClass levelX_PID;
extern PIDClass levelY_PID;

#endif

