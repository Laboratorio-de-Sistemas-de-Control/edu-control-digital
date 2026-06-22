#include <Controllino.h>
#include "Stone_HMI_Define.h"
HMI_ProtocolMsg hmi_msg;
String str;

// Variables para enviar a graficar datos cada cierto tiempo
unsigned long t_previo = 0;
const int intervalo    = 1;
int i                  = 0;


void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
}

void loop() {
  Uart_HMI_Data_Receive();
  /*Write your code here*/
  unsigned long t_actual = millis();

  // Verifica si ha pasado suficiente tiempo para graficar (se hace para evitar un bloqueo de flujo)
  if (t_actual - t_previo >= 1000) {
    t_previo = t_actual;

    // Envía a graficar un punto al final del Line Series del Chart View
    STONE_push_series("line_series", "line_series1", i);      //Envía un valor del eje X a graficar en el line_series1 que se pondrá al final
    STONE_push_series("line_series", "line_series2", i+2000); //Envía un valor del eje X a graficar en el line_series2 que se pondrá al final
    Serial.println(i);

    // Incrementa el contador 
    i=i+250;
    if (i > 7000) {
      i = 0;
    }
  }
}