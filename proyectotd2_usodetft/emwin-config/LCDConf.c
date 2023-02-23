/*********************************************************************
*                SEGGER Microcontroller GmbH                         *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2021  SEGGER Microcontroller GmbH                *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V6.26 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The software  has been licensed to  Cypress Semiconductor Corporation,
whose registered  office is situated  at 198 Champion Ct. San Jose, CA 
95134 USA  solely for the  purposes of creating  libraries for Cypress
PSoC3 and  PSoC5 processor-based devices,  sublicensed and distributed
under  the  terms  and  conditions  of  the  Cypress  End User License
Agreement.
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
Licensing information
Licensor:                 SEGGER Microcontroller Systems LLC
Licensed to:              Cypress Semiconductor Corp, 198 Champion Ct., San Jose, CA 95134, USA
Licensed SEGGER software: emWin
License number:           GUI-00319
License model:            Cypress Services and License Agreement, signed June 9th/10th, 2009
                          and Amendment Number One, signed June 28th, 2019 and July 2nd, 2019
                          and Amendment Number Two, signed September 13th, 2021 and September 18th, 2021
                          and Amendment Number Three, signed May 2nd, 2022 and May 5th, 2022
Licensed platform:        Any Cypress platform (Initial targets are: PSoC3, PSoC5)
----------------------------------------------------------------------
Support and Update Agreement (SUA)
SUA period:               2009-06-12 - 2022-07-27
Contact to extend SUA:    sales@segger.com
----------------------------------------------------------------------
File        : LCDConf.c
Purpose     : Display configuration file for GUIDRV_Bitplains to be
              used without any display controller.
---------------------------END-OF-HEADER------------------------------
*/
#include "SPIemwin.h"

#if defined(EMWIN_ENABLED)

#include "GUI.h"
#include "GUIDRV_FlexColor.h"
#include "LCDConf.h"
#include "SPI_st7789v.h"


