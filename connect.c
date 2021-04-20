#include "simplelink.h"
#include "sl_common.h"




#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "connect.h"



#define SL_STOP_TIMEOUT        0xFF



#define SERVER  "SERVER"

#define GETINSTRUCT "GET /greengarden/instruction.php HTTP/1.0\r\n\r\n"
#define GETINSTRUCTIONCOUNT "GET /greengarden/getinstructioncount.php HTTP/1.0\r\n\r\n"


#define GETTIME "GET /greengarden/gettime.php HTTP/1.0\r\n\r\n"
#define REQUEST "POST /greengarden/garden.php HTTP/1.1\r\nHost: 192.168.1.8\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 14\r\n\r\nid=107&val=jul\r\n\r\n"


#define SERVER  "SERVER"


#define BAUD_RATE           115200
#define MAX_RECV_BUFF_SIZE  1024
#define MAX_SEND_BUFF_SIZE  512
#define MAX_HOSTNAME_SIZE   40
#define MAX_PASSKEY_SIZE    32
#define MAX_SSID_SIZE       32

//#define HOST_NAME              "httpbin.org"
#define HOST_PORT              80


#define READ_SIZE       1450
#define MAX_BUFF_SIZE   1460
#define SPACE           32
//





/* HTTP Client lib include */
//#include <http/client/httpcli.h>

/* JSON Parser include */
#include "jsmn.h"

_u32  g_PingPacketsRecv = 0;
_u32  g_GatewayIP = 0;

_u32 g_Status;
_u32 g_DestinationIP;
_u32 g_BytesReceived; /* variable to store the file size */
_u8  g_buff[MAX_BUFF_SIZE+1];




struct{
  char Recvbuff[MAX_RECV_BUFF_SIZE];
  char SendBuff[MAX_SEND_BUFF_SIZE];
  char HostName[MAX_HOSTNAME_SIZE];
  unsigned long DestinationIP;
  int SockID;
}appData;



struct{
  int currDay;
  int currMonth;
  int currYear;
  int currHour;
  int currMinute;
  int currSecond;
}theTime;




// these three strings will be filled by getWeather
#define MAXLEN 100
char City[MAXLEN];
char Temperature[MAXLEN];
char Weather[MAXLEN];
char Id[MAXLEN];
char Score[MAXLEN];
char Edxpost[MAXLEN];


#define APPLICATION_VERSION "1.3.0"

#define SL_STOP_TIMEOUT        0xFF

/* Use bit 32:
 *      1 in a 'status_variable', the device has completed the ping operation
 *      0 in a 'status_variable', the device has not completed the ping operation
 */
#define STATUS_BIT_PING_DONE  31

#define HOST_NAME       "www.ti.com"

/*
 * Values for below macros shall be modified for setting the 'Ping' properties
 */
#define PING_INTERVAL   1000    /* In msecs */
#define PING_TIMEOUT    3000    /* In msecs */
#define PING_PKT_SIZE   20      /* In bytes */
#define NO_OF_ATTEMPTS  3


#define IS_PING_DONE(status_variable)           GET_STATUS_BIT(status_variable, \
                                                               STATUS_BIT_PING_DONE)




/*
 * ASYNCHRONOUS EVENT HANDLERS -- Start
 */

