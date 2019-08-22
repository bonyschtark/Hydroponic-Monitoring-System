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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SysTick.h"


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
    float averageVoltage;
    averageVoltage = median(buffer, SCOUNT) * (float)3.3 / 4096.0;

    float compensationCoefficient=1.0+0.02*(temperature-25.0);    //temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
    float compensationVoltage=averageVoltage/compensationCoefficient;  //temperature compensation
    tdsValue=(133.42*compensationVoltage*compensationVoltage*compensationVoltage - 255.86*compensationVoltage*compensationVoltage + 857.39*compensationVoltage)*0.5; //convert voltage value to tds value

    int mytds = (int) tdsValue;

    myVal = SendTDS(1, mytds);

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

    float phVoltage;
    phVoltage = median(phBuffer, phCount)*(float) 3.3/4096;
    double newPh;
    newPh = -6.3829 * phVoltage + 16.76;

    phVal = 3.5*phVoltage;

    int myph = (int) phVal;

    result = SendDecimal(2, newPh);


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



    // activate clock for Port N
SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R12;
        // allow time for clock to stabilize
while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R12) == 0){};
//  ADC0_InitSWTriggerSeq3_Ch0();    // initialize ADC0, software trigger, PE3/AIN0


ADC0_InitSWTriggerSeq3(0);       // initialize ADC0, software trigger, PE3/AIN0

// activate clock for Port J
SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R8;
  // allow time for clock to stabilize
while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R8) == 0){};
GPIO_PORTJ_DIR_R |= 0x01;       // make PJ0 in (PJ0 built-in SW1)
GPIO_PORTJ_AFSEL_R &= ~0x01;     // disable alt funct on PJ0
GPIO_PORTJ_DEN_R |= 0x01;        // enable digital I/O on PJ0
  // configure PJ0 as GPIO
GPIO_PORTJ_PCTL_R = (GPIO_PORTJ_PCTL_R&0xFFFFFFF0)+0x00000000;
GPIO_PORTJ_AMSEL_R &= ~0x01;     // disable analog functionality on PJ0


// activate clock for Port L
SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R10;
while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R10) == 0){};

/*

GPIO_PORTL_DIR_R |= 0x01;       // make PL0 in (PJ0 built-in SW1)
GPIO_PORTL_AFSEL_R &= ~0x01;     // disable alt funct on PJ0
GPIO_PORTL_DEN_R |= 0x01;        // enable digital I/O on PJ0
  // configure PJ0 as GPIO
GPIO_PORTL_PCTL_R = (GPIO_PORTL_PCTL_R&0xFFFFFFF0)+0x00000000;
//GPIO_PORTL_AMSEL_R &= ~0x01;     // disable analog functionality on PJ0

*/

    GPIO_PORTL_DIR_R |= 0x0F;       // make PL0 in (PJ0 built-in SW1)
    GPIO_PORTL_AFSEL_R &= ~0x0F;     // disable alt funct on PJ0
    GPIO_PORTL_DEN_R |= 0x0F;        // enable digital I/O on PJ0


/*

    GPIO_PORTL_DATA_R |= 0x01;

    //GPIO_PORTL0 = 1;
        SysTick_Wait10ms(100);
        //GPIO_PORTL0 = 0;

        GPIO_PORTL_DATA_R &= ~0x01;

        GPIO_PORTL_DATA_R |= 0x02;

       // GPIO_PORTL1 = 1;
        SysTick_Wait10ms(100);
        GPIO_PORTL_DATA_R &= ~0x02;
      //  GPIO_PORTL1 = 0;
*/



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






