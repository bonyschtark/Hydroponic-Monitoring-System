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
#include <stdint.h>
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
#include "connect.h"



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



int getInternalTemp()  {

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

                     return TempValueC;
                 }


}


int currentMinutes = 0;
int currentHour = 0;
int currentDay = 0;
int currentMonth = 0;
int currentYear = 0;
int currentSecond = 0;
int lastSyncMinutes = 0;



void init()  {

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





    getPh();



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




    retVal = updateLocalTime();
    currentMinutes = getCurrentMinute();
    currentHour = getCurrentHour();
    currentDay = getCurrentDay();
    currentMonth = getCurrentMonth();
    currentYear = getCurrentYear();
    currentSecond = getCurrentSecond();
    lastSyncMinutes = currentMinutes;
    int previousMeasureHour = currentHour - 1;

    while (1)
    {

        if (lastSyncMinutes / 10 != currentMinutes / 10)
        {
            retVal = updateLocalTime();
            currentMinutes = getCurrentMinute();
            currentHour = getCurrentHour();
            currentDay = getCurrentDay();
            currentMonth = getCurrentMonth();
            currentYear = getCurrentYear();
            currentSecond = getCurrentSecond();

        }

        if (previousMeasureHour != currentHour)
        {
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
















