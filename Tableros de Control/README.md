# Tableros de Control — Controllino Mega y ESP32 PLC 14

Repositorio de recursos para las prácticas de la asignatura de **Control Digital**. Contiene programas de ejemplo, archivos de interfaz HMI y el código base de cada práctica, organizado por equipo.

---

## Equipos del tablero

El tablero de control integra dos PLCs programables y dos opciones de pantalla HMI:

| Equipo | Descripción |
|---|---|
| **Controllino Mega** | PLC basado en el ATmega2560, compatible con el entorno Arduino. Se programa con el IDE de Arduino usando la librería `Controllino`. |
| **ESP32 PLC 14** | PLC basado en ESP32 con 14 I/O digitales. Incluye manual de usuario, datasheet y drivers CP210x en la carpeta del equipo. |
| **HMI Stone** | Pantalla táctil programada con el software STONE TOOL Box (archivos `.st`). Se comunica con el Controllino por puerto serial. |
| **HMI Coolmay** | Pantalla táctil alternativa. Sus interfaces se diseñan con el software Coolmay (archivos `.vxf`). |

---

## Estructura del repositorio

```
Tableros de Control/
├── Controllino Mega/
│   └── Programas ejemplo/        # Sketches de referencia para el Controllino
├── ESP32 PLC/                    # Manual, datasheet y drivers del ESP32 PLC 14
├── HMI Stone/
│   ├── Ejemplos/                 # Proyectos HMI de ejemplo con su código Arduino
│   ├── Infografía/               # Material de referencia visual
│   └── Instaladores/             # Software para diseñar interfaces Stone
└── Prácticas/
    ├── Practica1/                # E/S digitales
    ├── Practica2/                # Control básico con HMI
    └── Practica3/                # Control de motor DC
```

---

## Prácticas

> **Nota:** Los archivos de código incluidos en cada práctica son el **punto de partida** entregado al inicio de la sesión. Durante la práctica, los estudiantes los modifican y amplían para resolver los retos propuestos.

### Práctica 1 — Entradas y salidas digitales

Introducción al manejo de pines digitales del Controllino Mega.

- `Practica1_Salidas`: enciende los LEDs del tablero de forma secuencial usando `delay()`.
- `Practica1_Botones`: enciende y apaga un LED mediante dos botones físicos (entradas I16 e I17).

### Práctica 2 — Control básico con HMI

Control de la salida PWM de un LED desde una pantalla HMI.

- `Practica2_Stone`: el usuario ajusta el duty cycle (0–100 %) desde un *spin box* en la pantalla Stone. El Controllino recibe el valor por Serial2 y lo convierte a una señal PWM.
- `Practica2_Coolmay`: misma funcionalidad con pantalla Coolmay.

### Práctica 3 — Control de motor DC

Control de velocidad de un motor DC con retroalimentación de RPM.

- `Practica3_Stone`: el usuario ajusta el duty cycle desde un *slider* en la pantalla Stone. El Controllino mide las RPM mediante conteo de pulsos con interrupciones y un timer de hardware (Timer1 del ATmega2560). Los valores de duty cycle y RPM se grafican en tiempo real sobre la pantalla HMI.
- `Practica3_Coolmay`: misma funcionalidad con pantalla Coolmay.

---

## Librería HMI Stone

Las carpetas que involucran la pantalla Stone incluyen los siguientes archivos de soporte:

| Archivo | Función |
|---|---|
| `Stone_HMI_Define.h` | Definiciones y constantes del protocolo Stone |
| `Stone_HMI_Send/Recv.cpp` | Envío y recepción de tramas seriales |
| `Stone_HMI_CmdWidget/General/System.cpp` | Comandos para controlar widgets, sistema y funciones generales |
| `yyjson.cpp / .h` | Biblioteca JSON usada internamente por el protocolo |
| `Procesar_HMI.cpp / .h` | Capa de abstracción implementada para solicitar y leer valores de widgets fácilmente |

---

## Fuentes

- Controllino: https://www.controllino.com/
- Industrial Shields (ESP32 PLC): https://www.industrialshields.com/
- HMI Stone: https://www.stoneitech.com/
- HMI Coolmay: https://en.coolmay.com/  

> **Sobre los archivos de la librería Stone:** La librería Arduino oficial para comunicación con las pantallas Stone no se distribuye públicamente; se obtiene solicitándola directamente al fabricante. Los archivos incluidos en este repositorio (`Stone_HMI_*.cpp/h`, `yyjson`) fueron proporcionados por Stone tras dicha solicitud y se comparten aquí para facilitar el trabajo en el laboratorio. La librería del Controllino, los recursos del ESP32 PLC 14 y el software Coolmay sí están disponibles libremente en los sitios de cada fabricante.
