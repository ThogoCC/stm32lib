/**  Copyright ©2017 Thogo Tech. All Rights Reserved.
* 
* @file: i2c.c
* @description:  The soft and hard implementation of iic protocol
* @author: Thogo Team
* @version: 0.0.1
* @create_at: 2017/04/07
* @update_at: 2017/04/07
* 
*
*/

#include "i2c.h"


// soft  iic macro
#define SCL_H         GPIO_SetBits(GPIOB , GPIO_Pin_6) 
#define SCL_L         GPIO_ResetBits(GPIOB , GPIO_Pin_6)
#define SDA_H         GPIO_SetBits(GPIOB , GPIO_Pin_7)
#define SDA_L         GPIO_ResetBits(GPIOB , GPIO_Pin_7) 
#define SCL_read      GPIO_ReadInputDataBit(GPIOB , GPIO_Pin_6)
#define SDA_read      GPIO_ReadInputDataBit(GPIOB , GPIO_Pin_7)

volatile uint8_t i2c_ucFastMode;

/**
*
* @brief: iic soft delay function
* @args: None
* @returns: None
*
*/
static void 
SoftDelay(void)
{ 
    uint8_t i = 0;
    for(i = 0; i < 12; i++)
    {
        __nop();
    }
    /*
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    */
    if(!i2c_ucFastMode)
    {
        i = 240;
        while(i--);
    }
}


/**
*
* @brief: The initiation of soft iic
* @args: None
* @returns: None
*
*/ 
void
i2c_SoftInitiate(void)
{
    static uint8_t ucInitiationLock = 0;
    // iic has already being initiated, just return
    if(ucInitiationLock == 1)
    {
        return;
    }
    GPIO_InitTypeDef  GPIO_InitStructure; 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE );
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    ucInitiationLock = 1;
}


/**
*
* @brief: Send soft iic starting signal 
* @args: None
* @returns: 0(failed),1(success)
*
*/
static uint8_t 
SoftStart(void)
{
    SDA_H;
    SCL_H;
    SoftDelay();
    // When SDA's level is low, it indicates  that the bus is busy, then return 
    if(!SDA_read)
    {
        return 0;
    }
    SDA_L;
    SoftDelay();
    // When SDA's level is high,
    // it indicates that some errors occured on bus, then return
    if(SDA_read)
    {
        return 0;    
    }
    SDA_L;
    SoftDelay();
    return 1;    
}


/**
*
* @brief: Send soft iic stop signal
* @args: None
* @returns: None
*
*/
static void 
SoftStop(void)
{
    SCL_L;
    SoftDelay();
    SDA_L;
    SoftDelay();
    SCL_H;
    SoftDelay();
    SDA_H;
    SoftDelay();
}


/**
*
* @brief: Actually, I don't know the purpose of this shit code.
*   I just have to say ......... fuck it. 
*   finnaly, the purpose of this shit code is to send ask siganl  
* @args: None
* @returns: None
*
*/
static void 
SoftAsk(void)
{
    SCL_L;
    SoftDelay();
    SDA_L;
    SoftDelay();
    SCL_H;
    SoftDelay();
    SCL_L;
    SoftDelay();
}


/**
*
* @brief: shit shit shit , there are all shit in this code area.
* Send noask signal
* @args: None
* @returns: None
*
*/
static void 
SoftNoAsk(void)
{
    SCL_L;
    SoftDelay();
    SDA_H;
    SoftDelay();
    SCL_H;
    SoftDelay();
    SCL_L;
    SoftDelay();
}


/**
*
* @brief: Wait asking signal
* @args: None
* @returns: 0(sucess), 1(failed)
*
*/
static uint8_t 
SoftWaitAsk(void)     
{
    uint8_t ErrTime = 0;
    SCL_L;
    SoftDelay();
    SDA_H;            
    SoftDelay();
    SCL_H;
    SoftDelay();
    while(SDA_read)
    {
        ErrTime++;
        if(ErrTime > 50)
        {
            SoftStop();
            return 1;
        }
    }
    SCL_L;
    SoftDelay();
    return 0;
}


/**
*
* @brief: Write one byte to iic bus  using soft iic
* @args: ucData, one byte data
* @returns: None
*
*/
void
i2c_SoftWriteByte(uint8_t ucData) 
{
    u8 i=8;
    while(i--)
    {
        SCL_L;
        SoftDelay();
        if(ucData & 0x80)
        {
            SDA_H;  
        }
        else 
        {
            SDA_L;
        }          
        ucData <<= 1;
        SoftDelay();
        SCL_H;
        SoftDelay();
    }
    SCL_L;
}  


