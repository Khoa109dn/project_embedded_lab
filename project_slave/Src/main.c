#include "stm32f103xb.h"
#include <stdio.h>

void SystemClock_Config(void) {
    RCC->CR |= RCC_CR_HSEON;
    while (!(RCC->CR & RCC_CR_HSERDY));
    RCC->CFGR |= RCC_CFGR_PLLSRC | RCC_CFGR_PLLMULL9;
    RCC->CR |= RCC_CR_PLLON;
    while (!(RCC->CR & RCC_CR_PLLRDY));
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
}

void Init_NodeB(void) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN | RCC_APB2ENR_AFIOEN | RCC_APB2ENR_USART1EN;
    RCC->APB1ENR |= RCC_APB1ENR_CAN1EN | RCC_APB1ENR_TIM2EN | RCC_APB1ENR_I2C1EN;

   
    GPIOA->CRL |= (0x0B << 0);  
    GPIOA->CRH |= (0x0B << 4);   
    GPIOA->CRH |= (0x04 << 12) | (0x0B << 16); 

   
    USART1->BRR = 0x271;
    USART1->CR1 |= USART_CR1_TE | USART_CR1_UE;

   
    TIM2->PSC = 72 - 1;
    TIM2->ARR = 20000 - 1;
    TIM2->CCMR1 |= (6 << 4);
    TIM2->CCER |= TIM_CCER_CC1E;
    TIM2->CR1 |= TIM_CR1_CEN;

   
    CAN1->MCR |= CAN_MCR_INRQ;
    while (!(CAN1->MSR & CAN_MSR_INAK));
    CAN1->BTR = (12 << 16) | (1 << 20) | (8 - 1);
    CAN1->MCR &= ~CAN_MCR_INRQ;

    CAN1->FMR |= CAN_FMR_FINIT;
    CAN1->sFilterRegister[0].FR1 = 0;
    CAN1->FA1R |= CAN_FA1R_FACT0;
    CAN1->FMR &= ~CAN_FMR_FINIT;
}

void Set_Servo(int angle) {
    TIM2->CCR1 = 500 + (angle * 2000 / 180);
}

void UART_Send(char *str) {
    while(*str) {
        while(!(USART1->SR & USART_SR_TXE));
        USART1->DR = *str++;
    }
}

int main(void) {
    SystemClock_Config();
    Init_NodeB();
    char buf[50];

    while (1) {
        if (CAN1->RF0R & CAN_RF0R_FMP0) {
            uint8_t temp = (uint8_t)(CAN1->sFIFOMailBox[0].RDLR & 0xFF);
            CAN1->RF0R |= CAN_RF0R_RFOM0; 

            int angle = (temp > 35) ? 0 : 90; 
            Set_Servo(angle);

            sprintf(buf, "Temp: %d C | Servo: %d deg\r\n", temp, angle);
            UART_Send(buf);

           
        }
    }
}
