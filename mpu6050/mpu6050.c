/**  Copyright ©2017 Thogo Tech. All Rights Reserved.
* 
* @file: mpu6050.c
* @description: mpu6050 chip operation fucntion implementation 
* @author: Thogo Team
* @version: 0.0.1
* @create_at: 2017/04/08
* @update_at: 2017/04/10
*   
* 
*/
#include "mpu6050.h"
#include "i2c.h"
#include "stmsys.h"
#include <stdio.h>

u8 mpu6050_ok;

extern volatile uint8_t i2c_ucFastMode;


/**
*
* @desc: Write bit to device 
* @args: ucSlaveAddr, the slave device address
*        ucRegAddr, register address
*        ucBitNum, the bit position in byte
*        ucData, bit data
* @returns: uint8_t, 0(success), 1(failed)
*/
static uint8_t
WriteBit(uint8_t ucSlaveAddr, uint8_t ucRegAddr, uint8_t ucBitNum, uint8_t ucData)
{
    uint8_t  ucByte;
    mpu6050I2C_RECV_DATA(ucSlaveAddr, ucRegAddr, 1, &ucByte);
    ucByte = (ucData != 0) ? (ucByte | (1 << ucBitNum)) : (ucByte & ~(1 << ucBitNum));
    mpu6050_ok = (mpu6050I2C_SEND_BYTE(ucSlaveAddr, ucRegAddr, ucByte));
    return mpu6050_ok;
}


/**
*
* @desc: Write multi bits to the specific device
* @args;
*        ucSlaveAddr. the slave device address
*        ucRegAddr, register adress
*        ucBitStart, the starting position 
*        ucLen, the bit length
*        ucData, bits data
* @returns: None
*
*/
static void 
WriteBits(uint8_t ucSlaveAddr, uint8_t ucRegAddr,uint8_t ucBitStart, uint8_t ucLen, uint8_t ucData)
{
    
    uint8_t ucByte;
    uint8_t ucMask;
    mpu6050I2C_RECV_DATA(ucSlaveAddr, ucRegAddr, 1, &ucByte);
    ucMask = (0xFF << (ucBitStart + 1)) | 0xFF >> ((8 - ucBitStart) + ucLen - 1);
    ucData <<= (8 - ucLen);
    ucData >>= (7 - ucBitStart);
    ucByte &= ucMask;
    ucByte |= ucData;
    mpu6050I2C_SEND_BYTE(ucSlaveAddr, ucRegAddr, ucByte);
}


/**
*
* @desc: Set mpu6050 deivice sample rate division
* @args: ulFreq, the sample frequency 
* @returns: None
*/
void 
mpu6050_SetSampleRateDivision(uint32_t ulFreq)
{
    mpu6050I2C_SEND_BYTE(MPU6050_ADDR, MPU6050_RA_SMPLRT_DIV,1000/ulFreq - 1);
}


/**
* 
* @desc：　Set mpu6050 clock source
* @args: ulClkSource, the value can be the following options:
* 0       | Internal oscillator
* 1       | PLL with X Gyro reference
* 2       | PLL with Y Gyro reference
* 3       | PLL with Z Gyro reference
* 4       | PLL with external 32.768kHz reference
* 5       | PLL with external 19.2MHz reference
* 6       | Reserved
* 7       | Stops the clock and keeps the timing generator in reset
* @returns: None
*
*/
void 
mpu6050_SetClockSource(uint8_t ucClkSource)
{
    WriteBits(MPU6050_ADDR, MPU6050_RA_PWR_MGMT_1, \
        MPU6050_PWR1_CLKSEL_BIT, MPU6050_PWR1_CLKSEL_LENGTH, ucClkSource);
}


/**
* @desc: Set full-scale gyroscope range.
* @args: ucRange, the range New full-scale gyroscope range value
* @returns: None
* 
*/
void 
mpu6050_SetFullScaleGyroRange(uint8_t ucRange) 
{
    WriteBits(MPU6050_ADDR, MPU6050_RA_GYRO_CONFIG, \
        MPU6050_GCONFIG_FS_SEL_BIT, MPU6050_GCONFIG_FS_SEL_LENGTH, ucRange);
    WriteBits(MPU6050_ADDR, MPU6050_RA_GYRO_CONFIG, 7, 3, 0x00);   
}


/**
*
* @desc: Set full-scale accelerator range
* @args: ucRange,
* @returns: None
*
*/
void 
mpu6050_SetFullScaleAccelRange(uint8_t ucRange)
{
    WriteBits(MPU6050_ADDR, MPU6050_RA_ACCEL_CONFIG, \
        MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, ucRange);
    WriteBits(MPU6050_ADDR, MPU6050_RA_ACCEL_CONFIG, 7, 3, 0x00);
}


/**
*
* @desc： Enable or disable sleep mode 
* @args: ucEnabled, 1(enable), 0(disable)
* @returns: None
*
*/
void 
mpu6050_SetSleepEnabled(uint8_t ucEnabled)
{
    WriteBit(MPU6050_ADDR, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, ucEnabled);
}


