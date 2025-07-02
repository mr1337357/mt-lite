struct minheap_s
{
    int size;
    int count;
    void **elem;
    int *prio;
};

typedef struct minheap_s minheap;

int minheap_init(minheap *mh, int size);
void *minheap_pop(minheap *mh);
int minheap_push(minheap *mh, int prio, void *elem);
