# stm32 mcu common using library 



## Dependencies

- stm32 standard library
- ....


## Usage


### usart usage

```c

#include "usart.h"


/**
*
* @brief: handle usart rx , this function will be called when usart's rx interrupt occured
* @args: ucByte, the data byte that usart received
* @returns: None
*
*/
void
_HandleUsartRX(uint8_t ucByte)
{
    // fill here with you logical code
}


int
main(void)
{
    // The detail parameters signature, please refer to usart.h
    usart_Initiate(USART1,  115200, 1);
    // register usart rx interrupt callback
    usart_RegisterRXCallback(USART1, _HandleUsartRX)
    while(1)
    {
        usart_Printf(USART1, "hello world %s", "stm32");
    }
    return 0;
}

```

---------------------------


### key usage

```c

#include "key.h"


void
_HandleKeyEvent(KeyEvent_t * pxEvent)
{
    switch(pxEvent->ulId)
    {
        case 0:
            // handle key 0 event
            break;
        case 1:
            // handle key 1 event
            break;
        case 2:
            // handle key 2 event
            break;
    }
}



int 
main(void)
{

    // GPIOA.8 ===> id = 0, key 0
    // GPIOA.9 ===> id = 1, key 1
    // GPIOA.10 ===> id = 2, key 2
    key_Initiate(GPIOA, 0x01 << 10 | 0x01 << 9 | 0x01 << 8);
    // register event callback function ,this callback function be called
    //  when key event occured
    key_RegisterEventCallback(_HandleKeyEvent);
    while(1)
    {

    }
    return 0;
}


```

-----------


### rtc usage

```c

#include "rtc.h"

DateTime_t dt;

// initiate the begining datetime
dt.ulSecond = 40;
dt.ucMin = 41;
dt.ucHour = 15;
dt.ucDay = 4;
dt.ucMonth = 6;
dt.ulYear = 2017;

// initiate stm32 RTC 
// note that this function must be called firstly, whenever \
//   you want to stm32's RTC hardware 
rtc_Initiate(&dt);

// get current datetime
rtc_GetDateTime(&dt);

// set current datetime;
dt.ucHour = 19;
rtc_SetDateTime(&dt);

// set RTC alarm datetime
dt.ucHourt = 20;
rtc_SetAlarmDateTime(&dt);

// this function will called when RTC's alarm interrupt occurrs
static void 
_HandleRTCAlarm(DateTime_t * pxDT)
{
   // do your logic code here
}

// when you write your RTC alarm ISR callback, you need to \ 
//  register it by calling following function
rtc_RegisterAlarmCallback(_HandleRTCAlarm);


// this helper function can convert your datetime to seconds
rtc_DT2Seconds(&dt);


```
