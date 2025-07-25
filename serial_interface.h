#ifndef __SERIAL_INTERFACE_H__
#define SERIAL_INTERFACE_H__

void serial_init();

void serial_update();

void serial_send(uint8_t *buffer, int len);

#endif
