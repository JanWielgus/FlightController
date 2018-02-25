#include "komunikacja.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QString>

Komunikacja komun;


void _protOdbierz(const uint8_t* _buffer, size_t _size)
{
    komun.odbierzPriv(_buffer, _size);
}

Komunikacja::Komunikacja()
{
    // nothing
}



void Komunikacja::initialize()
{
    pSerial->setArduinoSerial(arduino);
    pSerial->setPacketHandler(_protOdbierz);
}



void Komunikacja::odbierz()
{
    pSerial->update();
}



void Komunikacja::odbierzPriv(const uint8_t* bufferR, size_t PacketSize)
{
    if (bufferR[1] == KOMUN_RAMKA_ARDU_LIVE_TYPE && PacketSize == KOMUN_RAMKA_ARDU_LIVE_SIZE && sprawdzSumeKontr(bufferR, PacketSize))
    {
        pilot.throttle = bufferR[3];
        pilot.rotate = bufferR[4];
        pilot.tilt_TB = bufferR[5];
        pilot.tilt_LR = bufferR[6];

        // Jeżeli mniej niż ....size to zmniejszyć wielkość ....size
        //il_odebranych = 5; // ustawić tyle ile odebrano (do obsługi protezy)
        qDebug() << "Rozkodowano ramkę mumer 1 od arduino :)" << bufferR[4] << " " << bufferR[3];

    }

    else if (bufferR[1] == KOMUN_RAMKA_ARDU_2_TYPE && PacketSize == KOMUN_RAMKA_ARDU_2_SIZE && sprawdzSumeKontr(bufferR, PacketSize))
    {
        // costam = bufferR[2];
        // itd...
        // Jeżeli mniej niż ....size to zmniejszyć wielkość ....size
        //il_odebranych = 5; // ustawić tyle ile odebrano (do obsługi protezy)
    }
}



void Komunikacja::wyslij(uint8_t typRamki)
{
    buforT[1] = typRamki;

    if (typRamki == KOMUN_RAMKA_PC_1_TYPE)
    {
        buforT[2] = 5;
        buforT[3] = 77;
        buforT[4] = 78;
        // itd...
        // pamiętać o wielkości!!!

        buforT[0] = liczSumeKontr(buforT, KOMUN_RAMKA_PC_1_SIZE);

        pSerial->send(buforT, KOMUN_RAMKA_PC_1_SIZE);
    }

    else if (typRamki == KOMUN_RAMKA_PC_PARAMETERS_TYPE)
    {
        buforT[2] = conf.kP_level.bajt[0];
        buforT[3] = conf.kP_level.bajt[1];
        buforT[4] = conf.kP_level.bajt[2];
        buforT[5] = conf.kP_level.bajt[3];

        buforT[6] = conf.kI_level.bajt[0];
        buforT[7] = conf.kI_level.bajt[1];
        buforT[8] = conf.kI_level.bajt[2];
        buforT[9] = conf.kI_level.bajt[3];

        buforT[10] = conf.kD_level.bajt[0];
        buforT[11] = conf.kD_level.bajt[1];
        buforT[12] = conf.kD_level.bajt[2];
        buforT[13] = conf.kD_level.bajt[3];

        buforT[14] = conf.I_level_limiter;

        // dalej reszta...

        buforT[0] = liczSumeKontr(buforT, KOMUN_RAMKA_PC_PARAMETERS_SIZE);

        pSerial->send(buforT, KOMUN_RAMKA_PC_PARAMETERS_SIZE);
    }
}



bool Komunikacja::sprawdzSumeKontr(const uint8_t *buffer, size_t PacketSize)
{
    uint8_t suma_kontrolna = buffer[1];
    for(unsigned int i=2; i<PacketSize; i++)
    suma_kontrolna ^= buffer[i];	//xor'owanie kolejnych bajt?w

    if(suma_kontrolna==buffer[0]) return true;
    else return false;
}



uint8_t Komunikacja::liczSumeKontr(const uint8_t *buffer, size_t PacketSize)
{
    uint8_t suma_kontrolna = buffer[1];
    for(unsigned int i=2; i<PacketSize; i++)
    suma_kontrolna ^= buffer[i];	//xor'owanie kolejnych bajt?w

    return suma_kontrolna;
}
