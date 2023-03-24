#include <stdint.h>

#include <gtest/gtest.h>
#include <payload.h>

// Payload constructor tests
TEST(PayloadConstructorTest, EmptyConstructorGivesEmptyBuffer) {
  const uint8_t zeroArray[10] = { 0 };
  const Payload zeroInitPayload { zeroArray };

  const Payload emptyPayload {};

  EXPECT_EQ(emptyPayload, zeroInitPayload);
}

TEST(PayloadConstructorTest, PayloadCopyConstructorGivesSamePayload) {
  Payload originalPayload { };
  originalPayload.StrictSetVersionControl(2);
  originalPayload.SetBatteryOkFlag(true);
  originalPayload.StrictSetTemperature(100.0f);
  originalPayload.StrictSetHumidity(50.0f);
  originalPayload.StrictSetGasLevels(0.15f);
  originalPayload.StrictSetGpsCoordinates({155.25f, -129.798f});

  const Payload copyPayload { originalPayload };

  EXPECT_EQ(copyPayload, originalPayload);
}

TEST(PayloadConstructorTest, PayloadRvalueConstructorGivesSamePayload) {
  Payload moveablePayload { };
  moveablePayload.StrictSetVersionControl(2);
  moveablePayload.SetBatteryOkFlag(true);
  moveablePayload.StrictSetTemperature(100.0f);
  moveablePayload.StrictSetHumidity(50.0f);
  moveablePayload.StrictSetGasLevels(0.15f);
  moveablePayload.StrictSetGpsCoordinates({155.25f, -129.798f});

  const Payload copyPayload { moveablePayload };

  const Payload movedPayload { std::move(moveablePayload) };

  EXPECT_EQ(movedPayload, copyPayload);
}

TEST(PayloadConstructorTest, ArrayConstructorGivesCorrectPayload) {
  const uint8_t testArray[10] = {255, 127, 63, 31, 16, 8, 4, 2};

  const Payload payload { testArray };

  const uint8_t* const payloadBuffer = payload.GetBuffer();
  const auto payloadSize = payload.GetSize();
  for(auto i = 0; i < payloadSize; ++i)
  {
    EXPECT_EQ(payloadBuffer[i], testArray[i]);
  }
}

// Payload operator tests
TEST(PayloadOperatorTest, PayloadCopyAssignmentGivesSamePayload) {
  const uint8_t testArray[10] = {255, 127, 63, 31, 16, 8, 4, 2};
  const Payload originalPayload { testArray };

  const Payload copyPayload = originalPayload;

  EXPECT_EQ(copyPayload, originalPayload);
}

TEST(PayloadOperatorTest, PayloadRvalueAssignmentGivesSamePayload) {
  const uint8_t testArray[10] = {255, 127, 63, 31, 16, 8, 4, 2};
  const Payload moveablePayload { testArray };
  
  const Payload copyPayload = moveablePayload;

  const Payload movedPayload = std::move(moveablePayload);

  EXPECT_EQ(movedPayload, copyPayload);
}

TEST(PayloadOperatorTest, PayloadIsEqualToCopyPayloadGivesTrue) {
  const uint8_t testArray[10] = {255, 127, 63, 31, 16, 8, 4, 2};
  const Payload originalPayload { testArray };

  const Payload copyPayload { originalPayload };

  EXPECT_EQ(copyPayload == originalPayload, true);
}

TEST(PayloadOperatorTest, PayloadIsEqualToDifferentPayloadGivesFalse) {
  uint8_t testArray[10] = {255, 127, 63, 31, 16, 8, 4, 2};
  const Payload firstPayload { testArray };

  testArray[0] = 1;
  const Payload differentPayload { testArray };

  EXPECT_EQ(differentPayload == firstPayload, false);
}

// Payload general tests
class PayloadGeneralTest : public ::testing::Test {
 protected:
  // void SetUp() override {}
  // void TearDown() override {}

  Payload payload;
};

