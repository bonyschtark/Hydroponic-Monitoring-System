
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

int32_t SendTDS(int id, int val);
int32_t SendDecimal(char* id, double decimalVal);
int32_t GetInstruction();
int32_t SendAcknowledgment(int instructId);

int getCurrentDay();
int getCurrentMonth();
int getCurrentYear();
int getCurrentHour();
int getCurrentMinute();
int getCurrentSecond();




#define GPIO_PORTJ0             (*((volatile uint32_t *)0x40060004))
#define GPIO_PORTJ_DIR_R        (*((volatile uint32_t *)0x40060400))
#define GPIO_PORTJ_AFSEL_R      (*((volatile uint32_t *)0x40060420))
#define GPIO_PORTJ_PUR_R        (*((volatile uint32_t *)0x40060510))
#define GPIO_PORTJ_DEN_R        (*((volatile uint32_t *)0x4006051C))
#define GPIO_PORTJ_AMSEL_R      (*((volatile uint32_t *)0x40060528))
#define GPIO_PORTJ_PCTL_R       (*((volatile uint32_t *)0x4006052C))
#define SYSCTL_RCGCGPIO_R       (*((volatile uint32_t *)0x400FE608))
#define SYSCTL_RCGCGPIO_R8      0x00000100  // GPIO Port J Run Mode Clock
                                            // Gating Control
#define SYSCTL_PRGPIO_R         (*((volatile uint32_t *)0x400FEA08))
#define SYSCTL_PRGPIO_R8        0x00000100  // GPIO Port J Peripheral Ready






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


int numberOfInstructions;


struct instruct {
    int idInstruction;
    int devicenum;
    int param1;
    int param2;
    int param3;
    int param4;
    int executed;
};


struct instruct instructions[40];


#endif // __TM4C1294NCPDT_H__