/*!
    \brief This function handles WLAN events

    \param[in]      pWlanEvent is the event passed to the handler

    \return         None

    \note

    \warning
*/
void SimpleLinkWlanEventHandler(SlWlanEvent_t *pWlanEvent)
{
    if(pWlanEvent == NULL)
    {
        CLI_Write((_u8 *)" [WLAN EVENT] NULL Pointer Error \n\r");
        return;
    }

    switch(pWlanEvent->Event)
    {
        case SL_WLAN_CONNECT_EVENT:
        {
            SET_STATUS_BIT(g_Status, STATUS_BIT_CONNECTION);

            /*
             * Information about the connected AP (like name, MAC etc) will be
             * available in 'slWlanConnectAsyncResponse_t' - Applications
             * can use it if required
             *
             * slWlanConnectAsyncResponse_t *pEventData = NULL;
             * pEventData = &pWlanEvent->EventData.STAandP2PModeWlanConnected;
             *
             */
        }
        break;

        case SL_WLAN_DISCONNECT_EVENT:
        {
            slWlanConnectAsyncResponse_t*  pEventData = NULL;

            CLR_STATUS_BIT(g_Status, STATUS_BIT_CONNECTION);
            CLR_STATUS_BIT(g_Status, STATUS_BIT_IP_ACQUIRED);

            pEventData = &pWlanEvent->EventData.STAandP2PModeDisconnected;

            /* If the user has initiated 'Disconnect' request, 'reason_code' is SL_USER_INITIATED_DISCONNECTION */
            if(SL_WLAN_DISCONNECT_USER_INITIATED_DISCONNECTION == pEventData->reason_code)
            {
                CLI_Write((_u8 *)" Device disconnected from the AP on application's request \n\r");
            }
            else
            {
                CLI_Write((_u8 *)" Device disconnected from the AP on an ERROR..!! \n\r");
            }
        }
        break;

        default:
        {
            CLI_Write((_u8 *)" [WLAN EVENT] Unexpected event \n\r");
        }
        break;
    }
}

/*!
    \brief This function handles events for IP address acquisition via DHCP
           indication

    \param[in]      pNetAppEvent is the event passed to the handler

    \return         None

    \note

    \warning
*/
void SimpleLinkNetAppEventHandler(SlNetAppEvent_t *pNetAppEvent)
{
    if(pNetAppEvent == NULL)
    {
        CLI_Write((_u8 *)" [NETAPP EVENT] NULL Pointer Error \n\r");
        return;
    }

    switch(pNetAppEvent->Event)
    {
        case SL_NETAPP_IPV4_IPACQUIRED_EVENT:
        {
            SlIpV4AcquiredAsync_t *pEventData = NULL;

            SET_STATUS_BIT(g_Status, STATUS_BIT_IP_ACQUIRED);

            pEventData = &pNetAppEvent->EventData.ipAcquiredV4;
            g_GatewayIP = pEventData->gateway;
        }
        break;

        default:
        {
            CLI_Write((_u8 *)" [NETAPP EVENT] Unexpected event \n\r");
        }
        break;
    }
}

/*!
    \brief This function handles callback for the HTTP server events

    \param[in]      pHttpEvent - Contains the relevant event information
    \param[in]      pHttpResponse - Should be filled by the user with the
                    relevant response information

    \return         None

    \note

    \warning
*/
void SimpleLinkHttpServerCallback(SlHttpServerEvent_t *pHttpEvent,
                                  SlHttpServerResponse_t *pHttpResponse)
{
    /*
     * This application doesn't work with HTTP server - Hence these
     * events are not handled here
     */
    CLI_Write((_u8 *)" [HTTP EVENT] Unexpected event \n\r");
}


/*!
    \brief This function handles ping report events

    \param[in]      pPingReport holds the ping report statistics

    \return         None

    \note

    \warning
*/
void SimpleLinkPingReport(SlPingReport_t *pPingReport)
{
    SET_STATUS_BIT(g_Status, STATUS_BIT_PING_DONE);

    if(pPingReport == NULL)
    {
        CLI_Write((_u8 *)" [PING REPORT] NULL Pointer Error\r\n");
        return;
    }

    g_PingPacketsRecv = pPingReport->PacketsReceived;
}

/*!
    \brief This function handles general error events indication

    \param[in]      pDevEvent is the event passed to the handler

    \return         None
*/
void SimpleLinkGeneralEventHandler(SlDeviceEvent_t *pDevEvent)
{
    /*
     * Most of the general errors are not FATAL are are to be handled
     * appropriately by the application
     */
    CLI_Write((_u8 *)" [GENERAL EVENT] \n\r");
}

/*!
    \brief This function handles socket events indication

    \param[in]      pSock is the event passed to the handler

    \return         None
*/
void SimpleLinkSockEventHandler(SlSockEvent_t *pSock)
{
    /*
     * This application doesn't work w/ socket - Hence not handling these events
     */
    CLI_Write((_u8 *)" [SOCK EVENT] Unexpected event \n\r");
}
/*
 * ASYNCHRONOUS EVENT HANDLERS -- End
 */


