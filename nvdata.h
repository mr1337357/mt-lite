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

        int find_key(char *key);
        //state data
        const esp_partition_t *part;
        int start_offset;
        int end_offset;
        int numblocks;

        //working data
        uint8_t *tempdata;
        uint8_t tempkey[64];


};

extern NVData nvdata;
#endif
