#include "config.h"

//funcionpara habilitar reloj 

void Sysclock_Conf_8Mhz(void)
{
    /* 1. Habilitar HSE (Bit 16: HSEON) */
    RCC_CR |= 0x00010000; 

    /* 2. Esperar a que HSE est� listo (Bit 17: HSERDY) */
    while (!(RCC_CR & 0x00020000));

    /* 3. [PASO CR�TICO A�ADIDO] Configurar Latencia de FLASH */
    /* Para SYSCLK <= 24MHz, se requieren 0 Estados de Espera (LATENCY = 000). */
    /* Limpiamos los bits [2:0] del registro FLASH_ACR (Aunque 0 sea el reset, lo aseguramos) */
    /* Leemos el valor, aplicamos AND con m�scara 0xFFFFFFF8, y escribimos. */
    FLASH_ACR &= 0xFFFFFFF8; 

    /* 4. (Opcional) Configurar Prescalers (Valores de reset = 0, no se dividen) */
    /* HPRE=0, PPRE1=0, PPRE2=0. El reset de RCC_CFGR es 0x00, as� que no es necesario 
       limpiar bits aqu� si solo queremos 8MHz en todos los buses. */

    /* 5. Seleccionar HSE como Reloj del Sistema (Su c�digo original) */
    RCC_CFGR &= ~0x3;  /* Limpia bits SW[1:0] */
    RCC_CFGR |= 0x1;   /* Escribe '01' (Selecciona HSE) */

    /* 6. Esperar a que el hardware confirme el cambio (Su c�digo original) */
    /* Espera hasta que SWS[3:2] lea '01' (Mask 0xC, Valor 0x4) */
    while ((RCC_CFGR & 0xC) != 0x4); 
    
    /* 7. [OPCIONAL] Deshabilitar HSI (Su c�digo original) */
    RCC_CR &= 0xFFFFFFFE; /* Limpia el bit 0 (HSION) para ahorrar energ�a */
}



static void GPIO_Enable_Clock(uint32_t GPIOx_BASE)
{
    /* Habilitamos el reloj de AFIO (siempre requerido para GPIO avanzado) */
    RCC_APB2ENR |= RCC_APB2ENR_AFIOEN;

    /* Identificamos el puerto por su dirección base y habilitamos su reloj específico */
    if (GPIOx_BASE == GPIOA_BASE) {
        RCC_APB2ENR |= RCC_APB2ENR_IOPAEN;
    }
    else if (GPIOx_BASE == GPIOB_BASE) {
        RCC_APB2ENR |= RCC_APB2ENR_IOPBEN;
    }
    else if (GPIOx_BASE == GPIOC_BASE) {
        RCC_APB2ENR |= RCC_APB2ENR_IOPCEN;
    }
    else if (GPIOx_BASE == GPIOD_BASE) {
        RCC_APB2ENR |= RCC_APB2ENR_IOPDEN;
    }
    // (GPIOE no está incluido en sus defines de BASE, pero se añadiría aquí)
}


/* ------------------------------------------------------------------------- */
/* FUNCIÓN 1: CONFIGURACIÓN DE SALIDA (OUTPUT)                               */
/* ------------------------------------------------------------------------- */

/**
 * @brief Activa el reloj del puerto y configura un pin específico como SALIDA.
 * @param GPIOx_BASE  La dirección base del puerto (ej: GPIOC_BASE).
 * @param pin         El número de pin (0 a 15).
 * @param output_mode El modo de salida (ej: GPIO_OUTPUT_PP_50MHZ).
 */
