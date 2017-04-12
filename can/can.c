/**  Copyright ©2017 Thogo Tech. All Rights Reserved.
* 
* @file: can.c
* @description:  
* @author: Thogo Team
* @version: 0.1.0
* @create_at: 2017/03/04
* @update_at: 2017/04/12
*   
* 
*/

#include "can.h"
#include <stdio.h>

uint32_t can_BusInterruptCount=0;
    

/**
*
* @desc: Initiate can module of stm32
* @args： None
* @returns: None
*
*/
void 
can_Initiate(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    CAN_InitTypeDef CAN_InitStructure;
    CAN_FilterInitTypeDef CAN_FilterInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;    
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
    // Config Rx Pin
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    
    GPIO_Init(GPIOA, &GPIO_InitStructure);    
    // Config Tx Pin
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    // Reset can module
    CAN_DeInit(CAN1);
    CAN_StructInit(&CAN_InitStructure);
    CAN_InitStructure.CAN_TTCM = DISABLE;
    CAN_InitStructure.CAN_ABOM = DISABLE;
    CAN_InitStructure.CAN_AWUM = DISABLE;
    CAN_InitStructure.CAN_NART = DISABLE;
    CAN_InitStructure.CAN_RFLM = DISABLE;
    CAN_InitStructure.CAN_TXFP = DISABLE;
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
    // can baudrate = CAN_CLOCK/((*.CAM_BS1 + *.CAN_BS2 + 1)*can_prescaler)
    // = 1M
    CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
    CAN_InitStructure.CAN_BS1 = CAN_BS1_2tq;
    CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq;
    CAN_InitStructure.CAN_Prescaler = 6;
    CAN_Init(CAN1, &CAN_InitStructure);
    // Enable CAM FIFO0 Message Interrupt
    CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);

    CAN_FilterInitStructure.CAN_FilterNumber=0;
    // Config filter mode to identification mask mode
    CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;
    // Using 32bits Filter
    CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;
    // Set Filter Id to zero
    CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;
    CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
    // Set Filter Mask Id to zero
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000; 
    CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
    // Connected Filter to FIFO0
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;
    CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
    CAN_FilterInit(&CAN_FilterInitStructure);

    // Config RX Interrupt
    NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


/**
*
*@Desc: send testing data
*@Args: ucData, testing data
*@Returns: None
*
*/
void 
can_SendTestingData(uint8_t ucData )
{
    uint32_t i = 0;
    CanTxMsg TxMessage;
    TxMessage.StdId = canSTD_ID;
    TxMessage.ExtId = canEXT_ID;
    TxMessage.RTR = CAN_RTR_DATA;
    TxMessage.IDE = CAN_ID_STD;
    TxMessage.DLC = 5;
    for(i = 0; i < 8; i++)
    {
        TxMessage.Data[i] = ucData;
    }
    // Send data
    CAN_Transmit(CAN1, &TxMessage);
}    

/**
*
*@Desc: Send integer number 
*@Args: ulData, integer number
*       ucDataId, data id
*       ucCanStdId, can standard id
*@Returns: None
*
*/
void 
can_SendInteger(uint32_t ulData, uint8_t ucDataId, uint32_t ulCanStdId)
{
    uint32_t i = 0;
    CanTxMsg xTxMessage;
    uint32_t ulTempData = ulData;
    
    xTxMessage.StdId = ulCanStdId;
    xTxMessage.ExtId = canEXT_ID;
    xTxMessage.RTR = CAN_RTR_DATA;
    xTxMessage.IDE = CAN_ID_STD;
    xTxMessage.DLC = 6;
    xTxMessage.Data[0] = 0xa4;
    xTxMessage.Data[1] = ucDataId;
    for(i = 2; i < xTxMessage.DLC; i++)
    {
        xTxMessage.Data[i+2] = ((uint8_t*)&ulTempData)[i-2];
    }
    CAN_Transmit(CAN1, &xTxMessage);
}


/**
*
*@Desc: Send Float number
*@Args: 
*      fData, float number
*      ucDataId, data related id
*      ulCanStdId, can standard id
*@Returns: None
*/
void
can_SendFloat(float fData, uint8_t ucDataId, uint32_t ulCanStdId)
{
    uint32_t i = 0;
    CanTxMsg xTxMessage;
    Float_t xDataContainer;
    xDataContainer.fData = fData;
    
    xTxMessage.StdId = ulCanStdId;
    xTxMessage.ExtId = canEXT_ID;
    // Send data frame
    xTxMessage.RTR = CAN_RTR_DATA;
    // Use standard id
    xTxMessage.IDE = CAN_ID_STD;
    // Set data length
    xTxMessage.DLC = 6;
    // Set data header to 0xa4
    xTxMessage.Data[0] = 0xa4;
    // Set data id
    xTxMessage.Data[1] = ucDataId;
    // Fill data can message struct
    for(i = 2 ; i < xTxMessage.DLC; i++)
    {
        xTxMessage.Data[i] = xDataContainer.ucByte[i-2];
    }
    // Send data
    CAN_Transmit(CAN1, &xTxMessage);
}
    