TEST_F(PayloadGeneralTest, StrictSetVersionControlGivesCorrectPayload)
{
  payload.StrictSetVersionControl(10);

  const uint8_t correctArray[10] = { 160, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  const Payload correctPayload { correctArray };

  EXPECT_EQ(payload, correctPayload);
}

TEST_F(PayloadGeneralTest, StrictSetVersionControlKillsOnTooHighInput)
{
  EXPECT_DEATH(payload.StrictSetVersionControl(18), ".*");
  EXPECT_DEATH(payload.StrictSetVersionControl(128), ".*");
  EXPECT_DEATH(payload.StrictSetVersionControl(255), ".*");
}

TEST_F(PayloadGeneralTest, GetVersionControlGivesCorrectValue)
{
  EXPECT_EQ(payload.GetVersionControl(), 0);

  payload.StrictSetVersionControl(14);
  EXPECT_EQ(payload.GetVersionControl(), 14);
}

TEST_F(PayloadGeneralTest, SetBatteryOkFlagGivesCorrectPayload)
{
  payload.SetBatteryOkFlag(true);

  const uint8_t correctArray[10] = { 8, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  const Payload correctPayload { correctArray };

  EXPECT_EQ(payload, correctPayload);
}

TEST_F(PayloadGeneralTest, GetBatteryOkFlagGivesCorrectValue)
{
  EXPECT_EQ(payload.GetBatteryOkFlag(), false);

  payload.SetBatteryOkFlag(true);
  EXPECT_EQ(payload.GetBatteryOkFlag(), true);
}

TEST_F(PayloadGeneralTest, StrictSetTemperatureGivesCorrectPayload)
{
  payload.StrictSetTemperature(100.5f);

  const uint8_t correctArray[10] = { 0, 188, 0, 0, 0, 0, 0, 0, 0, 0 };
  const Payload correctPayload { correctArray };

  EXPECT_EQ(payload, correctPayload);
}

TEST_F(PayloadGeneralTest, StrictSetTemperatureKillsOnTooLowInput)
{
  EXPECT_DEATH(payload.StrictSetTemperature(-50.1f), ".*");
  EXPECT_DEATH(payload.StrictSetTemperature(-129.999f), ".*");
  EXPECT_DEATH(payload.StrictSetTemperature(-123456.789f), ".*");
}

TEST_F(PayloadGeneralTest, StrictSetTemperatureKillsOnTooHighInput)
{
  EXPECT_DEATH(payload.StrictSetTemperature(154.8f), ".*");
  EXPECT_DEATH(payload.StrictSetTemperature(300.19f), ".*");
  EXPECT_DEATH(payload.StrictSetTemperature(123456.789f), ".*");
}

TEST_F(PayloadGeneralTest, GetTemperatureGivesCorrectValue)
{
  EXPECT_NEAR(payload.GetTemperature(), -50.0f, 0.2f);

  payload.StrictSetTemperature(154.7f);
  EXPECT_NEAR(payload.GetTemperature(), 154.7f, 0.2f);

  payload.StrictSetTemperature(100.0f);
  EXPECT_NEAR(payload.GetTemperature(), 100.0f, 0.2f);

  payload.StrictSetTemperature(74.15f);
  EXPECT_NEAR(payload.GetTemperature(), 74.15f, 0.2f);

  payload.StrictSetTemperature(50.1f);
  EXPECT_NEAR(payload.GetTemperature(), 50.1f, 0.2f);

  payload.StrictSetTemperature(25.9999f);
  EXPECT_NEAR(payload.GetTemperature(), 25.9999f, 0.2f);

  payload.StrictSetTemperature(0.0f);
  EXPECT_NEAR(payload.GetTemperature(), 0.0f, 0.2f);

  payload.StrictSetTemperature(-26.34f);
  EXPECT_NEAR(payload.GetTemperature(), -26.34f, 0.2f);

  payload.StrictSetTemperature(-50.0);
  EXPECT_NEAR(payload.GetTemperature(), -50.0f, 0.2f);
}

TEST_F(PayloadGeneralTest, StrictSetHumidityGivesCorrectPayload)
{
  payload.StrictSetHumidity(100.0f);

  const uint8_t correctArray[10] = { 0, 0, 63, 128, 0, 0, 0, 0, 0, 0 };
  const Payload correctPayload { correctArray };

  EXPECT_EQ(payload, correctPayload);
}

TEST_F(PayloadGeneralTest, StrictSetHumidityKillsOnTooLowInput)
{
  EXPECT_DEATH(payload.StrictSetHumidity(-0.01f), ".*");
  EXPECT_DEATH(payload.StrictSetHumidity(-5.32f), ".*");
  EXPECT_DEATH(payload.StrictSetHumidity(-123456.789f), ".*");
}

TEST_F(PayloadGeneralTest, StrictSetHumidityKillsOnTooHighInput)
{
  EXPECT_DEATH(payload.StrictSetHumidity(100.01f), ".*");
  EXPECT_DEATH(payload.StrictSetHumidity(105.32f), ".*");
  EXPECT_DEATH(payload.StrictSetHumidity(123456.789f), ".*");
}

TEST_F(PayloadGeneralTest, GetHumidityGivesCorrectValue)
{
  EXPECT_NEAR(payload.GetHumidity(), 0.0f, 0.78125f);

  payload.StrictSetHumidity(100.0f);
  EXPECT_NEAR(payload.GetHumidity(), 100.0f, 0.78125f);

  payload.StrictSetHumidity(99.5f);
  EXPECT_NEAR(payload.GetHumidity(), 99.5f, 0.78125f);

  payload.StrictSetHumidity(70.15f);
  EXPECT_NEAR(payload.GetHumidity(), 70.15f, 0.78125f);

  payload.StrictSetHumidity(32.212f);
  EXPECT_NEAR(payload.GetHumidity(), 32.212f, 0.78125f);

  payload.StrictSetHumidity(0.5f);
  EXPECT_NEAR(payload.GetHumidity(), 0.5f, 0.78125f);

  payload.StrictSetHumidity(0.0f);
  EXPECT_NEAR(payload.GetHumidity(), 0.0f, 0.78125f);
}

TEST_F(PayloadGeneralTest, StrictSetGasLevelsGivesCorrectPayload)
{
  payload.StrictSetGasLevels(2.6f);

  const uint8_t correctArray[10] = { 0, 0, 0, 110, 0, 0, 0, 0, 0, 0 };
  const Payload correctPayload { correctArray };

  EXPECT_EQ(payload, correctPayload);
}

TEST_F(PayloadGeneralTest, StrictSetGasLevelsKillsOnTooLowInput)
{
  EXPECT_DEATH(payload.StrictSetGasLevels(-0.01f), ".*");
  EXPECT_DEATH(payload.StrictSetGasLevels(-5.32f), ".*");
  EXPECT_DEATH(payload.StrictSetGasLevels(-123456.789f), ".*");
}

TEST_F(PayloadGeneralTest, StrictSetGasLevelsKillsOnTooHighInput)
{
  EXPECT_DEATH(payload.StrictSetGasLevels(3.01f), ".*");
  EXPECT_DEATH(payload.StrictSetGasLevels(5.32f), ".*");
  EXPECT_DEATH(payload.StrictSetGasLevels(123456.789f), ".*");
}

TEST_F(PayloadGeneralTest, GetGasLevelsGivesCorrectValue)
{
  EXPECT_NEAR(payload.GetGasLevels(), 0.0f, 0.070315f);

  payload.StrictSetGasLevels(3.0f);
  EXPECT_NEAR(payload.GetGasLevels(), 3.0f, 0.070315f);

  payload.StrictSetGasLevels(2.56f);
  EXPECT_NEAR(payload.GetGasLevels(), 2.56f, 0.070315f);

  payload.StrictSetGasLevels(1.5f);
  EXPECT_NEAR(payload.GetGasLevels(), 1.5f, 0.070315f);

  payload.StrictSetGasLevels(1.212f);
  EXPECT_NEAR(payload.GetGasLevels(), 1.212f, 0.070315f);

  payload.StrictSetGasLevels(0.5f);
  EXPECT_NEAR(payload.GetGasLevels(), 0.5f, 0.070315f);

  payload.StrictSetGasLevels(0.0f);
  EXPECT_NEAR(payload.GetGasLevels(), 0.0f, 0.070315f);
}

TEST_F(PayloadGeneralTest, StrictSetGpsCoordinatesGivesCorrectPayload)
{
  payload.StrictSetGpsCoordinates( { 154.35f, -98.3651f } );

  const uint8_t correctArray[10] = { 0, 0, 0, 0, 51, 4, 140, 12, 116, 221 };
  const Payload correctPayload { correctArray };

  EXPECT_EQ(payload, correctPayload);
}

TEST_F(PayloadGeneralTest, StrictSetGpsCoordinatesKillsOnTooLowLatitudeInput)
{
  EXPECT_DEATH(payload.StrictSetGpsCoordinates( { -180.001f, 0.0f } ), ".*");
  EXPECT_DEATH(payload.StrictSetGpsCoordinates( { -360.0f, 0.0f } ), ".*");
  EXPECT_DEATH(payload.StrictSetGpsCoordinates( { -123456.789f, 0.0f } ), ".*");
}

TEST_F(PayloadGeneralTest, StrictSetGpsCoordinatesKillsOnTooHighLatitudeInput)
{
  EXPECT_DEATH(payload.StrictSetGpsCoordinates( { 180.001f, 0.0f } ), ".*");
  EXPECT_DEATH(payload.StrictSetGpsCoordinates( { 360.0f, 0.0f } ), ".*");
  EXPECT_DEATH(payload.StrictSetGpsCoordinates( { 123456.789f, 0.0f } ), ".*");
}

TEST_F(PayloadGeneralTest, StrictSetGpsCoordinatesKillsOnTooLowLongtitudeInput)
{
  EXPECT_DEATH(payload.StrictSetGpsCoordinates( { 0.0f, -180.001f } ), ".*");
  EXPECT_DEATH(payload.StrictSetGpsCoordinates( { 0.0f, -360.0f } ), ".*");
  EXPECT_DEATH(payload.StrictSetGpsCoordinates( { 0.0f, -123456.789f } ), ".*");
}

TEST_F(PayloadGeneralTest, StrictSetGpsCoordinatesKillsOnTooHighLongtitudeInput)
{
  EXPECT_DEATH(payload.StrictSetGpsCoordinates( { 0.0f, 180.001f } ), ".*");
  EXPECT_DEATH(payload.StrictSetGpsCoordinates( { 0.0f, 360.0f } ), ".*");
  EXPECT_DEATH(payload.StrictSetGpsCoordinates( { 0.0f, 123456.789f } ), ".*");
}

TEST_F(PayloadGeneralTest, GetGpsCoordinatesGivesCorrectValue)
{
  auto currentGpsCoordinates = payload.GetGpsCoordinates();
  EXPECT_NEAR(currentGpsCoordinates.mLatitude, -180.0f, 0.000101f);
  EXPECT_NEAR(currentGpsCoordinates.mLongtitude, -180.0f, 0.000101f);

  payload.StrictSetGpsCoordinates( { 154.35f, -98.3651f } );
  currentGpsCoordinates = payload.GetGpsCoordinates();
  EXPECT_NEAR(currentGpsCoordinates.mLatitude, 154.35f, 0.000101f);
  EXPECT_NEAR(currentGpsCoordinates.mLongtitude, -98.3651f, 0.000101f);

  payload.StrictSetGpsCoordinates( { 180.0f, -180.0f } );
  currentGpsCoordinates = payload.GetGpsCoordinates();
  EXPECT_NEAR(currentGpsCoordinates.mLatitude, 180.0f, 0.000101f);
  EXPECT_NEAR(currentGpsCoordinates.mLongtitude, -180.0f, 0.000101f);

  payload.StrictSetGpsCoordinates( { 90.0f, -90.0f } );
  currentGpsCoordinates = payload.GetGpsCoordinates();
  EXPECT_NEAR(currentGpsCoordinates.mLatitude, 90.0f, 0.000101f);
  EXPECT_NEAR(currentGpsCoordinates.mLongtitude, -90.0f, 0.000101f);

  payload.StrictSetGpsCoordinates( { 45.1234f, -45.1234f } );
  currentGpsCoordinates = payload.GetGpsCoordinates();
  EXPECT_NEAR(currentGpsCoordinates.mLatitude, 45.1234f, 0.000101f);
  EXPECT_NEAR(currentGpsCoordinates.mLongtitude, -45.1234f, 0.000101f);
}

TEST_F(PayloadGeneralTest, GetBufferGivesCorrectPointer)
{
  const void* payloadPtr = static_cast<const void*>(&payload);
  const void* bufferPtr =  static_cast<const void*>(payload.GetBuffer());

  EXPECT_EQ(bufferPtr, payloadPtr);
}

TEST_F(PayloadGeneralTest, GetSizeGivesCorrectValue)
{
  EXPECT_EQ(payload.GetSize(), 10);
}