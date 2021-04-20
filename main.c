/*
 * main.c - Sample application to switch to STA mode, connect and ping AP
 *
 * Copyright (C) 2014 Texas Instruments Incorporated - http://www.ti.com/
 *
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/*
 * Application Name     -   Getting started with Wi-Fi STATION mode
 * Application Overview -   This is a sample application demonstrating how to
 *                          start CC3100 in WLAN-Station mode and connect to a
 *                          Wi-Fi access-point. The application connects to an
 *                          access-point and ping's the gateway. It also checks
 *                          for an internet connectivity by pinging "www.ti.com"
 * Application Details  -   http://processors.wiki.ti.com/index.php/CC31xx_Getting_Started_with_WLAN_Station
 *                          doc\examples\getting_started_with_wlan_station.pdf
 */

/**/
#include "simplelink.h"
#include "sl_common.h"
#include "ADCSWTrigger.h"
#include "PLL.h"
#include "tm4c1294ncpdt.h"
#include "driverlib/sysctl.h"
#include <stdbool.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SysTick.h"
#include "PWM.h"


#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "inc/hw_memmap.h"




/*
#include "ds18b20.h"
#include "types.h"

#define UART_TXD   0x02             // TXD on P1.1 (Timer0_A.OUT0)
#define UART_RXD   0x04             // RXD on P1.2 (Timer0_A.CCI1A)
#define UART_TBIT_DIV_2 (16000000 / (9600 * 2))
#define UART_TBIT (16000000 / 9600)

#define delayMicroseconds(n) __delay_cycles(n<<4)

// The One Wire bus is connected to this pin
#define WIRE_PORT 7 //Remember you need a 4.7k pullup resistor here.

#define LOW 0
#define HIGH 1
#define INPUT 0
#define OUTPUT 1
#define FALSE 0
#define TRUE 1


//Zeke's mod's
int  OW_Reset( int Pin );
void OW_WriteByte(int Pin, char d);
char OW_ReadByte(int Pin);

// Routines written by Peter H Anderson
// http://www.phanderson.com/arduino/ds18b20_1.html
char digitalRead( char Pin );
void pinMode( int Pin, int Out );
void digitalWrite( int Pin, int Bit );

// Software UART routines
void TimerA_UART_init( void );
UINT8 UART_putHexByte( UINT8 data);
UINT8 UART_PutDec32( UINT32 data);
void TimerA_UART_tx( UINT8 byte );

unsigned int  txData;    // UART internal variable for TX
unsigned char rxBuffer;  // Received UART character

*/

