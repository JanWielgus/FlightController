// FiltrKalmana.h

#ifndef _FILTRKALMANA_h
#define _FILTRKALMANA_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class FiltrKalmanaClass
{
	public:

	void init(double angle = 0.001, double bias = 0.003, double measure = 0.03);
	double update(double newValue, double newRate);

	private:

	double Q_angle, Q_bias, R_measure;
	double K_angle, K_bias, K_rate;
	double P[2][2], K[2];
	double S, y;
	double dt, kt;
};

//extern FiltrKalmanaClass FiltrKalmana;

#endif

