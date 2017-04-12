/**  Copyright ©2017 Thogo Tech. All Rights Reserved.
* 
* @file: i2c.h
* @description:  The API decleration of iic 
* @author: Thogo Team
* @version: 0.0.1
* @create_at: 2017/04/07
* @update_at: 2017/04/07
* 
*
*/

#ifndef  __I2C_H
#define  __I2C_H

#ifdef __cplusplus
extern "C"
{
#endif
    
#include "stm32f10x.h"

extern volatile uint8_t i2c_ucFastMode;    
    
void    i2c_SoftInitiate(void);
void    i2c_SoftWriteByte(uint8_t ucData);
uint8_t i2c_SoftReadByte(uint8_t ucAsk);
uint8_t i2c_SoftSendByte(uint8_t ucSlaveAddr, uint8_t ucRegAddr,uint8_t ucData);
uint8_t i2c_SoftRecvByte(uint8_t ucSlaveAddr, uint8_t ucRegAddr,uint8_t *pucData);
uint8_t i2c_SoftSendData(uint8_t ucSlaveAddr, uint8_t ucRegAddr, uint8_t ucLen, uint8_t * pucData);
uint8_t i2c_SoftRecvData(uint8_t ucSlaveAddr, uint8_t ucRegAddr, uint8_t ucLen, uint8_t * pucData);


#ifdef __cplusplus
}
#endif

#endif
