#include <ModbusMaster.h>

// DE and RE pin mapping
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
  // Set virtual node to device with ID 1
  node.begin(1, Serial);

  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
}

void loop(){
  uint8_t result;
  //Request data from registers with temperature and himidity
  result = node.readInputRegisters(0x0001, 2);

  if(result == node.ku8MBSuccess){
    // Display data with 1 decimal place accuracy
    Serial.print("Temperature: ");
    Serial.println(node.getResponseBuffer(0) / 10.0f);
    Serial.print("Humidity: ");
    Serial.println(node.getResponseBuffer(1) / 10.0f);
  } else {
    // Print error code
    Serial.print("Modbus Error: ");
    Serial.println(result);
  }
  delay(1000);
}