/*!
    \brief This function configure the SimpleLink device in its default state. It:
           - Sets the mode to STATION
           - Configures connection policy to Auto and AutoSmartConfig
           - Deletes all the stored profiles
           - Enables DHCP
           - Disables Scan policy
           - Sets Tx power to maximum
           - Sets power policy to normal
           - Unregisters mDNS services
           - Remove all filters

    \param[in]      none

    \return         On success, zero is returned. On error, negative is returned
*/
_i32 configureSimpleLinkToDefaultState()
{
    SlVersionFull   ver = {0};
    _WlanRxFilterOperationCommandBuff_t  RxFilterIdMask = {0};

    _u8           val = 1;
    _u8           configOpt = 0;
    _u8           configLen = 0;
    _u8           power = 0;

    _i32          retVal = -1;
    _i32          mode = -1;

    mode = sl_Start(0, 0, 0);
    ASSERT_ON_ERROR(mode);

    /* If the device is not in station-mode, try configuring it in station-mode */
    if (ROLE_STA != mode)
    {
        if (ROLE_AP == mode)
        {
            /* If the device is in AP mode, we need to wait for this event before doing anything */
            while(!IS_IP_ACQUIRED(g_Status)) { _SlNonOsMainLoopTask(); }
        }

        /* Switch to STA role and restart */
        retVal = sl_WlanSetMode(ROLE_STA);
        ASSERT_ON_ERROR(retVal);

        retVal = sl_Stop(SL_STOP_TIMEOUT);
        ASSERT_ON_ERROR(retVal);

        retVal = sl_Start(0, 0, 0);
        ASSERT_ON_ERROR(retVal);

        /* Check if the device is in station again */
        if (ROLE_STA != retVal)
        {
            /* We don't want to proceed if the device is not coming up in station-mode */
            ASSERT_ON_ERROR(DEVICE_NOT_IN_STATION_MODE);
        }
    }

    /* Get the device's version-information */
    configOpt = SL_DEVICE_GENERAL_VERSION;
    configLen = sizeof(ver);
    retVal = sl_DevGet(SL_DEVICE_GENERAL_CONFIGURATION, &configOpt, &configLen, (_u8 *)(&ver));
    ASSERT_ON_ERROR(retVal);

    /* Set connection policy to Auto + SmartConfig (Device's default connection policy) */
    retVal = sl_WlanPolicySet(SL_POLICY_CONNECTION, SL_CONNECTION_POLICY(1, 0, 0, 0, 1), NULL, 0);
    ASSERT_ON_ERROR(retVal);

    /* Remove all profiles */
    retVal = sl_WlanProfileDel(0xFF);
    ASSERT_ON_ERROR(retVal);

    /*
     * Device in station-mode. Disconnect previous connection if any
     * The function returns 0 if 'Disconnected done', negative number if already disconnected
     * Wait for 'disconnection' event if 0 is returned, Ignore other return-codes
     */
    retVal = sl_WlanDisconnect();
    if(0 == retVal)
    {
        /* Wait */
        while(IS_CONNECTED(g_Status)) { _SlNonOsMainLoopTask(); }
    }

    /* Enable DHCP client*/
    retVal = sl_NetCfgSet(SL_IPV4_STA_P2P_CL_DHCP_ENABLE,1,1,&val);
    ASSERT_ON_ERROR(retVal);

    /* Disable scan */
    configOpt = SL_SCAN_POLICY(0);
    retVal = sl_WlanPolicySet(SL_POLICY_SCAN , configOpt, NULL, 0);
    ASSERT_ON_ERROR(retVal);

    /* Set Tx power level for station mode
       Number between 0-15, as dB offset from max power - 0 will set maximum power */
    power = 0;
    retVal = sl_WlanSet(SL_WLAN_CFG_GENERAL_PARAM_ID, WLAN_GENERAL_PARAM_OPT_STA_TX_POWER, 1, (_u8 *)&power);
    ASSERT_ON_ERROR(retVal);

    /* Set PM policy to normal */
    retVal = sl_WlanPolicySet(SL_POLICY_PM , SL_NORMAL_POLICY, NULL, 0);
    ASSERT_ON_ERROR(retVal);

    /* Unregister mDNS services */
    retVal = sl_NetAppMDNSUnRegisterService(0, 0);
    ASSERT_ON_ERROR(retVal);

    /* Remove  all 64 filters (8*8) */
    pal_Memset(RxFilterIdMask.FilterIdMask, 0xFF, 8);
    retVal = sl_WlanRxFilterSet(SL_REMOVE_RX_FILTER, (_u8 *)&RxFilterIdMask,
                       sizeof(_WlanRxFilterOperationCommandBuff_t));
    ASSERT_ON_ERROR(retVal);

    retVal = sl_Stop(SL_STOP_TIMEOUT);
    ASSERT_ON_ERROR(retVal);

    retVal = initializeAppVariables();
    ASSERT_ON_ERROR(retVal);

    return retVal; /* Success */
}

