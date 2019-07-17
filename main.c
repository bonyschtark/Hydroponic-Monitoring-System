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


/*
   if(millis()-analogSampleTimepoint > 40U)     //every 40 milliseconds,read the analog value from the ADC
   {
     analogSampleTimepoint = millis();
     analogBuffer[analogBufferIndex] = analogRead(TdsSensorPin);    //read the analog value and store into the buffer
     analogBufferIndex++;
     if(analogBufferIndex == SCOUNT)
         analogBufferIndex = 0;
   }
   static unsigned long printTimepoint = millis();
   if(millis()-printTimepoint > 800U)
   {
      printTimepoint = millis();
      for(copyIndex=0;copyIndex<SCOUNT;copyIndex++)
        analogBufferTemp[copyIndex]= analogBuffer[copyIndex];
      averageVoltage = getMedianNum(analogBufferTemp,SCOUNT) * (float)VREF / 1024.0; // read the analog value more stable by the median filtering algorithm, and convert to voltage value
      float compensationCoefficient=1.0+0.02*(temperature-25.0);    //temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
      float compensationVolatge=averageVoltage/compensationCoefficient;  //temperature compensation
      tdsValue=(133.42*compensationVolatge*compensationVolatge*compensationVolatge - 255.86*compensationVolatge*compensationVolatge + 857.39*compensationVolatge)*0.5; //convert voltage value to tds value
      //Serial.print("voltage:");
      //Serial.print(averageVoltage,2);
      //Serial.print("V   ");
      Serial.print("TDS Value:");
      Serial.print(tdsValue,0);
      Serial.println("ppm");


*/


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


    /*

    if(lastSyncMinutes / 10 != currentMinutes / 10)  {
        SynchronizeTime();
    }

    //if(previousMeasureHour != currentHour)  {
        getTDS();
        sendTDS();
        previousMeasureHour = currentHour;
    //}

    if(lastMeasurementHour < currentHour)  {
        getTDS();
        getPH();
        getTemp();
        getMoisture();
        lastMeasurementHour = currentHour;
        getInstructions();
    }

    */

      int SCOUNT = 40;
      int buffer[40];
      int i;

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

      retVal = SendTDS(1, mytds);


    //SysTick_Wait10ms(10000);

    //retVal = Lab16();



      while(1)  {

      }
      return 0;
}