void GPIO_Config_Output(uint32_t GPIOx_BASE, uint8_t pin, uint8_t output_mode)
{
    volatile uint32_t* p_CRL;
    volatile uint32_t* p_CRH;
    uint8_t pos_offset;
    uint32_t mascara_clear;

    /* 1. Activar el Reloj del Periférico (Paso 1 del usuario) */
    GPIO_Enable_Clock(GPIOx_BASE);

    /* 2. Configurar el Pin (CRL o CRH) */
    if (pin < 8) // Usar registro CRL (Pines 0-7)
    {
        p_CRL = (volatile uint32_t*)(GPIOx_BASE + GPIOX_CRL); // Apuntar a CRL
        pos_offset = pin * 4; // Cada pin usa 4 bits
        
        mascara_clear = ~(0xFU << pos_offset); // Máscara para limpiar (ej: 0xFFFFFF0F para Pin 1)
        *p_CRL &= mascara_clear;                
        *p_CRL |= (output_mode << pos_offset);  
    }
    else // Usar registro CRH (Pines 8-15)
    {
        p_CRH = (volatile uint32_t*)(GPIOx_BASE + GPIOX_CRH); // Apuntar a CRH
        pos_offset = (pin - 8) * 4; // Pin 8 es offset 0 en CRH
        
        mascara_clear = ~(0xFU << pos_offset);
        *p_CRH &= mascara_clear;                
        *p_CRH |= (output_mode << pos_offset);  
    }
}


/* ------------------------------------------------------------------------- */
/* FUNCIÓN 2: CONFIGURACIÓN DE ENTRADA (INPUT)                               */
/* ------------------------------------------------------------------------- */

/**
 * @brief Activa el reloj del puerto y configura un pin específico como ENTRADA.
 * @param GPIOx_BASE  La dirección base del puerto (ej: GPIOA_BASE).
 * @param pin         El número de pin (0 a 15).
 * @param input_mode  El modo de entrada (GPIO_INPUT_ANALOG, FLOATING, o PUPD).
 * @param pull_setting Opcional: Usar GPIO_PULL_UP o GPIO_PULL_DOWN. Solo aplica si 
 * el modo es GPIO_INPUT_PUPD. Ignorado para otros modos.
 */
void GPIO_Config_Input(uint32_t GPIOx_BASE, uint8_t pin, uint8_t input_mode, uint8_t pull_setting)
{
    volatile uint32_t* p_CRL;
    volatile uint32_t* p_CRH;
    volatile uint32_t* p_ODR;
    uint8_t pos_offset;
    uint32_t mascara_clear;

    /* 1. Activar el Reloj del Periférico */
    GPIO_Enable_Clock(GPIOx_BASE);
    
    /* 2. Configurar el Modo del Pin (CRL o CRH) */
    if (pin < 8)
    {
        p_CRL = (volatile uint32_t*)(GPIOx_BASE + GPIOX_CRL);
        pos_offset = pin * 4;
        mascara_clear = ~(0xFU << pos_offset);
        
        *p_CRL &= mascara_clear;               
        *p_CRL |= (input_mode << pos_offset); 
    }
    else
    {
        p_CRH = (volatile uint32_t*)(GPIOx_BASE + GPIOX_CRH);
        pos_offset = (pin - 8) * 4;
        mascara_clear = ~(0xFU << pos_offset);
        
        *p_CRH &= mascara_clear;              
        *p_CRH |= (input_mode << pos_offset); 
    }

    /* 3. Manejar Pull-Up / Pull-Down (Solo si se seleccionó ese modo) */
    /* El STM32F1 usa el registro ODR para seleccionar entre Pull-Up y Pull-Down */
    if (input_mode == GPIO_INPUT_PUPD)
    {
        p_ODR = (volatile uint32_t*)(GPIOx_BASE + GPIOX_ODR);
        
        if (pull_setting == GPIO_PULL_UP)
        {
            *p_ODR |= (1 << pin); // Poner ODR a 1 activa Pull-Up
        }
        else
        {
            *p_ODR &= ~(1 << pin); // Poner ODR a 0 activa Pull-Down
        }
    }
}



/* ------------------------------------------------------------------------- */
/* FUNCIÓN 3: DIGITAL WRITE (Equivalente a Arduino)                          */
/* ------------------------------------------------------------------------- */

/**
 * @brief Escribe un estado ALTO o BAJO en un pin de salida.
 * @note  Utiliza los registros atómicos BSRR y BRR (seguro para interrupciones).
 * @param GPIOx_BASE  La dirección base del puerto (ej: GPIOC_BASE).
 * @param pin         El número de pin (0 a 15).
 * @param state       El estado (GPIO_PIN_HIGH o GPIO_PIN_LOW).
 */
