#ifndef CONFIG
#define CONFIG

#include <stdint.h>
//apuntadores para registros de configuracion

/* ========= Registros RCC (Reset and Clock Control) ========= */
#define RCCBASE 0x40021000 //REGISTRO BASE DE RCC
/* ========= Registro de control de reloj ========= */
#define RCC_CR      (*(volatile uint32_t*)(RCCBASE + 0x00)) // REGISTRO BASE DE RCC + EL OFFSET DE RCC_CR "0x00"
/* ========= Registro de configuración de reloj ========= */
#define RCC_CFGR    (*(volatile uint32_t*)(RCCBASE + 0x04)) // REGISTRO BASE DE RCC + EL OFFSET DE RCC_CFGR "0x04"
/* ========= Registro de habilitación de reloj periféricos en APB1 ========= */
#define RCC_APB1ENR (*(volatile uint32_t*)(RCCBASE + 0x1C)) // REGISTRO BASE DE RCC + EL OFFSET DE RCC_APB2ENR "0x1C"
/* ========= Registro de habilitación de reloj periféricos en APB2 ========= */
#define RCC_APB2ENR (*(volatile uint32_t*)(RCCBASE + 0x18)) // REGISTRO BASE DE RCC + EL OFFSET DE RCC_APB2ENR "0x18"



/* ========= Registros Base de los GPIOX) ========= */
#define GPIOA_BASE 0x40010800
#define GPIOB_BASE 0x40010C00
#define GPIOC_BASE 0x40011000
#define GPIOD_BASE 0x40011400
#define GPIOE_BASE 0x40011800

/* ========= (OFFSET DE LOS REGISTROS DE GPIO) ========= */
#define GPIOX_BRR 0x14
#define GPIOX_BSRR 0x10
#define GPIOX_CRH 0x04
#define GPIOX_CRL 0x00
#define GPIOX_IDR 0x08
#define GPIOX_ODR 0x0C

/* ========= (Registro base de TIM1) ========= */
#define TIM1BASE 0x40012C00

/* ========= (OFFSET DE EL REGISTRO TIM1) ========= */
#define TIM1_CR1   0x00  /* Control register 1 */
#define TIM1_CR2   0x04  /* Control register 2 */
#define TIM1_SMCR  0x08  /* Slave mode control register */
#define TIM1_DIER  0x0C  /* DMA/Interrupt enable register */
#define TIM1_SR    0x10  /* Status register */
#define TIM1_EGR   0x14  /* Event generation register */
#define TIM1_CCMR1 0x18  /* Capture/compare mode register 1 */
#define TIM1_CCMR2 0x1C  /* Capture/compare mode register 2 */
#define TIM1_CCER  0x20  /* Capture/compare enable register */
#define TIM1_CNT  0x24  /* Counter */
#define TIM1_PSC   0x28  /* Prescaler */
#define TIM1_ARR   0x2C  /* Auto-reload register */
#define TIM1_RCR   0x30  /* Repetition counter register */
#define TIM1_CCR1  0x34  /* Capture/compare register 1 */
#define TIM1_CCR2  0x38  /* Capture/compare register 2 */
#define TIM1_CCR3  0x3C  /* Capture/compare register 3 */
#define TIM1_CCR4  0x40  /* Capture/compare register 4 */
#define TIM1_BDTR  0x44  /* Break and dead-time register */
#define TIM1_DCR   0x48  /* DMA control register */
#define TIM1_DMAR  0x4C  /* DMA address for full transfer */








//prototipos

void Sysclock_Conf_8Mhz (void);





#endif