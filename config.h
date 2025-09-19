#ifndef CONFIG_H
#define CONFIG_H
#include <stdint.h>
//apuntadores para registros de configuracion


/* ========= Registros FLASH Interface ========= */
#define FLASHBASE 0x40022000
#define FLASH_ACR     (*(volatile uint32_t*)(FLASHBASE + 0x00))

/* ========= Registros RCC (Reset and Clock Control) ========= */
#define RCCBASE 0x40021000 //REGISTRO BASE DE RCC
/* ========= Registro de control de reloj ========= */
#define RCC_CR      (*(volatile uint32_t*)(RCCBASE + 0x00)) // REGISTRO BASE DE RCC + EL OFFSET DE RCC_CR "0x00"
/* ========= Registro de configuraci�n de reloj ========= */
#define RCC_CFGR    (*(volatile uint32_t*)(RCCBASE + 0x04)) // REGISTRO BASE DE RCC + EL OFFSET DE RCC_CFGR "0x04"
/* ========= Registro de habilitaci�n de reloj perif�ricos en APB1 ========= */
#define RCC_APB1ENR (*(volatile uint32_t*)(RCCBASE + 0x1C)) // REGISTRO BASE DE RCC + EL OFFSET DE RCC_APB2ENR "0x1C"
/* ========= Registro de habilitaci�n de reloj perif�ricos en APB2 ========= */
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



#define GPIO_OUTPUT_PP_10MHZ    0x1  /* MODE=01, CNF=00 */
#define GPIO_OUTPUT_PP_2MHZ     0x2  /* MODE=10, CNF=00 */
#define GPIO_OUTPUT_PP_50MHZ    0x3  /* MODE=11, CNF=00 */

#define GPIO_OUTPUT_OD_10MHZ    0x5 /* MODE=01, CNF=01 */
#define GPIO_OUTPUT_OD_2MHZ     0x6  /* MODE=10, CNF=01 */
#define GPIO_OUTPUT_OD_50MHZ    0x7  /* MODE=11, CNF=01 */

/* --- MODOS DE FUNCION ALTERNATIVA (AF OUTPUT) --- */
/* CNF=10 (AF Push-Pull), CNF=11 (AF Open-Drain) */
#define GPIO_AF_PP_10MHZ        0x9  /* MODE=01, CNF=10 */
#define GPIO_AF_PP_2MHZ         0xA  /* MODE=10, CNF=10 */
#define GPIO_AF_PP_50MHZ        0xB  /* MODE=11, CNF=10 */

#define GPIO_AF_OD_10MHZ        0xD  /* MODE=01, CNF=11 */
#define GPIO_AF_OD_2MHZ         0xE  /* MODE=10, CNF=11 */
#define GPIO_AF_OD_50MHZ        0xF  /* MODE=11, CNF=11 */


/* --- MODOS DE ENTRADA (INPUT) --- */
/* (MODE = 00) */
#define GPIO_INPUT_ANALOG       0x0  /* CNF=00 */
#define GPIO_INPUT_FLOATING     0x4  /* CNF=01 (Valor de Reset) */
#define GPIO_INPUT_PUPD         0x8  /* CNF=10 (Input con Pull-up/Pull-down) */


/* --- Definiciones de estado para Pull-up/Pull-down (para registro ODR) --- */
#define GPIO_PULL_DOWN 0
#define GPIO_PULL_UP   1


/* ========= ESTADOS DE PIN (Niveles Lógicos) ========= */
#define GPIO_PIN_LOW    0U
#define GPIO_PIN_HIGH   1U


/* --- Bits de habilitación de Reloj GPIO (para usar con RCC_APB2ENR) --- */

#define RCC_APB2ENR_AFIOEN 0x01U
#define RCC_APB2ENR_IOPAEN 0x04U
#define RCC_APB2ENR_IOPBEN 0x08U
#define RCC_APB2ENR_IOPCEN 0x10U
#define RCC_APB2ENR_IOPDEN 0x20U
#define RCC_APB2ENR_IOPEEN 0x40U




/* ========= (OFFSET DE EL REGISTRO TIM1) ========= */
#define TIM1_CR1   0x00  /* Control register 1 */
#define TIM1_SR    0x10  /* Status register */
#define TIM1_EGR   0x14  /* Event generation register */
#define TIM1_CNT   0x24  /* Counter */
#define TIM1_PSC   0x28  /* Prescaler */
#define TIM1_ARR   0x2C  /* Auto-reload register */
/* (Sus otros offsets de TIM1...) */

/* --- Bits de habilitación de Reloj (Hex) --- */
#define RCC_APB2ENR_AFIOEN 0x01U
/* (Sus otros defines de GPIO...) */
/* NECESITAMOS AÑADIR EL RELOJ DE TIM1 (Bit 11 en APB2ENR) */
#define RCC_APB2ENR_TIM1EN 0x800U  /* (1 << 11) */


/* --- Bits de control de TIM1 --- */
#define TIM_CR1_CEN (0x01U)  /* Bit 0: Counter Enable */
#define TIM_EGR_UG  (0x01U)  /* Bit 0: Update Generation */
#define TIM_SR_UIF  (0x01U)  /* Bit 0: Update Interrupt Flag */


/* ========= (OFFSET DE EL REGISTRO TIM1) ========= */
// (Sus defines de TIM1 terminan aquí)

