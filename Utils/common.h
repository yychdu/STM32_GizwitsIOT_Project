/**
************************************************************
* @file         common.h
* @brief        Generic tools
* @author       Gizwits
* @date         2017-07-19
* @version      V03030000
* @copyright    Gizwits
*
* @note         Gizwits is only for smart hardware
*               Gizwits Smart Cloud for Smart Products
*               Links | Value Added | Open | Neutral | Safety | Own | Free | Ecology
*               www.gizwits.com
*
***********************************************************/
#ifndef _COMMON_H_
#define _COMMON_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#ifndef ICACHE_FLASH_ATTR
#define ICACHE_FLASH_ATTR
#endif

typedef void (*gizTimerFunc_t)(void *arg );

/**
* @name Log print macro definition
* @{
*/
#define GIZWITS_LOG printf                          ///<Run log print
#define PROTOCOL_DEBUG                              ///<Protocol data print

#ifndef GIZWITS_LOG_NOFORMAT
#define GIZWITS_LOG_NOFORMAT printf
#endif

/**@} */


#define gizTimer_t multiTimer_t

#define FLASH_BYTEALIGN(a) ((a%4 == 0) ? a : (a/4 + 1)*4)

#define gizMemcmp memcmp
#define gizMemcpy memcpy
#define gizMemset memset
#define gizMalloc malloc
#define gizFree   free
#define gizStrlen strlen
#define gizStrncasecmp strncasecmp

//ntp time
#define ONE_SECOND (1)
#define ONE_MINUTE (60 * ONE_SECOND)
#define ONE_HOUR (60 * ONE_MINUTE)

#define DAY_SEC (24*60*60)
#define TIMEZONE_DEFAULT 8

//Off Chip
#define FLASH_SECTOR_SIZE 4096

//gGateway Param Totol Size 64KB
#define PARAM_SECTOR_START 257
#define PARAM_SECTOR_END 273
/** Error type enumeration */
typedef enum
{
    ERR_SUCCESS                 = 0,        /// <successful execution
                                            
    // Memory                               
    ERR_MEMORY                  = -0x0101,  /// <Request memory failed
    ERR_SPACE                   = -0x0102,  /// <lack of buffer space
    ERR_ILLEGAL_ADDR            = -0x0103,  /// <illegal address access,
                                            
    // parameter                            
    ERR_PARAM_ILLEGAL           = -0x0201,  /// <parameter is illegal
    ERR_PARAM_ADDR_NULL         = -0x0202,  /// <parameter address is empty
                                            
    //FLASH                                 
    ERR_FLASH_READ              = -0x0301,  /// <FLASH read error
    ERR_FLASH_WRITE             = -0x0302,  /// <FLASH write error
    ERR_FLASH_ADDR              = -0x0303,  /// <FLASH address error
    ERR_FLASH_SAVE              = -0x0304,  /// <FLASH save error
                                            
    //Mathematical calculation              
    ERR_MATH_DIV                = -0x0401,  /// <Mathematical calculation divisor is illegal
    ERR_MATH_RANGE              = -0x0402,  /// <Mathematical calculation of the range of illegal
    ERR_MATH_OVERSTEP           = -0x0403,  /// <Mathematical calculation across the border
                                            
    //Data search                           
    ERR_SEARCH_NO_ELEMENT       = -0x0501,  /// <search element failed
                                            
    //Non common error type                 
    ERR_ID_ILLEGAL              = -0x0601,  /// <ID is illegal
} errorCodeType_t;

#pragma pack(1)
typedef struct
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint32_t ntp;
} gizTime_t;
#pragma pack()

uint8_t gizProtocolSum(uint8_t *buf, uint32_t len);
uint16_t exchangeBytes(uint16_t value);
uint32_t exchangeWord(uint32_t	value);
int32_t returnWeekDay(uint32_t iYear, uint32_t iMonth, uint32_t iDay);
gizTime_t returnNtp2Wt(uint32_t ntp);

void getOnePasscode(uint8_t *data, uint32_t len);
void str2Hex(char *pbDest, char *pbSrc, int32_t nLen);
void hex2Str(unsigned char *pbDest, unsigned char *pbSrc, int32_t nLen);
uint8_t char2hex(char A , char B);
int8_t checkStringIsNotZero(const char *data);
int8_t checkStringIsZero(const char *data);


#ifdef __cplusplus
}
#endif

#endif
