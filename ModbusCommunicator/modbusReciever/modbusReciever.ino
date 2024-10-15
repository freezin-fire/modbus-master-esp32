#include <ModbusMaster.h>

#define MAX485_DE     23
#define MAX485_RE_NEG 23

ModbusMaster node;

void preTransmission(){
  digitalWrite(MAX485_RE_NEG, 1);
  digitalWrite(MAX485_DE, 1);
}

void postTransmission(){
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);
}

void setup(){
  pinMode(MAX485_RE_NEG, OUTPUT);
  pinMode(MAX485_DE, OUTPUT);
  
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);

  Serial.begin(9600);
  // Serial2.begin(9600);
  node.begin(1, Serial);

  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
}

void loop(){
  uint8_t result;

  result = node.readInputRegisters(0x0001, 2);

  if(result == node.ku8MBSuccess){
    Serial.print("Temperature: ");
    Serial.println(node.getResponseBuffer(0) / 10.0f);
    Serial.print("Humidity: ");
    Serial.println(node.getResponseBuffer(1) / 10.0f);
    // Serial.println(result);
  } else {
    Serial.print("Modbus Error: ");
    // Serial.println(node.getResponseBuffer(0));
    Serial.println(result);
  }
  delay(1000);
}