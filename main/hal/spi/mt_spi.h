#include <stdint.h>
int spi_init();

int spi_register_device(int cs, int speed, int mode);

int spi_transact(int handle, uint8_t *data, uint8_t len);
