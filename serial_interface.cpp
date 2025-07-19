#include <Arduino.h>
#define BUFFER_LENGTH 300
uint8_t input_buffer[BUFFER_LENGTH];
int input_length = 0;

void serial_init()
{
}

void serial_attach(uint8_t *header, int len, void (*handler)(void))
{
}

void serial_update()
{
   while(Serial.available())
   {
      uint8_t inchar = Serial.read();
      if(input_length >= 2)
      {
         if(input_length < BUFFER_LENGTH)
         {
            input_buffer[input_length++] = inchar;
         }
         if(inchar == '\n')
         {
            //TODO handle message
            memset(input_buffer,0,BUFFER_LENGTH);
            input_length = 0;
         }
      }
      else if(input_length < 2)
      {
         if(inchar == '~')
         {
            input_buffer[input_length++] = inchar;
         }
         else
         {
            input_length = 0;
         }
      }
   }
}

void serial_send(uint8_t *buffer, int len)
{
    Serial.write('~');
    Serial.write('~');
    Serial.write(buffer,len);
    Serial.write('\n');
}
