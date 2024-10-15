#include <ModbusRTUSlave.h>
// #include <ModbusSlave0.h>

#define MAX485_DE 21
#define MAX485_RE 22

#define HOLDING_REGS_SIZE 10


ModbusRTUSlave slave(Serial, MAX485_DE);
// ModbusSlave slave(1, 21);

uint16_t holdingRegs[HOLDING_REGS_SIZE];

void setup(){
  pinMode(MAX485_RE, OUTPUT);
  pinMode(MAX485_DE, OUTPUT);
  
  // slave.cbVector[CB_WRITE_COILS] = readAnalogIn

  digitalWrite(MAX485_RE, 0);
  digitalWrite(MAX485_DE, 0);

  Serial1.begin(115200, SERIAL_8N1);
  Serial.begin(115200);
  
  slave.begin(1, Serial1);

  slave.configureHoldingRegisters(holdingRegs, HOLDING_REGS_SIZE);

  holdingRegs[0] = 123;
}

void loop(){
  slave.poll();

  Serial.print("Reg 0: ");
  Serial.println(holdingRegs[0]);

  delay(1000);
}