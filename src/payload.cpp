#include "payload.h"

#include <assert.h>
#include <stdint.h>
#include <string.h>

Payload::Payload()
  : mPayload{ 0 }
{
}

Payload::Payload(const uint8_t* const payload)
{
  memcpy(mPayload, payload, 10);
}

bool Payload::operator==(const Payload& rhs) const
{
  const uint8_t* const rhsPayloadBuffer = rhs.GetBuffer();
  const uint8_t* const lhsPayloadBuffer = this->GetBuffer();
  const auto payloadSize = this->GetSize();
  for (size_t i = 0; i < payloadSize; ++i)
  {
    if (lhsPayloadBuffer[i] != rhsPayloadBuffer[i])
    {
      return false;
    }
  }

  return true;
}

void Payload::StrictSetVersionControl(const uint8_t version)
{
  assert(version <= 15);

  mPayload[0] &= 0xF;
  mPayload[0] |= (version << 4);
}

uint8_t Payload::GetVersionControl() const
{
  return (mPayload[0] >> 4);
}

void Payload::SetBatteryOkFlag(const bool batteryOkFlag)
{
  mPayload[0] &= 0xF7;
  mPayload[0] |= (batteryOkFlag << 3);
}

bool Payload::GetBatteryOkFlag() const
{
  return (mPayload[0] & 0x8);
}

void Payload::StrictSetTemperature(const float temperature)
{
  assert(temperature >= -50.0f && temperature <= 154.7f);
  
  const uint16_t convertedTemperature = static_cast<uint16_t>((temperature + 50.0f) * 5);
  
  mPayload[1] &= 0x0;
  mPayload[1] |= (convertedTemperature >> 2);
  
  mPayload[2] &= 0x3F;
  mPayload[2] |= ((convertedTemperature & 0x3) << 6);
}

float Payload::GetTemperature() const
{
  const uint16_t rawInteger = ((mPayload[1] << 2) | ((mPayload[2] & 0xC0) >> 6));
  return ((rawInteger / 5.0f) - 50);
}

void Payload::StrictSetHumidity(const float humidityPercentage)
{
  assert(humidityPercentage >= 0.0f && humidityPercentage <= 100.0f);
  const uint8_t convertedHumidityPercentage = static_cast<uint8_t>(humidityPercentage / 100 * 127);
  mPayload[2] &= 0xC0;
  mPayload[2] |= ((convertedHumidityPercentage & 0x7F) >> 1);

  mPayload[3] &= 0x7F;
  mPayload[3] |= (convertedHumidityPercentage << 7);
    
}

float Payload::GetHumidity() const
{
  const uint8_t rawInteger = (((mPayload[2] & 0x3F) << 1) | (mPayload[3] >> 7));
  return (rawInteger / 127.0f * 100);
}

void Payload::StrictSetGasLevels(const float gasLevels)
{
  assert(gasLevels >= 0.0f && gasLevels <= 3.0f);
  const uint8_t convertedGasLevels = static_cast<uint8_t>(gasLevels * 42.666f);
  
  mPayload[3] &= 0x80;
  mPayload[3] |= convertedGasLevels;
}

float Payload::GetGasLevels() const
{
  const uint8_t rawInteger = (mPayload[3] & 0x7F);
  return (rawInteger / 42.666f);
}

void Payload::StrictSetGpsCoordinates(const GpsCoords& gpsCoordinates)
{
  assert(gpsCoordinates.mLatitude >= -180.0 && gpsCoordinates.mLatitude <= 180.0);
  const uint32_t convertedLatitude = static_cast<uint32_t>((gpsCoordinates.mLatitude + 180.0) * 10000);
  mPayload[4] &= 0x0;
  mPayload[4] = (convertedLatitude >> 16);
  mPayload[5] &= 0x0;
  mPayload[5] = ((convertedLatitude & 0xFF00) >> 8);
  mPayload[6] &= 0x0;
  mPayload[6] = (convertedLatitude & 0xFF);
    
  assert(gpsCoordinates.mLongtitude >= -180.0 && gpsCoordinates.mLongtitude <= 180.0);
  const uint32_t convertedLongtitude = static_cast<uint32_t>((gpsCoordinates.mLongtitude + 180.0) * 10000);
  mPayload[7] &= 0x0;
  mPayload[7] = (convertedLongtitude >> 16);
  mPayload[8] &= 0x0;
  mPayload[8] = ((convertedLongtitude & 0xFF00) >> 8);
  mPayload[9] &= 0x0;
  mPayload[9] = (convertedLongtitude & 0xFF);
}

GpsCoords Payload::GetGpsCoordinates() const
{
  const uint32_t rawLatitudeInteger = ((static_cast<uint32_t>(mPayload[4]) << 16) | (static_cast<uint32_t>(mPayload[5]) << 8) | static_cast<uint32_t>(mPayload[6]));
  const uint32_t rawLongtitudeInteger = ((static_cast<uint32_t>(mPayload[7]) << 16) | (static_cast<uint32_t>(mPayload[8]) << 8) | static_cast<uint32_t>(mPayload[9]));
  
  return {rawLatitudeInteger / 10000.0 - 180.0, rawLongtitudeInteger / 10000.0 - 180.0};
}

const uint8_t* Payload::GetBuffer() const
{
  return mPayload;
}

size_t Payload::GetSize() const
{
  return 10;
}