#include <Arduino.h>
#include "EscTelemetry.h"

EscTelemetry::EscTelemetry(HardwareSerial *pHardSerial)
{
    _pHardSerial = pHardSerial;
};

//
// crc8 calculation
//
uint8_t EscTelemetry::updateCrc8( uint8_t crc, uint8_t crc_seed ) 
{
  static uint8_t crc_u;

  crc_u = crc;
  crc_u ^= crc_seed;

  for ( int i = 0; i < 8; i++ ) {
    crc_u = ( crc_u & 0x80 ) ? 0x7 ^ ( crc_u << 1 ) : ( crc_u << 1 );
  }

  return crc_u;
}

uint8_t EscTelemetry::crc8( uint8_t* buf, uint8_t buflen ) 
{
  static uint8_t crc;

  crc = 0;

  for ( int i = 0; i < buflen; i++ ) {
    crc = updateCrc8( buf[i], crc );
  }

  return crc;
}

void EscTelemetry::extractTelemetryData(int i) 
{
    switch (i)
    {
    case 0:
        temperature = telemetryBuffer[0];
        break;
    case 2:
        voltage = ( telemetryBuffer[1] << 8 ) | telemetryBuffer[2];
        break;
    case 4:
        current = ( telemetryBuffer[3] << 8 ) | telemetryBuffer[4];
        break;
    case 6:
        consumption = ( telemetryBuffer[5] << 8 ) | telemetryBuffer[6];
    case 8:
        rpm = ( telemetryBuffer[7] << 8 ) | telemetryBuffer[8];
        break;
    case 9:
        valid = ( telemetryBuffer[9] == crc8( &telemetryBuffer[9], ESC_TELEMETRY_LENGTH - 1 ) );

        if (!valid)
        {
            do {
            _pHardSerial->clear( );
            delayMicroseconds(TLM_BYTE_TIME * 2);
            } while (_pHardSerial->available( ));

            lostPackageCounter++;
            i = 0;
        }
        
        break;
    default:
        break;
    }
}

void EscTelemetry::updateEscTelemetry()
{

  // Read all bytes in rx buffer up to packet length
  while (_pHardSerial->available() > 0) {     
    telemetryBuffer[i] = _pHardSerial->read( );   // read next Byte

    extractTelemetryData(i);

    if (i < ESC_TELEMETRY_LENGTH) i++;
    else i = 0;
  }
}

void EscTelemetry::printTelemetry()
{
      Serial.print("Temp");
      Serial.print(":");
      Serial.print(temperature);
      Serial.print(",");
      Serial.print("Volt");
      Serial.print(":");
      Serial.print(voltage);
      Serial.print(",");
      Serial.print("Amps");
      Serial.print(":");
      Serial.print(current);
      Serial.print(",");
      Serial.print("mAh");
      Serial.print(":");
      Serial.print(consumption);
      Serial.print(",");
      Serial.print("rpm");
      Serial.print(":");
      Serial.print(rpm);
      Serial.print(",");
      Serial.print("valid");
      Serial.print(":");
      Serial.println(valid);
}