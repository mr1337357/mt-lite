#include <Arduino.h>

#include "nvdata.h"
#define MAX_BLOCKS 16

/*******
 * entry layout: uint8_t len; uint8_t *key(1 to 62 bytes); uint8_t 0; uint8_t *data(1 to 64 bytes);
 */

NVData nvdata;

NVData::NVData() {
}

int NVData::init(char *partname) {
   int i;
   part = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, partname);
   tempdata = (uint8_t *)malloc(part->erase_size);
   Serial.printf("part size %d erase size %d\n",part->size,part->erase_size);
   numblocks = part->size / part->erase_size;
   if (numblocks > MAX_BLOCKS) {
      numblocks = MAX_BLOCKS;
   }
   which_half = numblocks;
   block_read(0);
   if ((tempdata[0] & 0xC0) == 0x80) {
      writecounter = tempdata[0] & 0x3F;
      which_half = 0;
   }
   block_read(numblocks >> 1);
   if ((tempdata[0] & 0xC0) == 0x80) {
      if ((which_half != 0) || ((tempdata[0] & 0x3F) == (writecounter + 1))) {
         writecounter = tempdata[0] & 0x3F;
         which_half = numblocks >> 1;
      }
   }

   if (which_half == numblocks) {
      Serial.printf("no valid nvdata. formatting.\n");
      start_half(0);
      which_half = 0;
   }
   return 0;
}

int NVData::start_half(int half)
{
   int i;
   int end = numblocks / 2;
   for (i = 0; i < end; i++)
   {
      block_clear(i + half);
   }
   for (i = 1; i < part->erase_size; i++)
   {
      tempdata[i] = 0xFF;
   }
   tempdata[0] = (writecounter | 0x80) & 0xBF;
   block_write(half);
   return 0;
}

int NVData::block_read(int block) {
   Serial.printf("read block %d\n",block);
   if (block == current_block) {
      return 0;
   }
   esp_partition_read(part, block * part->erase_size, tempdata, part->erase_size);
   current_block = block;
   return 0;
}

int NVData::block_write(int block) {
   esp_partition_write(part, block * part->erase_size, tempdata, part->erase_size);
   current_block = block;
   return 0;
}

int NVData::block_clear(int block) {
   esp_partition_erase_range(part, block * part->erase_size, part->erase_size);
   return 0;
}

int NVData::next(int start) {
  return 0;
}

int find_key(uint8_t *key, int len)
{
  return -1;
}

int NVData::set(char *key, uint8_t *value, int len) {
   int i;
   int keylen = strlen(key);
   if (keylen > 62) {
      keylen = 62;
   }
   if (len > 64) {
      len = 64;
   }
   for (i = 0; i < keylen; i++) {
      tempkey[i] = key[i] & 0x7F;
   }
   return -1;
}

int NVData::get(char *key, uint8_t *value, int *len) {
   int i;
   int keylen = strlen(key);
   if (keylen > 62) {
      keylen = 62;
   }
   if (*len > 64) {
      *len = 64;
   }
   for (i = 0; i < keylen; i++) {
      tempkey[i] = key[i] & 0x7F;
   }
   return -1;
}