/**
*
*@Desc: can RX ISR
*@Args: None
*@Returns: None
*
*/
void USB_LP_CAN1_RX0_IRQHandler(void)
{
    
    CanRxMsg xRxMessage;
    can_BusInterruptCount++;    
    if(CAN_GetITStatus(CAN1,CAN_IT_FMP0) != RESET)
    {
        CAN_ClearITPendingBit(CAN1, CAN_IT_FF0);
        CAN_ClearFlag(CAN1, CAN_FLAG_FF0); 
        CAN_Receive(CAN1, CAN_FIFO0, &xRxMessage);
    }
}        


/**
*
*@Desc: get can bus interrupt count
*@Args: None
*@Returns: uint32_t, interrupt count
*
*/
uint32_t 
can_GetBusIntCount(void)
{
    return can_BusInterruptCount;
}


/**
*
*@Desc: clear can bus interrupt counter
*@Args: None
*@Returns: None
*
*/
void
can_ClearButIntCount(void)
{
    can_BusInterruptCount=0;
}


/**
*
*@Desc: Test can bus in loop mode using polling 
*@Args: None
*@Returns: 0(success), -1(failed)
*
*/
int32_t
can_Test(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    CAN_InitTypeDef CAN_InitStructure;
    CAN_FilterInitTypeDef CAN_FilterInitStructure;
    CanTxMsg xTxMessage;
    CanRxMsg xRxMessage;
    uint32_t i;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
    
    // Configure CAN pin: RX 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    // Configure CAN pin: TX 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // CAN bus configuration
    // CAN register init 
    CAN_DeInit(CAN1);
    CAN_StructInit(&CAN_InitStructure);
    CAN_InitStructure.CAN_TTCM = DISABLE; 
    CAN_InitStructure.CAN_ABOM = DISABLE; 
    CAN_InitStructure.CAN_AWUM = DISABLE; 
    CAN_InitStructure.CAN_NART = DISABLE; 
    CAN_InitStructure.CAN_RFLM = DISABLE; 
    CAN_InitStructure.CAN_TXFP = DISABLE; 
    // Loopback mode
    CAN_InitStructure.CAN_Mode = CAN_Mode_LoopBack; 
    CAN_InitStructure.CAN_SJW = CAN_SJW_1tq; 
    CAN_InitStructure.CAN_BS1 = CAN_BS1_2tq; 
    CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq; 
    CAN_InitStructure.CAN_Prescaler = 5; 
    CAN_Init(CAN1, &CAN_InitStructure);
    // CAN filter configuration
    CAN_FilterInitStructure.CAN_FilterNumber = 0; 
    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
    CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000; 
    CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000; 
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0; 
    // Enable filter
    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
    CAN_FilterInit(&CAN_FilterInitStructure);
    
    // Send Testing data
    xTxMessage.StdId = 0x11; 
    // TxMessage.ExtId 
    xTxMessage.IDE = CAN_ID_STD;
    xTxMessage.RTR = CAN_RTR_DATA; 
    xTxMessage.DLC = 2; 
    xTxMessage.Data[0] = 0xCA; 
    xTxMessage.Data[1] = 0xFE; 
    CAN_Transmit(CAN1, &xTxMessage);
    i = 0;
    // Waiting data being send
    while((CAN_TransmitStatus(CAN1, 0) != CANTXOK) && (i != 0xFF))
    {
        i++;
    }
    i=0;
    // Waiting data being received
    while((CAN_MessagePending(CAN1, CAN_FIFO0) < 1) && (i != 0xFF))
    {
        i++;
    }
    // Receive testing data
    xRxMessage.StdId = 0x00; 
    xRxMessage.IDE = CAN_ID_STD; 
    xRxMessage.DLC = 0; 
    xRxMessage.Data[0]=0x00;
    xRxMessage.Data[1]=0x00;
    CAN_Receive(CAN1, CAN_FIFO0, &xRxMessage);
    // Id error
    if (xRxMessage.StdId != 0x11)
    {
        return -1;
    }
    // Id mode error
    if (xRxMessage.IDE != CAN_ID_STD)
    {
        return -1;
    }
    // Data length error
    if (xRxMessage.DLC != 2)
    {
        return -1;
    }
    // Data error
    if ((xRxMessage.Data[0]<<8|xRxMessage.Data[1])!= 0xCAFE)
    {
        return -1;
    }
    // Testing passed
    return 0; 
}

