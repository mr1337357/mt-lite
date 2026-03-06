#include <Arduino.h>

#include "picopb.h"
#include "nvdata.h"

#include <RadioLib.h>


#define BUFFER_LENGTH 300
uint8_t input_buffer[BUFFER_LENGTH];
int input_length = 0;
int frame_length = 0;

void serial_init()
{
}
uint8_t send_buffer[256];

void mt_send(uint8_t *buff, int len);

void decode(uint8_t *buff, int len)
{
   int id;
   size_t size;
   pb_type t;
   picopb *pb = new picopb(buff,len);
   t = pb->decode_next(&id,&size);
   if(t == pb_type::VARINT)
   {
    if(id == 1)
    {
      uint64_t pktid;
      pb->read_varint(&pktid);
      //transmit packet
      t = pb->decode_next(&id,&size);
      if(t == pb_type::STRING)
      {
        if(id == 2)
        {
          pb->read_string(send_buffer,256);
          mt_send(send_buffer,size);
        }
      }
    }
   }
   while(1)
   {
      t = pb->decode_next(&id,&size);
      //Serial.printf("type: %d id: %d size: %d\n",t,id,size);
      if(t == pb_type::INVALID)
      {
         return;
      }
      else
      {
        if(id == 1)
         pb->skip();
      }
   }
   delete pb;
}

void led_on(int ms);

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
         led_on(400);
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
   Serial.write(len>>8);
   Serial.write(len);
   Serial.write(buffer,len);
}
