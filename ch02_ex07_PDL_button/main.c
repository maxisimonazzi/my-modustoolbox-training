#include "cy_pdl.h"
#include "cybsp.h"

int main(void)
{
    cy_rslt_t result;

    /* Initialize the device and board peripherals */
    result = cybsp_init() ;
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* Enable global interrupts */
    __enable_irq();

	// Var to record the buttons state
	// 0 is pressed
	// 1 is not pressed
    bool buttonState = 0;

    for (;;)
    {
    	// Read the state of the button
    	buttonState = Cy_GPIO_Read(CYBSP_USER_BTN_PORT, CYBSP_USER_BTN_NUM);
    	if(buttonState == 1U){
    		// Turn off LED
    		Cy_GPIO_Set(CYBSP_USER_LED_PORT, CYBSP_USER_LED_NUM);
    	}
    	else{
    		// Turn on LED
    		Cy_GPIO_Clr(CYBSP_USER_LED_PORT, CYBSP_USER_LED_NUM);
    	}
    }
}
