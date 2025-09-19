#include "max7219.h"
// "config.h" ya está incluido a través de "max7219.h"

/* ------------------------------------------------------------------------- */
/* FUNCIÓN 11: (Privada) Envío de Comando al MAX7219                          */
/* ------------------------------------------------------------------------- */
/**
 * @brief Envía un paquete de 16 bits (comando + dato) al MAX7219.
 * @note  Esta es la función principal que maneja el protocolo SPI y CS.
 */
void MAX7219_SendCommand(uint8_t reg_addr, uint8_t data)
{
    /* 1. Bajar el Chip Select (LOAD/CS) para iniciar la comunicación */
    GPIO_WritePin(MAX7219_CS_PORT_BASE, MAX7219_CS_PIN, GPIO_PIN_LOW);

    /* 2. Enviar el paquete de 16 bits (enviamos la dirección de 8 bits primero) */
    /* Usamos la función SPI que creamos, que envía Y recibe simultáneamente. */
    /* Ignoramos el byte recibido (MISO) durante el envío de comandos. */
    SPI1_SendReceive_Byte(reg_addr);

    /* 3. Enviar el dato de 8 bits */
    SPI1_SendReceive_Byte(data);

    /* 4. Esperar a que SPI termine la transmisión (BSY Flag = 0) */
    SPI1_Wait_BSY();

    /* 5. Subir el Chip Select (LOAD/CS). Esto LATCHES (carga) los datos en el MAX7219. */
    GPIO_WritePin(MAX7219_CS_PORT_BASE, MAX7219_CS_PIN, GPIO_PIN_HIGH);
}


/* ------------------------------------------------------------------------- */
/* FUNCIÓN 12: (Pública) Inicialización del MAX7219                           */
/* ------------------------------------------------------------------------- */
/**
 * @brief Inicializa el controlador MAX7219 y el SPI1.
 */
void MAX7219_Init(void)
{
    /* 1. Inicializar el hardware SPI1 Maestro (definido en config.c) */
    /* Esto ya configura PA5, PA6, PA7 como AF y habilita el SPI1. */
    SPI1_Init_Master();

    /* 2. Inicializar el pin CS (LOAD) manual (definido en max7219.h) */
    /* Lo configuramos como Salida Push-Pull. (El reloj del puerto ya 
       está habilitado por la función GPIO_Config_Output) */
    GPIO_Config_Output(MAX7219_CS_PORT_BASE, MAX7219_CS_PIN, GPIO_OUTPUT_PP_50MHZ);

    /* 3. Asegurarse de que CS esté ALTO (inactivo) al inicio */
    GPIO_WritePin(MAX7219_CS_PORT_BASE, MAX7219_CS_PIN, GPIO_PIN_HIGH);

    /* 4. Configurar los registros internos del MAX7219 a un estado conocido */
    
    /* Modo Test: OFF (0x0F, 0x00) */
    MAX7219_SendCommand(MAX7219_REG_DISPLAY_TEST, MAX7219_TEST_OFF);
    
    /* Límite de Escaneo: Mostrar los 8 dígitos (0x0B, 0x07) */
    MAX7219_SendCommand(MAX7219_REG_SCAN_LIMIT, MAX7219_SCAN_LIMIT_ALL_8);

    /* Modo de Decodificación: Sin decodificación BCD (0x09, 0x00) */
    /* Esto nos permite controlar los 7 segmentos individualmente (acceso RAW) */
    MAX7219_SendCommand(MAX7219_REG_DECODE_MODE, MAX7219_DECODE_ALL);

    /* Intensidad: Nivel medio (ej. 7 sobre 15) (0x0A, 0x07) */
    MAX7219_SendCommand(MAX7219_REG_INTENSITY, 0x07);

    /* Limpiar la pantalla (escribir 0 en todos los dígitos) */
    MAX7219_ClearAll();
    
    /* Finalmente, Encender el chip (salir de modo Shutdown) (0x0C, 0x01) */
    MAX7219_SendCommand(MAX7219_REG_SHUTDOWN, MAX7219_SHUTDOWN_ON);
}


/* ------------------------------------------------------------------------- */
/* FUNCIÓN 13: (Pública) Limpiar Pantalla                                    */
/* ------------------------------------------------------------------------- */
void MAX7219_ClearAll(void)
{
    uint8_t i;
    for (i = 0; i < 8; i++)
    {
        /* Escribimos 0 (todos los segmentos apagados) al Dígito i. */
        /* (El registro del Dígito 0 es 0x01, Dígito 1 es 0x02, etc.) */
        MAX7219_SendCommand(MAX7219_REG_DIGIT_0 + i, 0x00);
    }
}


/* ------------------------------------------------------------------------- */
/* FUNCIÓN 14: (Pública) Escribir en Dígito (RAW)                            */
/* ------------------------------------------------------------------------- */
/**
 * @brief Escribe un patrón de segmento crudo en un dígito (0-7).
 * @param digit El dígito (0-7).
 * @param segments Patrón de bits (DP, A, B, C, D, E, F, G).
 */
void MAX7219_SetDigitRaw(uint8_t digit, uint8_t segments)
{
    if (digit >= 8) return; // Protección simple
    
    /* Los registros de dígitos (0x01 a 0x08) coinciden con (dígito + 1) */
    MAX7219_SendCommand(digit + 1, segments);
}


/* ------------------------------------------------------------------------- */
/* FUNCIÓN 15: (Pública) Establecer Brillo                                   */
/* ------------------------------------------------------------------------- */
void MAX7219_SetIntensity(uint8_t intensity)
{
    if (intensity > 15) intensity = 15; // Asegurar que esté en rango (0x0F)
    
    MAX7219_SendCommand(MAX7219_REG_INTENSITY, intensity);
}