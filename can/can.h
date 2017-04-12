/**  Copyright ©2017 Thogo Tech. All Rights Reserved.
* 
* @file: can.h
* @description:  
* @author: Thogo Team
* @version: 0.1.0
* @create_at: 2017/03/04
* @update_at: 2017/04/07
* 
*/

#ifndef __CAN_H
#define __CAN_H

#ifdef __cplusplus
extern "C"
{
#endif
	
#include "stm32f10x.h"


#define    canEXT_ID         0x00 
#define    canSTD_ID         0X600

/*
#define    Can_M1_StdId      0x601
#define    Can_M2_StdId      0x602
#define    Can_M3_StdId      0x603
#define    Can_HC_StdId      0x604
*/
	
typedef union
{
    uint8_t ucByte[4];
	float fData;
}Float_t;

	

// Initiate can module, this function must be called firstly, 
// whenever you want to send data using can bus
void can_Initiate(void);

// Send testing data 
void can_SendTestingData(uint8_t ucData);

// Send 32bit unsigned integer
void can_SendInteger(uint32_t ulData, uint8_t ucDataId, uint32_t ulCanStdId);

// Send float number
void can_SendFloat(float fData, uint8_t ucDataId, uint32_t ulCanStdId);

// Clear the number of can intterrupt occured
void can_ClearBusIntCount(void);

// Get the number of can intterrupt occured
uint32_t can_GetBusIntCount(void);

// Testing can bus, don't need to call can_Init() function firstly,
// just call this function directly
// when return 0, it indicates can bus is working fine
// wehn return -1, it indicates can bus is not working
int32_t can_Test(void);

#ifdef __cplusplus
}
#endif

#endif