/*!
    \brief Connecting to a WLAN Access point

    This function connects to the required AP (SSID_NAME).
    The function will return once we are connected and have acquired IP address

    \param[in]  None

    \return     0 on success, negative error-code on error

    \note

    \warning    If the WLAN connection fails or we don't acquire an IP address,
                We will be stuck in this function forever.
*/
_i32 establishConnectionWithAP()
{
    SlSecParams_t secParams = {0};
    _i32 retVal = 0;

    secParams.Key = (_i8 *)PASSKEY;
    secParams.KeyLen = pal_Strlen(PASSKEY);
    secParams.Type = SEC_TYPE;

    retVal = sl_WlanConnect((_i8 *)SSID_NAME, pal_Strlen(SSID_NAME), 0, &secParams, 0);
    ASSERT_ON_ERROR(retVal);

    /* Wait */
    while((!IS_CONNECTED(g_Status)) || (!IS_IP_ACQUIRED(g_Status))) { _SlNonOsMainLoopTask(); }

    return SUCCESS;
}

/*!
    \brief This function checks the LAN connection by pinging the AP's gateway

    \param[in]  None

    \return     0 on success, negative error-code on error
*/
_i32 checkLanConnection()
{
    SlPingStartCommand_t pingParams = {0};
    SlPingReport_t pingReport = {0};

    _i32 retVal = -1;

    CLR_STATUS_BIT(g_Status, STATUS_BIT_PING_DONE);
    g_PingPacketsRecv = 0;

    /* Set the ping parameters */
    pingParams.PingIntervalTime = PING_INTERVAL;
    pingParams.PingSize = PING_PKT_SIZE;
    pingParams.PingRequestTimeout = PING_TIMEOUT;
    pingParams.TotalNumberOfAttempts = NO_OF_ATTEMPTS;
    pingParams.Flags = 0;
    pingParams.Ip = g_GatewayIP;

    /* Check for LAN connection */
    retVal = sl_NetAppPingStart( (SlPingStartCommand_t*)&pingParams, SL_AF_INET,
                                 (SlPingReport_t*)&pingReport, SimpleLinkPingReport);
    ASSERT_ON_ERROR(retVal);

    /* Wait */
    while(!IS_PING_DONE(g_Status)) { _SlNonOsMainLoopTask(); }

    if(0 == g_PingPacketsRecv)
    {
        /* Problem with LAN connection */
        ASSERT_ON_ERROR(LAN_CONNECTION_FAILED);
    }

    /* LAN connection is successful */
    return SUCCESS;
}

