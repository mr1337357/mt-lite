#include <Arduino.h>

#include "nvdata.h"
#define MAX_BLOCKS 16

/*******
 * entry layout: uint8_t len; uint8_t *key(1 to 62 bytes); uint8_t 0; uint8_t *data(1 to 64 bytes);
 */

NVData nvdata;

NVData::NVData()
{
}

int NVData::init(char *partname)
{
  int i;
  int j;
  part = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, partname);
  tempdata = (uint8_t *)malloc(part->erase_size);
  numblocks = part->size/part->erase_size;
  if(numblocks > MAX_BLOCKS)
  {
    numblocks = MAX_BLOCKS;
  }
  start_offset = 0;
  for(i=0;start_offset == 0 && i<numblocks;i++)
  {
    esp_partition_read(part,i * part->erase_size, tempdata, part->erase_size);
    Serial.printf("read\n");
    for(j=0;j<part->erase_size;j++)
    {
      if(tempdata[j] != 0x00)
      {
        start_offset = i * part->erase_size + j;
        break;
      }
    }
  }
  j = 0;
  i=start_offset/part->erase_size;
  while(i<numblocks)
  {
    esp_partition_read(part,i * part->erase_size, tempdata, part->erase_size);
    if(tempdata[j] == 255)
    {
      end_offset = i * part->erase_size + j;
      break;
    }
    j += tempdata[j];
    if(j > part->erase_size)
    {
      i++;
      j-= part->erase_size;
    }
  }
  Serial.printf("start offset %d\n",start_offset);
  Serial.printf("end offset %d\n",end_offset);
  return 0;
}

int NVData::set(char *key,uint8_t *value, int len)
{
    int i;
    int keylen = strlen(key);
    if(keylen > 62)
    {
        keylen = 62;
    }
    if(len > 64)
    {
        len = 64;
    }
    for(i=0;i<keylen;i++)
    {
        tempkey[i] = key[i]&0x7F;
    }
    return -1;
}

int NVData::get(char *key,uint8_t *value, int *len)
{
    int i;
    int keylen = strlen(key);
    if(keylen > 62)
    {
        keylen = 62;
    }
    if(*len > 64)
    {
        *len = 64;
    }
    for(i=0;i<keylen;i++)
    {
        tempkey[i] = key[i]&0x7F;
    }
    return -1;
}
