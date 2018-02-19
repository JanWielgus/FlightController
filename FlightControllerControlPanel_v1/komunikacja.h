#ifndef KOMUNIKACJA_H
#define KOMUNIKACJA_H

//#include <QObject>
//#include <QWidget>
#include "PacketSerial.h"
#include <QtSerialPort/QSerialPort>
#include <QByteArray>


class Komunikacja
{
public:
    Komunikacja();

    QSerialPort* arduino;
    PacketSerial* pSerial;
    uint8_t* buforT;

    static const quint16 arduino_uno_vendor_id = 6790;
    static const quint16 arduino_uno_product_id = 29987;
    unsigned int il_odebranych;

    // --- ramki ---
    // wysylane
    const uint8_t KOMUN_RAMKA_PC_1_SIZE = 5;
    const uint8_t KOMUN_RAMKA_PC_1_TYPE = 0x00;

    const uint8_t KOMUN_RAMKA_PC_PARAMETERS_SIZE = 30;
    const uint8_t KOMUN_RAMKA_PC_PARAMETERS_TYPE = 0x01;
    // odbierane
    const uint8_t KOMUN_RAMKA_ARDU_LIVE_SIZE = 5;
    const uint8_t KOMUN_RAMKA_ARDU_LIVE_TYPE = 0x00;

    const uint8_t KOMUN_RAMKA_ARDU_2_SIZE = 30;
    const uint8_t KOMUN_RAMKA_ARDU_2_TYPE = 0x01;


    void odbierzPriv(const uint8_t* buffer, size_t size);
    void wyslij(uint8_t typRamki);
    bool sprawdzSumeKontr(const uint8_t* buffer, size_t PacketSize);	//xor'owanie
    uint8_t liczSumeKontr(const uint8_t* buffer, size_t PacketSize);
    void initialize(); // inicjalizacja
    void odbierz();
};

extern Komunikacja komun;

#endif // KOMUNIKACJA_H
