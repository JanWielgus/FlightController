#ifndef PACKETSERIAL_H
#define PACKETSERIAL_H

#include <QObject>
#include <QWidget>
#include <QtSerialPort/QSerialPort>
#include <COBS.h>

class PacketSerial
{
public:
    PacketSerial();

    QSerialPort *arduinoSerial;
    typedef void (*PacketHandlerFunction)(const uint8_t* buffer, size_t size);

    const unsigned int MAX_SEND_SIZE = 60;

    size_t BufferSize = 256;
    uint8_t PacketMarker = 0;

    //uint8_t *_recieveBuffer;
    uint8_t _recieveBuffer[256];
    size_t _recieveBufferIndex;

    PacketHandlerFunction _onPacketFunction;

    // ===== FUNKCJE =====
    void setArduinoSerial(QSerialPort *sserial);
    //void begin(unsigned long baud);
    void update();
    void send(const uint8_t* buffer, size_t size);
    void setPacketHandler(PacketHandlerFunction onPacketFunction);
};

#endif // PACKETSERIAL_H
