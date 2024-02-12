#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// Replace these with your WiFi credentials
const char *ssid = "YOUR_WIFI_SSID";
const char *password = "YOUR_WIFI_PASSWORD";

// IFTTT webhook key and event name
const char *iftttWebhookKey = "bfOh-57I2CvFbJitZqX1gH";
const char *eventName = "angle_exceeded";
const int angleCompensate = 0;
// Define pin numbers
const int angleSensorPin = A0; // Analog pin for angle sensor
const int relayPin = 4;        // Digital pin for relay control

// Define threshold angle
const float thresholdAngle = 45.0; // Threshold angle in degrees

float readAngle()
{
  // Read analog value from sensor
  int sensorValue = analogRead(angleSensorPin);

  // Convert analog value to angle (example: linear conversion)
  // angle = map(sensorValue, 0, 1023, 0, 360);
  float angle = map(analogRead(A0), 96, 927, -90, 90) + angleCompensate;
  Serial.println(angle);
  return angle;
}

void sendIFTTTMessage()
{
  // Create HTTPClient object
  HTTPClient http;
  WiFiClient client;
  // Construct URL with webhook key and event name
  String url = "http://maker.ifttt.com/trigger/";
  url += eventName;
  url += "/with/key/";
  url += iftttWebhookKey;

  // Send HTTP POST request
  http.begin(client, url);
  int httpResponseCode = http.GET();
  if (httpResponseCode > 0)
  {
    Serial.print("IFTTT message sent, response code: ");
    Serial.println(httpResponseCode);
  }
  else
  {
    Serial.print("Failed to send IFTTT message, error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();
}

void setup()
{
  Serial.begin(9600);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("WiFi connected");

  // Set relay pin as output
  pinMode(relayPin, OUTPUT);
}

void loop()
{
  // Read angle from sensor
  float angle = readAngle();

  // Check if angle exceeds threshold
  if (angle > thresholdAngle)
  {
    // Activate relay
    digitalWrite(relayPin, HIGH);

    // Send message via IFTTT webhook
    sendIFTTTMessage();
  }
  else
  {
    // Deactivate relay
    digitalWrite(relayPin, LOW);
  }

  delay(1000);
}
