
#ifndef __CONNECT_H
#define __CONNECT_H




int32_t GetHostIP(void);
int32_t Lab16(void);
int32_t updateLocalTime(void);
int CreateConnection(void);
_i32 configureSimpleLinkToDefaultState();
_i32 establishConnectionWithAP();
_i32 checkLanConnection();
_i32 checkInternetConnection();
_i32 initializeAppVariables();
void displayBanner();
int getCurrentDay();
int getCurrentMonth();
int getCurrentYear();
int getCurrentHour();
int getCurrentMinute();
int getCurrentSecond();




/////* Application specific status/error codes */
typedef enum{
    DEVICE_NOT_IN_STATION_MODE = -0x7D0,        /* Choosing this number to avoid overlap with host-driver's error codes */
    INVALID_HEX_STRING = DEVICE_NOT_IN_STATION_MODE - 1,
    TCP_RECV_ERROR = INVALID_HEX_STRING - 1,
    TCP_SEND_ERROR = TCP_RECV_ERROR - 1,
    FILE_NOT_FOUND_ERROR = TCP_SEND_ERROR - 1,
    INVALID_SERVER_RESPONSE = FILE_NOT_FOUND_ERROR - 1,
    FORMAT_NOT_SUPPORTED = INVALID_SERVER_RESPONSE - 1,
    FILE_WRITE_ERROR = FORMAT_NOT_SUPPORTED - 1,
    INVALID_FILE = FILE_WRITE_ERROR - 1,


        LAN_CONNECTION_FAILED = -0x7D0,        /* Choosing this number to avoid overlap with host-driver's error codes */
        INTERNET_CONNECTION_FAILED = LAN_CONNECTION_FAILED - 1,

        STATUS_CODE_MAX = -0xBB8

}e_AppStatusCodes;





/*
 * GLOBAL VARIABLES -- Start
 */


//_i32 g_SockID = 0;

/*
 * GLOBAL VARIABLES -- End
 */





/*
 * GLOBAL VARIABLES -- Start
 */

/*
 * GLOBAL VARIABLES -- End
 */


/*
 * STATIC FUNCTION DEFINITIONS -- Start
 */


#endif // __TM4C1294NCPDT_H__


/*
 * STATIC FUNCTION DEFINITIONS -- End
 */



/*
 * STATIC FUNCTION DEFINITIONS -- End
 */
