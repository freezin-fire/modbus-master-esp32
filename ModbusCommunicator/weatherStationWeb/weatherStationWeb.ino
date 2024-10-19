#include <WiFi.h>
#include <HTTPClient.h>
#include <ModbusMaster.h>

// Replace with your Wi-Fi SSID and password
const char* ssid = "Airtel_kris_9998";
const char* password = "air23985";

// REPLACE with your Domain name and URL path or IP address with path
const char* serverName = "http://weatherdata.free.nf/esp-post-data.php";

// Keep this API Key value to be compatible with the PHP code.
// If you change the apiKeyValue value, the PHP file /post-esp-data.php also needs to have the same key
String apiKeyValue = "tPmAT5bgfe40536l";

String sensor_status = "Offline";

// DE and RE pin mapping
#define MAX485_DE 23
#define MAX485_RE_NEG 23

ModbusMaster node;

void preTransmission() {
  digitalWrite(MAX485_RE_NEG, 1);
  digitalWrite(MAX485_DE, 1);
}

void postTransmission() {
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);
}

void setup() {
  pinMode(MAX485_RE_NEG, OUTPUT);
  pinMode(MAX485_DE, OUTPUT);

  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);

  Serial.begin(9600);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  // Set virtual node to device with ID 1
  node.begin(1, Serial);

  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;

    // Your Domain name with URL path or IP address with path
    http.begin(client, serverName);

    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    uint16_t result;

    // Reading the 8 Holding registers according to the sensor specifications
    result = node.readHoldingRegisters(0x0000, 8);
    String httpRequestData;

    if (result == node.ku8MBSuccess) {
      sensor_status = "Online";
      httpRequestData = "api_key=" + apiKeyValue + "&sensor_status=" + sensor_status
                        + "&wind_dir=" + String(node.getResponseBuffer(0))
                        + "&wind_speed=" + String(node.getResponseBuffer(1))
                        + "&ambnt_temp=" + String(node.getResponseBuffer(2) / 10.0f)
                        + "&humidity=" + String(node.getResponseBuffer(3) / 10.0f)
                        + "&pyranometer=" + String(node.getResponseBuffer(4))
                        + "&rain=" + String(node.getResponseBuffer(5))
                        + "&mod_temp=" + String(node.getResponseBuffer(6) / 10.0f)
                        + "&barometer=" + String(node.getResponseBuffer(7) / 10.0f)
                        + "";

      Serial.print("Wind direction: ");
      Serial.print(node.getResponseBuffer(0));
      Serial.println(" Deg");
      Serial.print("Wind Speed: ");
      Serial.print(node.getResponseBuffer(1));
      Serial.println(" kmph");
      Serial.print("Amb Temp: ");
      Serial.print(node.getResponseBuffer(2) / 10.0f);
      Serial.println(" Deg C");
      Serial.print("Humidity: ");
      Serial.print(node.getResponseBuffer(3) / 10.0f);
      Serial.println(" Per.");
      Serial.print("Pyranometer: ");
      Serial.print(node.getResponseBuffer(4));
      Serial.println(" w/mm2");
      Serial.print("Rain: ");
      Serial.print(node.getResponseBuffer(5));
      Serial.println(" mm");
      Serial.print("Mod Temperature: ");
      Serial.print(node.getResponseBuffer(6) / 10.0f);
      Serial.println(" Deg C");
      Serial.print("Barometer Pressure: ");
      Serial.print(node.getResponseBuffer(7) / 10.0f);
      Serial.println(" hpa");
    } else {
      // Print error code
      httpRequestData = "api_key=" + apiKeyValue + "&sensor_status=" + sensor_status
                        + "&wind_dir=0"
                        + "&wind_speed=0"
                        + "&ambnt_temp=0"
                        + "&humidity=0"
                        + "&pyranometer=0"
                        + "&rain=0"
                        + "&mod_temp=0"
                        + "&barometer=0"
                        + "";

      Serial.print("Modbus Error: ");
      Serial.println(result);
    }


    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);

    int httpResponseCode = http.POST(httpRequestData);

    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }

  delay(15000);
}
