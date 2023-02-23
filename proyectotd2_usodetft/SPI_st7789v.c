 #include "SPI_st7789v.h"
 #include "cyhal.h"


 cyhal_spi_t mSPI;
 uint32_t spi_master_frequency = 25000000;
 static const SPI_st7789v_pins_t *pins;

 static inline void write_data(uint8_t data)
 {
     cyhal_spi_send(&mSPI,data);
 }
 static inline uint8_t read_data(void)
 {
     uint32_t receive_data = 0u;
     cyhal_spi_recv(&mSPI, &receive_data);
     return receive_data;
 }
 cy_rslt_t SPI_st7789v_init8(const SPI_st7789v_pins_t *data)
 {
     pins = data;
     cy_rslt_t rslt = cyhal_spi_init(&mSPI, pins->MOSI, pins->MISO, pins->SCK, pins->SS, NULL, 8, CYHAL_SPI_MODE_11_MSB
 , false);
     if (CY_RSLT_SUCCESS == rslt)
     rslt = cyhal_spi_set_frequency(&mSPI, spi_master_frequency);
     if (CY_RSLT_SUCCESS == rslt)
     rslt = cyhal_gpio_init(pins->dc, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, 0u);
     if (CY_RSLT_SUCCESS == rslt)
     rslt = cyhal_gpio_init(pins->rst, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, 1u);
     return rslt;
 }
 void SPI_st7789v_write_reset_pin(bool value)
 {
     cyhal_gpio_write(pins->rst, value);
 }
 void SPI_st7789v_write_command(uint8_t data)
 {
     cyhal_gpio_write(pins->dc, 0u);
     write_data(data);
 }
 void SPI_st7789v_write_data(uint8_t data)
 {
     cyhal_gpio_write(pins->dc, 1u);
     write_data(data);
 }
 void SPI_st7789v_write_command_stream(uint8_t *data, int num)
 {
     int i;

     cyhal_gpio_write(pins->dc, 0u);

     for (i = 0; i < num; i++)
     {
         write_data(data[i]);
     }
 }
 void SPI_st7789v_write_data_stream(uint8_t *data, int num)
 {
     int i;

     cyhal_gpio_write(pins->dc, 1u);

     for (i = 0; i < num; i++)
     {
         write_data(data[i]);
     }
 }
 uint8_t SPI_st7789v_read_data(void)
 {
     uint8_t data;
     cyhal_gpio_write(pins->dc, 1u);
     data = read_data();
     return data;
 }
 void SPI_st7789v_read_data_stream(uint8_t *data, int num)
 {
     cyhal_gpio_write(pins->dc, 1u);
     for (int i = 0; i < num; i++)
     {
         data[i] = read_data();
     }
 }



