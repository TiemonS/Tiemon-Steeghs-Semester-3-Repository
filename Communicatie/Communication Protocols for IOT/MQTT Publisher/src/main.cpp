#include <Arduino.h>
#include "PubSubClient.h"
#include <WiFi.h>

#include "payload.hpp"

//van chatgpt

// WiFi credentials
const char* ssid = "tiemon";
const char* ssid2 = "FabriekS3";
const char* password = "koelkast";
const char* password2 = "Semester3";

// MQTT broker settings
const char* mqtt_server = "192.168.43.161";
const int mqtt_port = 1883;
const char* mqtt_user = "username";
const char* mqtt_password = "password";
const char* mqtt_topic = "test";

// Pin definitions
const int motion_pin = 14;
const int ldr_pin = 32;

// Initialize WiFi client and MQTT client
WiFiClient wifi_client;
PubSubClient mqtt_client(wifi_client);

void reconnect();

void setup() {
  pinMode(ldr_pin, INPUT);
  pinMode(motion_pin, INPUT);

  Serial.begin(9600);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Set up MQTT client
  mqtt_client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  // Reconnect to MQTT broker if connection is lost
  if (!mqtt_client.connected()) {
    reconnect();
  }

  // Read motion sensor value
  int motion_value = digitalRead(motion_pin);
  // Serial.print("BewegingSensor data:");
  // Serial.println(motion_value);

  // Read LDR sensor value
  int ldr_value = analogRead(ldr_pin);
  //Serial.println(ldr_value);

  // Create MQTT message payload
  //String payload = "Motion: " + String(motion_value) + ", LDR: " + String(ldr_value);
  //String payload = String(motion_value) + String(motion_value);

  PayloadData payload;
  payload.motionValue = motion_value;
  payload.ldrValue = ldr_value;

  // Publish payload as byte array
  mqtt_client.publish(mqtt_topic, (const uint8_t*)&payload, sizeof(payload));


  // Publish MQTT message
  //mqtt_client.publish(mqtt_topic, payload.c_str());

  //Wait for a moment
  delay(500);
}

void reconnect() {
  // Loop until we're reconnected to MQTT broker
  while (!mqtt_client.connected()) {
    Serial.println("Attempting MQTT connection...");

    // Attempt to connect to MQTT broker
    if (mqtt_client.connect("ESP32CPublisher", mqtt_user, mqtt_password)) {
      Serial.println("Connected to MQTT broker");
      mqtt_client.subscribe("test");
    } else {
      Serial.print("Failed to connect to MQTT broker, rc=");
      Serial.print(mqtt_client.state());
      Serial.println(" Trying again in 5 seconds...");
      delay(5000);
    }
  }
}