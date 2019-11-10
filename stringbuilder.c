#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "simplelink.h"
#include "sl_common.h"




//#define REQUEST "GET /query?city=Austin%20Texas&id=Jonathan%20Valvano&greet=Hello%20from%20Austin,%20Jon%20and%20Ramesh&edxcode=8086  HTTP/1.1\r\nUser-Agent: Keil\r\nHost: embedded-systems-server.appspot.com\r\n\r\n"
//#define SERVER "localhost/greengarden/green.php"
//#define REQUEST "GET /greengarden/green.php?id=100&val=test1 HTTP/1.0\r\n\r\n"
//Content-Type: application/x-www-form-urlencoded


#define SENDTDS "POST /greengarden/green.php HTTP/1.1\r\nHost: 192.168.1.8\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: "

#define SENDINSTACK "GET /greengarden/sendinstructionack.php?id="

#define SENDINSTACK2 " HTTP/1.0\r\n\r\n"



int contentLength = 0;
#define SENDTDS2 "\r\n\r\nid="
#define SENDTDS3 "&val="
#define SENDTDS4 "\r\n\r\n"

char buildstr[512] = "";



char strcontentlength[5] = "";

char string[50] = "text ";
char buffer[50];

char mystring[50] = "";
char mystring2[50] = "";







char* buildTDS(int id, int val)  {


    sprintf(mystring, "%d", id);
    sprintf(mystring2, "%d", val);

    CLI_Write((_u8 *) "\r\nmystring: ");
    CLI_Write((_u8 *) mystring);
    CLI_Write((_u8 *) "\r\nmystring2: ");
    CLI_Write((_u8 *) mystring2);



    contentLength = 3 + strlen(mystring) + 5 +strlen(mystring2);

    sprintf(strcontentlength, "%d", contentLength);
    CLI_Write((_u8 *) strcontentlength);


    strcpy(buildstr, SENDTDS);

    strcat(buildstr, strcontentlength);
    strcat(buildstr, SENDTDS2);

    strcat(buildstr, mystring);
    strcat(buildstr, SENDTDS3);

    strcat(buildstr, mystring2);
    strcat(buildstr, SENDTDS4);
    return buildstr;
}


char* buildDecimal(char* id, double decimalVal)  {


    //sprintf(mystring, "%d", id);
    sprintf(mystring2, "%.6f", decimalVal);


    strcpy(mystring, id);



    contentLength = 3 + strlen(id) + 5 + strlen(mystring2);

    sprintf(strcontentlength, "%d", contentLength);


    strcpy(buildstr, SENDTDS);

    strcat(buildstr, strcontentlength);
    strcat(buildstr, SENDTDS2);

    strcat(buildstr, mystring);
    strcat(buildstr, SENDTDS3);

    strcat(buildstr, mystring2);
    strcat(buildstr, SENDTDS4);

    return buildstr;
}



char* buildack(int id)  {

    sprintf(mystring, "%d", id);

    contentLength = 3 + strlen(mystring);

    sprintf(strcontentlength, "%d", contentLength);


    strcpy(buildstr, SENDINSTACK);

    strcat(buildstr, mystring);
    strcat(buildstr, SENDINSTACK2);

    return buildstr;
}
