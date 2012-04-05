#include "spi.h"

#define SCLK    BIT5
#define SDI     BIT7
#define SDO     BIT6
#define CS      BIT4


void spi_init(void)
{
    P1DIR |= SCLK | SDO | CS;
    P1DIR &= ~SDI;

    // enable SDI, SDO, SCLK, master mode, MSB, output enabled, hold in reset
    USICTL0 = USIPE7 | USIPE6 | USIPE5 | USIMST | USIOE | USISWRST;

    // SMCLK / 128
    USICKCTL = USIDIV_7 + USISSEL_2;

    // clock phase
    USICTL1 |= USICKPH;

    // release from reset
    USICTL0 &= ~USISWRST;
}


void spi_cs_assert(void)
{
    // assert CS
    P1OUT &= ~CS;
}

void spi_cs_deassert(void)
{
    // deassert CS
    P1OUT |= CS;
}

uint8_t spi_write8(uint8_t c)
{
    USISRL = c;
    // clear interrupt flag
    USICTL1 &= ~USIIFG;
    // set number of bits to send, begins tx
    USICNT=8;

    // wait for tx
    while((USICTL1 & USIIFG) != 0x01);

    c = USISRL;

    return c;
}

