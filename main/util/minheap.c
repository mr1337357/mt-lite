#include <stdlib.h>

#include "util/minheap.h"

#define PARENT(idx) ((idx-1)/2)
#define LEFT(idx) ((2 * idx) + 1)
#define RIGHT(idx) ((2 * idx) + 2)

#define POW2(x) (!(x&(x-1)))

static void minheap_swap(minheap *mh, int i, int j)
{
    int pt;
    void *dt;
    pt = mh->prio[i];
    dt = mh->elem[i];
    mh->prio[i] = mh->prio[j];
    mh->elem[i] = mh->elem[j];
    mh->prio[j] = pt;
    mh->elem[j] = dt;
}
    

static void minheap_sink(minheap *mh)
{
    int i = 0;
    int l = 0;
    int r = 0;
    int m = 0;
    while(i < mh->count)
    {
        l = LEFT(i);
        r = RIGHT(i);
        m = i;
        if(l < mh->count && mh->prio[l] < mh->prio[m])
        {
            m = l;
        }
        if(r < mh->count && mh->prio[r] < mh->prio[m])
        {
            m = r;
        }
        if(m != i)
        {
            minheap_swap(mh,i,m);
            i = m;
        }
        else
        {
            i = mh->count;
        }
    }
}

static void minheap_bubble(minheap *mh)
{
    int i = mh->count - 1;
    int p;
    while(i > 0)
    {
        p = PARENT(i);
        if(mh->prio[p] > mh->prio[i])
        {
            minheap_swap(mh,p,i);
            i = p;
        }
        else
        {
            i = 0;
        }
    }
}

int minheap_init(minheap *mh, int size)
{
    if(POW2(size))
    {
        mh->count = 0;
        mh->size = size;
        mh->elem = malloc(sizeof(void *) * size);
        mh->prio = malloc(sizeof(int) * size);
        return 0;
    }
    return -1;
}

void *minheap_pop(minheap *mh)
{
    void *rv = 0;
    if(mh->count > 0)
    {
        mh->count -= 1;
        rv = mh->elem[0];
        mh->elem[0] = mh->elem[mh->count];
        mh->prio[0] = mh->prio[mh->count];
        minheap_sink(mh);
    }
    return rv;
}

int minheap_push(minheap *mh, int prio, void *elem)
{
    if(mh->count == mh->size)
    {
        return -1;
    }
    mh->prio[mh->count] = prio;
    mh->elem[mh->count] = elem;
    mh->count++;
    minheap_bubble(mh);
    return 0;
}