/**
*
* @brief: Read one data byte from HSB to LSB, 
*    if ucAsk is equal to 1 ,then sending ask signal,
*    otherwise sending noask signal
* @args: ucAsk,
* @returns: byte
*
*/
uint8_t 
i2c_SoftReadByte(uint8_t ucAsk) 
{ 
    uint32_t i = 8;
    uint8_t ReceiveByte = 0;
    SDA_H;                
    while(i--)
    {
      ReceiveByte <<= 1;      
      SCL_L;
      SoftDelay();
      SCL_H;
      SoftDelay();    
      if(SDA_read)
      {
          ReceiveByte |= 0x01;
      }
    }
    SCL_L;
    if (ucAsk)
    {
        SoftAsk();
    }
    else
    {
        SoftNoAsk();  
    }
    return ReceiveByte;
} 


/**
*
* @brief: Send one byte data
* @args: ucSlaveAddr, slave device address
*        ucRegAddr, register adddress
*        ucData, data byte
* @returns: 0(success), 1(failed)
*
*/
uint8_t
i2c_SoftSendByte(uint8_t ucSlaveAddr, uint8_t ucRegAddr,uint8_t ucData)
{
    SoftStart();
    i2c_SoftWriteByte(ucSlaveAddr << 1);   
    if(SoftWaitAsk())
    {
        SoftStop();
        return 1;
    }
    i2c_SoftWriteByte(ucRegAddr);       
    SoftWaitAsk();    
    i2c_SoftWriteByte(ucData);
    SoftWaitAsk();   
    SoftStop(); 
    return 0;
}


/**
*
* @brief: Receive one byte using soft iic
* @args: ucSalveAddr, salve device address
*        ucRegAddr, register address
*        pucData, the pointer  of byte
* @returns: 0(success), 1(failed)
*
*/
uint8_t 
i2c_SoftRecvByte(uint8_t ucSlaveAddr, uint8_t ucRegAddr,uint8_t *pucData)
{              
    SoftStart();
    i2c_SoftWriteByte(ucSlaveAddr << 1); 
    if(SoftWaitAsk())
    {
        SoftStop();
        return 1;
    }
    i2c_SoftWriteByte(ucRegAddr);     
    SoftWaitAsk();
    SoftStart();
    i2c_SoftWriteByte( ucSlaveAddr << 1 | 0x01);
    SoftWaitAsk();
    *pucData = i2c_SoftReadByte(0);
    SoftStop();
    return 0;
}    


/**
*
* @brief: Send data using soft iic
* @args: ucSalveAddr, salve device address
*        ucRegAddr, register address
*        ucLen, data length
*        pucData, the pointer  of data
* @returns: 0(success), 1(failed)
*
*/
uint8_t 
i2c_SoftSendData(uint8_t ucSlaveAddr, uint8_t ucRegAddr, uint8_t ucLen, uint8_t * pucData)
{    
    SoftStart();
    i2c_SoftWriteByte(ucSlaveAddr << 1); 
    if(SoftWaitAsk())
    {
        SoftStop();
        return 1;
    }
    i2c_SoftWriteByte(ucRegAddr); 
    SoftWaitAsk();
    while(ucLen--) 
    {
        i2c_SoftWriteByte(*pucData++); 
        SoftWaitAsk();
    }
    SoftStop();
    return 0;
}


/**
*
* @brief: Receive data using soft iic
* @args: ucSalveAddr, salve device address
*        ucRegAddr, register address
*        ucLen, data length
*        pucData, the pointer  of byte
* @returns: 0(success), 1(failed)
*
*/
uint8_t 
i2c_SoftRecvData(uint8_t ucSlaveAddr, uint8_t ucRegAddr, uint8_t ucLen, uint8_t * pucData)
{    
    SoftStart();
    i2c_SoftWriteByte(ucSlaveAddr << 1); 
    if(SoftWaitAsk())
    {
        SoftStop();
        return 1;
    }
    i2c_SoftWriteByte(ucRegAddr); 
    SoftWaitAsk();
    SoftStart();
    i2c_SoftWriteByte(ucSlaveAddr << 1 | 0x01); 
    SoftWaitAsk();
    while(ucLen) 
    {
        if(ucLen == 1)
        {
            *pucData = i2c_SoftReadByte(0);
        }
        else
        {
            *pucData = i2c_SoftReadByte(1);
        }
        pucData++;
        ucLen--;
    }
    SoftStop();
    return 0;
}
