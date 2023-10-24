#include <SPI_slave.hpp>

volatile byte buff[BUFFER_SIZE];
volatile byte idx = 0;
volatile byte data = 0;

// DRE: Data register is empty, transmit new data
void SERCOM1_0_Handler() {
    SERCOM1->SPI.DATA.reg = 0b11111111;
}

// TXC: Transmit complete
void SERCOM1_1_Handler() {
    // Clear transmit complete interrupt
    SERCOM1->SPI.INTFLAG.bit.TXC = 1;
}

// RXC: Receive is Complete
void SERCOM1_2_Handler() {
    data = SERCOM1->SPI.DATA.reg;
    // Serial.println(data);
    if (idx < BUFFER_SIZE) {
        buff[idx++] = data;
    }
}

// SSL: Slave select Low
void SERCOM1_3_Handler() {
    SERCOM1->SPI.INTFLAG.bit.SSL = 1;   // Clear SSL Interrupt Flag
}

// Initialize SPI
void initSPI() {
    MCLK->APBAMASK.bit.SERCOM1_ = 1;    // Enable APB Clock for SERCOM1
    GCLK->PCHCTRL[SERCOM1_GCLK_ID_CORE].reg = GCLK_PCHCTRL_GEN_GCLK0 | GCLK_PCHCTRL_CHEN; // Configure a Generick clock to clock the SPI

    // Map SERCOM PADs to corresponding pin

    // Map D16 to SERCOM1[0]
    PORT->Group[PORTC].PINCFG[22].bit.PMUXEN = 0x1;
    PORT->Group[PORTC].PMUX[22 >> 1].bit.PMUXE = 0x2;

    // Map D17 to SERCOM1[1]
    PORT->Group[PORTC].PINCFG[23].bit.PMUXEN = 0x1;
    PORT->Group[PORTC].PMUX[23 >> 1].bit.PMUXO = 0x2;

    // Map D6 to SERCOM1[2]
    PORT->Group[PORTD].PINCFG[20].reg |= 0x1;
    PORT->Group[PORTD].PMUX[20 >> 1].bit.PMUXE = 0x2;

    // Map D7 to SERCOM1[3]
    PORT->Group[PORTD].PINCFG[21].reg |= 0x1;
    PORT->Group[PORTD].PMUX[21 >> 1].bit.PMUXO = 0x2;
    
    SERCOM1->SPI.CTRLA.bit.ENABLE = 0;    // Disable SPI to access enable-protected registers
    while (SERCOM1->SPI.SYNCBUSY.bit.ENABLE);       // Wait to finish writing CTRLA.ENABLE BIT

    // Reset SPI
    SERCOM1->SPI.CTRLA.bit.SWRST = 1;
    while (SERCOM1->SPI.CTRLA.bit.SWRST || SERCOM1->SPI.SYNCBUSY.bit.SWRST);    // Wait for reset to complete

    // Configure SPI
    SERCOM1->SPI.CTRLA.bit.ENABLE = 0;    // Disable SPI to access enable-protected registers
    while (SERCOM1->SPI.SYNCBUSY.bit.ENABLE);       // Wait to finish writing CTRLA.ENABLE BIT
    SERCOM1->SPI.CTRLA.bit.MODE = 0x2;  // slave mode
    SERCOM1->SPI.CTRLA.bit.CPOL = 0;    //SCK is low when idle, leading edge is rising edge
    SERCOM1->SPI.CTRLA.bit.CPHA = 0;    //data sampled on leading sck edge and changed on a trailing sck edge
    SERCOM1->SPI.CTRLA.bit.FORM = 0x0;  //Frame format = SPI
    SERCOM1->SPI.CTRLA.bit.DIPO = 0x3;  // DATA PAD3 MOSI is used as input
    SERCOM1->SPI.CTRLA.bit.DOPO = 0;    //DATA PAD0 MISO is used as output
    SERCOM1->SPI.CTRLB.bit.CHSIZE = 0;  // 8-bit character size
    SERCOM1->SPI.CTRLA.bit.DORD = 0;    // MSB first
    
    // Enable interrupts
    NVIC_EnableIRQ(SERCOM1_0_IRQn);
    NVIC_SetPriority(SERCOM1_0_IRQn, 2);
    NVIC_EnableIRQ(SERCOM1_1_IRQn);
    NVIC_SetPriority(SERCOM1_1_IRQn, 2);
    NVIC_EnableIRQ(SERCOM1_2_IRQn);
    NVIC_SetPriority(SERCOM1_2_IRQn, 2);
    NVIC_EnableIRQ(SERCOM1_3_IRQn);
    NVIC_SetPriority(SERCOM1_3_IRQn, 2);

    SERCOM1->SPI.INTENSET.bit.ERROR = 0x1;
    SERCOM1->SPI.INTENSET.bit.SSL = 0x1;
    SERCOM1->SPI.INTENSET.bit.RXC = 0x1;
    SERCOM1->SPI.INTENSET.bit.TXC = 0x1;
    SERCOM1->SPI.INTENSET.bit.DRE = 0x1;

    // Do this last

    // Enable SPI
    SERCOM1->SPI.CTRLA.bit.ENABLE = 1;
    while (SERCOM1->SPI.SYNCBUSY.bit.ENABLE);

    // Enable receiving data
    SERCOM1->SPI.CTRLB.bit.RXEN = 1;
    while (SERCOM1->SPI.SYNCBUSY.bit.CTRLB);
}

void clearBuffer() {
    volatile byte* p = buff;
    int n = BUFFER_SIZE;
    while (n-- > 0) *p++ = 0;
}

byte getByte() {
    return data;
}