/**
*
* @desc: Enable or disbale iic master mode
* @args: ucEnabled,
* @returns: None
*
*/
void 
mpu6050_SetI2CMasterModeEnabled(uint8_t ucEnabled)
{
    WriteBit(MPU6050_ADDR, MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_I2C_MST_EN_BIT, ucEnabled);
}


/**
*
* @desc: Enable iic bypass 
* @args: ucEnabled
* @returns: None
*
*/
void 
mpu6050_SetI2CBypassEnabled(uint8_t ucEnabled)
{
    WriteBit(MPU6050_ADDR, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_I2C_BYPASS_EN_BIT, ucEnabled);
}


/**
*
* @desc: Set mpu6050 default low pass filter
* @args: ucFilter, please refer to mpu6050.h
* @returns: None
*
*/
void 
mpu6050_SetDefaultLowPassFilter(uint8_t ucFilter)
{
    WriteBits(MPU6050_ADDR, MPU6050_RA_CONFIG, MPU6050_CFG_DLPF_CFG_BIT, \
        MPU6050_CFG_DLPF_CFG_LENGTH, ucFilter);
}

/**
*
* @desc: Config Intterupt I just guess
* @args: None
* @returns: None
*/
void
mpu6050_ConfigInt(void)
{


}


/**
*
* @desc: Initiate mpu6050 
* @args: ucDFilter, default filter
* @returns: None
*/
void
mpu6050_Initiate(uint8_t ucDFilter)
{ 
    uint8_t ucDefaultFilter = 1;
    mpu6050_ConfigInt();
    switch(ucDFilter)
    {
        case MPU6050_DLPF_BW_5:
        case MPU6050_DLPF_BW_10:
        case MPU6050_DLPF_BW_20:
        case MPU6050_DLPF_BW_42:
        case MPU6050_DLPF_BW_98:
        case MPU6050_DLPF_BW_188:
        case MPU6050_DLPF_BW_256:
            ucDefaultFilter = ucDFilter;
            break;
        default:
            ucDefaultFilter = MPU6050_DLPF_BW_42;
            break;
    }
    mpu6050I2C_INITIATE();
    // Enable normal working mode
    mpu6050_SetSleepEnabled(0);
    stmsys_DelayMs(10);
    // Set clock source
    mpu6050_SetClockSource(MPU6050_CLOCK_PLL_ZGYRO);
    stmsys_DelayMs(10);
    // Set sample rate division to 1kHz
    mpu6050_SetSampleRateDivision(1000); 
    stmsys_DelayMs(10);
    // Set gyroscope max range to +/-2000 degree per second
    mpu6050_SetFullScaleGyroRange(MPU6050_GYRO_FS_2000);
    stmsys_DelayMs(10);
    // Set gravity to +/-8g
    mpu6050_SetFullScaleAccelRange(MPU6050_ACCEL_FS_8);
    stmsys_DelayMs(10);
    mpu6050_SetDefaultLowPassFilter(ucDefaultFilter);
    stmsys_DelayMs(10);
    mpu6050_SetI2CMasterModeEnabled(0);
    stmsys_DelayMs(10);
    mpu6050_SetI2CBypassEnabled(1);
    stmsys_DelayMs(10);
}


/**
*
* @desc: read data from mpu6050
* @args: None
* @returns; None
*
*/
void 
mpu6050_ReadData(Mpu6050Data_t  * pxData)
{

    uint8_t ucRawDataBuffer[14];
    mpu6050I2C_RECV_DATA(MPU6050_ADDR, MPU6050_RA_ACCEL_XOUT_H, 14 , ucRawDataBuffer);
    i2c_ucFastMode=1;
    pxData->xAcclerator.sX = ((((int16_t)ucRawDataBuffer[0]) << 8) | ucRawDataBuffer[1]);
    pxData->xAcclerator.sY = ((((int16_t)ucRawDataBuffer[2]) << 8) | ucRawDataBuffer[3]);
    pxData->xAcclerator.sZ = ((((int16_t)ucRawDataBuffer[4]) << 8) | ucRawDataBuffer[5]);
    pxData->xGyroscope.sX = ((((int16_t)ucRawDataBuffer[8]) << 8) | ucRawDataBuffer[9]);
    pxData->xGyroscope.sY = ((((int16_t)ucRawDataBuffer[10]) << 8) | ucRawDataBuffer[11]);
    pxData->xGyroscope.sZ = ((((int16_t)ucRawDataBuffer[12]) << 8) | ucRawDataBuffer[13]);
}


/**
*
* @desc: Test mup6050, before call this function, mpu6050_Initiate must be called
* @args: None
* @returns: int32_t, 0(success), -1(failed)
*
*/
int32_t
mpu6050_Test(void)
{
    if(WriteBit(MPU6050_ADDR, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, 0x01))
    {
        return -1;
    }
    stmsys_DelayMs(10);
    if(WriteBit(MPU6050_ADDR, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, 0x00))
    {
        return -1;
    }
    return 0;
}

