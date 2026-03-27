# 🌿 Sistema de Invernadero Automatizado con LPC845

Este proyecto implementa un sistema de control autónomo para un invernadero utilizando el microcontrolador **NXP LPC845-BRK**. El sistema monitorea variables ambientales críticas y acciona dispositivos de control climático y riego para mantener condiciones óptimas, sin necesidad de conexión a internet.
NOTA: El proyecto se encuentra en la carpeta 'Invernadero'

## 🚀 Características Principales

* **Monitoreo en Tiempo Real:** Lectura continua de temperatura ambiente, humedad del suelo, nivel de luz y nivel del tanque de agua.
* **Control Climático Automático:** Apertura de ventana (mediante servomotor) y encendido de extractor/ventilador cuando se superan los umbrales de temperatura o humedad máxima.
* **Riego Inteligente:** Activación de bomba de agua basada en la humedad del suelo, con bloqueo de seguridad si el tanque de agua está vacío.
* **Iluminación Suplementaria:** Encendido de luces LED cuando la luz natural es insuficiente.
* **Interfaz de Usuario Local (HMI):** Visualización de datos y alertas en una pantalla OLED I2C.
* **Configuración Persistente:** Menú interactivo mediante teclado matricial para ajustar los límites de los sensores. Los datos se guardan en la **Memoria Flash interna (IAP)**, asegurando que no se pierdan ante cortes de energía.

## 🛠️ Hardware y Componentes

* **Placa de Desarrollo:** NXP LPC845-BRK
* **Sensores (Entradas Analógicas):**
    * Termistor NTC 100k (Temperatura)
    * Sensor de Humedad de Suelo (Capacitivo/Resistivo)
    * Fotorresistencia / LDR (Luz)
    * Sensor de Nivel de Agua Analógico
* **Actuadores:**
    * Servomotor SG90 (Control de Ventana)
    * Ventilador / Extractor (5V/12V con driver)
    * Bomba de Riego (Mini bomba sumergible)
    * Tira/Módulo LED (Iluminación)
* **Periféricos de Interfaz:**
    * Pantalla OLED 0.96" (SSD1306) via I2C
    * Teclado Matricial (Membrana o Push-buttons)

## 📌 Mapa de Conexiones (Pinout)


El firmware está desarrollado en **C** utilizando el SDK de NXP. 
* **SCTimer:** Genera la señal PWM de 50Hz requerida para el control preciso del servomotor.
* **ADC:** Configurado en modo sincrónico, leyendo los 4 canales mediante disparos por software (`SoftwareTriggerConvSeqA`).
* **IAP (In-Application Programming):** Se utiliza el **Sector 30 (0x7800)** de la memoria Flash para almacenar una estructura `GreenhouseConfig` que contiene los límites configurados por el usuario y una *Magic Key* para validación de datos.

## 🖥️ Instrucciones de Uso (Interfaz)

1.  **Pantalla Principal:** Muestra los valores actuales de los sensores y los límites de referencia. Indica alertas como "Ventilando..." o "!! SIN AGUA !!".
2.  **Ingreso al Menú:** Presiona la tecla `#` en el teclado numérico.
3.  **Navegación:**
    * Selecciona `1`, `2` o `3` para modificar los límites de Luz, Humedad o Temperatura respectivamente.
    * Usa el teclado numérico para ingresar el nuevo valor.
    * Usa `*` para borrar el último dígito ingresado.
    * Usa `#` para confirmar y guardar el valor.
4.  **Guardado:** Al confirmar, el sistema mostrará "GUARDADO OK!" indicando que los nuevos parámetros se han escrito exitosamente en la memoria Flash.
