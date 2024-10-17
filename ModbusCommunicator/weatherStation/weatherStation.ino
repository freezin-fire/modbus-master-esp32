#include <ModbusMaster.h>

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
  // Set virtual node to device with ID 1
  node.begin(1, Serial);

  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
}

void loop() {
  uint16_t result;
  
  // Reading the 8 Holding registers according to the sensor specifications
  result = node.readHoldingRegisters(0x0000, 8);

  if (result == node.ku8MBSuccess) {
    Serial.println("-------------------------------------");
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
    Serial.print("Modbus Error: ");
    Serial.println(result);
  }

  delay(1000);
}