#if defined(__cplusplus)
extern "C"
{
#endif

/*********************************************************************
*
*       Layer configuration
*
**********************************************************************
*/
//
// Physical display size
//
#define XSIZE_PHYS MTB_DISPLAY_SIZE_X
#define YSIZE_PHYS MTB_DISPLAY_SIZE_Y

//
// Color conversion
//   The color conversion functions should be selected according to
//   the color mode of the target display. Details can be found in
//   the chapter "Colors" in the emWin user manual.
//
#define COLOR_CONVERSION GUICC_M565

#define DISPLAY_DRIVER GUIDRV_FLEXCOLOR

/*********************************************************************
*
*       Configuration checking
*
**********************************************************************
*/
//
// Checking display driver configuration
//

#ifndef   VXSIZE_PHYS
    #define VXSIZE_PHYS XSIZE_PHYS
#endif
#ifndef   VYSIZE_PHYS
    #define VYSIZE_PHYS YSIZE_PHYS
#endif
#ifndef   XSIZE_PHYS
    #error Physical X size of display is not defined!
#endif
#ifndef   YSIZE_PHYS
    #error Physical Y size of display is not defined!
#endif
#ifndef   COLOR_CONVERSION
    #error Color conversion not defined!
#endif
#ifndef   DISPLAY_DRIVER
    #error No display driver defined!
#endif

//
// Number of bytes per line
//

/*********************************************************************
*
*       ST7789V Commands
*
* See the datasheet for additional details:
* https://www.newhavendisplay.com/appnotes/datasheets/LCDs/ST7789V.pdf
*
**********************************************************************
*/
#define ST7789V_CMD_NOP       (0x00) /* Empty command */
#define ST7789V_CMD_SWRESET   (0x01) /* Software Reset */
#define ST7789V_CMD_RDDID     (0x04) /* Read Display ID */
#define ST7789V_CMD_RDDST     (0x09) /* Read Display Status */
#define ST7789V_CMD_RDDPM     (0x0A) /* Read Display Power Mode */
#define ST7789V_CMD_RDDMADCTL (0x0B) /* Read Display MADCTL */
#define ST7789V_CMD_RDDCOLMOD (0x0C) /* Read Display Pixel Format */
#define ST7789V_CMD_RDDIM     (0x0D) /* Read Display Image Mode */
#define ST7789V_CMD_RDDSM     (0x0E) /* Read Display Signal Mode */
#define ST7789V_CMD_RDDSDR    (0x0F) /* Read Display Self-Diagnostic Result */
#define ST7789V_CMD_SLPIN     (0x10) /* Sleep in */
#define ST7789V_CMD_SLPOUT    (0x11) /* Sleep Out */
#define ST7789V_CMD_PTLON     (0x12) /* Partial Display Mode On */
#define ST7789V_CMD_NORON     (0x13) /* Normal Display Mode On */
#define ST7789V_CMD_INVOFF    (0x20) /* Display Inversion Off */
#define ST7789V_CMD_INVON     (0x21) /* Display Inversion On */
#define ST7789V_CMD_GAMSET    (0x26) /* Gamma Set */
#define ST7789V_CMD_DISPOFF   (0x28) /* Display Off */
#define ST7789V_CMD_DISPON    (0x29) /* Display On */
#define ST7789V_CMD_CASET     (0x2A) /* Column Address Set */
#define ST7789V_CMD_RASET     (0x2B) /* Row Address Set */
#define ST7789V_CMD_RAMWR     (0x2C) /* Memory Write */
#define ST7789V_CMD_RAMRD     (0x2E) /* Memory Read */
#define ST7789V_CMD_PTLAR     (0x30) /* Partial Area */
#define ST7789V_CMD_VSCRDEF   (0x33) /* Vertical Scrolling Definition */
#define ST7789V_CMD_TEOFF     (0x34) /* Tearing Effect Line OFF */
#define ST7789V_CMD_TEON      (0x35) /* Tearing Effect Line On */
#define ST7789V_CMD_MADCTL    (0x36) /* Memory Data Access Control */
#define ST7789V_CMD_VSCSAD    (0x37) /* Vertical Scroll Start Address of RAM */
#define ST7789V_CMD_IDMOFF    (0x38) /* Idle Mode Off */
#define ST7789V_CMD_IDMON     (0x39) /* Idle mode on */
#define ST7789V_CMD_COLMOD    (0x3A) /* Interface Pixel Format */
#define ST7789V_CMD_WRMEMC    (0x3C) /* Write Memory Continue */
#define ST7789V_CMD_RDMEMC    (0x3E) /* Read Memory Continue */
#define ST7789V_CMD_STE       (0x44) /* Set Tear Scanline */
#define ST7789V_CMD_GSCAN     (0x45) /* Get Scanline */
#define ST7789V_CMD_WRDISBV   (0x51) /* Write Display Brightness */
#define ST7789V_CMD_RDDISBV   (0x52) /* Read Display Brightness Value */
#define ST7789V_CMD_WRCTRLD   (0x53) /* Write CTRL Display */
#define ST7789V_CMD_RDCTRLD   (0x54) /* Read CTRL Value Display */
#define ST7789V_CMD_WRCACE    (0x55) /* Wr. Content Adaptive Brightness Control & Color Enhance */
#define ST7789V_CMD_RDCABC    (0x56) /* Read Content Adaptive Brightness Control */
#define ST7789V_CMD_WRCABCMB  (0x5E) /* Write CABC Minimum Brightness */
#define ST7789V_CMD_RDCABCMB  (0x5F) /* Read CABC Minimum Brightness */
#define ST7789V_CMD_RDABCSDR  (0x68) /* Read Automatic Brightness Control Self-Diagnostic Result */
#define ST7789V_CMD_RDID1     (0xDA) /* Read ID1 */
#define ST7789V_CMD_RDID2     (0xDB) /* Read ID2 */
#define ST7789V_CMD_RDID3     (0xDC) /* Read ID3 */
#define ST7789V_CMD_RAMCTRL   (0xB0) /* RAM Control */
#define ST7789V_CMD_RGBCTRL   (0xB1) /* RGB Interface Control */
#define ST7789V_CMD_PORCTRL   (0xB2) /* Porch Setting */
#define ST7789V_CMD_FRCTRL1   (0xB3) /* Frame Rate Control 1 (In partial mode/ idle colors) */
#define ST7789V_CMD_PARCTRL   (0xB5) /* Partial mode Control */
#define ST7789V_CMD_GCTRL     (0xB7) /* Gate Control */
#define ST7789V_CMD_GTADJ     (0xB8) /* Gate On Timing Adjustment */
#define ST7789V_CMD_DGMEN     (0xBA) /* Digital Gamma Enable */
#define ST7789V_CMD_VCOMS     (0xBB) /* VCOMS Setting */
#define ST7789V_CMD_LCMCTRL   (0xC0) /* LCM Control */
#define ST7789V_CMD_IDSET     (0xC1) /* ID Code Setting */
#define ST7789V_CMD_VDVVRHEN  (0xC2) /* VDV and VRH Command Enable */
#define ST7789V_CMD_VRHS      (0xC3) /* VRH Set */
#define ST7789V_CMD_VDVS      (0xC4) /* VDV Set */
#define ST7789V_CMD_VCMOFSET  (0xC5) /* VCOMS Offset Set */
#define ST7789V_CMD_FRCTRL2   (0xC6) /* Frame Rate Control in Normal Mode */
#define ST7789V_CMD_CABCCTRL  (0xC7) /* CABC Control */
#define ST7789V_CMD_REGSEL1   (0xC8) /* Register Value Selection 1 */
#define ST7789V_CMD_REGSEL2   (0xCA) /* Register Value Selection 2 */
#define ST7789V_CMD_PWMFRSEL  (0xCC) /* PWM Frequency Selection */
#define ST7789V_CMD_PWCTRL1   (0xD0) /* Power Control 1 */
#define ST7789V_CMD_VAPVANEN  (0xD2) /* Enable VAP/VAN signal output */
#define ST7789V_CMD_CMD2EN    (0xDF) /* Command 2 Enable */
#define ST7789V_CMD_PVGAMCTRL (0xE0) /* Positive Voltage Gamma Control */
#define ST7789V_CMD_NVGAMCTRL (0xE1) /* Negative Voltage Gamma Control */
#define ST7789V_CMD_DGMLUTR   (0xE2) /* Digital Gamma Look-up Table for Red */
#define ST7789V_CMD_DGMLUTB   (0xE3) /* Digital Gamma Look-up Table for Blue */
#define ST7789V_CMD_GATECTRL  (0xE4) /* Gate Control */
#define ST7789V_CMD_SPI2EN    (0xE7) /* SPI2 Enable */
#define ST7789V_CMD_PWCTRL2   (0xE8) /* Power Control 2 */
#define ST7789V_CMD_EQCTRL    (0xE9) /* Equalize time control */
#define ST7789V_CMD_PROMCTRL  (0xEC) /* Program Mode Control */
#define ST7789V_CMD_PROMEN    (0xFA) /* Program Mode Enable */
#define ST7789V_CMD_NVMSET    (0xFC) /* NVM Setting */
#define ST7789V_CMD_PROMACT   (0xFE) /* Program action */

static void GMT130_V1_TFT_InitController(void)
{
    /* Reset the display controller */
    SPI_st7789v_write_reset_pin(0u);
    GUI_Delay(100);
    SPI_st7789v_write_reset_pin(1u);
    GUI_Delay(50);

    SPI_st7789v_write_command(ST7789V_CMD_DISPOFF);
    SPI_st7789v_write_command(ST7789V_CMD_SLPOUT);    /* Exit Sleep mode */

    GUI_Delay(100);

    SPI_st7789v_write_command(ST7789V_CMD_MADCTL);
    SPI_st7789v_write_data(0xA0);    /* MADCTL: memory data access control */

    SPI_st7789v_write_command(ST7789V_CMD_COLMOD);
    SPI_st7789v_write_data(0x65);    /* COLMOD: Interface Pixel format */

    SPI_st7789v_write_command(ST7789V_CMD_PORCTRL);
    SPI_st7789v_write_data(0x0C);
    SPI_st7789v_write_data(0x0C);
    SPI_st7789v_write_data(0x00);
    SPI_st7789v_write_data(0x33);
    SPI_st7789v_write_data(0x33);    /* PORCTRK: Porch setting */

    SPI_st7789v_write_command(ST7789V_CMD_GCTRL);
    SPI_st7789v_write_data(0x35);    /* GCTRL: Gate Control */

    SPI_st7789v_write_command(ST7789V_CMD_VCOMS);
    SPI_st7789v_write_data(0x2B);    /* VCOMS: VCOM setting */

    SPI_st7789v_write_command(ST7789V_CMD_LCMCTRL);
    SPI_st7789v_write_data(0x2C);    /* LCMCTRL: LCM Control */

    SPI_st7789v_write_command(ST7789V_CMD_VDVVRHEN);
    SPI_st7789v_write_data(0x01);
    SPI_st7789v_write_data(0xFF);    /* VDVVRHEN: VDV and VRH Command Enable */

    SPI_st7789v_write_command(ST7789V_CMD_VRHS);
    SPI_st7789v_write_data(0x11);    /* VRHS: VRH Set */

    SPI_st7789v_write_command(ST7789V_CMD_VDVS);
    SPI_st7789v_write_data(0x20);    /* VDVS: VDV Set */

    SPI_st7789v_write_command(ST7789V_CMD_FRCTRL2);
    SPI_st7789v_write_data(0x0F);    /* FRCTRL2: Frame Rate control in normal mode */

    SPI_st7789v_write_command(ST7789V_CMD_PWCTRL1);
    SPI_st7789v_write_data(0xA4);
    SPI_st7789v_write_data(0xA1);    /* PWCTRL1: Power Control 1 */

    SPI_st7789v_write_command(ST7789V_CMD_PVGAMCTRL);
    SPI_st7789v_write_data(0xD0);
    SPI_st7789v_write_data(0x00);
    SPI_st7789v_write_data(0x05);
    SPI_st7789v_write_data(0x0E);
    SPI_st7789v_write_data(0x15);
    SPI_st7789v_write_data(0x0D);
    SPI_st7789v_write_data(0x37);
    SPI_st7789v_write_data(0x43);
    SPI_st7789v_write_data(0x47);
    SPI_st7789v_write_data(0x09);
    SPI_st7789v_write_data(0x15);
    SPI_st7789v_write_data(0x12);
    SPI_st7789v_write_data(0x16);
    SPI_st7789v_write_data(0x19);    /* PVGAMCTRL: Positive Voltage Gamma control */

    SPI_st7789v_write_command(ST7789V_CMD_NVGAMCTRL);
    SPI_st7789v_write_data(0xD0);
    SPI_st7789v_write_data(0x00);
    SPI_st7789v_write_data(0x05);
    SPI_st7789v_write_data(0x0D);
    SPI_st7789v_write_data(0x0C);
    SPI_st7789v_write_data(0x06);
    SPI_st7789v_write_data(0x44);
    SPI_st7789v_write_data(0x40);
    SPI_st7789v_write_data(0x0E);
    SPI_st7789v_write_data(0x1C);
    SPI_st7789v_write_data(0x18);
    SPI_st7789v_write_data(0x16);
    SPI_st7789v_write_data(0x19);    /* NVGAMCTRL: Negative Voltage Gamma control */

    SPI_st7789v_write_command(ST7789V_CMD_RASET);
    SPI_st7789v_write_data(0x00);
    SPI_st7789v_write_data(0x00);
    SPI_st7789v_write_data(0x00);
    SPI_st7789v_write_data(0xEF);    /* Y address set */

    SPI_st7789v_write_command(ST7789V_CMD_CASET);
    SPI_st7789v_write_data(0x00);
    SPI_st7789v_write_data(0x00);
    SPI_st7789v_write_data(0x01);
    SPI_st7789v_write_data(0x3F);    /* X address set */

    SPI_st7789v_write_command(ST7789V_CMD_INVON);

    GUI_Delay(10);

    SPI_st7789v_write_command(ST7789V_CMD_DISPON);
}

void LCD_X_Config(void)
{
    GUI_DEVICE * pDevice;
    CONFIG_FLEXCOLOR Config = {0};
    GUI_PORT_API PortAPI = {0};
    //
    // Set the display driver and color conversion
    //
    pDevice = GUI_DEVICE_CreateAndLink(DISPLAY_DRIVER, COLOR_CONVERSION, 0, 0);
    //
    // Display size configuration
    //
    LCD_SetSizeEx (0, XSIZE_PHYS,  YSIZE_PHYS);
    LCD_SetVSizeEx(0, VXSIZE_PHYS, VYSIZE_PHYS);
    //
    // Orientation
    //
 //   Config.Orientation   = GUI_MIRROR_Y | GUI_SWAP_XY;
    GUIDRV_FlexColor_Config(pDevice, &Config);
    //
    // Set controller and operation mode
    //
    PortAPI.pfWrite8_A0  = SPI_st7789v_write_command;
    PortAPI.pfWrite8_A1  = SPI_st7789v_write_data;
    PortAPI.pfWriteM8_A1 = SPI_st7789v_write_data_stream;
    PortAPI.pfRead8_A1   = SPI_st7789v_read_data;
    PortAPI.pfReadM8_A1  = SPI_st7789v_read_data_stream;


    GUIDRV_FlexColor_SetFunc(pDevice, &PortAPI, GUIDRV_FLEXCOLOR_F66709, GUIDRV_FLEXCOLOR_M16C0B8);

}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       LCD_X_Config
*
* Purpose:
*   Called during the initialization process in order to set up the
*   display driver configuration.
*
*/


/*********************************************************************
*
*       LCD_X_DisplayDriver
*
* Purpose:
*   This function is called by the display driver for several purposes.
*   To support the according task, the routine needs to be adapted to
*   the display controller. Note that the commands marked
*   "optional" are not cogently required and should only be adapted if
*   the display controller supports these features.
*
* Parameter:
*   LayerIndex - Zero based layer index
*   Cmd        - Command to be executed
*   pData      - Pointer to a data structure.
*
* Return Value:
*   < -1 - Error
*     -1 - The command is not handled.
*      0 - OK.
*/
int LCD_X_DisplayDriver(unsigned LayerIndex, unsigned Cmd, void * pData)
{
  int r;

  GUI_USE_PARA(LayerIndex);
  GUI_USE_PARA(pData);

  switch (Cmd)
  {
  case LCD_X_INITCONTROLLER:
	  GMT130_V1_TFT_InitController();
    r = 0;
    break;

  default:
    r = -1;
    break;
  }

  return r;
}

#if defined(__cplusplus)
}
#endif

#endif /* defined(EMWIN_ENABLED) */

/*************************** End of file ****************************/