/*!
    \brief This function checks the internet connection by pinging
           the external-host (HOST_NAME)

    \param[in]  None

    \return     0 on success, negative error-code on error
*/
_i32 checkInternetConnection()
{
    SlPingStartCommand_t pingParams = {0};
    SlPingReport_t pingReport = {0};

    _u32 ipAddr = 0;

    _i32 retVal = -1;

    CLR_STATUS_BIT(g_Status, STATUS_BIT_PING_DONE);
    g_PingPacketsRecv = 0;

    /* Set the ping parameters */
    pingParams.PingIntervalTime = PING_INTERVAL;
    pingParams.PingSize = PING_PKT_SIZE;
    pingParams.PingRequestTimeout = PING_TIMEOUT;
    pingParams.TotalNumberOfAttempts = NO_OF_ATTEMPTS;
    pingParams.Flags = 0;
    pingParams.Ip = g_GatewayIP;

    /* Check for Internet connection */
    retVal = sl_NetAppDnsGetHostByName((_i8 *)HOST_NAME, pal_Strlen(HOST_NAME), &ipAddr, SL_AF_INET);
    ASSERT_ON_ERROR(retVal);

    /* Replace the ping address to match HOST_NAME's IP address */
    pingParams.Ip = ipAddr;

    /* Try to ping HOST_NAME */
    retVal = sl_NetAppPingStart( (SlPingStartCommand_t*)&pingParams, SL_AF_INET,
                                 (SlPingReport_t*)&pingReport, SimpleLinkPingReport);
    ASSERT_ON_ERROR(retVal);

    /* Wait */
    while(!IS_PING_DONE(g_Status)) { _SlNonOsMainLoopTask(); }

    if (0 == g_PingPacketsRecv)
    {
        /* Problem with internet connection*/
        ASSERT_ON_ERROR(INTERNET_CONNECTION_FAILED);
    }

    /* Internet connection is successful */
    return SUCCESS;
}

/*!
    \brief This function initializes the application variables

    \param[in]  None

    \return     0 on success, negative error-code on error
*/
_i32 initializeAppVariables()
{
    g_Status = 0;
    g_PingPacketsRecv = 0;
    g_GatewayIP = 0;

    return SUCCESS;
}

/*!
    \brief This function displays the application's banner

    \param      None

    \return     None
*/
void displayBanner()
{
    CLI_Write((_u8 *)"\n\r\n\r");
    CLI_Write((_u8 *)" Getting started with station application - Version ");
    CLI_Write((_u8 *) APPLICATION_VERSION);
    CLI_Write((_u8 *)"\n\r*******************************************************************************\n\r");
}

void performcleanup()  {
    uint32_t i;
    char *pt = NULL;
    /* find ticker name in response*/
        pt = strstr(appData.Recvbuff, "id");
        i = 0;
        if( NULL != pt ){
          pt = pt + 5; // skip over id":"
          while((i<MAXLEN)&&(*pt)&&(*pt!='\"')){
            Id[i] = *pt; // copy into Id string
            pt++; i++;
          }
        }
        Id[i] = 0;

    /* find score Value in response */
        pt = strstr(appData.Recvbuff, "\"score\"");
        i = 0;
        if( NULL != pt ){
          pt = pt + 8; // skip over "score":
          while((i<MAXLEN)&&(*pt)&&(*pt!=',')){
            Score[i] = *pt; // copy into Score string
            pt++; i++;
          }
        }
        Score[i] = 0;

    /* find edxpost in response */
        pt = strstr(appData.Recvbuff, "edxpost");
        i = 0;
        if( NULL != pt ){
          pt = pt + 9; // skip over edxpost":
          for(i=0; i<8; i++){
            Edxpost[i] = *pt; // copy into Edxpost string
            pt++;
          }
        }
        Edxpost[i] = 0;
}



int32_t Lab16(void){


  memcpy(appData.HostName,SERVER,strlen(SERVER));
  if(GetHostIP() == 0){
    if( (appData.SockID = CreateConnection()) < 0 ) return -1;

/* HTTP GET string. */
    strcpy(appData.SendBuff,REQUEST);
// 1) change Austin Texas to your city
// 2) you can change metric to imperial if you want temperature in F
    /* Send the HTTP GET string to the open TCP/IP socket. */
    sl_Send(appData.SockID, appData.SendBuff, strlen(appData.SendBuff), 0);

    CLI_Write((_u8 *) appData.SendBuff);


/* Receive response */
    sl_Recv(appData.SockID, &appData.Recvbuff[0], MAX_RECV_BUFF_SIZE, 0);
    appData.Recvbuff[strlen(appData.Recvbuff)] = '\0';




    CLI_Write((_u8 *) appData.Recvbuff);

    performcleanup();

    sl_Close(appData.SockID);
  }

  return 0;
}







