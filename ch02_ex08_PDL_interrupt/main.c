#include "cy_pdl.h"
#include "cybsp.h"

#define PORT_INTR_MASK  (0x00000001UL << CYBSP_USER_BTN_PORT_NUM)

void Interrupt_Handler(void){
	// Get interrupt cause
	uint32_t intrSrc = Cy_GPIO_GetInterruptCause0();
	/* Check if the interrupt was from the button's port */
	if(PORT_INTR_MASK == (intrSrc & PORT_INTR_MASK)){
		/* Clear the interrupt */
		Cy_GPIO_ClearInterrupt(CYBSP_USER_BTN_PORT, CYBSP_USER_BTN_NUM);
		// Toggle LED
		Cy_GPIO_Inv(CYBSP_USER_LED_PORT, CYBSP_USER_LED_NUM);
	}
}

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

    // Interrupt config structure
     cy_stc_sysint_t intrCfg =
 	{
 		/*.intrSrc =*/ CYBSP_USER_BTN_IRQ,
 		/*.intrPriority =*/ 3UL
 	};

     /* Initialize the interrupt with vector for Interrupt_Handler */
 	Cy_SysInt_Init(&intrCfg, &Interrupt_Handler);
 	/* Send the button through the glitch filter */
 	Cy_GPIO_SetFilter(CYBSP_USER_BTN_PORT, CYBSP_USER_BTN_NUM);
 	/* Enable the interrupt */
 	NVIC_EnableIRQ(intrCfg.intrSrc);

    for (;;)
    {
    	/* Do nothing */
    }
}
