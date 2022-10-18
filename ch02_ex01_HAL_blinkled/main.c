#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"


int main(void)
{
    cy_rslt_t result;

    result = cybsp_init() ;
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    result = cyhal_gpio_init(CYBSP_USER_LED, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, CYBSP_LED_STATE_OFF);
	if (result != CY_RSLT_SUCCESS)
	{
		CY_ASSERT(0);
	}

    __enable_irq();

    for (;;)
    {
    	cyhal_gpio_write(CYBSP_USER_LED, CYBSP_LED_STATE_ON);
    	cyhal_system_delay_ms(250);
    	cyhal_gpio_write(CYBSP_USER_LED, CYBSP_LED_STATE_OFF);
    	cyhal_system_delay_ms(250);
    }
}
