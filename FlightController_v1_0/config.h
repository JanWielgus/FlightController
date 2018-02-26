/*
 * config.h
 *
 * Created: 27.10.2017 18:55:44
 *  Author: Jan Wielgus
 *
 *	Plk konfiguracji wszystkich parametrów drona
 */ 


#ifndef CONFIG_H_
#define CONFIG_H_


//<<<<<<<<<=====================     KOMUNIKACJA     =====================>>>>>>>>>

	//-------Pamiêtaj o zmianie wielkoœci ramek po zmianie protoko³u!!!-------
	
	//      ===  TYLKO TABLICE O WYMIARACH TAKICH JAK RAMKA, NIE WIÊKSZE !!!  ===
	
	#define serialPort 0	           //Serial: 0, Serial1: 1, Serial2: 2, Serial3: 3
	#define BAUD_RATE 9600
	#define COMMUNICATION_PER_SECOND 20     // Iloœæ wywo³ania funkcji komunikacji w ci¹gu sekundy
	#define MAX_SEND_SIZE 25           // Wielkosæ najwiêkszej ramki

	// Szablon: nadawca_RAMKA_nazwa_SIZE/TYPE
	
	// -- odebrane --
	#define PILOT_RAMKA_STEROWANIE_SIZE 3
	#define PILOT_RAMKA_STEROWANIE_TYPE 0x00
	
	#define PILOT_RAMKA_DANE_SIZE 4
	#define PILOT_RAMKA_DANE_TYPE 0x01
	
	#define PILOT_RAMKA_TEST_SIZE 8
	#define PILOT_RAMKA_TEST_TYPE 0x02
	
	#define PILOT_RAMKA_CONFIG_SIZE 23
	#define PILOT_RAMKA_CONFIG_TYPE 0x03
	// -- nadawane --
	#define DRON_RAMKA_POZYCJA_SIZE 6
	#define DRON_RAMKA_POZYCJA_TYPE 0x05
	
	#define DRON_RAMKA_TEST_SIZE 6
	#define DRON_RAMKA_TEST_TYPE 0x06

	#define MAX_ILOSC_ZGUBIONYCH_RAMEK 2    // Po prezkroczeniu wykrywany jest brak po³¹czenia z pilotem


//<<<<<<<<<=====================     SENSORS     =====================>>>>>>>>>

	#define PITCH_OFFSET 0   // Offset osi x
	#define ROLL_OFFSET  0   // Offset osi y
	
	#define INTERRUPT_PIN 2  // use pin 2 on Arduino Uno & most boards


//<<<<<<<<<=====================     MOTORS     =====================>>>>>>>>>

	#define TLmotorPin 5      // Top left
	#define TRmotorPin 6      // Top right
	#define BRmotorPin 9      // Back left
	#define BLmotorPin 10     // Back right
	
	#define MOTOR_MIN 1000    // Minimum value on the motor
	#define MOTOR_MAX 2000    // Maximum value on the motor
	#define MOTOR_IDLE 1000   // Value when motor is idle


//<<<<<<<<<=====================     PID     =====================>>>>>>>>>

	#define D_error_divisor 4 // dzielnik obliczonego uchybu
	
	
//<<<<<<<<<=====================     AUTO CALCULATIONS     =====================>>>>>>>>>
	
	#define COMMUNICATION_WAIT_TIME (int(1000/COMMUNICATION_PER_SECOND)-2)
	


#endif /* CONFIG_H_ */

