//Encender todos los leds del tablero secuencialmente
/* Se importa la librería de Controllino
   Se declara los pines D como salida.
*/
#include <Controllino.h> // Librería de controllino

void setup() {
  pinMode(CONTROLLINO_D0, OUTPUT);  // Salida digital D0
  pinMode(CONTROLLINO_D1, OUTPUT);  // Salida digital D1
  pinMode(CONTROLLINO_D2, OUTPUT);  // Salida digital D2
  pinMode(CONTROLLINO_D6, OUTPUT);  // Salida digital D6
  pinMode(CONTROLLINO_D7, OUTPUT);  // Salida digital D7
  pinMode(CONTROLLINO_D8, OUTPUT);  // Salida digital D8
  pinMode(CONTROLLINO_D12, OUTPUT); // Salida digital D12
  pinMode(CONTROLLINO_D13, OUTPUT); // Salida digital D13
  pinMode(CONTROLLINO_D14, OUTPUT); // Salida digital D14
}

void loop() {
  digitalWrite(CONTROLLINO_D0, HIGH);
  delay(500);
  digitalWrite(CONTROLLINO_D0, LOW);

  digitalWrite(CONTROLLINO_D1, HIGH);
  delay(500);
  digitalWrite(CONTROLLINO_D1, LOW);

  digitalWrite(CONTROLLINO_D2, HIGH);
  delay(500);
  digitalWrite(CONTROLLINO_D2, LOW);

  digitalWrite(CONTROLLINO_D6, HIGH);
  delay(500);
  digitalWrite(CONTROLLINO_D6, LOW);

  digitalWrite(CONTROLLINO_D7, HIGH);
  delay(500);
  digitalWrite(CONTROLLINO_D7, LOW);

  digitalWrite(CONTROLLINO_D8, HIGH);
  delay(500);
  digitalWrite(CONTROLLINO_D8, LOW);

  digitalWrite(CONTROLLINO_D12, HIGH);
  delay(500);
  digitalWrite(CONTROLLINO_D12, LOW);

  digitalWrite(CONTROLLINO_D13, HIGH);
  delay(500);
  digitalWrite(CONTROLLINO_D13, LOW);

  digitalWrite(CONTROLLINO_D14, HIGH);
  delay(500);
  digitalWrite(CONTROLLINO_D14, LOW);
}