void GPIO_WritePin(uint32_t GPIOx_BASE, uint8_t pin, uint8_t state)
{
    if (state == GPIO_PIN_HIGH)
    {
        /* Escribimos en el registro BSRR (Bit Set). 
           Esto PONE el pin 13 en ALTO (atómico) */
        ( *(volatile uint32_t *)(GPIOx_BASE + GPIOX_BSRR) ) = (1 << pin);
    }
    else // state == GPIO_PIN_LOW
    {
        /* Escribimos en el registro BRR (Bit Reset). 
           Esto PONE el pin 13 en BAJO (atómico) */
        ( *(volatile uint32_t *)(GPIOx_BASE + GPIOX_BRR) ) = (1 << pin);
    }
}


/* ------------------------------------------------------------------------- */
/* FUNCIÓN 4: DIGITAL READ (Equivalente a Arduino)                           */
/* ------------------------------------------------------------------------- */

/**
 * @brief Lee el estado lógico de un pin de entrada.
 * @note  La lectura siempre se realiza desde el Input Data Register (IDR).
 * @param GPIOx_BASE  La dirección base del puerto (ej: GPIOA_BASE).
 * @param pin         El número de pin (0 a 15).
 * @return            El estado (GPIO_PIN_HIGH (1) o GPIO_PIN_LOW (0)).
 */
uint8_t GPIO_ReadPin(uint32_t GPIOx_BASE, uint8_t pin)
{
    /* Apuntamos al Input Data Register (IDR) */
    volatile uint32_t* p_IDR = (volatile uint32_t*)(GPIOx_BASE + GPIOX_IDR);
    
    /* Leemos el valor de 32 bits de todo el puerto */
    uint32_t port_value = *p_IDR;

    /* Aislamos el bit específico del pin que nos interesa */
    if ((port_value & (1 << pin)) != 0)
    {
        return GPIO_PIN_HIGH; // El pin está en ALTO
    }
    else
    {
        return GPIO_PIN_LOW;  // El pin está en BAJO
    }
}

/* ------------------------------------------------------------------------- */
/* FUNCIÓN 5: INICIALIZACIÓN DE TIM1 PARA DELAY                              */
/* ------------------------------------------------------------------------- */


/**
 * @brief Configura el TIM1 para que funcione como una base de tiempo de 1 microsegundo (1 MHz).
 * @note  Esto asume que el reloj de APB2 (que alimenta a TIM1) es de 8 MHz.
 */
void DELAY_TIM1_Init(void)
{
    /* Apuntadores a los registros de TIM1 (usando sus defines de config.h) */
    volatile uint32_t* p_TIM1_CR1 = (volatile uint32_t*)(TIM1BASE + TIM1_CR1);
    volatile uint32_t* p_TIM1_PSC = (volatile uint32_t*)(TIM1BASE + TIM1_PSC);
    volatile uint32_t* p_TIM1_ARR = (volatile uint32_t*)(TIM1BASE + TIM1_ARR);
    volatile uint32_t* p_TIM1_EGR = (volatile uint32_t*)(TIM1BASE + TIM1_EGR);

    /* 1. Habilitar el reloj para TIM1 (Bit 11 en APB2ENR) */
    RCC_APB2ENR |= RCC_APB2ENR_TIM1EN;

    /* 2. Configurar el Prescaler (PSC) */
    /* Queremos 1 MHz (tick de 1us). Reloj de entrada = 8 MHz. */
    /* Fórmula: PSC = (Reloj_Entrada / Frec_Deseada) - 1 */
    /* PSC = (8000000 / 1000000) - 1 = 7 */
    *p_TIM1_PSC = 7;

    /* 3. Configurar el Auto-Reload Register (ARR) al máximo */
    /* Lo configuramos como un contador libre de 16 bits. */
    /* 0xFFFF = 65535 microsegundos max. */
    *p_TIM1_ARR = 0xFFFF;

    /* 4. Generar un evento de actualización (UG bit) */
    /* Esto es CRÍTICO: fuerza la carga inmediata del valor del Prescaler (PSC). */
    *p_TIM1_EGR |= TIM_EGR_UG;

    /* 5. Habilitar el contador (CEN bit) */
    /* El contador ahora está corriendo libremente a 1 MHz. */
    *p_TIM1_CR1 |= TIM_CR1_CEN;
}

