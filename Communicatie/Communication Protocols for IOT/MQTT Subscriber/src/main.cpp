#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include "PubSubClient.h"

#include "payload.hpp"

const int Led = 12;

const char* ssid = "tiemon";
const char* password = "koelkast";

String lastMessage;
const char* mqttServer = "192.168.43.161";
const int mqttPort = 1883;
const char* mqtt_user = "username";
const char* mqtt_password = "password";
const char* mqtt_topic = "test";


WiFiClient espClient;
PubSubClient client(espClient);

void reconnect()
{
  while (!client.connected())
  {
    Serial.print("Connecting to MQTT broker...");
    if (client.connect("ESP32Subscriber", mqtt_user, mqtt_password))
    {
      Serial.println("connected");
      client.subscribe(mqtt_topic); // Subscribe to the desired MQTT topic(s)
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 5 seconds");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length)
{
  Serial.print("Received message on topic: ");
  Serial.println(topic);
  Serial.print("Message: ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  lastMessage = "";
  for (int i = 0; i < length; i++)
  {
    lastMessage += (char)payload[i];
  }
  Serial.println();
}

void setup()
{
  pinMode(Led, OUTPUT);

  Serial.begin(9600);
  digitalWrite(Led, HIGH);
  WiFi.begin(ssid, password);
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi!");
  client.subscribe(mqtt_topic);
}

void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
  if (lastMessage == "Motion 1")
  {
    digitalWrite(Led, HIGH);
  }
  else if (lastMessage == "Motion 0")
  {
    digitalWrite(Led, LOW);
  }
}