int32_t updateLocalTime(void){


  memcpy(appData.HostName,SERVER,strlen(SERVER));
  if(GetHostIP() == 0){
    if( (appData.SockID = CreateConnection()) < 0 ) return -1;

/* HTTP GET string. */
    strcpy(appData.SendBuff,GETTIME);
// 1) change Austin Texas to your city
// 2) you can change metric to imperial if you want temperature in F
    /* Send the HTTP GET string to the open TCP/IP socket. */
    sl_Send(appData.SockID, appData.SendBuff, strlen(appData.SendBuff), 0);

    //CLI_Write((_u8 *) appData.SendBuff);


/* Receive response */
    sl_Recv(appData.SockID, &appData.Recvbuff[0], MAX_RECV_BUFF_SIZE, 0);
    appData.Recvbuff[strlen(appData.Recvbuff)] = '\0';


    char *myTime = strstr(appData.Recvbuff, "time: ");
    int doubledigitminute = 0;

    theTime.currDay = ((int) (myTime[6] - '0')) * 10 + (int) (myTime[7] - '0');
    theTime.currMonth = ((int)(myTime[9] - '0')) * 10 + (int) (myTime[10] - '0');
    theTime.currYear = ((int)(myTime[12] - '0')) * 1000 + ((int)(myTime[13] - '0')) * 100 + ((int)(myTime[14] - '0')) * 10 +((int)(myTime[15] - '0'));
    theTime.currHour = ((int)(myTime[17] - '0')) * 10 + ((int)(myTime[18] - '0'));
    if('0' < myTime[21] && myTime[21] < '9')  {
        theTime.currMinute = ((int)(myTime[20] - '0')) * 10 + ((int)(myTime[21] - '0'));
        doubledigitminute = 1;
    }  else {
        theTime.currMinute = ((int)(myTime[20]));
    }

    if(doubledigitminute == 1)  {
        if('0' < myTime[24] && myTime[24] < '9')  {
            theTime.currSecond = ((int)myTime[23] - '0') * 10 + (int)(myTime[24] - '0');
        }
        else
        {
            theTime.currSecond = (int)(myTime[23] - '0');
        }
    }
    else
    {
        if('0' < myTime[23] && myTime[23] < '9')
        {

            theTime.currSecond = ((int)myTime[22] - '0') * 10 + (int)(myTime[23] - '0');

        }
        else
        {

            theTime.currSecond = (int)(myTime[22] - '0');

        }

    }


    CLI_Write((_u8 *) "Updated Time");

   // CLI_Write((_u8 *));

    performcleanup();

    sl_Close(appData.SockID);
  }

  return 0;
}




int32_t SendTDS(int id, int val){


  memcpy(appData.HostName,SERVER,strlen(SERVER));
  if(GetHostIP() == 0){
    if( (appData.SockID = CreateConnection()) < 0 ) return -1;

/* HTTP GET string. */
    strcpy(appData.SendBuff, buildTDS(id, val));
// 1) change Austin Texas to your city
// 2) you can change metric to imperial if you want temperature in F
    /* Send the HTTP GET string to the open TCP/IP socket. */
    sl_Send(appData.SockID, appData.SendBuff, strlen(appData.SendBuff), 0);

    CLI_Write((_u8 *) appData.SendBuff);


/* Receive response */
    sl_Recv(appData.SockID, &appData.Recvbuff[0], MAX_RECV_BUFF_SIZE, 0);
    appData.Recvbuff[strlen(appData.Recvbuff)] = '\0';




    CLI_Write((_u8 *) appData.Recvbuff);
    performcleanup();

    sl_Close(appData.SockID);
  }

  return 0;
}



int32_t SendDecimal(char* id, double decimalVal){


  memcpy(appData.HostName,SERVER,strlen(SERVER));
  if(GetHostIP() == 0){
    if( (appData.SockID = CreateConnection()) < 0 ) return -1;



/* HTTP GET string. */
    strcpy(appData.SendBuff, buildDecimal(id, decimalVal));
// 1) change Austin Texas to your city
// 2) you can change metric to imperial if you want temperature in F
    /* Send the HTTP GET string to the open TCP/IP socket. */
    sl_Send(appData.SockID, appData.SendBuff, strlen(appData.SendBuff), 0);



/* Receive response */
    sl_Recv(appData.SockID, &appData.Recvbuff[0], MAX_RECV_BUFF_SIZE, 0);
    appData.Recvbuff[strlen(appData.Recvbuff)] = '\0';

    performcleanup();

    sl_Close(appData.SockID);
  }

  return 0;
}





