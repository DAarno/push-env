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

#include <Arduino_MKRENV.h>
#include <WiFiNINA.h>

void sendValue(WiFiClient &client, const char *key, float value,
               char separator = '&') {
  Serial.print(key);
  Serial.print('=');
  Serial.println(value);

  client.print(separator);
  client.print(key);
  client.print('=');
  client.print(value);
}

void sendDataToServer(WiFiClient &client) {
  Serial.println("\nRead environment data ...");
  EnvData env = ReadEnv();

  // Make a HTTP request
  client.print("GET /texttv/temp.cgi");

  sendValue(client, "temp", env.temperature, '?');
  sendValue(client, "humidity", env.humidity);
  sendValue(client, "pressure", env.pressure);
  sendValue(client, "illuminance", env.illuminance);
  sendValue(client, "uva", env.uva);
  sendValue(client, "uvb", env.uvb);
  sendValue(client, "uvIndex", env.uvIndex);

  client.println(" HTTP/1.1");
  client.print("Host: ");
  client.println(server_addr());
  client.println("Connection: close");
  client.println();
}

void pushEnvironmentToServer() {
  Serial.print("\nStarting connection to server: ");
  Serial.println(server_addr());
  WiFiClient client;
  if (!client.connect(server_addr(), 80)) {
    Serial.println("Connection failed!");
    return;
  }

  sendDataToServer(client);

  for (int i = 0; !client.available(); i++) {
    delay(100);
    if (i > 10) {
      Serial.println("Giving up");
      client.stop();
      return;
    }
  }

  // This is a hack, because reasons... (connection won't close)
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }
  Serial.write("Finished reading data");
  Serial.println();
  Serial.println("disconnecting from server.");
  client.stop();
}
