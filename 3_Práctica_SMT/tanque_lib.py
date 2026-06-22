"""
tanque_lib.py
=============
Librería de comunicación OPC UA para el Sistema Multitanque
(Gemelo Digital - Tankatwin Trial).

Funciones disponibles
---------------------
    conectar()              → conecta al servidor OPC UA
    desconectar(cliente)    → cierra la conexión
    leer_nivel(nodos, tanque) → lee el nivel de un tanque [cm]
    leer_niveles(nodos)     → lee H1 y H2 en una sola llamada [cm]
    encender(nodos, actuador) → activa un actuador (bomba o válvula)
    apagar(nodos, actuador)   → desactiva un actuador
    apagar_todo(nodos)      → desactiva todos los actuadores (seguridad)

Servidor OPC UA : opc.tcp://localhost:44840
Variables leídas : NivelTanque1, NivelTanque2  [cm, Double]
Variables escritas: CtrlBomba, CtrlSValve, CtrlBValve  [0.0 / 1.0, Double]

Dependencias
------------
    pip install opcua
"""

# =============================================================================
# Carga de librerías
# =============================================================================
from opcua import Client, ua
import sys


# =============================================================================
# Constantes internas (no modificar)
# =============================================================================
_URL_DEFAULT = "opc.tcp://localhost:44840"

_NODOS_ID = {
    "NivelTanque1": "ns=1;s=NivelTanque1",
    "NivelTanque2": "ns=1;s=NivelTanque2",
    "CtrlBomba"   : "ns=1;s=CtrlBomba",
    "CtrlSValve"  : "ns=1;s=CtrlSValve",
    "CtrlBValve"  : "ns=1;s=CtrlBValve",
}


# =============================================================================
# Definición de funciones
# =============================================================================

def conectar(url: str = _URL_DEFAULT) -> tuple:
    """
    Conecta al servidor OPC UA del simulador y retorna el cliente y los nodos.

    Argumentos
    ----------
    url : str  (opcional)
        Dirección del servidor. Por defecto: "opc.tcp://localhost:44840".

    Retorna
    -------
    tuple : (Client, dict)
        - cliente : objeto de conexión OPC UA.
        - nodos   : diccionario con los 5 nodos del sistema.

    Ejemplo
    -------
        cliente, nodos = conectar()
    """
    cliente = Client(url)
    try:
        print(f"Conectando a {url} ...")
        cliente.connect()
        print("¡Conexión exitosa!\n")
    except Exception as e:
        print(f"[ERROR] No se pudo conectar: {e}")
        print("Verifique que el simulador Tankatwin esté en ejecución.")
        sys.exit(1)

    nodos = {}
    for nombre, node_id in _NODOS_ID.items():
        try:
            nodos[nombre] = cliente.get_node(node_id)
        except Exception as e:
            print(f"[ERROR] No se pudo obtener el nodo '{nombre}': {e}")
            cliente.disconnect()
            sys.exit(1)

    return cliente, nodos


def desconectar(cliente: Client) -> None:
    """
    Cierra la conexión OPC UA de forma segura.

    Argumentos
    ----------
    cliente : Client
        Objeto de conexión retornado por conectar().

    Retorna
    -------
    None

    Ejemplo
    -------
        desconectar(cliente)
    """
    try:
        cliente.disconnect()
        print("Desconectado del servidor OPC UA.")
    except Exception as e:
        print(f"[WARN] Error al desconectar: {e}")


def leer_nivel(nodos: dict, tanque: str) -> float | None:
    """
    Lee el nivel de un tanque específico.

    Argumentos
    ----------
    nodos  : dict   — retornado por conectar().
    tanque : str    — "NivelTanque1"  o  "NivelTanque2".

    Retorna
    -------
    float | None
        Nivel en centímetros [cm], o None si hay error de comunicación.

    Ejemplo
    -------
        h1 = leer_nivel(nodos, "NivelTanque1")
        print(f"Tanque 1: {h1:.2f} cm")
    """
    try:
        return float(nodos[tanque].get_value())
    except Exception as e:
        print(f"[ERROR] leer_nivel({tanque}): {e}")
        return None


def leer_niveles(nodos: dict) -> tuple:
    """
    Lee el nivel de ambos tanques en una sola llamada.

    Argumentos
    ----------
    nodos : dict  — retornado por conectar().

    Retorna
    -------
    tuple : (float | None, float | None)
        (h1, h2) en centímetros [cm].

    Ejemplo
    -------
        h1, h2 = leer_niveles(nodos)
        print(f"H1 = {h1:.2f} cm  |  H2 = {h2:.2f} cm")
    """
    h1 = leer_nivel(nodos, "NivelTanque1")
    h2 = leer_nivel(nodos, "NivelTanque2")
    return h1, h2


def encender(nodos: dict, actuador: str) -> None:
    """
    Activa (enciende / abre) un actuador.

    Actuadores disponibles
    ----------------------
        "CtrlBomba"   → enciende la bomba
        "CtrlSValve"  → abre la válvula solenoide (T1 → T2)
        "CtrlBValve"  → abre la válvula de bola (descarga T2)

    Argumentos
    ----------
    nodos    : dict  — retornado por conectar().
    actuador : str   — nombre del actuador (ver lista arriba).

    Retorna
    -------
    None

    Ejemplo
    -------
        encender(nodos, "CtrlBomba")    # enciende la bomba
        encender(nodos, "CtrlSValve")   # abre la válvula solenoide
    """
    _escribir(nodos, actuador, 1)


def apagar(nodos: dict, actuador: str) -> None:
    """
    Desactiva (apaga / cierra) un actuador.

    Actuadores disponibles
    ----------------------
        "CtrlBomba"   → apaga la bomba
        "CtrlSValve"  → cierra la válvula solenoide
        "CtrlBValve"  → cierra la válvula de bola

    Argumentos
    ----------
    nodos    : dict  — retornado por conectar().
    actuador : str   — nombre del actuador.

    Retorna
    -------
    None

    Ejemplo
    -------
        apagar(nodos, "CtrlBomba")    # apaga la bomba
        apagar(nodos, "CtrlSValve")   # cierra la válvula solenoide
    """
    _escribir(nodos, actuador, 0)


def apagar_todo(nodos: dict) -> None:
    """
    Apaga todos los actuadores del sistema.
    Llama siempre a esta función antes de terminar el programa.

    Argumentos
    ----------
    nodos : dict  — retornado por conectar().

    Retorna
    -------
    None

    Ejemplo
    -------
        apagar_todo(nodos)
    """
    for actuador in ("CtrlBomba", "CtrlSValve", "CtrlBValve"):
        _escribir(nodos, actuador, 0)
    print("Todos los actuadores apagados.")


# =============================================================================
# Función interna (no llamar directamente)
# =============================================================================
def _escribir(nodos: dict, actuador: str, valor: int) -> None:
    """Escribe 0.0 o 1.0 en un nodo de control OPC UA (tipo Double)."""
    if actuador not in nodos:
        print(f"[ERROR] Actuador desconocido: '{actuador}'")
        return
    try:
        nodos[actuador].set_value(ua.Variant(float(valor), ua.VariantType.Double))
    except Exception as e:
        print(f"[ERROR] No se pudo escribir en '{actuador}': {e}")
