
#include "Max30102Sensor.h"
#include "TempSensor.h"
#include "esp_wifi.h"
#include "esp_bt.h"

#include <RH_RF95.h>

// Define custom pins
#define NSS 5  // Slave Select (NSS) pin
#define RST 14 // Reset pin
#define DIO0 2

// Singleton instance of the radio driver with custom pins
RH_RF95 rf95(NSS, DIO0);

Max30102Sensor particleSensor;
TempSensor tempSensor;

#define STAND_BY_TIME 3 // In minutes

void setup()
{
  // Disable Wi-Fi
  esp_wifi_deinit();
  esp_bt_controller_deinit();
  // Disable Bluetooth

  Serial.begin(115200);
  Serial.println("Initializing...");
  Serial.println("Current Wi-Fi Status: ");

  wifi_mode_t mode;
  esp_wifi_get_mode(&mode);
  Serial.println(mode);

  Serial.println("Current Bluetooth Status: ");
  Serial.println(esp_bt_controller_get_status());

  if (!rf95.init())
  {
    Serial.println("LoRa init failed");
    while (1)
      ;
  }

  // Set frequency to 433 MHz
  if (!rf95.setFrequency(433.0))
  {
    Serial.println("setFrequency failed");
    while (1)
      ;
  }

  // Set transmit power to 20 dBm (max for SX1278)
  rf95.setTxPower(20, false);
  //   rf95.setThisAddress(MY_ADDRESS);
  //   rf95.setHeaderFrom(TARGET_ADDRESS);

  Serial.println("LoRa transmitter ready!");

  particleSensor.setup();
  tempSensor.setup();

  unsigned startTime = millis();
  unsigned endTime = startTime + STAND_BY_TIME * 3600 ;

  while (millis() < endTime)
  {
    particleSensor.measureBeats();
    Serial.print("Avg BPM=");
    Serial.print(particleSensor.getHeartRate());
    Serial.println();
    Serial.print("Temp in C: ");
    Serial.println(tempSensor.getTemperature());
    Serial.println();
  }

  std::string bindMsg = "HR: " + std::to_string(particleSensor.getHeartRate()) + " Temp: " + std::to_string(tempSensor.getTemperature());
  const char *message = bindMsg.c_str();
  rf95.send((uint8_t *)message, strlen(message));
  rf95.waitPacketSent();

  Serial.println("Message sent!");

  while (!rf95.available())
  {
  }

  Serial.println("Packet received!");
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);
  if (rf95.recv(buf, &len))
  {
    Serial.print("Received: ");
    buf[len] = '\0';
    Serial.println((char *)buf);
  }
  else
  {
    Serial.println("Receive failed!");
  }

  Serial.println("Going to sleep now");
  delay(1000);
  Serial.flush();
  esp_deep_sleep_start();
}

void loop()
{
  // Nothing here
}