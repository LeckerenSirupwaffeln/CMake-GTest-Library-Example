#include <stdint.h>

/**
 * @brief Used to pack latitude and longtitude GPS coordinates together.
 * 
 */
struct GpsCoords
{
  double mLatitude;
  double mLongtitude;
};

/**
 * @brief Used to construct the SFFA payload.
 * 
 */
class Payload {
    
public:
  Payload();
  Payload(const Payload&) = default;
  Payload(Payload&&) = default;
  Payload(const uint8_t* const payload);
  Payload& operator=(const Payload&) = default;
  Payload& operator=(Payload&&) = default;
  bool operator==(const Payload& rhs) const;

  /**
   * @brief Used to set the version control value in the payload.
   * 
   * @param version Used to denote the version control. Valid range: [0 to 15].
   */
  void StrictSetVersionControl(const uint8_t version);

  /**
   * @brief Used to get the version control value from the payload.
   * 
   * @return uint8_t Used to denote the version control.
   */
  uint8_t GetVersionControl() const;

  /**
   * @brief Used to set the battery OK flag in the payload.
   * 
   * @param batteryOkFlag Used to denote the battery status.
   */
  void SetBatteryOkFlag(const bool batteryOkFlag);

  /**
   * @brief Used to get the battery OK flag from the payload.
   * 
   * @return true Used to denote that the battery status is okay.
   * @return false Used to denote that the battery status is not okay.
   */
  bool GetBatteryOkFlag() const;
    
  /**
   * @brief Used to set the temperature value in the payload.
   * 
   * @param temperature Used to denote the temperature in Celsius. Valid range: [-50.0f to 154.7f].
   */
  void StrictSetTemperature(const float temperature);

  /**
   * @brief Used to get the temperature value from the payload.
   * 
   * @return float Used to denote the temperature in Celsius.
   */
  float GetTemperature() const;
    
  /**
   * @brief Used to set the humidity value in the payload.
   * 
   * @param humidityPercentage Used to denote the humidity in percentage. Valid range: [0.0f to 100.0f].
   */
  void StrictSetHumidity(const float humidityPercentage);

  /**
   * @brief Used to get the humidity value from the payload.
   * 
   * @return float Used to denote the humidity in percentage.
   */
  float GetHumidity() const;
    
  /**
   * @brief Used to set the gas levels value in the payload.
   * 
   * @param gasLevels Used to denote the gas levels in DC voltage. Valid range: [0.0f to 3.0f].
   */
  void StrictSetGasLevels(const float gasLevels);

  /**
   * @brief Used to get the gas levels from the payload.
   * 
   * @return float used to denote the gas levels in DC voltage.
   */
  float GetGasLevels() const;
    
  /**
   * @brief Used to set the GPS coordinates values in the payload.
   * 
   * @param gpsCoordinates Used to denote the GPS coordinates in Latitude and Longtitude. Valid range: [-180.0 to 180.0]
   */
  void StrictSetGpsCoordinates(const GpsCoords& gpsCoordinates);

  /**
   * @brief Used to get the GPS coordinates values from the payload.
   * 
   * @return GpsCoords Used to denote the GPS coordinates in Latitude and Longtitude.
   */
  GpsCoords GetGpsCoordinates() const;
    
  /**
   * @brief Used to get the pointer to the buffer of the payload.
   * 
   * @return const uint8_t* Used to denote the pointer to the buffer of the payload.
   */

  const uint8_t* GetBuffer() const;

  /**
   * @brief Used to get the size of the buffer of the payload.
   * 
   * @return size_t Used to denote the size of the buffer of the payload in bytes.
   */
  size_t GetSize() const;
    
private:
  /*[mPayload Start]
    [Version Control, 4 bits, default=00]
    [Battery OK flag, 1 bit, 1=OK, 0=BAD]
    [Undefined, 3 bits]
    [Temperature, 10 bits, originally a float value ranging from -50.0C to 154.7C, encoded by: (adding 50, multiplying by 5, and converting to uint16_t)]
    [Humidity, 7 bits, originally a float percentage ranging from 0.0% to 100.0%, encoded by: (dividing by 100, multipliying by 127 and converting to uint8_t)]
    [Gas Levels, 7 bits, originally a float value ranging from 0.0V to 3.0V, encoded by: (multiplying by 42.666f and converting to uint8_t)]
    [GPS 48 bits, 24 bits latitude, 24 bits longtitude, both values originally a double ranging from -180.000 to 180.000, encoded by: (adding 180, multiplying by 10000 and converting to uint32_t and using only the LSB 24-bits)]
  [mPayload End] */
  uint8_t mPayload[10];
};