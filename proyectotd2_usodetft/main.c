#include "cyhal.h"
#include "cybsp.h"
#include "cy_pdl.h"
#include "cy_retarget_io.h"
#include "GUI.h"
#include "SPI_st7789v.h"
#include "LCDConf.h"

// Defino los pins que voy a usar con la pantalla TFT
const SPI_st7789v_pins_t tft_pins =
{
	    .MOSI = P9_0,
	    .MISO = P9_1,
	    .SCK = P9_2,
	    .SS = P9_3,
	    .dc = P9_7,
		.rst= P9_4,
};

int x = 125; // coordenada incial x
int y = 125; // coordenada inicial y
int a = 5; // variable de ajueste (grueso, medio, fino)
int tc = 2; // tamaño del circulo
int limxizquierda = 20;
int limitexderecha = 220;
int limyarriba = 50;
int limiteyabajo = 220;
int fondo[2]={				GUI_LIGHTGREEN,
							GUI_GRAY,}; // declaro vector con posibles fondos de pantalla
int i = 0; // variable de ayuda para seleccionar fondo de pantalla
int dibujo[2]={				GUI_LIGHTGREEN,
							GUI_BLACK,}; // declaro vector con posibles fondos de pantalla
int j = 0; // variable de ayuda para seleccionar si se dibuja en pantalla o no


