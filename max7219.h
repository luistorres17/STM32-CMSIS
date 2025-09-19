#ifndef MAX7219_H
#define MAX7219_H

/* ---------------------------------------------------------------------- */
/* 1. INCLUIR SU BIBLIOTECA BASE                                          */
/* Necesitamos acceso a sus funciones GPIO y SPI definidas en config.h     */
/* ---------------------------------------------------------------------- */
#include "config.h" // Asegúrese de que este archivo esté en la ruta del proyecto

/* ---------------------------------------------------------------------- */
/* 2. CONFIGURACIÓN DEL PIN CHIP SELECT (CS/LOAD) (DEFINIDO POR EL USUARIO) */
/* ¡USTED DEBE CONFIGURAR ESTO!                                            */
/* Estos defines le dicen a esta biblioteca qué puerto y pin usar           */
/* para el Chip Select (LOAD) del MAX7219.                                 */
/* Usaremos PA4 como ejemplo (Puerto GPIOA_BASE, Pin 4).                   */
/* ---------------------------------------------------------------------- */

#define MAX7219_CS_PORT_BASE   (GPIOA_BASE)
#define MAX7219_CS_PIN         (4)


/* ---------------------------------------------------------------------- */
/* 3. REGISTROS (OP-CODES) DEL MAX7219 (Según la hoja de datos)             */
/* Estos son los comandos que enviamos por SPI.                           */
/* ---------------------------------------------------------------------- */
#define MAX7219_REG_NO_OP         (0x00) // No Operación (usado para encadenamiento)
#define MAX7219_REG_DIGIT_0       (0x01) // Dirección del Dígito 0
#define MAX7219_REG_DIGIT_1       (0x02)
#define MAX7219_REG_DIGIT_2       (0x03)
#define MAX7219_REG_DIGIT_3       (0x04)
#define MAX7219_REG_DIGIT_4       (0x05)
#define MAX7219_REG_DIGIT_5       (0x06)
#define MAX7219_REG_DIGIT_6       (0x07)
#define MAX7219_REG_DIGIT_7       (0x08) // Dirección del Dígito 7 (el último)
#define MAX7219_REG_DECODE_MODE   (0x09) // Modo de decodificación BCD
#define MAX7219_REG_INTENSITY     (0x0A) // Intensidad (brillo)
#define MAX7219_REG_SCAN_LIMIT    (0x0B) // Límite de escaneo (cuántos dígitos mostrar)
#define MAX7219_REG_SHUTDOWN      (0x0C) // Apagado / Encendido
#define MAX7219_REG_DISPLAY_TEST  (0x0F) // Modo de prueba (enciende todos los LEDs)

/* Datos para los registros */
#define MAX7219_SHUTDOWN_OFF      (0x00) // Apagado (modo shutdown)
#define MAX7219_SHUTDOWN_ON       (0x01) // Encendido (modo normal)
#define MAX7219_DECODE_NONE       (0x00) // Sin decodificación BCD (acceso directo a 7 seg)
#define MAX7219_DECODE_ALL        (0xFF) // Decodificación BCD para todos los 8 dígitos
#define MAX7219_SCAN_LIMIT_ALL_8  (0x07) // Mostrar los 8 dígitos (0 a 7)
#define MAX7219_TEST_OFF          (0x00)
#define MAX7219_TEST_ON           (0x01)


/* ---------------------------------------------------------------------- */
/* 4. PROTOTIPOS DE FUNCIÓN PÚBLICA                                       */
/* ---------------------------------------------------------------------- */

/**
 * @brief Inicializa el hardware SPI1 y configura el MAX7219 a un estado operativo.
 */
void MAX7219_Init(void);

/**
 * @brief Envía un byte de datos a un registro (dirección) específico del MAX7219.
 * @param reg_addr La dirección del registro (ej: MAX7219_REG_DIGIT_0).
 * @param data El dato a escribir en ese registro.
 */
void MAX7219_SendCommand(uint8_t reg_addr, uint8_t data);

/**
 * @brief Establece el brillo (intensidad) de la pantalla.
 * @param intensity Valor de 0 (mínimo) a 15 (máximo).
 */
void MAX7219_SetIntensity(uint8_t intensity);

/**
 * @brief Escribe un valor de segmento directo en un dígito específico.
 * @note  Requiere que el modo DECODE esté desactivado (Decode_None).
 * @param digit El dígito a escribir (0 a 7).
 * @param segments Patrón de bits para los 7 segmentos (Formato: DP-A-B-C-D-E-F-G).
 */
void MAX7219_SetDigitRaw(uint8_t digit, uint8_t segments);

/**
 * @brief Limpia los 8 dígitos de la pantalla (escribe 0 en segmentos).
 */
void MAX7219_ClearAll(void);


#endif /* MAX7219_H */