#include "config.h"

//funcionpara habilitar reloj 

void Sysclock_Conf_8Mhz(void)
{
    /* 1. Habilitar HSE (Bit 16: HSEON) */
    RCC_CR |= 0x00010000; 

    /* 2. Esperar a que HSE esté listo (Bit 17: HSERDY) */
    while (!(RCC_CR & 0x00020000));

    /* 3. [PASO CRÍTICO AÑADIDO] Configurar Latencia de FLASH */
    /* Para SYSCLK <= 24MHz, se requieren 0 Estados de Espera (LATENCY = 000). */
    /* Limpiamos los bits [2:0] del registro FLASH_ACR (Aunque 0 sea el reset, lo aseguramos) */
    /* Leemos el valor, aplicamos AND con máscara 0xFFFFFFF8, y escribimos. */
    FLASH_ACR &= 0xFFFFFFF8; 

    /* 4. (Opcional) Configurar Prescalers (Valores de reset = 0, no se dividen) */
    /* HPRE=0, PPRE1=0, PPRE2=0. El reset de RCC_CFGR es 0x00, así que no es necesario 
       limpiar bits aquí si solo queremos 8MHz en todos los buses. */

    /* 5. Seleccionar HSE como Reloj del Sistema (Su código original) */
    RCC_CFGR &= ~0x3;  /* Limpia bits SW[1:0] */
    RCC_CFGR |= 0x1;   /* Escribe '01' (Selecciona HSE) */

    /* 6. Esperar a que el hardware confirme el cambio (Su código original) */
    /* Espera hasta que SWS[3:2] lea '01' (Mask 0xC, Valor 0x4) */
    while ((RCC_CFGR & 0xC) != 0x4); 
    
    /* 7. [OPCIONAL] Deshabilitar HSI (Su código original) */
    RCC_CR &= 0xFFFFFFFE; /* Limpia el bit 0 (HSION) para ahorrar energía */
}

