//LIBRERIAS
#include <Controllino.h>
#include "Stone_HMI_Define.h" // Librería ofiical de HMI Stone
#include "Procesar_HMI.h"     // Librería implementada para procesar respuestas del HMI

// VARIABLES PARA PWM DEL MOTOR
const int pin_motor        = CONTROLLINO_D0;              // Pin de salida PWM al motor
int8_t    slider_DutyCycle = 0;                           // Valor leído del slider del HMI
char label2_text[10];                                     // Char para mostrar el Duty Cycle en el label2 del HMI

// VARIABLES PARA CONTEO DE PULSOS y RPM
const int entrada                    = CONTROLLINO_IN1;   // Pin de entrada de pulsos
volatile unsigned long conteo_pulsos = 0;                 // Contador de pulsos
char label4_text[10];                                     // Char para mostrar las RPM en el label4 del HMI
float rpm                            = 0;                 // RPM calculadas
const uint16_t PULSOS_POR_REV        = 36;                // Número de pulsos por revolución (Datos del EPC)
const float fs                = 1; 

// VARIABLES PARA CONTROLAR EL TIEMPO DE ENVIO DE DATOS AL HMI
unsigned long t_previo=0;
unsigned long t_previo1=0;

// FUNCIONES ADICIONALES 
void contarPulso();

void setup() {
  Serial.begin(115200);   // Comunicación serial con el PC
  Serial2.begin(115200);  // Comunicación serial con el HMI
  STONE_push_series("line_series", "line_series1", 0); //Envía un valor del eje X a graficar en el line_series1 que se pondrá al final
  STONE_push_series("line_series", "line_series2", 0); //Envía un valor del eje X a graficar en el line_series2 que se pondrá al final
  Stone_HMI_Set_Value("slider", "slider1", NULL, 0);
  
  pinMode(entrada, INPUT);
  pinMode(pin_motor, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(entrada), contarPulso, FALLING);
  noInterrupts();
  TCCR1A = 0b00000000;     // Todo apagado, modo normal registro A. Counter1 del ATMEGA2560
  TCCR1B = 0b00000000;     // Todo apagado, modo normal registro B. Counter1 del ATMEGA2560
  TCCR1B |= B00000100;     // Configuración de preescaler a 256 (BIT CS12)
  TIMSK1 |= B00000010;     // Habilitar interrupción por comparación usando el registro TIMSK1 (modo CTC)
  OCR1A = 62500/fs; // Establecer valor TOP para generar interrupción cada 1 segundo (valor a comparar calculado)
  interrupts();
  HMI_init(); // Inicialización del sistema de colas para procesar las respuestas del HMI
}

void loop() {
  if(millis()-t_previo1>=10){
    slider_DutyCycle= HMI_get_value("slider", "slider1");  // Solicita el valor del widget slider1
    int pwmValue = map(slider_DutyCycle, 0, 100, 0, 255);  // COnversion del duty cycle
    analogWrite(pin_motor, pwmValue);
    t_previo1=millis();
  }
  
  if(millis()-t_previo>=100){
    t_previo=millis();
    dtostrf(slider_DutyCycle, 7, 2, label2_text);     // Convertir float a char
    dtostrf(rpm, 7, 2, label4_text);                  // Convertir float a char
    Stone_HMI_Set_Text("label","label2",label2_text); // Envía el texto del duty cycle al label2
    Stone_HMI_Set_Text("label","label4",label4_text); // Envía el texto de las rpm al label4

    //Envía un valor del eje X a graficar en el line_series1 que se pondrá al final
    STONE_push_series("line_series", "line_series1", slider_DutyCycle);      
    
    //Envía un valor del eje X a graficar en el line_series2 que se pondrá al final
    STONE_push_series("line_series", "line_series2", rpm); 
    
  }
}

// Interrupción por TIMER1 para muestrear las RPM debido a que la librería del HMI se demora mucho
ISR(TIMER1_COMPA_vect){     
  TCNT1=0;          // Resetea el timer (Timer 1 a cero)

  // Calcular RPM (pulsos por segundo / pulsos por revolución) * 60
  rpm = (float(conteo_pulsos)*60)*fs / (PULSOS_POR_REV);

  // Mostrar las RPM
  Serial.print("\nRPM: ");
  Serial.println(rpm);
  Serial.print("Pulsos: ");
  Serial.println(conteo_pulsos);

  conteo_pulsos=0;  // Resetea los pulsos 
}

// Interrupción por Hardware para contar los pulsos del motor
void contarPulso() {
  conteo_pulsos++;  // Incrementar contador al detectar pulso
}