int main(void)
{
   cy_rslt_t result;

   cyhal_quaddec_t objdetquad_H; //Declaro objetos para detector de posicion H y V
   cyhal_quaddec_t objdetquad_V;
   uint32_t cuentah = 0x1000, previah = 0x1000; //Declaro variables para contar posicion
   uint32_t cuentav = 0x1000, previav = 0x1000;

    // Inicializo el objeto del decodificador de cuadratura. No uso index ni clock.
    result = cyhal_quaddec_init(&objdetquad_H, P5_0, P5_1, NC, CYHAL_QUADDEC_RESOLUTION_2X, NULL, 100000);
    CY_ASSERT(CY_RSLT_SUCCESS == result);
    result = cyhal_quaddec_init(&objdetquad_V, P8_0, P8_1, NC, CYHAL_QUADDEC_RESOLUTION_2X, NULL, 100000);
    CY_ASSERT(CY_RSLT_SUCCESS == result);

    // Inicializo el decodificador de cuadratura
    result = cyhal_quaddec_start(&objdetquad_H);
    CY_ASSERT(CY_RSLT_SUCCESS == result);
    result = cyhal_quaddec_start(&objdetquad_V);
    CY_ASSERT(CY_RSLT_SUCCESS == result);

    // Inicializo los dos pines GPIO para los botones
    result = cyhal_gpio_init(P9_6, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_PULLUP, 1);
    CY_ASSERT(CY_RSLT_SUCCESS == result);
    result = cyhal_gpio_init(P5_5, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_PULLUP, 1);
    CY_ASSERT(CY_RSLT_SUCCESS == result);

	#if defined (CY_DEVICE_SECURE)
		cyhal_wdt_t wdt_obj;

		// Limpio el timer del watchdog para que no se active el reseteo
		result = cyhal_wdt_init(&wdt_obj, cyhal_wdt_get_max_timeout_ms());
		CY_ASSERT(CY_RSLT_SUCCESS == result);
		cyhal_wdt_free(&wdt_obj);
	#endif

    // Inicializo la BSP
    result = cybsp_init();
    CY_ASSERT(CY_RSLT_SUCCESS == result);

    // Habilito interrupciones
    __enable_irq();

    // Conecto por SPI la pantalla TFT utiliznado los pines definidos previamente
    result = SPI_st7789v_init8(&tft_pins);
    CY_ASSERT(result == CY_RSLT_SUCCESS);

    // Comienzo con la rutina de dibujado de la interfaz
    // Inicializo la GUI, elijo color de fondo, limpio la pantalla, defino color y tipografia de las letras y
    // dibujos, dibujo dos cuadrados como marco, dibujo un circulo en la posicion central.
    GUI_Init();
    GUI_SetBkColor(fondo[i]);
    GUI_Clear();
    GUI_SetColor(GUI_BLACK);
	GUI_SetFont(&GUI_Font8x16);
	GUI_DrawRect(10, 10, LCD_GetXSize() - 10, LCD_GetYSize() - 10);
	GUI_DrawRect(5, 5, LCD_GetXSize() - 5, LCD_GetYSize() - 5);
	GUI_FillCircle(x, y, tc);
	// GUI_DispStringAt("TECNICAS DIGITALES 2 - 2022", 15, 20);

	// Lazo infinito
    while(1)
    {
        //
        // DIBUJO ETIQUETAS DE INTERFAZ
        //
    	// Dibujo etiquetas de posicion y desplazamiento
        GUI_DispStringAt("x:", 15, 20);
        GUI_DispDec(x, 3);
        GUI_DispString(" y:");
        GUI_DispDec(y, 3);
        GUI_DispString(" ajuste: ");
        if (a == 5)
            {
        	GUI_DispString("grueso");
            }
        	else if (a == 3)
        		{
        		GUI_DispString("medio ");
        		}
        	else
            		{
        		GUI_DispString("fino  ");
            		}

        //
        // CONTROL HORIZONTAL
        //
        // Guardo el valor del encoder en una variable
        cuentah = cyhal_quaddec_read_counter(&objdetquad_H);

/*      GUI_DispStringAt("h:", 15, 50);
        GUI_DispDec(cuentah, 6);
        GUI_DispString("-");
        GUI_DispDec(previah, 6);

		Estos comandos comentados me sirven para visualizar por pantalla los valores que toma la primera lectura del detector de cuadratura
		para poder entender por que siempre al encender el display se cumple que la cuenta es mayor a la previa.
*/

        // Verifico si el valor de esta ultima variable es mayor al anterior. De ser asi, ejecuto una rutina para giro en sentido horario.
        if (cuentah > previah)
        	{
        	// Dibujo un circulo con el mismo color del fondo, sobre el circulo que se muestra en pantalla para "borrarlo"
        	GUI_SetColor(dibujo[j]);
    		GUI_FillCircle(x, y, tc);
    		// Ahora muevo la coordenada x del punto, a pixeles hacia la izquierda.
    		x = x - a;
    		// Si x es menor a 20 (que es el limite de la pantalla), lo vuelvo a 20 y de esta forma nunca se dibuja el circulo fuera de ese limite.
    		if (x < limxizquierda)
    		{
    			x = limxizquierda;
    		}
    		// Dibujo un circulo de color nuevo sobre la nueva posicion
    		GUI_SetColor(GUI_BLACK);
        	GUI_FillCircle(x, y, tc);
        	GUI_Delay(10);
        	}
        	// Verifico si el valor de esta ultima variable es menor al anterior. De ser asi, ejecuto una rutina para giro en sentido antihorario.
			else if (cuentah < previah)
			{
				// Dibujo un circulo con el mismo color del fondo, sobre el circulo que se muestra en pantalla para "borrarlo"
				GUI_SetColor(dibujo[j]);
				GUI_FillCircle(x, y, tc);
				// Ahora muevo la coordenada x del punto, a pixeles hacia la derecha.
				x = x + a;
				// Si x es menor a 220 (que es el limite de la pantalla), lo vuelvo a 220 y de esta forma nunca se dibuja el circulo fuera de ese limite.
				if (x > limitexderecha)
				{
					x = limitexderecha;
				}
				// Dibujo un circulo de color nuevo sobre la nueva posicion
				GUI_SetColor(GUI_BLACK);
				GUI_FillCircle(x, y, tc);
				GUI_Delay(10);
			}
			// Verifico si el valor de esta ultima variable es igual al anterior. De ser asi, ejecuto la rutina para este caso.
			else
			{
				// no hago nada
			}
        // Coloco como valor previo, el valor actual de la cuenta.
        previah = cuentah;



        //
        // CONTROL VERTICAL
        //
        // Guardo el valor del encoder en una variable
        cuentav = cyhal_quaddec_read_counter(&objdetquad_V);

/*      GUI_DispStringAt("v:", 15, 100);
        GUI_DispDec(cuentav, 6);
        GUI_DispString("-");
        GUI_DispDec(previav, 6);

		Estos comandos comentados me sirven para visualizar por pantalla los valores que toma la primera lectura del detector de cuadratura
		para poder entender por que siempre al encender el display se cumple que la cuenta es mayor a la previa.
*/

        // Verifico si el valor de esta ultima variable es mayor al anterior. De ser asi, ejecuto una rutina para giro en sentido horario.
        if (cuentav > previav)
        	{
        	// Dibujo un circulo con el mismo color del fondo, sobre el circulo que se muestra en pantalla para "borrarlo"
        	GUI_SetColor(dibujo[j]);
    		GUI_FillCircle(x, y, tc);
    		// Ahora muevo la coordenada y del punto, a pixeles hacia abajo.
    		y = y - a;
    		// Si x es menor a 50 (que es el limite de la pantalla), lo vuelvo a 50 y de esta forma nunca se dibuja el circulo fuera de ese limite.
    		if (y < limyarriba)
    		{
    			y = limyarriba;
    		}
    		// Dibujo un circulo de color nuevo sobre la nueva posicion
    		GUI_SetColor(GUI_BLACK);
        	GUI_FillCircle(x, y, tc);
        	GUI_Delay(10);
        	}
        	// Verifico si el valor de esta ultima variable es menor al anterior. De ser asi, ejecuto una rutina para giro en sentido antihorario.
			else if (cuentav < previav)
			{
				// Dibujo un circulo con el mismo color del fondo, sobre el circulo que se muestra en pantalla para "borrarlo"
				GUI_SetColor(dibujo[j]);
				GUI_FillCircle(x, y, tc);
				// Ahora muevo la coordenada y del punto, a pixeles hacia arriba
				y = y + a;
				// Si x es menor a 220 (que es el limite de la pantalla), lo vuelvo a 220 y de esta forma nunca se dibuja el circulo fuera de ese limite.
				if (y > limiteyabajo)
				{
					y = limiteyabajo;
				}
				// Dibujo un circulo de color nuevo sobre la nueva posicion
				GUI_SetColor(GUI_BLACK);
				GUI_FillCircle(x, y, tc);
				GUI_Delay(10);
			}
			// Verifico si el valor de esta ultima variable es igual al anterior. De ser asi, ejecuto la rutina para este caso.
			else
			{
				// no hago nada
			}
        // Coloco como valor previo, el valor actual de la cuenta.
        previav = cuentav;

        //
        // CONTROL DE BOTONES
        //
        // Verifico si se pulsa el boton del encoder horizontal, y cambio el valor del ajuste.
        if(!cyhal_gpio_read(P9_6))
        {
        	cyhal_system_delay_ms(250);
        	if (a==5)
        		{
        		a = 3;
        		}
        		else if (a == 3)
        		{
        			a = 1;
        		}
        		else
        		{
        			a = 5;
        		}
        }


		// Verifico si se pulsa el boton del encoder vertical, y cambio el valor del fondo.
		if(!cyhal_gpio_read(P5_5))
		{
			cyhal_system_delay_ms(250);
			if (i==0)
				{
					i=1;
					j=1;
				}


			// Una vez cambiado el color del fondo, redibujo nuevamente la interfaz.
			GUI_SetBkColor(fondo[i]);
			GUI_Clear();
			GUI_SetColor(GUI_BLACK);
			GUI_SetFont(&GUI_Font8x16);
			GUI_DrawRect(10, 10, LCD_GetXSize() - 10, LCD_GetYSize() - 10);
			GUI_DrawRect(5, 5, LCD_GetXSize() - 5, LCD_GetYSize() - 5);
			GUI_FillCircle(x, y, tc);
		}

    }
}

