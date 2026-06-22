"""
main_estudiante.py
==================
Archivo principal de la práctica: Control ON-OFF con histéresis.

INSTRUCCIONES
-------------
1. Asegúrate de que el simulador Tankatwin esté corriendo en Modo Standalone.
2. Ejecuta este archivo:  python main_estudiante.py
3. La Sección 1 ya está completa — muestra cómo usar la librería.
4. Completa la Sección 2 con el controlador ON-OFF.

Dependencias
------------
    pip install opcua
    Archivo tanque_lib.py en la misma carpeta.
"""

# =============================================================================
# Carga de librerías
# =============================================================================
import time
from tanque_lib import (
    conectar, desconectar,
    leer_niveles,
    encender, apagar, apagar_todo,
)

# =============================================================================
# Parámetros (modificar si cree necesario)
# =============================================================================
TS = 1.0        # Tiempo de muestreo en segundos

# =============================================================================
# Código main
# =============================================================================

# 1. Conectar al servidor OPC UA
cliente, nodos = conectar()

try:

    # -------------------------------------------------------------------------
    # SECCIÓN 1 — Prueba de sensores y actuadores
    # -------------------------------------------------------------------------

    # Leer y mostrar los niveles actuales de ambos tanques [cm]
    h1, h2 = leer_niveles(nodos)
    print(f"Nivel Tanque 1 : {h1:.2f} cm")
    print(f"Nivel Tanque 2 : {h2:.2f} cm\n")

    # Encender y apagar cada actuador para verificar que funcionan
    print("Encendiendo BOMBA por 3 segundos...")
    encender(nodos, "CtrlBomba")
    time.sleep(3)
    apagar(nodos, "CtrlBomba")
    print("Bomba apagada.\n")

    print("Abriendo VÁLVULA SOLENOIDE por 3 segundos...")
    encender(nodos, "CtrlSValve")
    time.sleep(3)
    apagar(nodos, "CtrlSValve")
    print("Válvula solenoide cerrada.\n")

    print("Abriendo VÁLVULA DE BOLA por 3 segundos...")
    encender(nodos, "CtrlBValve")
    time.sleep(3)
    apagar(nodos, "CtrlBValve")
    print("Válvula de bola cerrada.\n")

    # -------------------------------------------------------------------------
    # SECCIÓN 2 — Controlador ON-OFF (Colocar en este bloque)
    # -------------------------------------------------------------------------
    print("Iniciando bucle de control. Presiona Ctrl+C para detener.\n")

    # Estado inicial de cada actuador
    estado_bomba  = 0
    estado_svalve = 0
    estado_bvalve = 0

    while True:

        # 2.1 Leer los niveles actuales
        h1, h2 = leer_niveles(nodos)

        if h1 is None or h2 is None:
            print("Error de lectura, reintentando...")
            time.sleep(TS)
            continue

        # 2.2 Mostrar el estado actual en consola
        print(f"H1={h1:.2f} cm  H2={h2:.2f} cm  "
              f"| Bomba={'ON ' if estado_bomba  else 'OFF'}"
              f"  SValve={'AB' if estado_svalve else 'CE'}"
              f"  BValve={'AB' if estado_bvalve else 'CE'}")

        # IMPLEMENTAR CONTROLADOR ON OFF CON HISTÉRESIS 
        # ─────────────────────────────────────────────────────────────────────

        # 2.3 Esperar el siguiente ciclo de muestreo
        time.sleep(TS)

except KeyboardInterrupt:
    print("\nControlador detenido por el usuario.")

finally:
    # Siempre apagar todo antes de salir (se ejecuta aunque haya error)
    apagar_todo(nodos)
    desconectar(cliente)
