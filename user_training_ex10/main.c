#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"

int ciclo=100;
cyhal_pwm_t pwm_obj;

static void button1(void *handler_arg, cyhal_gpio_event_t event)
{
	cyhal_gpio_toggle(CYBSP_USER_LED);
}

static void button2(void *handler_arg, cyhal_gpio_event_t event)
{
    for(int i=0;i<10;i++)
	{
		cyhal_gpio_toggle(P9_0);
		cyhal_system_delay_ms(250);
	}
}

static void button3(void *handler_arg, cyhal_gpio_event_t event)
{
	ciclo-=20;
	if(ciclo<0)ciclo=100;
	cyhal_pwm_set_duty_cycle(&pwm_obj, ciclo, 200);
	cyhal_system_delay_ms(250);
}

cyhal_gpio_callback_data_t cb_data_bot1 =
{
	.callback     = button1,
	.callback_arg = NULL
};

cyhal_gpio_callback_data_t cb_data_bot2 =
{
	.callback     = button2,
	.callback_arg = NULL
};

cyhal_gpio_callback_data_t cb_data_bot3 =
{
	.callback     = button3,
	.callback_arg = NULL
};

int main(void)
{
    cy_rslt_t result;

    result = cybsp_init() ;
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* Inicializar led de usuario y boton de usuario */
    cyhal_gpio_init(CYBSP_USER_LED, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, CYBSP_LED_STATE_OFF);
	cyhal_gpio_init(CYBSP_USER_BTN, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_PULLUP, CYBSP_BTN_OFF);

    /* Inicializar led ambar y boton de ambar */
    cyhal_gpio_init(P9_0, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, 0);
    cyhal_gpio_init(P8_0, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_PULLUP, 1);
    //cyhal_gpio_init(P9_5, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, 0);
    //cyhal_gpio_init(P9_2, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, 0);
    //Otra forma de hacerlo es configurar algun pin GPIO como salida 0 para conectar a masa los botones y leds y utilizar menos cables

    /* Inicializar led azul y boton de azul */
	cyhal_gpio_init(P9_4, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_PULLUP, 1);
	/* Inicializo pwm */
	cyhal_pwm_init(&pwm_obj, P6_0, NULL);
	cyhal_pwm_set_duty_cycle(&pwm_obj, ciclo, 1000);
	cyhal_pwm_start(&pwm_obj);

	/* GPIO interrupt para boton usuario */
	cyhal_gpio_register_callback(CYBSP_USER_BTN, &cb_data_bot1);
	cyhal_gpio_enable_event(CYBSP_USER_BTN, CYHAL_GPIO_IRQ_FALL, 1, true);

	/* GPIO interrupt para boton ambar */
	cyhal_gpio_register_callback(P8_0, &cb_data_bot2);
	cyhal_gpio_enable_event(P8_0, CYHAL_GPIO_IRQ_FALL, 2, true);

	/* GPIO interrupt para boton azul */
	cyhal_gpio_register_callback(P9_4, &cb_data_bot3);
	cyhal_gpio_enable_event(P9_4, CYHAL_GPIO_IRQ_FALL, 3, true);

    __enable_irq();

    for (;;)
    {
    	//Enter deep sleep mode
		cyhal_syspm_deepsleep();
    }

}