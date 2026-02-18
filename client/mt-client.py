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
        
if __name__ == '__main__':
    log = open('test.log','w')
    dev = serial.Serial(sys.argv[1],115200)
    mesht = mt_lite(dev)
    while True:
        mesht.update()
        pkt = mesht.get()
        if pkt != None:
            print('packet')
            print(pkt)
            
        time.sleep(.1)
