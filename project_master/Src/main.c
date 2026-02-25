#include "stm32f103xb.h"

// Cấu hình Clock 72MHz
void SystemClock_Config(void) {
    RCC->CR |= RCC_CR_HSEON;
    while (!(RCC->CR & RCC_CR_HSERDY));
    RCC->CFGR |= RCC_CFGR_PLLSRC | RCC_CFGR_PLLMULL9;
    RCC->CR |= RCC_CR_PLLON;
    while (!(RCC->CR & RCC_CR_PLLRDY));
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
}

void Init_NodeA(void) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN | RCC_APB2ENR_AFIOEN;
    RCC->APB1ENR |= RCC_APB1ENR_CAN1EN | RCC_APB1ENR_I2C1EN;

    // CAN Pins: PA11 (RX), PA12 (TX)
    GPIOA->CRH &= ~(0xFF << 12);
    GPIOA->CRH |= (0x04 << 12) | (0x0B << 16);

    // I2C1 Pins: PB6 (SCL), PB7 (SDA)
    GPIOB->CRL |= (0xFF << 24); // AF Open-drain

    // Cấu hình CAN 500kbps
    CAN1->MCR |= CAN_MCR_INRQ;
    while (!(CAN1->MSR & CAN_MSR_INAK));
    CAN1->BTR = (12 << 16) | (1 << 20) | (8 - 1);
    CAN1->MCR &= ~CAN_MCR_INRQ;
    while (CAN1->MSR & CAN_MSR_INAK);

    // Cấu hình I2C1 (Cơ bản cho AHT20)
    I2C1->CR2 = 36;
    I2C1->CCR = 180;
    I2C1->CR1 |= I2C_CR1_PE;
}

void CAN_SendTemp(uint8_t temp) {
    CAN1->sTxMailBox[0].TIR = (0x123 << 21); // ID 0x123
    CAN1->sTxMailBox[0].TDTR = 1;           // Độ dài 1 byte
    CAN1->sTxMailBox[0].TDLR = temp;        // Dữ liệu nhiệt độ
    CAN1->sTxMailBox[0].TIR |= CAN_TI0R_TXRQ; // Gửi
}

int main(void) {
    SystemClock_Config();
    Init_NodeA();

    // Giả lập đọc AHT20 (Bạn thay bằng hàm đọc I2C thực tế)
    uint8_t mock_temp = 30;

    while (1) {
        CAN_SendTemp(mock_temp);

        // Giả lập nhiệt độ tăng dần để test
        mock_temp++;
        if(mock_temp > 45) mock_temp = 25;

        for(int i=0; i<1000000; i++); // Delay tạm
    }
}
