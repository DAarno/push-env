/*
   Copyright 2020 Google LLC

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "network-info.h"
#include "read-env.h"
#include "wifi.h"

#include "ArduinoLowPower.h"
#include <Arduino_MKRENV.h>
#include <SPI.h>
#include <WiFiNINA.h>

void pushEnvironmentToServer();

// Set to true to enter low power mode between reads, or false to busy
// wait (delay). Note that entering low power mode will disable serial
// output and will require device to be manually reset (double tap
// reset button) before new program can be uploaded.
static bool shouldDeepSleep = false;

// Set to enable or disable serial output, usually the inverse of
// shouldDeepSleep.
static bool useSerial = !shouldDeepSleep;

// How many seconds to sleep/wait between pushing environment readings
// to server.
static const uint32_t DEEP_SLEEP_SECONDS = 600;

// Initialize serial and wait for port to open
void initSerialOrHang() {
  Serial.begin(9600);
  // wait for serial port to connect. Needed for native USB port only
  while (!Serial) {}
  Serial.println("Serial initialized");
}

// Initialize MKRENV shield to read environment data
void initEnvOrHang() {
  if (!ENV.begin()) {
    Serial.println("Failed to initialize MKR ENV shield!");
    while (true) {}
  }
}

// Attempt to connect to Wifi network, retry every second until
// connected.
void connectWiFiOrHang() {
  int status = WL_IDLE_STATUS;
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(wifi_ssid());

    status = WiFi.begin(wifi_ssid(), wpa_pass());
    if (status != WL_CONNECTED) {
      delay(1000);
    }
  }
  Serial.println("Connected to wifi");
  printWifiStatus();
}

void setup() {
}

void beginHardwareOrHang() {
  if (useSerial) {
    initSerialOrHang();
  }
  initEnvOrHang();
  connectWiFiOrHang();
}

void endHardware() {
  WiFi.end();
  ENV.end();
}

void sleep() {
  if (shouldDeepSleep) {
    Serial.println("Entering deep sleep");
    LowPower.deepSleep(DEEP_SLEEP_SECONDS * 1000);
  } else {
    Serial.println("Busy waiting...");
    delay(DEEP_SLEEP_SECONDS * 1000);
  }
}

void loop() {
  beginHardwareOrHang();
  pushEnvironmentToServer();
  endHardware();
  sleep(); 
}