int32_t getNumberOfInstructions()  {
    int result = 0;


  memcpy(appData.HostName,SERVER,strlen(SERVER));
  if(GetHostIP() == 0){
    if( (appData.SockID = CreateConnection()) < 0 ) return -1;


/* HTTP GET string. */
    strcpy(appData.SendBuff,GETINSTRUCTIONCOUNT);
// 1) change Austin Texas to your city
// 2) you can change metric to imperial if you want temperature in F
    /* Send the HTTP GET string to the open TCP/IP socket. */
    sl_Send(appData.SockID, appData.SendBuff, strlen(appData.SendBuff), 0);


/* Receive response */
    sl_Recv(appData.SockID, &appData.Recvbuff[0], MAX_RECV_BUFF_SIZE, 0);
    appData.Recvbuff[strlen(appData.Recvbuff)] = '\0';

    int j = 0;
    int found = 0;
    int mydevice = 0;
    char strdevice[20] = "";
    char strcount[20] = "";
    int k = 0;
    while(j < strlen(appData.Recvbuff) && found == 0)  {
        if(appData.Recvbuff[j] == '*')  {
            if(appData.Recvbuff[j+1] == '*')  {
                k = 0;
                j = j+2;
                while(appData.Recvbuff[j] >= '0' && appData.Recvbuff[j] <= '9')  {
                    strdevice[k] = appData.Recvbuff[j];
                    k++;
                    j++;
                }
                    strdevice[k] = '\0';
                    found = 1;
            }
        }
        j++;
    }

    result = atoi(strdevice);

    performcleanup();

    sl_Close(appData.SockID);
  }

  return result;

}


int getNextInstruction()  {


  memcpy(appData.HostName,SERVER,strlen(SERVER));
  if(GetHostIP() == 0){
    if( (appData.SockID = CreateConnection()) < 0 ) return -1;


/* HTTP GET string. */
    strcpy(appData.SendBuff,GETINSTRUCT);
// 1) change Austin Texas to your city
// 2) you can change metric to imperial if you want temperature in F
    /* Send the HTTP GET string to the open TCP/IP socket. */
    sl_Send(appData.SockID, appData.SendBuff, strlen(appData.SendBuff), 0);


/* Receive response */
    sl_Recv(appData.SockID, &appData.Recvbuff[0], MAX_RECV_BUFF_SIZE, 0);
    appData.Recvbuff[strlen(appData.Recvbuff)] = '\0';

    //CLI_Write((_u8 *) appData.Recvbuff);

    int j = 0;
    int found = 0;
    int mydevice = 0;
    char strdevice[20] = "";
    char strparam1[20] = "";
    char strId[20] = "";
    int k = 0;
    while(j < strlen(appData.Recvbuff) && found == 0)  {
        if(appData.Recvbuff[j] == '*')  {
            if(appData.Recvbuff[j+1] == '*')  {
                k = 0;
                j = j+2;
                while(appData.Recvbuff[j] >= '0' && appData.Recvbuff[j] <= '9')  {
                    strdevice[k] = appData.Recvbuff[j];
                    k++;
                    j++;
                }
                    strdevice[k] = '\0';
                    found = 1;
            }
        }
        if(appData.Recvbuff[j] == '*')  {
            if(appData.Recvbuff[j+1] == 'P')  {
                k = 0;
                j = j+2;
                while(appData.Recvbuff[j] >= '0' && appData.Recvbuff[j] <= '9')  {
                    strparam1[k] = appData.Recvbuff[j];
                    k++;
                    j++;
                }
                strparam1[k] = '\0';
            }
        }
        if(appData.Recvbuff[j] == '*')  {
            if(appData.Recvbuff[j+1] == 'I')  {
                k = 0;
                j = j+2;
                while(appData.Recvbuff[j] >= '0' && appData.Recvbuff[j] <= '9')  {
                    strId[k] = appData.Recvbuff[j];
                    k++;
                    j++;
                }
                strId[k] = '\0';
            }
        }
        j++;
    }

    if(found == 1)  {
        instructions[numberOfInstructions].devicenum = atoi(strdevice);
        instructions[numberOfInstructions].param1 = atoi(strparam1);
        instructions[numberOfInstructions].idInstruction = atoi(strId);
        numberOfInstructions++;
    }  else {
        CLI_Write((_u8 *) "No Instructions Found");
    }

    performcleanup();

    sl_Close(appData.SockID);
  }

  return 0;
}



