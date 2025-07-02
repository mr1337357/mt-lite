#include "board.h"

enum dio1_config
{
    DIO1_INTERRUPT = 0,
};

enum dio2_config
{
    DIO2_ANTENNASWITCH = 0,
};

typedef struct
{
    int spi_handle;
    enum dio1_config d1cfg;
    enum dio2_config d2cfg;
} sx1262_config;