/*
void main(void)
{









//  int temp = 0;
 int i;
 int ROMCode[8];
 int ScratchPad[9];
 int HighByte, LowByte, TReading, SignBit, Tc_100, Whole, Fract;

  WDTCTL = WDTPW + WDTHOLD;               // Stop watchdog timer
  BCSCTL1 = CALBC1_16MHZ;
  DCOCTL  = CALDCO_16MHZ;

  P1OUT = UART_RXD;                       // RXD as IN
  P1SEL = UART_TXD + UART_RXD;            // Timer function for TXD/RXD pins
  P1DIR = UART_TXD;

  TimerA_UART_init();
  __enable_interrupt();


  while ( 1 )
  {
     if ( 1 == OW_Reset( WIRE_PORT ) )
     {
       OW_WriteByte( WIRE_PORT, 0x33 );  // Read ROM Code from DS18B20

       for( i=0; i<8; i++ )
       {
         // Read the values inside of the ROMCode array in the debugger window
         ROMCode[i] = OW_ReadByte( WIRE_PORT );
       }
     }

     if ( 1 == OW_Reset( WIRE_PORT ) )
     {
       OW_WriteByte( WIRE_PORT, SKIPROM );  // Skip ROM Code matching
       OW_WriteByte( WIRE_PORT, CONVERTTEMP );  // Read ROM Code from DS18B20
       OW_Reset( WIRE_PORT );
       OW_WriteByte( WIRE_PORT, SKIPROM );  // Skip ROM Code matching
       OW_WriteByte( WIRE_PORT, READSCRATCHPAD );  // Read ScratchPad from DS18B20

       for( i=0; i<9; i++ )
       {
         // Read the values inside of the ScratchPad array in the debugger window
         ScratchPad[i] = OW_ReadByte( WIRE_PORT );
       }
     }


     OW_Reset( WIRE_PORT );
     OW_WriteByte( WIRE_PORT, SKIPROM );         // Skip ROM Code matching
     OW_WriteByte( WIRE_PORT, CONVERTTEMP );     // Read ROM Code from DS18B20
     OW_Reset( WIRE_PORT );
     OW_WriteByte( WIRE_PORT, SKIPROM );         // Skip ROM Code matching
     OW_WriteByte( WIRE_PORT, READSCRATCHPAD );  // Read ScratchPad from DS18B20

     LowByte  = OW_ReadByte( WIRE_PORT );
     HighByte = OW_ReadByte( WIRE_PORT );

     // Thanks again "Mr. Anderson" for this code
     TReading = ( HighByte << 8 ) + LowByte;
     SignBit = TReading & 0x8000;  // test most sig bit
     if ( 1 == SignBit ) // negative
     {
       TReading = (TReading ^ 0xffff) + 1; // 2's comp
     }
     Tc_100 = ( 6 * TReading ) + TReading / 4;    // multiply by (100 * 0.0625) or 6.25

     Whole = Tc_100 / 100;  // separate off the whole and fractional portions
     Fract = Tc_100 % 100;

     //this will print the tempature in an int8 over serial every 1 second.
     UART_putHexByte( Whole );   // Transmit the hex value
     UART_putHexByte( Fract );
     UART_PutDec32( Tc_100 );    // Transmit the decimal value
     TimerA_UART_tx( ' ' );   // Transmit a space character

     // insert a breakpoint here and observe the variables in the watch window
     delayMicroseconds( 1000000 );
   }
}

int OW_Reset( int Pin )
{
 int presence;

 delayMicroseconds( 0 );  // delay for 0us
 digitalWrite( Pin, LOW );
 pinMode( Pin, OUTPUT );
 delayMicroseconds( 480 );  // delay for 480us
 pinMode( Pin, INPUT );
 delayMicroseconds( 70 );  // delay for 70us

 if( 0 == digitalRead( WIRE_PORT ) )
 {
   presence = TRUE;      // Presense pulse detected
 }
 else
 {
   presence = FALSE;     // ds18b20 is not there
 }

 delayMicroseconds( 410 );  // delay for 410us

 return ( presence );        // return the sampled presence pulse result
}

// Peter Anderson wrote this
void OW_WriteByte( int Pin, char d )
{
 char n;
 for(n=8; n!=0; n--)
 {
   if ( (d & 0x01) == 1 )
   {
     digitalWrite( Pin, LOW );
     pinMode( Pin, OUTPUT );
     delayMicroseconds( 5 );
     pinMode( Pin, INPUT );
     delayMicroseconds( 60 );
   }
   else
   {
     digitalWrite( Pin, LOW );
     pinMode( Pin, OUTPUT );
     delayMicroseconds( 60 );
     pinMode( Pin, INPUT );
   }
   d = d >> 1;
 }
}

// Peter Anderson wrote this
char OW_ReadByte( int pin )
{
 char d, n, b;

 for ( n=0; n<8; n++ )
 {
   digitalWrite( pin, LOW );
   pinMode( pin, OUTPUT);
   delayMicroseconds( 5 );
   pinMode( pin, INPUT );
   delayMicroseconds( 5 );
   b = digitalRead( pin );
   delayMicroseconds( 50 );
   d = (d >> 1) | (b << 7); // shift d to right and insert b in most sig bit position
 }
 return(d);
}

//------------------------------------------------------------------------------
// Arduino like digitalRead, pinMode, and digitalWrite helper functions.
//------------------------------------------------------------------------------
char digitalRead( char Pin )
{
 // Read P1IN and mask it with the 1wPin then shift it to the LSB position
 return ( (P1IN &  (1 << Pin) ) >> Pin );
}

void pinMode( int pin, int out )
{
 if( 1 == out )
 {
   P1DIR |= (1 << pin);
 }
 else
 {
   P1DIR &= (~(1 << pin));
 }
}

void digitalWrite(int Pin, int Bit)
{
 if ( 1 == ( Bit ) )
 {
   P1OUT |= ( 1 << Pin );      // if the Bit is HIGH then drive the 1wBus HIGH
 }
 else
 {
   P1OUT &= ( ~(1 << Pin) );   // if the Bit is LOW then drive the 1wBus LOW
 }
}

UINT8 UART_putHexByte(UINT8 data)
{
 unsigned char nibble;

 nibble = (data>>4) & 0x0F;

 if (nibble<10)
 {
   nibble=nibble+'0';
 }
 else
 {
   nibble = nibble-10+'a';
 }

 TimerA_UART_tx( nibble );

 nibble = data & 0x0F;

 if (nibble<10)
 {
   nibble = nibble+'0';
 }
 else
 {
   nibble = nibble-10+'a';
 }

 TimerA_UART_tx( nibble );

 return 1;
}

UINT8 UART_PutDec32(UINT32 data)
{
 UINT32 divisor, result, remainder;
 UINT8 leading;

 leading=1;

 remainder = data;

 for(divisor = 1000000000; divisor > 0; divisor /= 10)
 {
   result = remainder / divisor;
   remainder %= divisor;

   if (result)
     leading=0;

   if (!leading||(divisor==1))
     TimerA_UART_tx((UINT8)(result) + '0');
 }

 return 1;
}

//------------------------------------------------------------------------------
// Function configures Timer_A for full-duplex UART operation
//------------------------------------------------------------------------------
void TimerA_UART_init(void)
{
 TACCTL0 = OUT;                          // Set TXD Idle as Mark = '1'
 TACCTL1 = SCS + CM1 + CAP + CCIE;       // Sync, Neg Edge, Capture, Int
 TACTL = TASSEL_2 + MC_2;                // SMCLK, start in continuous mode
}

//------------------------------------------------------------------------------
// Outputs one byte using the Timer_A UART
//------------------------------------------------------------------------------
void TimerA_UART_tx(unsigned char byte)
{
 while (TACCTL0 & CCIE);                 // Ensure last char got TX'd
 TACCR0 = TAR;                           // Current state of TA counter
 TACCR0 += UART_TBIT;                    // One bit time till first bit
 TACCTL0 = OUTMOD0 + CCIE;               // Set TXD on EQU0, Int
 txData = byte;                          // Load global variable
 txData |= 0x100;                        // Add mark stop bit to TXData
 txData <<= 1;                           // Add space start bit
}

//------------------------------------------------------------------------------
// Timer_A UART - Transmit Interrupt Handler
//------------------------------------------------------------------------------
#pragma vector = TIMERA0_VECTOR
__interrupt void Timer_A0_ISR(void)
{
 static unsigned char txBitCnt = 10;

 TACCR0 += UART_TBIT;                    // Add Offset to CCRx
 if (txBitCnt == 0)                      // All bits TXed?
 {
   TACCTL0 &= ~CCIE;                   // All bits TXed, disable interrupt
   txBitCnt = 10;                      // Re-load bit counter
 }
 else
 {
   if (txData & 0x01)
   {
     TACCTL0 &= ~OUTMOD2;              // TX Mark '1'
   }
   else
   {
     TACCTL0 |= OUTMOD2;               // TX Space '0'
   }
   txData >>= 1;
   txBitCnt--;
 }
}

//------------------------------------------------------------------------------
// Timer_A UART - Receive Interrupt Handler
//------------------------------------------------------------------------------
#pragma vector = TIMERA1_VECTOR
__interrupt void Timer_A1_ISR(void)
{
 static unsigned char rxBitCnt = 8;
 static unsigned char rxData = 0;

 switch ( __even_in_range(TAIV, TAIV_TAIFG) )  // Use calculated branching
 {
   case TAIV_TACCR1:                           // TACCR1 CCIFG - UART RX
     TACCR1 += UART_TBIT;                 // Add Offset to CCRx
     if (TACCTL1 & CAP)                    // Capture mode = start bit edge
     {
       TACCTL1 &= ~CAP;                 // Switch capture to compare mode
       TACCR1 += UART_TBIT_DIV_2;       // Point CCRx to middle of D0
     }
     else
     {
       rxData >>= 1;
       if (TACCTL1 & SCCI)             // Get bit waiting in receive latch
       {
         rxData |= 0x80;
       }
       rxBitCnt--;
       if (rxBitCnt == 0)              // All bits RXed?
       {
         rxBuffer = rxData;           // Store in global variable
         rxBitCnt = 8;                // Re-load bit counter
         TACCTL1 |= CAP;              // Switch compare to capture mode
         __bic_SR_register_on_exit(LPM0_bits);  // Clear LPM0 bits from 0(SR)
       }
     }
   break;
 }
}







*/










