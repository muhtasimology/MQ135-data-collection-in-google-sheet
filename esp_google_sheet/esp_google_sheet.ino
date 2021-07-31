#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include "MQ135.h"

const char* ssid = "Spellbound";
const char* pass = "spellbound";
const char* host = "script.google.com";
const char* fingerprint = "1d ba 6d 5a 57 76 dd a7 49 61 96 79 39 36 9e 88 8b 3f e4 39";
String url;
int mq135 = A0;
int data = 0;

void setup() 
{
  Serial.begin(115200);
  delay(100);


  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Netmask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: ");
  Serial.println(WiFi.gatewayIP());
}
  void loop() {
    Serial.print("connecting to ");
    Serial.println(host);

    WiFiClientSecure client;

    const int httpPort = 443;
    if (!client.connect(host, httpPort)) {
      Serial.println("connection failed");
      return;
    }

    if (client.verify(fingerprint, host)) {
      Serial.println("certificate matches");
    } else {
      Serial.println("certificate doesn't match");
      data = analogRead(mq135);
      MQ135 gasSensor = MQ135(A0);
      float aqi = gasSensor.getPPM();
      url = "/macros/s/AKfycbwPOmHX201Ztiirrlb43bPHkwyHEvfJoRMgsn2RPjUcTkAXLuGmmacpY2xikoPmEbft/exec?func=addData&val=" + String(aqi);
            client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                         "Host: " + host + "\r\n" +
                         "Connection: close\r\n\r\n");
      delay(500);
      String section = "header";
      while (client.available()) {
        String line = client.readStringUntil('\r');
        Serial.print(line);
      }
      Serial.println();
      Serial.println("closing connection");
      delay(60000);
    }

  }
