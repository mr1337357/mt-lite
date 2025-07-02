#include <stdint.h>

struct mt_task_t 
{
    void (*mt_task_ptr)(void *);
    void *arg;
};