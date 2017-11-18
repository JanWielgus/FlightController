/*
 * config.h
 *
 * Created: 27.10.2017 18:55:44
 *  Author: Jan Wielgus
 *
 *	Plk konfiguracji wszystkich parametr�w drona
 */ 


#ifndef CONFIG_H_
#define CONFIG_H_


//<<<<<<<<<=====================     KOMUNIKACJA     =====================>>>>>>>>>

	//-------Pami�taj o zmianie wielko�ci ramek po zmianie protoko�u!!!-------
	
	//      ===  TYLKO TABLICE O WYMIARACH TAKICH JAK RAMKA, NIE WI�KSZE !!!  ===
	
	#define serialPort 0	           //Serial: 0, Serial1: 1, Serial2: 2, Serial3: 3
	#define BAUD_RATE 9600
	#define COMMUNICATION_PER_SECOND 20     // Ilo�� wywo�ania funkcji komunikacji w ci�gu sekundy
	#define MAX_SEND_SIZE 12           // Wielkos� najwi�kszej ramki

	// Szablon: nadawca_RAMKA_nazwa_SIZE/TYPE
	
	// -- odebrane --
	#define PILOT_RAMKA_STEROWANIE_SIZE 3
	#define PILOT_RAMKA_STEROWANIE_TYPE 0x00
	
	#define PILOT_RAMKA_DANE_SIZE 4
	#define PILOT_RAMKA_DANE_TYPE 0x01
	
	#define PILOT_RAMKA_TEST_SIZE 8
	#define PILOT_RAMKA_TEST_TYPE 0x02
	// -- nadawane --
	#define DRON_RAMKA_POZYCJA_SIZE 6
	#define DRON_RAMKA_POZYCJA_TYPE 0x05
	
	#define DRON_RAMKA_TEST_SIZE 6
	#define DRON_RAMKA_TEST_TYPE 0x06

	#define MAX_ILOSC_ZGUBIONYCH_RAMEK 2    // Po prezkroczeniu wykrywany jest brak po��czenia z pilotem


//<<<<<<<<<=====================     SENSORS     =====================>>>>>>>>>


//<<<<<<<<<=====================     MOTORS     =====================>>>>>>>>>

	#define TLmotorPin 5      // Top left
	#define TRmotorPin 6      // Top right
	#define BLmotorPin 9      // Back left
	#define BRmotorPin 10     // Back right
	
	#define MOTOR_MIN 1000    // Minimum value on the motor
	#define MOTOR_MAX 2000    // Maximum value on the motor
	#define MOTOR_IDLE 1000   // Value when motor is idle


//<<<<<<<<<=====================     PID     =====================>>>>>>>>>

	#define kP_level 3     // wzmocnienie P PID'u od poziomu (test: 3)
	#define kI_level 0     // wzmocnienie I PID'u od poziomu
	#define kD_level 0     // wzmocnienie D PID'u od poziomu  (test: 0.2)
	
	#define kP_yaw 1 // wzmocnienie P PD'u od osi z
	#define kD_yaw 1 // wzmocnienie D PD'u od osi z
	
	#define D_error_divisor 4 // dzielnik obliczonego uchybu
	
	
	
	
//<<<<<<<<<=====================     AUTO CALCULATIONS     =====================>>>>>>>>>
	
	#define COMMUNICATION_WAIT_TIME (int(1000/COMMUNICATION_PER_SECOND)-2)
	


#endif /* CONFIG_H_ */

