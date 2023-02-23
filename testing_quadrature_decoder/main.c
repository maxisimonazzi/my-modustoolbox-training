#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"

int main(void)
{
    cy_rslt_t result;
    cy_rslt_t rslt;

   cyhal_quaddec_t quaddec_obj;
   uint32_t count = 0x8000, prev_count = 0x8000;

    // Initialize the quadrature decoder object. Does not use index ('pin' is NC) and does not use a
    // pre-configured clock source ('clk' is NULL).
    rslt = cyhal_quaddec_init(&quaddec_obj, P8_0, P8_1, NC, CYHAL_QUADDEC_RESOLUTION_4X, NULL, 1000000);
    CY_ASSERT(CY_RSLT_SUCCESS == rslt);

    // Start the quadrature decoder
    rslt = cyhal_quaddec_start(&quaddec_obj);
    CY_ASSERT(CY_RSLT_SUCCESS == rslt);



    /* Initialize the device and board peripherals */
    result = cybsp_init() ;
    /*BSP init failed. Stop program execution */
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* Initialize the User LED */
    result = cyhal_gpio_init(CYBSP_USER_LED, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, CYBSP_LED_STATE_OFF);
	/* GPIO init failed. Stop program execution */
	if (result != CY_RSLT_SUCCESS)
	{
		CY_ASSERT(0);
	}

	/* Initialize the user button */
	result = cyhal_gpio_init(P10_4, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_PULLUP, 1);
	/* GPIO init failed. Stop program execution */
	if (result != CY_RSLT_SUCCESS)
	{
		CY_ASSERT(0);
	}
    __enable_irq();

    for (;;)
     {
     	//Button is active low
     	if(!cyhal_gpio_read(P10_4)){
     		cyhal_gpio_write(CYBSP_USER_LED, CYBSP_LED_STATE_ON);

     	}
     	else{
     		cyhal_gpio_write(CYBSP_USER_LED, CYBSP_LED_STATE_OFF);
     	}






        count = cyhal_quaddec_read_counter(&quaddec_obj);

        // Check for direction of change
        if (count > prev_count)
        {
        	cyhal_gpio_write(CYBSP_USER_LED, CYBSP_LED_STATE_ON);
     		cyhal_system_delay_ms(100);

        }
        else if (count < prev_count)
        {
        	cyhal_gpio_write(CYBSP_USER_LED, CYBSP_LED_STATE_ON);
        	cyhal_system_delay_ms(1000);
        	cyhal_gpio_write(CYBSP_USER_LED, CYBSP_LED_STATE_OFF);
        }
        else
        {
            // No change
        }

        prev_count = count;





    }
}