/*
 * temp = PH2
 * IN8 = PH3
 * IN1 = PN0
 * IN2 = PN1
 * IN3 = PN2
 * IN4 = PN3
 *
 *
 *
 *
 *
 *
 */




/* JSON Parser include */
#include "jsmn.h"
#include "connect.h"

int currentMinutes = 0;
int currentHour = 0;
int currentDay = 0;
int currentMonth = 0;
int currentYear = 0;
int currentSecond = 0;
int lastSyncMinutes = 0;
void configureWirelessConnection();



#define GPIO_PORTL_DATA_R       (*((volatile uint32_t *)0x400623FC))



#define GPIO_PORTL0             (*((volatile uint32_t *)0x40062004))

#define GPIO_PORTL1             (*((volatile uint32_t *)0x40062008))


float median(int x[], int n) {
    int temp;
    int i, j;
    // the following two loops sort the array x in ascending order
    for(i=0; i<n-1; i++) {
        for(j=i+1; j<n; j++) {
            if(x[j] < x[i]) {
                // swap elements
                temp = x[i];
                x[i] = x[j];
                x[j] = temp;
            }
        }
    }

    if(n%2==0) {
        // if there is an even number of elements, return mean of the two elements in the middle
        return((x[n/2] + x[n/2 - 1]) / 2.0);
    } else {
        // else return the element in the middle
        return x[n/2];
    }
}


