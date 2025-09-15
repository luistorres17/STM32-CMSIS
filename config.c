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







