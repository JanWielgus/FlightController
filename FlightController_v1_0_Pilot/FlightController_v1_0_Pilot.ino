// Kod pilota drona do wersji 1.0
//
// Author: Jan Wielgus
// Date: 26.10.2017r.
//

#include "Komunikacja.h"

SoftwareSerial software_serial(tx_pin, rx_pin); // HC-12 TX Pin, HC-12 RX Pin


long czas_ostatniego_pong = 0;
boolean ostatni_pong = false; //stan ostatniego odebranego pongu
boolean stan_sygnalu = false;



void setup()
{
	//Serial.begin(9600);
	kom.init(&software_serial);
}

void loop()
{
	//kom.odbierz();
	//Serial.println(kom.zmiennaTestowa.value);
	
	if (kom.pong.b0 != ostatni_pong)
	{
		ostatni_pong = kom.pong.b0;
		czas_ostatniego_pong = millis();
	}
	
	if ((millis() - czas_ostatniego_pong) > 1000) stan_sygnalu = false;
	else stan_sygnalu = true;
	
	kom.ping.b0 = !kom.ping.b0;
	
	kom.zmienna1 = analogRead(A0);
	
	kom.wyslij(PILOT_RAMKA_TEST_TYPE);

	if (stan_sygnalu == true) digitalWrite(LED_BUILTIN, HIGH);
	else digitalWrite(LED_BUILTIN, LOW);
	
	delay(50);
}

