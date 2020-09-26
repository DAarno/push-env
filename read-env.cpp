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

#include "read-env.h"

#include <algorithm>

#include <Arduino_MKRENV.h>

EnvData ReadEnv() {
  const int NUM_READS = 10;
  const int DELAY_MS = 1000 / NUM_READS;

  float temperature[NUM_READS];
  float humidity[NUM_READS];
  float pressure[NUM_READS];
  float illuminance[NUM_READS];
  float uva[NUM_READS];
  float uvb[NUM_READS];
  float uvIndex[NUM_READS];

  for (int i = 0; i < NUM_READS; i++) {
    temperature[i] = ENV.readTemperature();
    humidity[i] = ENV.readHumidity();
    pressure[i] = ENV.readPressure();
    illuminance[i] = ENV.readIlluminance();
    uva[i] = ENV.readUVA();
    uvb[i] = ENV.readUVB();
    uvIndex[i] = ENV.readUVIndex();

    delay(DELAY_MS);
  }

  std::sort(temperature, temperature + NUM_READS);
  std::sort(humidity, humidity + NUM_READS);
  std::sort(pressure, pressure + NUM_READS);
  std::sort(illuminance, illuminance + NUM_READS);
  std::sort(uva, uva + NUM_READS);
  std::sort(uvb, uvb + NUM_READS);
  std::sort(uvIndex, uvIndex + NUM_READS);

  EnvData d;
  int medianIndex = NUM_READS / 2;
  d.temperature = temperature[medianIndex];
  d.humidity = humidity[medianIndex];
  d.pressure = pressure[medianIndex];
  d.illuminance = illuminance[medianIndex];
  d.uva = uva[medianIndex];
  d.uvb = uvb[medianIndex];
  d.uvIndex = uvIndex[medianIndex];

  return d;
}
