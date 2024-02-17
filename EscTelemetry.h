#ifndef EscTelemetry_h
#define EscTelemetry_h

#include <Arduino.h>

#define ESC_TELEMETRY_LENGTH 10
#define TLM_BYTE_TIME 87    // duration of one byte transmission (us)

class EscTelemetry
{
private:
    
    uint8_t telemetryBuffer[ESC_TELEMETRY_LENGTH];

    uint8_t i;
    unsigned int lostPackageCounter;

    uint8_t temperature;    // esc temperature (°C)
    uint16_t voltage;       // voltage of the ESC power supply (0.01V)
    uint16_t current;       // esc current (0.01A)
    uint16_t consumption;   // esc consumption (mAh)
    uint16_t rpm;           // esc electrical rpm (100rpm)
    bool valid;             // result of communication CRC

    uint8_t updateCrc8(uint8_t crc, uint8_t crc_seed);
    uint8_t crc8(uint8_t* buf, uint8_t buflen);
    void extractTelemetryData(int i);

    HardwareSerial * _pHardSerial;


public:
    EscTelemetry(HardwareSerial *pHardSerial);
    void updateEscTelemetry();
    void printTelemetry();
};


#endif