void getTDS()  {

    ADC0_InitSWTriggerSeq3(0);       // initialize ADC0, software trigger, PE3/AIN0

    int SCOUNT = 40;
    int buffer[40];
    int i;
    int myVal;

    for(i = 0; i < SCOUNT; i++)  {

        SysTick_Wait10ms(10);
        buffer[i] = ADC0_InSeq3();
    }


    float tdsValue;
    float temperature = 25;
    double averageVoltage;
    averageVoltage = median(buffer, SCOUNT) * (double)3.3 / 4096.0;



    double compensationCoefficient=1.0+0.02*(temperature-25.0);    //temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
    double compensationVoltage=averageVoltage/compensationCoefficient;  //temperature compensation
    tdsValue=(133.42*compensationVoltage*compensationVoltage*compensationVoltage - 255.86*compensationVoltage*compensationVoltage + 857.39*compensationVoltage)*0.5; //convert voltage value to tds value

    //int mytds = (int) tdsValue;

    //myVal = SendTDS(1, mytds);

    myVal = SendDecimal("PE3", averageVoltage);


}




void getPh()  {
    ADC0_InitSWTriggerSeq3(1);       // initialize ADC0, software trigger, PE3/AIN0


    int result;
    result = ADC0_InSeq3();

    int phCount = 40;
    int phBuffer[40];
    int i;
    float phVal;

    for(i = 0; i < phCount; i++)  {

        SysTick_Wait10ms(10);
        phBuffer[i] = ADC0_InSeq3();
    }

    double phVoltage;
    phVoltage = median(phBuffer, phCount)*(float) 3.3/4096;
    double newPh;
    newPh = -6.3829 * phVoltage + 16.76;

    phVal = 3.5*phVoltage;

    int myph = (int) phVal;

    result = SendDecimal("PE2", phVoltage);


}


/*
 * Application's entry point
 */
