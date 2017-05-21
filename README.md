# stm32 mcu common using library 



## Usage

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