/* * ============================================================================
 * 5. DEFINICIONES DE SPI1 (Serial Peripheral Interface 1)
 * ============================================================================
 */

/* SPI1 está en el bus APB2 */
#define SPI1_BASE       (0x40013000U)

/* ========= (OFFSETS DE LOS REGISTROS DE SPI1) ========= */
#define SPI_CR1_OFFSET   0x00U /* SPI Control register 1 */
#define SPI_CR2_OFFSET   0x04U /* SPI Control register 2 */
#define SPI_SR_OFFSET    0x08U /* SPI Status register */
#define SPI_DR_OFFSET    0x0CU /* SPI Data register */
/* (Se omiten los registros CRC para una implementación básica) */

/* --- Bit de habilitación de Reloj SPI1 (para usar con RCC_APB2ENR) --- */
#define RCC_APB2ENR_SPI1EN (0x1000U)  /* (1 << 12) */

/* --- Bits de Configuración (para SPI_CR1) --- */
#define SPI_CR1_SPE      (0x40U)   /* Bit 6: SPI Enable */
#define SPI_CR1_MSTR     (0x04U)   /* Bit 2: Master Selection (1=Master) */
#define SPI_CR1_CPOL_LOW  (0x00U)  /* Bit 1: CPOL = 0 (Idle Low) */
#define SPI_CR1_CPOL_HIGH (0x02U)  /* Bit 1: CPOL = 1 (Idle High) */
#define SPI_CR1_CPHA_FIRST (0x00U) /* Bit 0: CPHA = 0 (Captura en el 1er borde) */
#define SPI_CR1_CPHA_SECOND (0x01U) /* Bit 0: CPHA = 1 (Captura en el 2do borde) */

/* Prescaler de Baud Rate (Relativo a PCLK2 = 8MHz) */
/* (Bits 5:3 - BR[2:0]) */
#define SPI_BR_PRESCALER_2   (0x00U) /* 8MHz / 2 = 4 MHz */
#define SPI_BR_PRESCALER_4   (0x08U) /* 8MHz / 4 = 2 MHz */
#define SPI_BR_PRESCALER_8   (0x10U) /* 8MHz / 8 = 1 MHz */
#define SPI_BR_PRESCALER_16  (0x18U) /* 8MHz / 16 = 500 KHz */
#define SPI_BR_PRESCALER_32  (0x20U)
#define SPI_BR_PRESCALER_64  (0x28U)
#define SPI_BR_PRESCALER_128 (0x30U)
#define SPI_BR_PRESCALER_256 (0x38U)

/* --- Software Slave Management (Modo Maestro más simple) --- */
#define SPI_CR1_SSM      (0x200U)  /* Bit 9: Software Slave Management Enable */
#define SPI_CR1_SSI      (0x100U)  /* Bit 8: Internal Slave Select (Debe ser 1 en Master con SSM=1) */

/* --- Banderas de Estado (para SPI_SR) --- */
#define SPI_SR_RXNE      (0x01U)   /* Bit 0: Receive buffer Not Empty flag */
#define SPI_SR_TXE       (0x02U)   /* Bit 1: Transmit buffer Empty flag */
#define SPI_SR_BSY       (0x80U)   /* Bit 7: Busy flag */


/* ========= PROTOTIPOS DE FUNCIONES ========= */

void Sysclock_Conf_8Mhz (void);
/* (Prototipos de GPIO...) */
void GPIO_Config_Output(uint32_t GPIOx_BASE, uint8_t pin, uint8_t output_mode);
void GPIO_Config_Input(uint32_t GPIOx_BASE, uint8_t pin, uint8_t input_mode, uint8_t pull_setting);
void GPIO_WritePin(uint32_t GPIOx_BASE, uint8_t pin, uint8_t state);
uint8_t GPIO_ReadPin(uint32_t GPIOx_BASE, uint8_t pin);


void TIM1_Counter_Init(void);
/* ... después de los otros defines de registros ... */
#define MAX7219_BCD_BLANK         (0x0F) // En modo BCD, 0x0F apaga el dígito

/* ... después de los otros prototipos ... */
/**
 * @brief Toma un número de 16 bits y lo muestra en los 8 dígitos.
 * @param number El número a mostrar (0 a 65535).
 */
void MAX7219_DisplayNumber(uint16_t number);

/* (Prototipos de Delay...) */
void DELAY_TIM1_Init(void);
void DELAY_TIM1_US(uint16_t us);
void DELAY_TIM1_MS(uint16_t ms);

/* --- NUEVOS PROTOTIPOS (SPI1) --- */
void SPI1_Init_Master(void);
uint8_t SPI1_SendReceive_Byte(uint8_t byte_data);
void SPI1_Wait_BSY(void);


/* ... (después de sus otros defines de TIM1) ... */
/* ========= (OFFSET DE EL REGISTRO TIM1 - ADICIONALES) ========= */
#define TIM1_SMCR  0x08  /* Slave mode control register */
#define TIM1_CCMR1 0x18  /* Capture/compare mode register 1 */
#define TIM1_CCER  0x20  /* Capture/compare enable register */


/* ========= PROTOTIPOS DE FUNCIONES ========= */
/* (Sus prototipos existentes...) */

/* --- NUEVO PROTOTIPO (Contador de Botón TIM1) --- */
void TIM1_ButtonCounter_Init(void);

#endif