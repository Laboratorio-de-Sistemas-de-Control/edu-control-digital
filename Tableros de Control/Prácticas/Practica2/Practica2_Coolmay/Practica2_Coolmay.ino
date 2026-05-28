/*
   Control de PWM en CONTROLLINO_D0 desde un HMI Coolmay
   usando un Numeric Entry (Holding Register 0)

   Comunicación: Modbus RTU - RS485
   Slave ID: 1
   Baudrate: 19200
*/

#include <ArduinoRS485.h>
#include <ArduinoModbus.h>
#include "Controllino.h"

const int pwmPin = CONTROLLINO_D0;

void setup() {

  // Iniciar Modbus RTU como slave ID 1
  if (!ModbusRTUServer.begin(1, 19200)) {
    while (1); // Si falla, se detiene aquí
  }

  // Pin de salida. 
  pinMode(pwmPin, OUTPUT); 

  // Configurar SOLO 1 Holding Register (HR0)
  ModbusRTUServer.configureHoldingRegisters(0x00, );

  // Valor inicial PWM = 50
  ModbusRTUServer.holdingRegisterWrite(0, 50);
}

void loop() {

  // Atender comunicación Modbus
  ModbusRTUServer.poll();

  // Leer el valor escrito desde el HMI (HR0)
  uint16_t pwmValue = ModbusRTUServer.holdingRegisterRead(0);

  // Limitar rango a 0–255 (PWM válido)
  if (pwmValue > 255) pwmValue = 255;

  // Aplicar PWM al pin
  analogWrite(pwmPin, pwmValue);
}
