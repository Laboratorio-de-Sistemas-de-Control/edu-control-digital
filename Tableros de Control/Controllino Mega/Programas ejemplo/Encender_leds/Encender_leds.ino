/*
Este ejemplo permite encender todos los leds del tablero de forma secuencia. 
*/

#include <Controllino.h> //Librería de controllino

// Vector con las salidas a usar y su tamaño
const int salidas[] = {CONTROLLINO_D0, CONTROLLINO_D1 , CONTROLLINO_D2 , 
                      CONTROLLINO_D6 , CONTROLLINO_D7 , CONTROLLINO_D8 , 
                      CONTROLLINO_D12, CONTROLLINO_D13, CONTROLLINO_D14};
const int tam       = sizeof(salidas)/(salidas[0]);
int contador        = 1; // Contador para avanzar el la lectura del vector de salidas. 

// Variables para el retardo no bloqueante
unsigned long  t_previo = 0;
const int periodo       = 1000;


void setup() {
  // Inicialización de la comunicación serial
  Serial.begin(9600);
  
  // Inicializar como salida cada pin del vector salidas[]
  for(int i=0; i<tam;i++){
    pinMode(salidas[i],OUTPUT);
  }

  // Inicialización de todos los leds apagados
  for(int i=0; i<tam;i++){
    digitalWrite(salidas[i],LOW);
  }
}

void loop() {
  unsigned long t_actual = millis();              // Tiempo desde que se ha ejecutado el programa

  if(t_actual-t_previo>=periodo){                 // Verifica si ha pasado el tiempo necesario
    t_previo=t_actual;                            // Actualización del tiempo previo

    if (contador!=0){                             // Encendido de leds actuales y apagado del led anterior
      digitalWrite(salidas[contador], HIGH);
      digitalWrite(salidas[contador-1], LOW);
    }else{
      digitalWrite(salidas[contador], HIGH);
      digitalWrite(salidas[tam-1], LOW);
    }
    
    contador+=1;                                  // Aumenta el contador en 1
    if (contador==tam){                           // si el contador es igual que el tamaño, se setea en 0 para empezar de nuevo
      contador=0;
    }
  }

}
