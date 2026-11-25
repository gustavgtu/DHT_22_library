idf_component_register(SRCS "GP_DHT_22.c"
                       PRIV_REQUIRES spi_flash
                       PRIV_REQUIRES esp_driver_gpio
                       PRIV_REQUIRES esp_timer
                       INCLUDE_DIRS "")