/* ------------------------------------------------------------------------- */
/* FUNCIÓN 6: DELAY EN MICROSEGUNDOS (US)                                    */
/* ------------------------------------------------------------------------- */
/**
 * @brief Retardo bloqueante (blocking delay) en microsegundos.
 * @param us Microsegundos a esperar (Valor máximo: 65535).
 */
void DELAY_TIM1_US(uint16_t us)
{
    volatile uint32_t* p_TIM1_CNT = (volatile uint32_t*)(TIM1BASE + TIM1_CNT);
    
    /* 1. Reiniciar el contador de hardware a 0 */
    *p_TIM1_CNT = 0;
    
    /* 2. Esperar (poll) activamente hasta que el contador de hardware (que corre a 1MHz)
          alcance el valor de microsegundos solicitado. */
    while (*p_TIM1_CNT < us);
}

/* ------------------------------------------------------------------------- */
/* FUNCIÓN 7: DELAY EN MILISEGUNDOS (MS)                                     */
/* ------------------------------------------------------------------------- */
/**
 * @brief Retardo bloqueante (blocking delay) en milisegundos.
 * @note  Simplemente llama al delay de US 1000 veces.
 * @param ms Milisegundos a esperar (Valor máximo: 65).
 */
void DELAY_TIM1_MS(uint16_t ms)
{
    /* Llamamos a la función de 1000us (1ms) tantas veces como sea necesario */
    /* (Limitamos a 65ms porque 1000 * 65 = 65000, cerca del límite de 65535 del uint16_t) */
    uint16_t i;
    for (i = 0; i < ms; i++)
    {
        DELAY_TIM1_US(1000); // Espera 1000 microsegundos (1 milisegundo)
    }
}

/* ------------------------------------------------------------------------- */
/* FUNCIÓN 8: INICIALIZACIÓN DE SPI1 (MODO MAESTRO)                          */
/* ------------------------------------------------------------------------- */
/**
 * @brief Configura SPI1 como Maestro (Master) con pines GPIO AF.
 * @note  Asume 8MHz de PCLK2 (reloj de APB2).
 * Configura los pines SPI1 por defecto:
 * PA5 = SCK (AF Output Push-Pull)
 * PA7 = MOSI (AF Output Push-Pull)
 * PA6 = MISO (Input Floating)
 * @note  Esta función NO configura el pin NSS/CS. El usuario DEBE configurar 
 * un pin GPIO separado (ej. PA4) como salida (GPIO_Config_Output)
 * y controlarlo manualmente con GPIO_WritePin (HIGH para deseleccionar, LOW para seleccionar).
 */
