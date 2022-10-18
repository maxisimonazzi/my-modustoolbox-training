#include "cy_pdl.h"
#include "cybsp.h"


int main(void)
{
    cy_rslt_t result;

    // UART context variable
	cy_stc_scb_uart_context_t UART_context;

    /* Initialize the device and board peripherals */
    result = cybsp_init() ;
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* Enable global interrupts */
    __enable_irq();

    /* Configure and enable the UART peripheral */
	Cy_SCB_UART_Init(SCB5, &scb_5_config, &UART_context);
	Cy_SCB_UART_Enable(SCB5);

    for (;;)
    {
    	Cy_GPIO_Set(CYBSP_USER_LED_PORT, CYBSP_USER_LED_NUM);
    	Cy_SCB_UART_PutString(SCB5, "LED OFF\n\r");
    	Cy_SysLib_Delay(250U);
    	Cy_GPIO_Clr(CYBSP_USER_LED_PORT, CYBSP_USER_LED_NUM);
    	Cy_SCB_UART_PutString(SCB5, "LED ON\n\r");
		Cy_SysLib_Delay(250U);
    }
}
