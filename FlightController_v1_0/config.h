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
	
	//#define serialNaPakiety Serial
	#define serialPort 0	  //Serial: 0, Serial1: 1, Serial2: 2, Serial3: 3
	#define BAUD_RATE 9600
	#define MAX_SEND_SIZE 12  // Wielkosæ najwiêkszej ramki

	#define RAMKA_STER_SIZE 3
	#define RAMKA_STER_TYPE 0x00
	#define RAMKA_DANE_SIZE 4
	#define RAMKA_DANE_TYPE 0x01
	#define RAMKA_DRON_SIZE 6 //odebrane

	#define MAX_ILOSC_ZGUBIONYCH_RAMEK 3    // Po prezkroczeniu wykrywany jest brak po³¹czenia z pilotem


//<<<<<<<<<=====================     SENSORS     =====================>>>>>>>>>


//<<<<<<<<<=====================     MOTORS     =====================>>>>>>>>>

	#define TLmotorPin 5   // Top left
	#define TRmotorPin 6   // Top right
	#define BLmotorPin 9   // Back left
	#define BRmotorPin 10  // Back right


//<<<<<<<<<=====================     PID     =====================>>>>>>>>>

	#define kP 1     // wzmocnienie P PID'u od poziomu
	#define kI 1     // wzmocnienie I PID'u od poziomu
	#define kD 1     // wzmocnienie D PID'u od poziomu
	
	#define kP_yaw 1 // wzmocnienie P PD'u od osi z
	#define kD_yaw 1 // wzmocnienie D PD'u od osi z


#endif /* CONFIG_H_ */