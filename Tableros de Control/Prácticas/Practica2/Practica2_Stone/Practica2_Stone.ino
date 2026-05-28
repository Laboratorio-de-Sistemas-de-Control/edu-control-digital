#include <Controllino.h>
#include "Stone_HMI_Define.h"
#include "Procesar_HMI.h"

const int led              = CONTROLLINO_D0; // Salida digital D0 
int       pwmValue         = 0;              // valor convertido (0-255)
float     dutyCyclePercent = 0;              // valor en porcentaje (0-100)

void setup() {
  Serial.begin(115200);   // Comunicación serial con el PC
  Serial2.begin(115200);  // Comunicación serial con el HMI
  pinMode(led, OUTPUT);   // led como salida
  HMI_init();             // Inicializa el sistema de colas para las respuestas el HMI
  Stone_HMI_Set_Value("spin_box", "spin_box1", NULL, 0);  // Pone en 0 el valor del spin box en el HMI. 
}

void loop() {
  dutyCyclePercent=HMI_get_value("spin_box", "spin_box1"); // Obtiene el valor del spin_box1

  if (dutyCyclePercent >= 0 && dutyCyclePercent <=100){
    pwmValue = map(dutyCyclePercent, 0, 100, 0, 255);      // Mapea el valor de duty cycle en porcentaje a valores de 0 a 255
    analogWrite(led, pwmValue);
    Serial.print("Duty cycle (%): ");
    Serial.print(dutyCyclePercent);
    Serial.print(" -> PWM value: ");
    Serial.println(pwmValue);
  }else {
    Serial.println("Ingresa un valor entre 0 y 100.");
  }
}