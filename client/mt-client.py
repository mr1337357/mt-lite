import sys
import select
import serial
import time
import datetime
import traceback

import pypb
from mt_lite import mt_lite
#import mt_packet
#import mt_crypto
        
sources = {}

dedup_naive = []

def check_dedup(pkt):
    global dedup_naive
    if pkt.seq in dedup_naive:
        return True
    dedup_naive.append(pkt.seq)
    if len(dedup_naive) > 100:
        dedup_naive = dedup_naive[1:]
    return False

def nodeinfo(pkt):
    pb = pypb.protobuf(pkt.payload[2])
    print(pb.to_map())
    for name, t, value in pb:
        print(name,t,value)
        

def pos(pkt):
    pb = pypb.protobuf(pkt.payload[2])
    print(pb.to_map())

if __name__ == '__main__':
    log = open('test.log','w')
    dev = serial.Serial(sys.argv[1],115200)
    mesht = mt_lite(dev)
    while True:
        mesht.update()
        pkt = mesht.get()
        if pkt != None:
            if check_dedup(pkt):
                continue
            if not pkt.src in sources:
                sources[pkt.src] = None
            print('packet')
            print('from: {}'.format(hex(pkt.src)))
            print('to:   {}'.format(hex(pkt.dest)))
            print('hops: {}/{}'.format(pkt.hopcount,pkt.maxhop))
            print('rssi: {}'.format(pkt.rssi))
            if pkt.decrypted == True:
                print(pkt.payload)
                if pkt.payload[1] == 1:
                    print('text')
                if pkt.payload[1] == 3:
                    print('pos')
                    pos(pkt)
                if pkt.payload[1] == 4:
                    print('nodeinfo')
                    nodeinfo(pkt)
            else:
                print('undecrypted')
            print('')
        time.sleep(.1)
