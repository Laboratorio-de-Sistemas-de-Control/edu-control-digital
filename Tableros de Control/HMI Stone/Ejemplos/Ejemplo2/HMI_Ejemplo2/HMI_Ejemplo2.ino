#include <Controllino.h>
#include "Stone_HMI_Define.h"
HMI_ProtocolMsg hmi_msg;
String str;

// Funciones de procesamiento de las repsuestas del HMI
float get_value(const char* tipo, const char* nombre);
String get_button(const char* nombre);
bool HMI_get_switch(const char* nombre);

// variables para los switches de la ventana de semáforos
bool switch1=false;
bool switch2=false;
bool switch3=false;

// Variables para controlar los tiempos de gráfica
unsigned long previousMillis = 0;
const long interval = 200; // Enviar cada 200 ms
int i = 0;

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
  pinMode(CONTROLLINO_D0, OUTPUT);
  digitalWrite(CONTROLLINO_D0, LOW);
}

void loop() {
  Uart_HMI_Data_Receive();
  /*Write your code here*/

  // VENTANA PRINCIPAL (HOME)
  if (get_button("button1")=="Click"){         // Verificamos si el botón button1 ha completado un click
    Stone_HMI_Open_Win("window_sem");
  } 
  if (get_button("button2")=="Click"){         // Verificamos si el botón button2 ha completado un click
    Stone_HMI_Open_Win("window_motor");
  }

  // VENTANA DE CONTROL DE SEMÁFORO             // Verificamos si el botón button3 ha completado un click
  if (get_button("button3")=="Click"){
    Stone_HMI_Open_Win("home_page");
  }

  // VENTANA DE CONTORL DEL MOTOR                // Verificamos si el botón button5 ha completado un click
  if (get_button("button5")=="Click"){
    Stone_HMI_Open_Win("home_page");
  }

  // Verifica si el switch1 ha cambiado de estado
  HMI_event_switch("switch1", &switch1);
  digitalWrite(CONTROLLINO_D0, switch1);


  // Verifica si el swith2 ha cambiado de estado
  HMI_event_switch("switch2",&switch2);
  digitalWrite(CONTROLLINO_D1, switch2);

  // Verifica si el swith3 ha cambiado de estado
  HMI_event_switch("switch3",&switch3);
  digitalWrite(CONTROLLINO_D2, switch3);


  unsigned long currentMillis = millis();

  // Verifica si ha pasado suficiente tiempo paragraficar (se hace para evitar un bloqueo de flujo)
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    STONE_push_series("line_series", "line_series1", i); //Envía un valor del eje X a graficar en el line_series que se pondrá al final de la gráfica
    Serial.println(i);

    i++;
    if (i > 90) {
      i = 0;
    }
  }
}

// Función para obtener el valor de un widget button
String get_button(const char* nombre) {
  /* Función que se encarga de obtener el último byte de 
  hmi_msg.data, que corresponde a la respuesta del HMI tras
  presionar un botón. 

  Si se desea usar respuestas personalizadas revisar la librería del
  fabricante. Tal caso implica que el valor a leer sea de 2 bytes. 

  Entradas: 
    nombre: Nombre del botón de la interfaz

  Retorna: 
    El estado de un botón, el cual puede ser:
      Press:       Cuando se presiona el botón
      Click:       Cuando se hace un click completo en el botón
      Release:     CUando se librera el botón
      Long Press:  CUando se mantiene presionado el botón
      Non pressed: Cuando no se presiona nada
      Sin datos:   Cuando no hay ua respuesta
  */
  String str = "";
  uint8_t dato;

  if (hmi_msg.len > 2) {
    for (int i = 0; i < hmi_msg.len - 1; i++) {
      str += (char)hmi_msg.data[i];
    }

    //Serial.println(hmi_msg.len);
    //Serial.print("Nombre recibido: ");
    //Serial.println(str);

    if (str==nombre){
      //dato=(hmi_msg.data[hmi_msg.len-2]<<8) | (hmi_msg.data[hmi_msg.len-1]); // En caso de esperar respuestas personalizadas.
      dato=hmi_msg.data[hmi_msg.len-1];
      switch (dato) {
        case 0x01:
          return "Press";
          break;
        case 0x02:
          return "Click";
          break;
        case 0x04:
          return "Release";
          break;
        case 0x03:
          return "Long Press";
          break;
        default:
          return "Non pressed";
          break;
      }
    }
  }
  return "Sin datos";
}

// Función para obtener el valor de un slider
float get_value(const char* tipo, const char* nombre){
  /*Función que se encarga de obtener los últimos 4 bytes de hmi_msg.data. Que 
  corresponden al valor que tiene un slider para convertirlos en un flotante.
  
  Los bytes iniciales del mensaje recibido hmi_msg corresponde al nombre del widget.

  Recibe: 
    tipo: Tipo de widget, por el momento solo slider

  Retorna: 
    Valor: Valor flotante que tiene el slider
  */
  uint8_t dato_reordenado[4];
  String str ="";
  float valor=-1;

  if (strcmp(tipo, "slider")==0){ // Compara las dos cadenas de texto
    Stone_HMI_Get_Value(tipo, nombre);

    if(hmi_msg.len>0){
      str = "";  
      for (int i = 0; i < hmi_msg.len-4; i++) { // Guarda el nombre del widget
        str += (char)hmi_msg.data[i]; 
      }

      if (str == nombre) {
        for(int i =hmi_msg.len; i>=hmi_msg.len-4; i--){ // Se toma los últimos 4 bytes y se los invierte
          dato_reordenado[hmi_msg.len-i]=hmi_msg.data[i-1];
        }
        memcpy(&valor, dato_reordenado, sizeof(dato_reordenado));  // Conversión de Bytes a float
        return valor;
      }else{
        Serial.print("Nombre de widget no encontrado");
        return valor=-1;
      }
    }
  }
}

void HMI_event_switch(const char* nombre_switch, bool* estado_switch){
  /*Debido a que no se puede obtener el estado actual del switch, ya que 
  solamente se puede saber cuando ocurre un cambio de estado esta función 
  cambia el valor de una variable cada que ocurre el evento de cambio de 
  estado del switch en la interfaz.

  Recibe:
    nombre_switch: Nombre dek widget del switch 
    estado_switch: Variable que está almacenando el estado del switch

  Retorna: 
    Modifica el estado de la variable estado_switch
  */

  if(hmi_msg.len>0){
    char nombre_widget[20];         // Array de caracteres para almacenar el nombre del widget
    int longitud_nombre= hmi_msg.len-1;  // Tamaño del nombre del widget (el último byte es el dato de estado del switch)

    // Si el nombre tiene más de 19 caracteres, solo toma los primeros 19
    if (longitud_nombre > sizeof(nombre_widget) - 1){longitud_nombre = sizeof(nombre_widget) - 1;}
    
    for (int i = 0; i < hmi_msg.len-1; i++) {     // Guarda el nombre del widget
      nombre_widget[i] = (char)hmi_msg.data[i]; 
    }
    nombre_widget[longitud_nombre]='\0';          // El último elemento se pone en \0 para indicar el fin de nombre

    // Comparamos el nombre con el del widget que queremos
    if (strcmp(nombre_widget, nombre_switch) == 0) {
      uint8_t dato = hmi_msg.data[hmi_msg.len - 1]; // Último byte = 0x00 o 0x01
      *estado_switch = (dato == 0x01); // Actualizamos el valor global

      Serial.print("Switch cambiado a: ");
      Serial.println(*estado_switch ? "ON" : "OFF");
    }
  }
}
