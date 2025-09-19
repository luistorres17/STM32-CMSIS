#include "max7219.h"
// "config.h" ya est� incluido a trav�s de "max7219.h"

/* ------------------------------------------------------------------------- */
/* FUNCI�N 11: (Privada) Env�o de Comando al MAX7219                          */
/* ------------------------------------------------------------------------- */
/**
 * @brief Env�a un paquete de 16 bits (comando + dato) al MAX7219.
 * @note  Esta es la funci�n principal que maneja el protocolo SPI y CS.
 */
void MAX7219_SendCommand(uint8_t reg_addr, uint8_t data)
{
    /* 1. Bajar el Chip Select (LOAD/CS) para iniciar la comunicaci�n */
    GPIO_WritePin(MAX7219_CS_PORT_BASE, MAX7219_CS_PIN, GPIO_PIN_LOW);

    /* 2. Enviar el paquete de 16 bits (enviamos la direcci�n de 8 bits primero) */
    /* Usamos la funci�n SPI que creamos, que env�a Y recibe simult�neamente. */
    /* Ignoramos el byte recibido (MISO) durante el env�o de comandos. */
    SPI1_SendReceive_Byte(reg_addr);

    /* 3. Enviar el dato de 8 bits */
    SPI1_SendReceive_Byte(data);

    /* 4. Esperar a que SPI termine la transmisi�n (BSY Flag = 0) */
    SPI1_Wait_BSY();

    /* 5. Subir el Chip Select (LOAD/CS). Esto LATCHES (carga) los datos en el MAX7219. */
    GPIO_WritePin(MAX7219_CS_PORT_BASE, MAX7219_CS_PIN, GPIO_PIN_HIGH);
}


/* ------------------------------------------------------------------------- */
/* FUNCI�N 12: (P�blica) Inicializaci�n del MAX7219                           */
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
       est� habilitado por la funci�n GPIO_Config_Output) */
    GPIO_Config_Output(MAX7219_CS_PORT_BASE, MAX7219_CS_PIN, GPIO_OUTPUT_PP_50MHZ);

    /* 3. Asegurarse de que CS est� ALTO (inactivo) al inicio */
    GPIO_WritePin(MAX7219_CS_PORT_BASE, MAX7219_CS_PIN, GPIO_PIN_HIGH);

    /* 4. Configurar los registros internos del MAX7219 a un estado conocido */
    
    /* Modo Test: OFF (0x0F, 0x00) */
    MAX7219_SendCommand(MAX7219_REG_DISPLAY_TEST, MAX7219_TEST_OFF);
    
    /* L�mite de Escaneo: Mostrar los 8 d�gitos (0x0B, 0x07) */
    MAX7219_SendCommand(MAX7219_REG_SCAN_LIMIT, MAX7219_SCAN_LIMIT_ALL_8);

    /* Modo de Decodificaci�n: Sin decodificaci�n BCD (0x09, 0x00) */
    /* Esto nos permite controlar los 7 segmentos individualmente (acceso RAW) */
    MAX7219_SendCommand(MAX7219_REG_DECODE_MODE, MAX7219_DECODE_ALL);

    /* Intensidad: Nivel medio (ej. 7 sobre 15) (0x0A, 0x07) */
    MAX7219_SendCommand(MAX7219_REG_INTENSITY, 0x07);

    /* Limpiar la pantalla (escribir 0 en todos los d�gitos) */
    MAX7219_ClearAll();
    
    /* Finalmente, Encender el chip (salir de modo Shutdown) (0x0C, 0x01) */
    MAX7219_SendCommand(MAX7219_REG_SHUTDOWN, MAX7219_SHUTDOWN_ON);
}


/* ------------------------------------------------------------------------- */
/* FUNCI�N 13: (P�blica) Limpiar Pantalla                                    */
/* ------------------------------------------------------------------------- */
void MAX7219_ClearAll(void)
{
    uint8_t i;
    for (i = 0; i < 8; i++)
    {
        /* Escribimos 0 (todos los segmentos apagados) al D�gito i. */
        /* (El registro del D�gito 0 es 0x01, D�gito 1 es 0x02, etc.) */
        MAX7219_SendCommand(MAX7219_REG_DIGIT_0 + i, 0x00);
    }
}


/* ------------------------------------------------------------------------- */
/* FUNCI�N 14: (P�blica) Escribir en D�gito (RAW)                            */
/* ------------------------------------------------------------------------- */
/**
 * @brief Escribe un patr�n de segmento crudo en un d�gito (0-7).
 * @param digit El d�gito (0-7).
 * @param segments Patr�n de bits (DP, A, B, C, D, E, F, G).
 */
void MAX7219_SetDigitRaw(uint8_t digit, uint8_t segments)
{
    if (digit >= 8) return; // Protecci�n simple
    
    /* Los registros de d�gitos (0x01 a 0x08) coinciden con (d�gito + 1) */
    MAX7219_SendCommand(digit + 1, segments);
}


/* ------------------------------------------------------------------------- */
/* FUNCI�N 15: (P�blica) Establecer Brillo                                   */
/* ------------------------------------------------------------------------- */
void MAX7219_SetIntensity(uint8_t intensity)
{
    if (intensity > 15) intensity = 15; // Asegurar que est� en rango (0x0F)
    
    MAX7219_SendCommand(MAX7219_REG_INTENSITY, intensity);
}


/* ------------------------------------------------------------------------- */
/* FUNCIÓN 16: (Pública) Mostrar Número en el Display                        */
/* ------------------------------------------------------------------------- */
/**
 * @brief Muestra un número de hasta 5 dígitos (0-65535) en la pantalla.
 * @note  Requiere que el MAX7219 esté en modo BCD (configurado en Init).
 */
void MAX7219_DisplayNumber(uint16_t number)
{
    uint8_t i;
    
    // Si el número es 0, lo mostramos en el primer dígito y limpiamos el resto.
    if (number == 0) {
        MAX7219_SendCommand(MAX7219_REG_DIGIT_0, 0);
        for (i = 1; i < 8; i++) {
            MAX7219_SendCommand(MAX7219_REG_DIGIT_0 + i, MAX7219_BCD_BLANK);
        }
        return;
    }

    // Para números > 0, extraemos y mostramos cada dígito.
    for (i = 0; i < 8; i++)
    {
        if (number > 0)
        {
            uint8_t digit_value = number % 10; // Extraer el último dígito (unidades)
            MAX7219_SendCommand(MAX7219_REG_DIGIT_0 + i, digit_value);
            number = number / 10; // Eliminar el último dígito
        }
        else // Si ya no hay más dígitos en el número...
        {
            // ...limpiamos los dígitos restantes en la pantalla.
            MAX7219_SendCommand(MAX7219_REG_DIGIT_0 + i, MAX7219_BCD_BLANK);
        }
    }
}