void SPI1_Init_Master(void)
{
    /* Apuntadores a los registros de SPI1 (usando sus defines de config.h) */
    volatile uint32_t* p_SPI1_CR1 = (volatile uint32_t*)(SPI1_BASE + SPI_CR1_OFFSET);
    
    /* 1. Habilitar Relojes (GPIOA, AFIO, y SPI1) */
    /* (La función GPIO_Enable_Clock ya habilita GPIOA y AFIO) */
    RCC_APB2ENR |= RCC_APB2ENR_SPI1EN; // Habilitar reloj SPI1 (Bit 12)

    /* 2. Configurar Pines GPIO para SPI1 */
    
    /* PA5 (SCK) y PA7 (MOSI): Salida del Maestro. Deben ser Función Alternativa Push-Pull. */
    GPIO_Config_Output(GPIOA_BASE, 5, GPIO_AF_PP_50MHZ); /* PA5 (SCK) */
    GPIO_Config_Output(GPIOA_BASE, 7, GPIO_AF_PP_50MHZ); /* PA7 (MOSI) */

    /* PA6 (MISO): Entrada al Maestro. Debe ser Input Flotante. */
    GPIO_Config_Input(GPIOA_BASE, 6, GPIO_INPUT_FLOATING, 0); /* (El Pull-setting es ignorado) */
    
    /* 3. Configurar Registros SPI1 (SPI_CR1) */
    uint32_t reg_cr1 = 0;
    
    /* Configurar Baud Rate: PCLK2 / 8 = 8MHz / 8 = 1 MHz. (Bits [5:3]) */
    reg_cr1 |= SPI_BR_PRESCALER_8; 
    
    /* Configurar CPOL=0 (Idle Low) y CPHA=0 (Captura en 1er borde) (Modo SPI 0) */
    reg_cr1 |= SPI_CR1_CPOL_LOW;
    reg_cr1 |= SPI_CR1_CPHA_FIRST;
    
    /* Configurar Software Slave Management (SSM) y Set Internal Slave Select (SSI) */
    /* Esto es crucial: le decimos al SPI que ignore el pin NSS de hardware y 
       nos permita controlarlo por software (GPIO). SSI debe ser 1 para evitar un error 
       de "Mode Fault" cuando MSTR=1. */
    reg_cr1 |= SPI_CR1_SSM;
    reg_cr1 |= SPI_CR1_SSI;

    /* Configurar como MAESTRO (MSTR) */
    reg_cr1 |= SPI_CR1_MSTR;

    /* Escribir la configuración en el registro CR1 */
    *p_SPI1_CR1 = reg_cr1;

    /* 4. Habilitar SPI (SPE - Bit 6). Esto DEBE ser el último paso. */
    *p_SPI1_CR1 |= SPI_CR1_SPE;
}


/* ------------------------------------------------------------------------- */
/* FUNCIÓN 9: TRANSFERENCIA SPI (Envío y Recepción Sincrónica)               */
/* ------------------------------------------------------------------------- */
/**
 * @brief Envía un byte por SPI1 y retorna el byte recibido simultáneamente.
 * @param byte_data El byte a transmitir. (Enviar 0xFF si solo se quiere leer).
 * @return El byte recibido del esclavo (leído del Data Register).
 */
uint8_t SPI1_SendReceive_Byte(uint8_t byte_data)
{
    /* Apuntadores a los registros DR y SR */
    volatile uint32_t* p_SPI1_DR = (volatile uint32_t*)(SPI1_BASE + SPI_DR_OFFSET);
    volatile uint32_t* p_SPI1_SR = (volatile uint32_t*)(SPI1_BASE + SPI_SR_OFFSET);

    /* 1. Esperar hasta que el buffer de transmisión (TXE) esté vacío */
    /* (Esperar a que Bit 1 (TXE) del Status Register (SR) sea 1) */
    while ( !(*p_SPI1_SR & SPI_SR_TXE) );
    
    /* 2. Escribir el byte en el Data Register (DR). Esto inicia la transmisión. */
    *p_SPI1_DR = byte_data;

    /* 3. Esperar hasta que el buffer de recepción (RXNE) esté lleno */
    /* (Esperar a que Bit 0 (RXNE) del Status Register (SR) sea 1) */
    while ( !(*p_SPI1_SR & SPI_SR_RXNE) );

    /* 4. Leer (y retornar) el dato recibido del Data Register (DR) */
    /* (La lectura de DR también limpia la bandera RXNE) */
    return (uint8_t)(*p_SPI1_DR);
}


/* ------------------------------------------------------------------------- */
/* FUNCIÓN 10: ESPERAR A QUE SPI TERMINE (Flag Busy)                         */
/* ------------------------------------------------------------------------- */
/**
 * @brief Espera hasta que la línea SPI (y el periférico) esté libre.
 * @note  Usar esto DESPUÉS de la última transferencia y ANTES de subir el pin Chip Select (CS/NSS).
 */
void SPI1_Wait_BSY(void)
{
    volatile uint32_t* p_SPI1_SR = (volatile uint32_t*)(SPI1_BASE + SPI_SR_OFFSET);

    /* Esperar hasta que el Bit 7 (BSY) sea 0 (No Ocupado) */
    while (*p_SPI1_SR & SPI_SR_BSY);
}




