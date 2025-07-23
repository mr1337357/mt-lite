#include <Arduino.h>
#define BUFFER_LENGTH 300
uint8_t input_buffer[BUFFER_LENGTH];
int input_length = 0;
int frame_length = 0;

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
      switch(input_length)
      {
         case 0:
            if(inchar == 0x94)
            {
               input_buffer[input_length++] = inchar;
            }
            break;
         case 1:
            if(inchar == 0xC3)
            {
               input_buffer[input_length++] = inchar;
            }
            else
            {
               input_length = 0;
            }
            break;
         case 2:
            frame_length = inchar;
            input_buffer[input_length++] = inchar;
            break;
         case 3:
            frame_length <<= 8;
            frame_length |= inchar;
            input_buffer[input_length++] = inchar;
            break;
         default:
            if(input_length < frame_length+4)
            {
               input_buffer[input_length++] = inchar;
            }
            break;
      }
      if(input_length == frame_length + 4)
      {
         //TODO handle message
         memset(input_buffer,0,BUFFER_LENGTH);
         input_length = 0;
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