int main(int argc, char** argv)
{

    PLL_Init();                      // 120 MHz
    SysTick_Init();

    _i32 retVal = -1;

    retVal = initializeAppVariables();
    ASSERT_ON_ERROR(retVal);

    /* Stop WDT and initialize the system-clock of the MCU */
    stopWDT();
    initClk();

    /* Configure command line interface */
    CLI_Configure();

    displayBanner();





    // must be as large as the FIFO for the sequencer in use.  This example
                 // uses sequence 3 which has a FIFO depth of 1.  If another sequence
                 // was used with a deeper FIFO, then the array size must be changed.
                 //
                 uint32_t ADCValues[1];

                 //
                 // These variables are used to store the temperature conversions for
                 // Celsius and Fahrenheit.
                 //
                 uint32_t TempValueC ;
                 uint32_t TempValueF ;

                 //
                 // The ADC0 peripheral must be enabled for use.
                 //
                 SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
                 SysCtlDelay(3);

                 //
                 // Enable sample sequence 3 with a processor signal trigger.  Sequence 3
                 // will do a single sample when the processor sends a singal to start the
                 // conversion.  Each ADC module has 4 programmable sequences, sequence 0
                 // to sequence 3.  This example is arbitrarily using sequence 3.
                 //
                 ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);

                 //
                 // Configure step 0 on sequence 3.  Sample the temperature sensor
                 // (ADC_CTL_TS) and configure the interrupt flag (ADC_CTL_IE) to be set
                 // when the sample is done.  Tell the ADC logic that this is the last
                 // conversion on sequence 3 (ADC_CTL_END).  Sequence 3 has only one
                 // programmable step.  Sequence 1 and 2 have 4 steps, and sequence 0 has
                 // 8 programmable steps.  Since we are only doing a single conversion using
                 // sequence 3 we will only configure step 0.  For more information on the
                 // ADC sequences and steps, reference the datasheet.
                 //
                 ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_TS | ADC_CTL_IE |
                                          ADC_CTL_END);

                 //
                 // Since sample sequence 3 is now configured, it must be enabled.
                 //
                 ADCSequenceEnable(ADC0_BASE, 3);

                 //
                 // Clear the interrupt status flag.  This is done to make sure the
                 // interrupt flag is cleared before we sample.
                 //
                 ADCIntClear(ADC0_BASE, 3);

                 //
                 // Sample the temperature sensor forever.  Display the value on the
                 // console.
                 //
                 while(1)
                 {
                     //
                     // Trigger the ADC conversion.
                     //
                     ADCProcessorTrigger(ADC0_BASE, 3);

                     //
                     // Wait for conversion to be completed.
                     //
                     while(!ADCIntStatus(ADC0_BASE, 3, false))
                     {
                     }

                     //
                     // Clear the ADC interrupt flag.
                     //
                     ADCIntClear(ADC0_BASE, 3);

                     //
                     // Read ADC Value.
                     //
                     ADCSequenceDataGet(ADC0_BASE, 3, ADCValues);

                     //
                     // Use non-calibrated conversion provided in the data sheet. I use floats in intermediate
                     // math but you could use intergers with multiplied by powers of 10 and divide on the end
                     // Make sure you divide last to avoid dropout.
                     //
                     TempValueC = (uint32_t)(147.5 - ((75.0*3.3 *(float)ADCValues[0])) / 4096.0);

                     //
                     // Get Fahrenheit value.  Make sure you divide last to avoid dropout.
                     //
                     TempValueF = ((TempValueC * 9) + 160) / 5;

                     //
                     // Display the temperature value on the console.

                     //
                     // This function provides a means of generating a constant length
                     // delay.  The function delay (in cycles) = 3 * parameter.  Delay
                     // 250ms arbitrarily.
                     //
                     SysCtlDelay(80000000 / 12);
                 }








    // activate clock for Port M
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R11;

    GPIO_PORTM_DIR_R &= ~0x10;       // make PL0 in (PJ0 built-in SW1)
    GPIO_PORTM_AFSEL_R &= ~0x10;     // disable alt funct on PJ0
    GPIO_PORTM_DEN_R |= 0x10;        // enable digital I/O on PJ0
    GPIO_PORTM_IS_R &= ~0x10;
    GPIO_PORTM_IBE_R &= ~0x10;
    GPIO_PORTM_IEV_R |= 0x10;
    GPIO_PORTM_ICR_R = 0x10;
    GPIO_PORTM_IM_R |= 0x10;
    NVIC_PRI18_R = (NVIC_PRI18_R&0xFFFFFF00)|0x00000040; // (g) priority 2
      NVIC_EN1_R |= 8;              // (h) enable interrupt 2 in NVIC









      PWM0A_Init(10, 5);        // initialize PWM0A/PF0, 1000 Hz, 75% duty
    //  PWM0B_Init(60000, 15000);        // initialize PWM0B/PF1, 1000 Hz, 25% duty
    //  PWM0A_Duty(6000);    // 10%
    //  PWM0A_Duty(15000);   // 25%
    //  PWM0A_Duty(45000);   // 75%

    //  PWM0A_Init(6000, 3000);           // initialize PWM0A/PF0, 10000 Hz, 50% duty
    //  PWM0A_Init(1500, 1350);           // initialize PWM0A/PF0, 40000 Hz, 90% duty
    //  PWM0A_Init(1500, 225);            // initialize PWM0A/PF0, 40000 Hz, 15% duty
    //  PWM0A_Init(60, 30);               // initialize PWM0A/PF0, 1 MHz, 50% duty

      EnableInterrupts();


    configureWirelessConnection();

    /*
     * Following function configures the device to default state by cleaning
     * the persistent settings stored in NVMEM (viz. connection profiles &
     * policies, power policy etc)
     *
     * Applications may choose to skip this step if the developer is sure
     * that the device is in its default state at start of application
     *
     * Note that all profiles and persistent settings that were done on the
     * device will be lost
     */


    retVal = updateLocalTime();
    currentMinutes = getCurrentMinute();
    currentHour = getCurrentHour();
    currentDay = getCurrentDay();
    currentMonth = getCurrentMonth();
    currentYear = getCurrentYear();
    currentSecond = getCurrentSecond();
    lastSyncMinutes = currentMinutes;
    int previousMeasureHour = currentHour - 1;



    while(1)  {

        if(lastSyncMinutes / 10 != currentMinutes / 10)  {
            retVal = updateLocalTime();
            currentMinutes = getCurrentMinute();
            currentHour = getCurrentHour();
            currentDay = getCurrentDay();
            currentMonth = getCurrentMonth();
            currentYear = getCurrentYear();
            currentSecond = getCurrentSecond();

        }

    if(previousMeasureHour != currentHour)  {
        getTDS();
        getPh();
        GetInstruction();
        ExecuteInstructions();
        previousMeasureHour = currentHour;
    }




          SysTick_Wait10ms(6000);
          currentMinutes++;
      }
      return 0;
}







