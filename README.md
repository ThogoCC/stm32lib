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


