#pragma once

#include <stdint.h>
#include "cy_result.h"
#include "cyhal_gpio.h"

#if defined(__cplusplus)
extern "C"
{
#endif
/** Configuration structure defining the pins used to communicate with the TFT display. */
typedef struct
{
    cyhal_gpio_t MOSI;  /**< Pin for the Display MOSI signal */
    cyhal_gpio_t MISO;  /**< Pin for the Display MISO signal */
    cyhal_gpio_t SCK;  /**< Pin for the Display SCK signal */
    cyhal_gpio_t SS;  /**< Pin for the Display SS signal */
    cyhal_gpio_t dc;
    cyhal_gpio_t rst;
} SPI_st7789v_pins_t;

/**
 * Initializes GPIOs for the software i8080 8-bit interface.
 * @param[in] pins Structure providing the pins to use for the display
 * @return CY_RSLT_SUCCESS if successfully initialized, else an error about
 * what went wrong
 */
 cy_rslt_t SPI_st7789v_init8(const SPI_st7789v_pins_t *pins);

/**
 * Sets value of the display Reset pin.
 * @param[in] value The value to set on the pin
 */
void SPI_st7789v_write_reset_pin(bool value);

/**
 * Writes one byte of data to the software i8080 interface with the LCD_DC pin
 * set to 0. Followed by a low pulse on the NWR line to complete the write.
 * @param[in] command The command to issue to the display
 */
void SPI_st7789v_write_command(uint8_t command);

/**
 * Writes one byte of data to the software i8080 interface with the LCD_DC pin
 * set to 1. Followed by a low pulse on the NWR line to complete the write.
 * @param[in] data The value to issue to the display
 */
void SPI_st7789v_write_data(uint8_t data);

/**
 * Writes multiple command bytes to the software i8080 interface with the LCD_DC
 * pin set to 0.
 * @param[in] data Pointer to the commands to send to the display
 * @param[in] num  The number of commands in the data array to send to the display
 */
void SPI_st7789v_write_command_stream(uint8_t *data, int num);

/**
 * Writes multiple bytes of data to the software i8080 interface with the LCD_DC
 * pin set to 1.
 * @param[in] data Pointer to the data to send to the display
 * @param[in] num  The number of bytes in the data array to send to the display
 */
void SPI_st7789v_write_data_stream(uint8_t *data, int num);

/**
 * Reads one byte of data from the software i8080 interface with the LCD_DC pin
 * set to 1.
 * @return The byte read from the display
 */
uint8_t SPI_st7789v_read_data(void);

/**
 * Reads multiple bytes of data from the software i8080 interface with the LCD_DC
 * pin set to 1.
 * @param[in,out]   data Pointer to where to store the bytes read from the display
 * @param[in]       num  The number of bytes to read from the display
 */
void SPI_st7789v_read_data_stream(uint8_t *data, int num);

/**
 * Free all resources used for the software i8080 interface.
 */


/** \} group_board_libs */
#if defined(__cplusplus)
}
#endif