void configureWirelessConnection()  {

    _i32 retVal = -1;

    retVal = configureSimpleLinkToDefaultState();
    if(retVal < 0)
    {
        if (DEVICE_NOT_IN_STATION_MODE == retVal)
            CLI_Write((_u8 *)" Failed to configure the device in its default state \n\r");

            LOOP_FOREVER();
    }


    CLI_Write((_u8 *)" Device is configured in default state \n\r");

/*
* Assumption is that the device is configured in station mode already
* and it is in its default state
*/
    retVal = sl_Start(0, 0, 0);
    if ((retVal < 0) ||
            (ROLE_STA != retVal) )
    {
        CLI_Write((_u8 *)" Failed to start the device \n\r");
        LOOP_FOREVER();
    }

    CLI_Write((_u8 *)" Device started as STATION \n\r");

    /* Connecting to WLAN AP */
    retVal = establishConnectionWithAP();
    if(retVal < 0)
    {
        CLI_Write((_u8 *)" Failed to establish connection w/ an AP \n\r");
        LOOP_FOREVER();
    }

    CLI_Write((_u8 *)" Connection established w/ AP and IP is acquired \n\r");
    CLI_Write((_u8 *)" Pinging...! \n\r");

    retVal = checkLanConnection();
    if(retVal < 0)
    {
        CLI_Write((_u8 *)" Device couldn't connect to LAN \n\r");
        LOOP_FOREVER();
    }

    CLI_Write((_u8 *)" Device successfully connected to the LAN\r\n");

    retVal = checkInternetConnection();
    if(retVal < 0)
    {
        CLI_Write((_u8 *)" Device couldn't connect to the internet \n\r");
        LOOP_FOREVER();
    }

    CLI_Write((_u8 *)" Device successfully connected to the internet \n\r");



    //retVal = Lab16();
    if(retVal == 0){  // valid

        CLI_Write((_u8 *)" Device successfully got the weather \n\r");

    }
}
















