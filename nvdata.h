#ifndef __NVData_H__
#define __NVData_H__

#include <Arduino.h>

class NVData
{
   public:
      NVData();
      int init(char *partname = "spiffs");
      int set(char *key,uint8_t *value, int len);
      int get(char *key,uint8_t *value, int *len);

   private:
      //API wrappers
      int block_read(int index);
      int block_write(int index);
      int block_clear(int index);
      int start_half(int index);
      int next(int start);
      int find_key(char *key);
      //state data
      const esp_partition_t *part;
      int which_half;
      unsigned int numblocks;
      int current_block;
      uint8_t writecounter;

      //working data
      uint8_t *tempdata;
      uint8_t tempkey[64];


};

extern NVData nvdata;
#endif
