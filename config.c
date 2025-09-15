#include "config.h"

//funcionpara habilitar reloj 

void Sysclock_Conf_8Mhz(void)
{
    /* 1. Habilitar HSE (Bit 16: HSEON) */
    RCC_CR |= 0x00010000; 

    /* 2. Esperar a que HSE esté listo (Bit 17: HSERDY) */
    /* Espera activa (polling) hasta que el hardware confirme que el cristal es estable. */
    while (!(RCC_CR & 0x00020000));

    /* 5. Seleccionar HSE como Reloj del Sistema (Código del usuario) */
    RCC_CFGR &= ~0x3;  /* Limpia bits SW[1:0] */
    RCC_CFGR |= 0x1;   /* Escribe '01' (Selecciona HSE) */

    /* 6. Esperar a que el hardware confirme el cambio (Código del usuario) */
    /* Espera hasta que SWS[3:2] lea '01' (Mask 0xC, Valor 0x4) */
    while ((RCC_CFGR & 0xC) != 0x4); 
    
    /* 7. [PASO OPCIONAL] Deshabilitar HSI (reloj interno) para ahorrar energía */
    /* Limpiamos el bit 0 (HSION) */
    RCC_CR &= 0xFFFFFFFE; 
}

