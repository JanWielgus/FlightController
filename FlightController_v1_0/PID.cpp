// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: t -*-

/// @file	PID.cpp
/// @brief	Generic PID algorithm

#include <math.h>
#include <Arduino.h>

#include "PID.h"

int32_t
PID::get_pid(int32_t error)
{
    uint32_t tnow = millis();
    uint32_t dt = tnow - _last_t;
    float output = 0;
    float delta_time;
    _last_t = tnow;

    delta_time = (float)dt * 0.001;

    // Compute proportional component
	float pro = error * _kp;
	outPro = (int8_t)pro;
    output += pro;

    // Compute derivative component if time has elapsed
    if (dt > 0) // NIE WIEM CZY POTRZEBNE BO I TAK TO SIE WYKONUJE STOSUNKOWO RZADKO !!!
	{
        float derivative;

		derivative = (error - _last_error) / delta_time;

        // discrete low pass filter, cuts out the
        // high frequency noise that can drive the controller crazy
        //float RC = 1/(2*M_PI*_fCut);
        //derivative = _last_derivative + (delta_time / (RC + delta_time)) * (derivative - _last_derivative);

        // update state
        _last_error = error;
        _last_derivative = derivative;

        // add in derivative component
		float der = _kd * derivative;
		outDer = (int8_t)der;
        output += der;

    // Compute integral component
        _integrator += (error * _ki) * delta_time;
		
		// anti wind-up
		_integrator = constrain(_integrator, -_imax, _imax);
		outInt = (int8_t)_integrator;
			
        output += _integrator;
    }

    return output;
}

void
PID::reset_I()
{
    _integrator = 0;
	// we use NAN (Not A Number) to indicate that the last 
	// derivative value is not valid
    _last_derivative = NAN;
}

