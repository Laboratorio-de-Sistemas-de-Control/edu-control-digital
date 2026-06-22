// Encendido y apagado de un LED del tablero mediante dos botones
#include <Controllino.h> //Librería de controllino

const int led             = CONTROLLINO_D0;    // Salida D0 del controllino
const int boton_encendido = CONTROLLINO_I16;   // Entrada I16 del controllino
const int boton_apagado   = CONTROLLINO_I17;   // Entrada I17 del controllino

void setup() {
  pinMode(led, OUTPUT);   // led como salida
  pinMode(boton_encendido, INPUT);  // boton_encendido como entrada
  pinMode(boton_apagado, INPUT);  // boton_apagado como entrada
}

void loop() {
  // Encender el led con el botonI16
  if (digitalRead(boton_encendido) == HIGH && digitalRead(boton_apagado)==LOW) {
    digitalWrite(led, HIGH);  // Encender LED
  }

  // Apagar el led con el botonI17
  if (digitalRead(boton_apagado) == HIGH) {
    digitalWrite(led, LOW);  // Encender LED
  }

  delay(10); // Pequeño retardo para evitar rebotes
}
