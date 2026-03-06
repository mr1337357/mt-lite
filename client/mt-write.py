import sys
import select
import serial
import time
import datetime
import traceback
import random

import mt_radio_serial
from mt_packet import mt_packet

import pypb
from mt_lite import mt_lite
from sql_wrapper import sqlwrapper
    

if __name__ == '__main__':
    log = open('test.log','w')
    
    users = sqlwrapper()
    radio = mt_radio_serial.mt_radio_serial(sys.argv[1])
    mesht = mt_lite(radio)
    sendpkt = mt_packet()
    sendpkt.src = 0x13371337
    sendpkt.seq = random.randint(0,2<<31)
    nodeinfo = pypb.protobuf()
    nodeinfo.encode(1,pypb.PB_STRING,'!13371337')
    nodeinfo.encode(2,pypb.PB_STRING,'INVALID NODE')
    nodeinfo.encode(3,pypb.PB_STRING,'peepee poopoo')
    sendmsg = pypb.protobuf()
    sendmsg.encode(1,pypb.PB_VARINT,4)
    sendmsg.encode(2,pypb.PB_STRING,nodeinfo.buffer)
    sendpkt.payload = sendmsg.buffer
    mesht.send(sendpkt)
    while True:
        mesht.update()
        pkt = mesht.get()
        if pkt != None:
            pkt.dump()
        time.sleep(.01)
