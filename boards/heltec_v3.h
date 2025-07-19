#ifndef __HELTEC_V3__
#define __HELTEC_V3__

// SPI pins
#define SS        GPIO_NUM_8
#define MOSI      GPIO_NUM_10
#define MISO      GPIO_NUM_11
#define SCK       GPIO_NUM_9
// Radio pins
#define DIO1      GPIO_NUM_14
#define RST_LoRa  GPIO_NUM_12
#define BUSY_LoRa GPIO_NUM_13

SPIClass* hspi = new SPIClass(HSPI);
SX1262 radio = new Module(SS, DIO1, RST_LoRa, BUSY_LoRa, *hspi);

#endif