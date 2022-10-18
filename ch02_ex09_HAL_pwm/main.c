#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"

int main(void)
{
    cy_rslt_t result;
    cyhal_pwm_t pwm_obj;

    /* Initialize the device and board peripherals */
    result = cybsp_init() ;
    if (result != CY_RSLT_SUCCESS){
        CY_ASSERT(0);
    }

    __enable_irq();

	/* Initialize PWM on the supplied pin and assign a new clock */
    result = cyhal_pwm_init(&pwm_obj, CYBSP_USER_LED, NULL);
    if (result != CY_RSLT_SUCCESS){
        CY_ASSERT(0);
    }

	/* Start the PWM output */
	result = cyhal_pwm_start(&pwm_obj);
    if (result != CY_RSLT_SUCCESS){
        CY_ASSERT(0);
    }

	while(true){
		for (int i = 100; i >= 0; i--){
			result = cyhal_pwm_set_duty_cycle(&pwm_obj, i, 10000);
			cyhal_system_delay_ms(25);
		}
	}
}
