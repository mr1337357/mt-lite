#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "util/minheap.h"

#include "boards.h"
#include "mt_task.h"

QueueHandle_t xQueue1;

void add_task(struct mt_task_t *task, bool isr)
{
    if(isr)
    {
        xQueueSendFromISR(xQueue1, task, 0);
    }
    else
    {
        xQueueSend(xQueue1, task, 0);
    }
}

void app_main(void)
{
    struct mt_task_t task;
    xQueue1 = xQueueCreate( 10, sizeof( struct mt_task_t ) );

    board_init();

    while(1)
    {
        if(xQueueReceive(xQueue1, &task, portMAX_DELAY))
        {
            task.mt_task_ptr(task.arg);
        }
    }
}
