#include <Arduino.h>

#include "picopb.h"

#define BUFFER_LENGTH 300
uint8_t input_buffer[BUFFER_LENGTH];
int input_length = 0;
int frame_length = 0;

void serial_init()
{
}

void decode(uint8_t *buff, int len)
{
   int id;
   size_t size;
   pb_type t;
   picopb *pb = new picopb(buff,len);
   while(1)
   {
      t = pb->decode_next(&id,&size);
      Serial.printf("type: %d id: %d size: %d\n",t,id,size);
      if(t == pb_type::INVALID)
      {
         return;
      }
      else
      {
         pb->skip();
      }
   }
   delete pb;
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
         Serial.printf("got frame size %d\n",frame_length);
         int i;
         for(i=0;i<input_length;i++)
         {
          Serial.printf("%02X ",input_buffer[i]);
         }
         Serial.printf("\n");
         decode(input_buffer+4,frame_length);
         memset(input_buffer,0,BUFFER_LENGTH);
         input_length = 0;
      }
   }
}

void serial_send(uint8_t *buffer, int len)
{
   Serial.write(0x94);
   Serial.write(0xC3);
   Serial.write(0x00);
   Serial.write(len);
   Serial.write(buffer,len);
}