int32_t SendAck(int instructId)  {


      memcpy(appData.HostName,SERVER,strlen(SERVER));
      if(GetHostIP() == 0){
        if( (appData.SockID = CreateConnection()) < 0 ) return -1;



    /* HTTP GET string. */
        strcpy(appData.SendBuff, buildack(instructId));
    // 1) change Austin Texas to your city
    // 2) you can change metric to imperial if you want temperature in F
        /* Send the HTTP GET string to the open TCP/IP socket. */
        sl_Send(appData.SockID, appData.SendBuff, strlen(appData.SendBuff), 0);

        CLI_Write((_u8 *) appData.SendBuff);


    /* Receive response */
        sl_Recv(appData.SockID, &appData.Recvbuff[0], MAX_RECV_BUFF_SIZE, 0);
        appData.Recvbuff[strlen(appData.Recvbuff)] = '\0';




        CLI_Write((_u8 *) appData.Recvbuff);
        performcleanup();

        sl_Close(appData.SockID);
      }

      return 0;
}






int32_t GetInstruction()
{

    int numInstructions = 0;
    numInstructions = getNumberOfInstructions();

    int retrievedCount = 0;

    while(retrievedCount < numInstructions)  {
        getNextInstruction();
        retrievedCount++;
    }
}


int32_t ExecuteInstructions()  {
    int i = 0;

    for(i = 0; i < numberOfInstructions; i++)  {
        if(instructions[i].devicenum == 1)  {
            GPIO_PORTJ0 = 1;
            SysTick_Wait10ms(instructions[i].param1 * 100);
            GPIO_PORTJ0 = 0;
            instructions[i].executed = 1;
        }
    }



    for(i = 0; i < numberOfInstructions; i++)  {
        if(instructions[i].executed == 1)  {
            SendAck(instructions[i].idInstruction);
        }
    }

    return i;

}






/*!
    \brief This function obtains the server IP address

    \param[in]      none

    \return         zero for success and -1 for error

    \warning
*/
int32_t GetHostIP(void){
  int32_t status = -1;


/*
  status = sl_NetAppDnsGetHostByName(appData.HostName,
                                       strlen(appData.HostName),
                                       &appData.DestinationIP,
                                       SL_AF_INET);
*/


  return 0;
}



/*!
    \brief Create TCP connection with openweathermap.org

    \param[in]      none

    \return         Socket descriptor for success otherwise negative

    \warning
*/
int CreateConnection(void){
  SlSockAddrIn_t  Addr;

  int sd = 0;
  int AddrSize = 0;
  int ret_val = 0;

  Addr.sin_family = SL_AF_INET;
  Addr.sin_port = sl_Htons(80);

    /* Change the DestinationIP endianity, to big endian */
  //Addr.sin_addr.s_addr = sl_Htonl(appData.DestinationIP);

    unsigned long mydestination = 0xc0a80105;

  Addr.sin_addr.s_addr = sl_Htonl(mydestination);

  AddrSize = sizeof(SlSockAddrIn_t);

  sd = sl_Socket(SL_AF_INET,SL_SOCK_STREAM, 0);
  if( sd < 0 ){
   // LCD_OutString("Error creating socket\r\n");
      CLI_Write((_u8 *)" ERROR CREATING SOCKET");
      return sd;
  }

  ret_val = sl_Connect(sd, ( SlSockAddr_t *)&Addr, AddrSize);
  if( ret_val < 0 ){
        /* error */
    //LCD_OutString("Error connecting to socket\r\n");


      CLI_Write((_u8 *)" ERROR CONNECTING TO SOCKET");

      return ret_val;
  }

  return sd;
}
